#ifndef _H_PARSER_NODE
#define _H_PARSER_NODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef union _ParserNode ParserNode;

typedef enum _PN_VALUE_TYPE
{
	E_PNVT_IDENTIFIER,
	E_PNVT_BOOL,
	E_PNVT_INT64,
	E_PNVT_UINT64,
	E_PNVT_HEX_INT64,
	E_PNVT_HEX_UINT64,
}PN_VALUE_TYPE;

typedef struct _PN_VALUE PN_VALUE;
typedef struct _PN_VALUE
{
	PN_VALUE_TYPE type;
	hpint64 i64;
	hpuint64 ui64;
	hpbool b;

};


typedef enum _E_PN_TYPE
{
	E_SNT_INT8 = E_HP_INT8,
	E_SNT_INT16 = E_HP_INT16,
	E_SNT_INT32 = E_HP_INT32,
	E_SNT_INT64 = E_HP_INT64,
	E_SNT_UINT8 = E_HP_UINT8,
	E_SNT_UINT16 = E_HP_UINT16,
	E_SNT_UINT32 = E_HP_UINT32,
	E_SNT_UINT64 = E_HP_UINT64,
	E_SNT_CHAR = E_HP_CHAR,
	E_SNT_BYTES = E_HP_BYTES,
	E_SNT_STRING = E_HP_STRING,
	E_SNT_DOUBLE = E_HP_DOUBLE,
	E_SNT_BOOL = E_HP_BOOL,
	E_SNT_NULL = E_HP_NULL,
	E_SNT_VECTOR = E_HP_VECTOR,
	E_SNT_OBJECT = E_HP_OBJECT,
}E_PN_TYPE;

#define MAX_IDENTIFIER_LENGTH 1024
typedef struct _SN_TYPE
{
	E_PN_TYPE type;
	char identifier[MAX_IDENTIFIER_LENGTH];
}PN_TYPE;


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
	hpuint64 sn_hex_uint64;
	hpint64 sn_hex_int64;
};


#endif//_H_PARSER_NODE

