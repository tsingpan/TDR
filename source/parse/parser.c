#include "parse/parser.h"
#include "error.h"

#include "scanner_l.h"
#include "parser_y.h"
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

static void on_document_begin(PARSER *self, const YYLTYPE *yylloc, const char *file_name)
{
	uint32_t i;

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_begin(self->generator_list[i], yylloc, file_name);
	}
}

static void on_document_end(PARSER *self, const YYLTYPE *yylloc, const char *file_name)
{
	uint32_t i;
	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_end(self->generator_list[i], yylloc, file_name);
	}
}

static void parser_push(PARSER *self, const char *file_name)
{
	int ret;
	ret = scanner_push(&self->scanner, file_name, yycINITIAL);
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

int tdrparse (scanner_t *self);
int32_t parser_parse(PARSER *self, const char* file_name, generator_t **generator_list, uint32_t generator_list_num)
{
	int32_t ret;	

	self->generator_list = generator_list;
	self->generator_num = generator_list_num;
	scanner_init(&self->scanner);
	
	parser_push(self, file_name);

	on_document_begin(self, NULL, file_name);

	//这玩意自动生成的
	ret = tdrparse(&self->scanner);
	on_document_end(self, NULL, file_name);
	scanner_pop(&self->scanner);
	symbols_clear(&self->symbols);
	return E_TD_NOERROR;
}

void parser_on_struct_begin(PARSER *self, const YYLTYPE *yylloc, const char* struct_name)
{
	if(scanner_size(&self->scanner) == 1)
	{
		uint32_t i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_struct_begin(self->generator_list[i], yylloc, struct_name);
		}
	}
}

void parser_on_field(PARSER *self, const YYLTYPE *yylloc, const syn_field_t *pn_field)
{
	if(scanner_size(&self->scanner) == 1)
	{
		uint32_t i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_field(self->generator_list[i], yylloc, pn_field);
		}
	}
}

void parser_on_struct_end(PARSER *self, const YYLTYPE *yylloc, const syn_struct_t *pn_struct)
{
	if(scanner_size(&self->scanner) == 1)
	{
		uint32_t i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_struct_end(self->generator_list[i], yylloc, pn_struct);
		}
	}
}

//do
void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const syn_definition_t *pn_definition)
{
	if(scanner_size(&self->scanner) == 1)
	{
		uint32_t i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_definition(self->generator_list[i], yylloc, pn_definition);
		}
	}

	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TLIBC_MAX_PATH_LENGTH];
		snprintf(file_name, TLIBC_MAX_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		file_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
		parser_push(self, file_name);
	}
}
