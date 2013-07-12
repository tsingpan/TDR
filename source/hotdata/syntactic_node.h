#ifndef _H_SYNTACTICNODE 
#define _H_SYNTACTICNODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef union _SyntacticNode SyntacticNode;

typedef enum _SN_VALUE_TYPE
{
	E_SNVT_IDENTIFIER,
	E_SNVT_BOOL,
	E_SNVT_INT64,
	E_SNVT_UINT64,
	E_SNVT_HEX_INT64,
	E_SNVT_HEX_UINT64,
}SN_VALUE_TYPE;

#define INVALID_INDEX 0xffffffff

typedef struct _SN_VALUE SN_VALUE;
typedef struct _SN_VALUE
{
	SN_VALUE_TYPE type;
	hpint64 i64;
	hpuint64 ui64;
	hpbool b;

	hpuint32 index;
};


typedef enum _E_SN_TYPE
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
}E_SN_TYPE;

#define MAX_IDENTIFIER_LENGTH 1024
typedef struct _SN_TYPE
{
	E_SN_TYPE type;
	char identifier[MAX_IDENTIFIER_LENGTH];
}SN_TYPE;


union _SyntacticNode
{
	SN_VALUE sn_value;	
	SN_TYPE sn_type;
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


typedef enum _SyntacticSymbolType
{
	E_SST_Value			= 0,
	E_SST_Type			= 1,
	E_SST_Struct		= 2,
	E_SST_Union			= 3,
	E_SST_Enum			= 4,
	E_SST_EnumDef		= 5,
	E_SST_Const			= 6,
}SyntacticSymbolType;

typedef struct SyntacticSymbol
{
	SyntacticSymbolType type;
	SyntacticNode sn;
}SyntacticSymbol;

#endif//_H_SYNTACTICNODE

