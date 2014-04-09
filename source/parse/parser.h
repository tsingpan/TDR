#ifndef _H_PARSER
#define _H_PARSER


#include "platform/tlibc_platform.h"

#include "parse/scanner.h"
#include "generator.h"
#include "symbols.h"
#include "definition.h"


typedef struct _PARSER PARSER;
struct _PARSER 
{
	scanner_t scanner;

	generator_t **generator_list;
	uint32_t generator_num;
		
	td_symbols_t symbols;

	ST_DEFINITION pn_definition;
};

void parser_init(PARSER *self);

int32_t parser_parse(PARSER *self, const char* file_name, generator_t **generator_list, uint32_t generator_list_num);

void parser_on_struct_begin(PARSER *self, const YYLTYPE *yylloc, const char *struct_name);

void parser_on_field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void parser_on_struct_end(PARSER *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct);

void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const ST_DEFINITION *pn_definition);

#endif//_H_PARSER
