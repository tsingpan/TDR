#ifndef _H_PARSER
#define _H_PARSER


#include "platform/tlibc_platform.h"

#include <stdio.h>
#include "parse/scanner.h"
#include "protocol/tlibc_abstract_writer.h"

#include "datrie/trie.h"
#include "error/error_msg.h"
#include "definition.h"

typedef union _PARSER_VALUE
{
	ST_Import sn_import;

	ST_TYPE sn_type;
	ST_VALUE sn_value;	
	tbytes sn_tok_identifier;

	ST_Const sn_const;

	tdouble pn_tok_double;


	const char* sn_tok_unixcomment;
	tbool sn_bool;
	tuint64 sn_uint64;
	tint64 sn_int64;
	tdouble sn_d;
	tuint64 sn_hex_uint64;
	tint64 sn_hex_int64;
	tstring sn_string;
	tchar sn_char;
	SN_SIMPLE_TYPE sn_st;


	ST_ARGUMENTS sn_arguments;
	SN_CONTAINER_TYPE sn_ct;

	ST_TYPEDEF sn_typedef;

	ST_TypeAnnotation sn_type_annotation;
	ST_TypeAnnotations sn_type_annotations;

	ST_UNIX_COMMENT sn_unix_comment;

	ST_ENUM_DEF sn_enum_def;
	ST_Parameter sn_parameter;
	ST_Parameters sn_parameters;

	ST_Expression sn_expression;
	ST_CONDITION sn_condition;

}PARSER_VALUE;


typedef struct _PARSER PARSER;
struct _PARSER 
{
	SCANNER_STACK scanner_stack;
		
	char file_name[MAX_FILE_NAME_LENGTH];

	//·ûºÅ±í
	Trie *symbols;
	char domain[MAX_FILE_NAME_LENGTH];

	ST_DEFINITION pn_definition;
	ST_FIELD_LIST pn_field_list;
	ST_FIELD pn_field;
	tbool in_union;
	tbool in_struct;
};

void parser_init(PARSER *self, const char* root_dir);

tint32 parser_parse(PARSER *self, const char* file_name);

void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const ST_DEFINITION *pn_definition);

#endif//_H_PARSER

