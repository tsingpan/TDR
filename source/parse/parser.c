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

tint32 parser_parse(PARSER *self, const char* file_name, GENERATOR **generator_list, tuint32 generator_list_num)
{
	tint32 ret;	
	tuint32 i;

	self->generator_list = generator_list;
	self->generator_num = generator_list_num;

	symbols_init(&self->symbols);
	scanner_stack_init(&self->scanner_stack);

	strncpy(self->file_name, file_name, TLIBC_MAX_FILE_PATH_LENGTH);
	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
	{
		scanner_stack_error_halt(&self->scanner_stack, NULL, E_TD_ERROR);
	}

	ret = tdataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
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
		if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
		{
			scanner_stack_error_halt(&self->scanner_stack, NULL, E_TD_ERROR, file_name);
		}
	}

	if(scanner_stack_get_num(&self->scanner_stack) == 1)
	{
		tuint32 i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_definition(self->generator_list[i], &self->pn_definition);
		}
	}
}

void parser_on_document_begin(PARSER *self)
{
	tuint32 i;
	if(scanner_stack_get_num(&self->scanner_stack) != 1)
	{
		goto SKIP;
	}

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_begin(self->generator_list[i], self->file_name);
	}

SKIP:
	return;
}

void parser_on_document_end(PARSER *self)
{
	tuint32 i;
	if(scanner_stack_get_num(&self->scanner_stack) != 1)
	{
		goto SKIP;
	}

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_end(self->generator_list[i], self->file_name);
	}

SKIP:
	return;
}
