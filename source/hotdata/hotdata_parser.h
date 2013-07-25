#ifndef _H_HOTDATA_PARSER
#define _H_HOTDATA_PARSER


#include "hotpot/hp_platform.h"
#include <stdarg.h>
#include <stdio.h>
#include "hotscript/hotlex.h"
#include "hotpot/hp_value.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "datrie/trie.h"
#include "language/language.h"


#include "parser_node.h"

#define MAX_SYMBOL_LIST_NUM 1024
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

/*
	hpuint32 symbol_list_num;
	SyntacticSymbol symbol_list[MAX_SYMBOL_LIST_NUM];
*/
	char file_name[MAX_FILE_NAME_LENGTH];

	//·ûºÅ±í
	Trie *symbols;
};

hpint32 data_parser_init(DATA_PARSER *self);

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib);



//handler

void dp_on_document_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_document_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_definition_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_const_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

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

void dp_on_tok_import(DATA_PARSER *self, const YYLTYPE *yylloc, const hpstring sn_tok_import);

void dp_on_value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64);

void dp_on_value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64);

void dp_on_value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpuint64 ui64);

void dp_on_value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpuint64 ui64);

void dp_on_value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_field_begin(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name);

void dp_on_field_end(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name);

void dp_on_vector_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_item_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_vector_item_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_null(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_bool(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbool b);

void dp_on_bytes(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes bytes);

void dp_on_string(DATA_PARSER *self, const YYLTYPE *yylloc, const hpchar *str);

void dp_on_typedef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_TYPE* type, const hpbytes sn_tok_identifier);

void dp_on_TypeAnnotations_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_TypeAnnotations_switch(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes *sn_tok_identifier);

void dp_on_TypeAnnotations_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_on_enum_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_union_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_struct_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

void dp_on_field_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier);

//do
void dp_do_import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_IMPORT* current, const hpstring sn_tok_import);

void dp_do_value_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbytes sn_identifier);

void dp_do_value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64);

void dp_do_value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64);

void dp_do_value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64);

void dp_do_value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64);

void dp_do_container_type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_CONTAINER_TYPE type);

void dp_do_simple_type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_SIMPLE_TYPE type);

void dp_do_type_object(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const hpbytes sn_tok_identifier);

/*
//check
void dp_check_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc,const SyntacticNode* sn_type, const hpbytes sn_tok_identifier);

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const SN_TYPE* sn_type, const hpbytes* sn_identifier, const SN_VALUE* sn_value);
*/

#endif//_H_HOTDATA_PARSER

