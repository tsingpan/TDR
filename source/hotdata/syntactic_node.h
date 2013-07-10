#ifndef _H_SYNTACTICNODE 
#define _H_SYNTACTICNODE


#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef enum _NODE_TYPE
{
	NT_VALUE		= 0,
}NODE_TYPE;

typedef struct _SN_VALUE
{
	hpbool is_identifier;
	HPVar var;
}SN_VALUE;


typedef union _SyntacticNodeBody
{
	SN_VALUE sn_value;
}SyntacticNodeBody;

typedef struct _SyntacticNode SyntacticNode;
struct _SyntacticNode
{
	NODE_TYPE type;
	SyntacticNodeBody body;


	HPVar var;
};



#endif//_H_SYNTACTICNODE

