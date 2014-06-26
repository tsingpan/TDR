#ifndef _H_PARSER
#define _H_PARSER


#include "platform/tdr_platform.h"

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
void parser_on_generator_definition(PARSER *self, const YYLTYPE *yylloc, const syn_definition_t *pn_definition);

//通过以下接口可以生成所有的代码
void parser_on_document_begin(PARSER *self, const char *file_name);

void parser_on_document_error(PARSER *self);

void parser_on_document_end(PARSER *self);

void parser_on_import(PARSER *self, const syn_import_t* syn_import);

void parser_on_typedef(PARSER *self, const syn_typedef_t *syn_typedef);

void parser_on_const(PARSER *self, const syn_const_t *syn_const);

void parser_on_enum_begin(PARSER *self, const char* name);

void parser_on_enum_field(PARSER *self, const enum_def_t* enum_def);

void parser_on_enum_end(PARSER *self, const char* name);

void parser_on_union_begin(PARSER *self, const char* name, const char *etype);

void parser_on_union_field(PARSER *self, const syn_union_field_t* union_field);

void parser_on_union_end(PARSER *self, const char* name);

void parser_on_struct_begin(PARSER *self, const char* name);

void parser_on_struct_field(PARSER *self, const syn_field_t* struct_field);

void parser_on_struct_end(PARSER *self, const char* name);

void parser_on_unit_comment(PARSER *self, const syn_unix_comment_t* comment);

#endif//_H_PARSER
