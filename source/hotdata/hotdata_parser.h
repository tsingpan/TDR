#ifndef _H_XML_PARSER
#define _H_XML_PARSER


#include "hotpot/hp_platform.h"
#include <stdarg.h>
#include <stdio.h>
#include "hotscript/hotlex.h"
#include "hotpot/hp_value.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "datrie/trie.h"
#include "language/language.h"


#include "syntactic_node.h"

#define MAX_ERROR_NUM 1024
typedef struct _DATA_PARSER DATA_PARSER;
struct _DATA_PARSER 
{
	SCANNER_STACK scanner_stack;
	HPAbstractWriter *writer;

	hpint32 result[MAX_ERROR_NUM];
	char result_str[MAX_ERROR_NUM][MAX_RESULT_STRING_LENGTH];
	hpuint32 result_num;

	const LanguageLib *language_lib;

	

	//符号表
	Trie *constant;				//记录已定义的常量符号
};

hpint32 data_parser_init(DATA_PARSER *self);

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib);



void dp_on_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier);

void dp_on_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier, const SyntacticNode* sn_value);

void dp_on_value_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, SyntacticNode* current, const SyntacticNode* sn_identifier);

void dp_on_import(DATA_PARSER *self, const YYLTYPE *yylloc, SyntacticNode* current, const SyntacticNode* sn_import);

//handler

void dp_on_document_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_document_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_import_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_import_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_const_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_const_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_const_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_typedef_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_typedef_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_struct_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_struct_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_union_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_union_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_enum_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_enum_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_tok_unixcomment_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_tok_unixcomment_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_tok_import(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_import);

void dp_on_value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64);

void dp_on_value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64);

void dp_on_value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_field_begin(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name);

void dp_on_field_end(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name);

void dp_on_vector_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_item_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_item_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_typedef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const SN_TYPE* type, const hpbytes sn_tok_identifier);

void dp_on_TypeAnnotations_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_TypeAnnotations_switch(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes *sn_tok_identifier);

void dp_on_TypeAnnotations_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_enum_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_union_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

#endif//_H_XML_PARSER

