#ifndef _H_PARSER_NODE
#define _H_PARSER_NODE


#include "platform/tlibc_platform.h"

#include "hotdata/syntactic_node.h"

typedef union _ParserNode ParserNode;

typedef ST_DEFINITION PN_DEFINITION;
typedef ST_Const PN_CONST;
typedef ST_VALUE PN_VALUE;
typedef ST_TYPE PN_TYPE;
typedef ST_TYPEDEF PN_TYPEDEF;
typedef tchar PN_CHAR;
typedef ST_Import PN_IMPORT;
typedef tbytes PN_IDENTIFIER;
typedef tdouble PN_TOK_DOUBLE;
typedef tstring PN_TOK_STRING;
typedef ST_ARGUMENTS PN_ARGUMENTS;
typedef ST_TypeAnnotation PN_TypeAnnotation;
typedef ST_TypeAnnotations PN_TypeAnnotations;
typedef ST_UNIX_COMMENT PN_UNIX_COMMENT;
typedef ST_ENUM_DEF PN_ENUM_DEF;
typedef ST_ENUM PN_ENUM;
typedef ST_Parameter PN_Parameter;
typedef ST_Parameters PN_Parameters;
typedef ST_Expression PN_Expression;
typedef ST_CONDITION PN_CONDITION;
typedef ST_FIELD PN_FIELD;
typedef ST_FIELD_LIST PN_FIELD_LIST;

union _ParserNode
{
	PN_IMPORT sn_import;

	PN_TYPE sn_type;
	PN_VALUE sn_value;	
	PN_IDENTIFIER sn_tok_identifier;

	PN_CONST sn_const;

	PN_TOK_DOUBLE pn_tok_double;


	const char* sn_tok_unixcomment;
	tbool sn_bool;
	tuint64 sn_uint64;
	tint64 sn_int64;
	tdouble sn_d;
	tuint64 sn_hex_uint64;
	tint64 sn_hex_int64;
	tstring sn_string;
	PN_CHAR sn_char;
	SN_SIMPLE_TYPE sn_st;


	PN_ARGUMENTS sn_arguments;
	SN_CONTAINER_TYPE sn_ct;

	PN_TYPEDEF sn_typedef;

	PN_TypeAnnotation sn_type_annotation;
	PN_TypeAnnotations sn_type_annotations;

	PN_UNIX_COMMENT sn_unix_comment;

	PN_ENUM_DEF sn_enum_def;
	PN_Parameter sn_parameter;
	PN_Parameters sn_parameters;

	PN_Expression sn_expression;
	PN_CONDITION sn_condition;

};


#endif//_H_PARSER_NODE

