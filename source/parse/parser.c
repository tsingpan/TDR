#include "parse/parser.h"
#include "error.h"

#include "tdata_y.h"
#include "tdata_l.h"
#include "parse/scanner.h"

#include "symbols.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parse/scanner.h"


static void on_document_begin(PARSER *self, const char *file_name)
{
	tuint32 i;

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_begin(self->generator_list[i], file_name);
	}
}

static void on_document_end(PARSER *self, const char *file_name)
{
	tuint32 i;
	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_end(self->generator_list[i], file_name);
	}
}


tint32 parser_parse(PARSER *self, const char* file_name, GENERATOR **generator_list, tuint32 generator_list_num)
{
	tint32 ret;	
	tuint32 i;

	self->generator_list = generator_list;
	self->generator_num = generator_list_num;

	symbols_init(&self->symbols);
	scanner_init(&self->scanner_stack);

	if(scanner_push(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
	{
		scanner_error(&self->scanner_stack, NULL, E_LS_UNKNOW);
	}
	on_document_begin(self, file_name);

	//这玩意自动生成的
	ret = tdataparse(&self->scanner_stack);
	on_document_end(self, file_name);
	scanner_pop(&self->scanner_stack);
done:
	symbols_fini(&self->symbols);
	return E_TD_NOERROR;
}


//do
void parser_on_definition(PARSER *self, const ST_DEFINITION *pn_definition)
{
	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TLIBC_MAX_FILE_PATH_LENGTH];
		snprintf(file_name, TLIBC_MAX_FILE_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		if(scanner_push(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
		{
			scanner_error(&self->scanner_stack, NULL, E_LS_UNKNOW, file_name);
		}
	}

	if(scanner_get_stack_deep(&self->scanner_stack) == 1)
	{
		tuint32 i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_definition(self->generator_list[i], &self->pn_definition);
		}
	}
}
