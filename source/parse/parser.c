#include "parse/parser.h"
#include "error.h"

#include "tdata_y.h"
#include "tdata_l.h"
#include "parse/scanner.h"
#include "globals.h"

#include "symbols.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parse/scanner.h"


void parser_init(PARSER *self)
{
	symbols_init(&self->symbols);
}

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

static void parser_push(PARSER *self, const char *source_dir, const char *file_name)
{
	int ret;
	ret = scanner_push(&self->scanner, source_dir, file_name, yycINITIAL);
	if(ret != E_TD_NOERROR)
	{
		switch(ret)
		{
		case E_TD_SCANNER_CAN_NOT_OPEN_FILE:
			scanner_error_halt(NULL, E_LS_CANNOT_OPEN_FILE, file_name);
		case E_TD_SCANNER_OUT_OF_MEMORY:
			scanner_error_halt(NULL, E_LS_OUT_OF_MEMORY, file_name);			
		default:
			scanner_error_halt(NULL, E_LS_UNKNOW);
		}		
	}
}

int tdataparse (SCANNER *self);
tint32 parser_parse(PARSER *self, const char* file_name, GENERATOR **generator_list, tuint32 generator_list_num)
{
	tint32 ret;	

	self->generator_list = generator_list;
	self->generator_num = generator_list_num;
	scanner_init(&self->scanner);
	
	parser_push(self, g_source_dir, file_name);

	on_document_begin(self, file_name);

	//这玩意自动生成的
	ret = tdataparse(&self->scanner);
	on_document_end(self, file_name);
	scanner_pop(&self->scanner);
	symbols_clear(&self->symbols);
	return E_TD_NOERROR;
}


//do
void parser_on_definition(PARSER *self, const ST_DEFINITION *pn_definition)
{
	if(scanner_size(&self->scanner) == 1)
	{
		tuint32 i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_definition(self->generator_list[i], &self->pn_definition);
		}
	}

	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TLIBC_MAX_PATH_LENGTH];
		snprintf(file_name, TLIBC_MAX_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		file_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
		parser_push(self, g_source_dir, file_name);
	}
}
