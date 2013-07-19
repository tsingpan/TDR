#ifndef _H_PARSER_NODE
#define _H_PARSER_NODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

#include "syntactic_node.h"

typedef union _ParserNode ParserNode;

typedef ST_VALUE PN_VALUE;
typedef ST_TYPE PN_TYPE;

union _ParserNode
{
	PN_VALUE sn_value;	
	PN_TYPE sn_type;
	hpbytes sn_tok_identifier;
	hpbytes sn_tok_import;
	hpbytes sn_import;
	hpbytes sn_tok_unixcomment;
	hpbool sn_bool;
	hpuint64 sn_uint64;
	hpint64 sn_int64;
	hpdouble sn_d;
	hpuint64 sn_hex_uint64;
	hpint64 sn_hex_int64;
	hpstring sn_string;
	hpchar sn_char;
	SN_SIMPLE_TYPE sn_st;
	SN_CONTAINER_TYPE sn_ct;
};


#endif//_H_PARSER_NODE

