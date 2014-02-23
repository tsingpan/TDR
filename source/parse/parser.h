#ifndef _H_PARSER
#define _H_PARSER


#include "tlibc/platform/tlibc_platform.h"

#include "parse/scanner.h"
#include "generator.h"
#include "symbols.h"
#include "definition.h"


typedef struct _PARSER PARSER;
struct _PARSER 
{
	SCANNER scanner;

	GENERATOR **generator_list;
	uint32_t generator_num;
		
	SYMBOLS symbols;

	ST_DEFINITION pn_definition;
};

void parser_init(PARSER *self);

int32_t parser_parse(PARSER *self, const char* file_name, GENERATOR **generator_list, uint32_t generator_list_num);

void parser_on_definition(PARSER *self, const ST_DEFINITION *pn_definition);

#endif//_H_PARSER
