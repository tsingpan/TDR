#ifndef _H_syntactic_node_hd_types
#define _H_syntactic_node_hd_types

#include "hotpot/hp_platform.h"


#ifndef MAX_STRLEN_LENGTH
#define MAX_STRLEN_LENGTH 1024
#endif//MAX_STRLEN_LENGTH

typedef enum _SN_VALUE_TYPE
{

	E_PNVT_IDENTIFIER = 0 ,
	E_PNVT_BOOL = 1 ,
	E_PNVT_INT64 = 2 ,
	E_PNVT_UINT64 = 3 ,
	E_PNVT_HEX_INT64 = 4 ,
	E_PNVT_HEX_UINT64 = 5 ,
}PN_VALUE_TYPE;

typedef union _UN_VALUE
{

	hpint64 i64;
	hpint64 i64;
	hpuint64 ui64;
	hpuint64 ui64;
	hpbool b;
	hpchar identifier[MAX_STRLEN_LENGTH];
}UN_VALUE;

typedef struct _SN_VALUE
{

	PN_VALUE_TYPE type;
	UN_VALUE val;
}PN_VALUE;

#ifndef MAX_PARAMETER_NUM
#define MAX_PARAMETER_NUM 1024
#endif//MAX_PARAMETER_NUM

typedef struct _ST_Parameter
{

	ST_Type type;
	hpchar identifier[MAX_STRING_LENGTH];
}ST_Parameter;

typedef struct _ST_Parameters
{

	hpuint32 par_list_num;
	ST_Parameter par_list[MAX_PARAMETER_NUM];
}ST_Parameters;

typedef enum _SN_SIMPLE_TYPE
{

	E_ST_INT8 = 0 ,
	E_ST_INT16 = 1 ,
	E_ST_INT32 = 2 ,
	E_ST_INT64 = 3 ,
	E_ST_UINT8 = 4 ,
	E_ST_UINT16 = 5 ,
	E_ST_UINT32 = 6 ,
	E_ST_UINT64 = 7 ,
	E_ST_CHAR = 8 ,
	E_ST_BOOL = 9 ,
}SN_SIMPLE_TYPE;

typedef enum _SN_CONTAINER_TYPE
{

	E_CT_VECTOR = 0 ,
	E_CT_STRING = 1 ,
}SN_CONTAINER_TYPE;

typedef enum _SN_TYPE
{

	E_SNT_SIMPLE = 1 ,
	E_SNT_CONTAINER = 2 ,
	E_SNT_OBJECT = 0 ,
}PN_TYPE;

typedef struct _ST_TYPE
{

	E_PN_TYPE type;
	SN_SIMPLE_TYPE st;
	SN_CONTAINER_TYPE ct;
	hpchar ot[MAX_STRING_LENGTH];
}ST_TYPE;

#ifndef MAX_ARGUMENT_NUM
#define MAX_ARGUMENT_NUM 16
#endif//MAX_ARGUMENT_NUM

typedef struct _ST_ARGUMENT
{

	hpchar id[MAX_STRING_LENGTH];
}ST_ARGUMENT;

typedef struct _ST_ARGUMENTS
{

	hpuint32 arg_list_num;
	ST_ARGUMENT arg_list[MAX_ARGUMENT_NUM];
}ST_ARGUMENTS;

typedef struct _ST_Expression
{

	hpbool neg;
	hpchar op0[MAX_STRLEN_LENGTH];
	hpchar operator[MAX_STRLEN_LENGTH];
	hpchar op1[MAX_STRLEN_LENGTH];
}ST_Expression;

typedef struct _ST_CONDITION
{

	hpbool empty;
	ST_Expression exp;
}ST_CONDITION;

typedef struct _ST_FIELD
{

	ST_CONDITION condition;
	ST_TYPE type;
	ST_ARGUMENTS args;
	hpchar identifier[MAX_STRING_LENGTH];
	ST_UNIX_COMMENT comment;
}ST_FIELD;

#ifndef MAX_TA_LIST_NUM
#define MAX_TA_LIST_NUM 1024
#endif//MAX_TA_LIST_NUM

typedef enum _TA_TYPE
{

	E_TA_SWITCH = 0 ,
	E_TA_LOWER_BOUND = 1 ,
	E_TA_UPPER_BOUND = 2 ,
}TA_TYPE;

typedef union _UN_TypeAnnotation
{

	hpchar ta_switch[MAX_STRING_LENGTH];
	ST_Value ta_lower_bound;
	ST_Value ta_upper_bound;
}UN_TypeAnnotation;

typedef struct _ST_TypeAnnotation
{

	TA_TYPE type;
	UN_TypeAnnotation ta;
}ST_TypeAnnotation;

typedef struct _ST_TypeAnnotations
{

	hpuint32 ta_list_num;
	ST_TypeAnnotation ta_list[MAX_TA_LIST_NUM];
}ST_TypeAnnotations;

typedef struct _ST_Import
{

	hpchar package_name[MAX_STRLEN_LENGTH];
}ST_Import;

typedef struct _ST_Const
{

	ST_TYPE type;
	PN_VALUE val;
}ST_Const;

#ifndef MAX_COMMENT_LENGTH
#define MAX_COMMENT_LENGTH 1024
#endif//MAX_COMMENT_LENGTH

typedef struct _ST_UNIX_COMMENT
{

	hpuint32 len;
	hpchar text[MAX_COMMENT_LENGTH];
}ST_UNIX_COMMENT;

typedef struct _ST_ENUM_DEF
{

	hpchar identifier[MAX_STRLEN_LENGTH];
	PN_VALUE val;
	ST_UNIX_COMMENT comment;
}ST_ENUM_DEF;

#ifndef MAX_ENUM_DEF_LIST_NUM
#define MAX_ENUM_DEF_LIST_NUM 65536
#endif//MAX_ENUM_DEF_LIST_NUM

typedef struct _ST_ENUM
{

	hpuint32 enum_def_list_num;
	ST_ENUM_DEF enum_def_list[MAX_ENUM_DEF_LIST_NUM];
}ST_ENUM;

#ifndef MAX_FIELD_LIST_NUM
#define MAX_FIELD_LIST_NUM 65536
#endif//MAX_FIELD_LIST_NUM

typedef struct _ST_STRUCT
{

	ST_TypeAnnotations ta;
	hpchar name[MAX_STRING_LENGTH];
	ST_Parameters parameters;
	hpuint32 field_list_num;
	ST_FIELD field_list[MAX_FIELD_LIST_NUM];
}ST_STRUCT;

typedef struct _ST_UNION
{

	ST_TypeAnnotations ta;
	hpchar name[MAX_STRING_LENGTH];
	ST_Parameters parameters;
	hpuint32 field_list_num;
	ST_FIELD field_list[MAX_FIELD_LIST_NUM];
}ST_UNION;

#endif//_H_syntactic_node_hd_types
