#ifndef _H_SYNTACTICNODE 
#define _H_SYNTACTICNODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _SyntacticNode SyntacticNode;

typedef enum _NODE_TYPE
{
	NT_VALUE				= 0,
	NT_TYPE					= 1,
	NT_TOK_IDENTIFIER		= 2,
	NT_IMPORT			= 3,
}NODE_TYPE;

typedef struct _SN_VALUE
{
	hpbool is_identifier;
	HPVar var;
	const SyntacticNode* sn;
}SN_VALUE;

#define MAX_IDENTIFIER_LENGTH 1024
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

typedef struct _SN_TYPE
{
	E_SN_TYPE type;
	char identifier[MAX_IDENTIFIER_LENGTH];
}SN_TYPE;


typedef union _SyntacticNodeBody
{
	SN_VALUE sn_value;	
	SN_TYPE sn_type;
	hpbytes sn_tok_identifier;
	hpbytes sn_tok_import;
}SyntacticNodeBody;


struct _SyntacticNode
{
	NODE_TYPE type;
	SyntacticNodeBody body;


	HPVar var;
};



#endif//_H_SYNTACTICNODE

