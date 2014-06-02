#ifndef _H_PARSER
#define _H_PARSER


#include "platform/tlibc_platform.h"

#include "parse/scanner.h"
#include "generator.h"
#include "symbols.h"
#include "definition.h"

#define DEP_SUFFIX "d"

typedef struct _PARSER PARSER;
struct _PARSER 
{
	scanner_t scanner;

	generator_t *generator;
			
	symbols_t symbols;

	syn_definition_t pn_definition;
};

void parser_init(PARSER *self);

int32_t parser_parse(PARSER *self, const char* file_name, generator_t *generator, int make_rule);

void parser_on_struct_begin(PARSER *self, const YYLTYPE *yylloc, const char *struct_name);

void parser_on_field(PARSER *self, const YYLTYPE *yylloc, const syn_field_t *pn_field);

void parser_on_struct_end(PARSER *self, const YYLTYPE *yylloc, const syn_struct_t *pn_struct);

void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const syn_definition_t *pn_definition);

#endif//_H_PARSER
