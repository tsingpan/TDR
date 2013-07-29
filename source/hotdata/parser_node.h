#ifndef _H_PARSER_NODE
#define _H_PARSER_NODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

#include "syntactic_node.h"

typedef union _ParserNode ParserNode;

typedef ST_DEFINITION PN_DEFINITION;
typedef ST_Const PN_CONST;
typedef ST_VALUE PN_VALUE;
typedef ST_TYPE PN_TYPE;
typedef ST_TYPEDEF PN_TYPEDEF;
typedef hpchar PN_CHAR;
typedef ST_Import PN_IMPORT;
typedef hpbytes PN_IDENTIFIER;
typedef hpdouble PN_TOK_DOUBLE;
typedef hpstring PN_TOK_STRING;
typedef ST_ARGUMENT PN_ARGUMENT;
typedef ST_ARGUMENTS PN_ARGUMENTS;
typedef ST_TypeAnnotation PN_TypeAnnotation;
typedef ST_TypeAnnotations PN_TypeAnnotations;
typedef ST_UNIX_COMMENT PN_UNIX_COMMENT;

union _ParserNode
{
	PN_IMPORT sn_import;
	hpstring sn_tok_import;

	PN_TYPE sn_type;
	PN_VALUE sn_value;	
	PN_IDENTIFIER sn_tok_identifier;

	PN_CONST sn_const;

	PN_TOK_DOUBLE pn_tok_double;


	const char* sn_tok_unixcomment;
	hpbool sn_bool;
	hpuint64 sn_uint64;
	hpint64 sn_int64;
	hpdouble sn_d;
	hpuint64 sn_hex_uint64;
	hpint64 sn_hex_int64;
	hpstring sn_string;
	PN_CHAR sn_char;
	SN_SIMPLE_TYPE sn_st;


	PN_ARGUMENT sn_argument;
	PN_ARGUMENTS sn_arguments;
	SN_CONTAINER_TYPE sn_ct;

	PN_TYPEDEF sn_typedef;

	PN_TypeAnnotation sn_type_annotation;
	PN_TypeAnnotations sn_type_annotations;

	PN_UNIX_COMMENT sn_unix_comment;
};


#endif//_H_PARSER_NODE

