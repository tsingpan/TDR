#ifndef _H_syntactic_node_hd_types
#define _H_syntactic_node_hd_types

#include "hotpot/hp_platform.h"


#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH 1024
#endif//MAX_STRING_LENGTH

#ifndef MAX_COMMENT_LENGTH
#define MAX_COMMENT_LENGTH 1024
#endif//MAX_COMMENT_LENGTH

typedef struct _ST_UNIX_COMMENT_OR_NOT
{

	hpbool empty;
	hpuint32 len;
	hpchar text[MAX_COMMENT_LENGTH];
}ST_UNIX_COMMENT_OR_NOT;

typedef enum _SN_VALUE_TYPE
{

	E_SNVT_IDENTIFIER = 0 ,
	E_SNVT_BOOL = 1 ,
	E_SNVT_INT64 = 2 ,
	E_SNVT_UINT64 = 3 ,
	E_SNVT_HEX_INT64 = 4 ,
	E_SNVT_HEX_UINT64 = 5 ,
}SN_VALUE_TYPE;

typedef union _UN_VALUE
{

	hpint64 i64;
	hpint64 hex_i64;
	hpuint64 ui64;
	hpuint64 hex_ui64;
	hpbool b;
	hpchar identifier[MAX_STRING_LENGTH];
}UN_VALUE;

typedef struct _ST_VALUE
{

	SN_VALUE_TYPE type;
	UN_VALUE val;
}ST_VALUE;

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
	E_ST_DOUBLE = 10 ,
}SN_SIMPLE_TYPE;

typedef enum _SN_CONTAINER_TYPE
{

	E_CT_VECTOR = 0 ,
	E_CT_STRING = 1 ,
}SN_CONTAINER_TYPE;

typedef enum _SN_TYPE
{

	E_SNT_SIMPLE = 0 ,
	E_SNT_CONTAINER = 1 ,
	E_SNT_OBJECT = 2 ,
}SN_TYPE;

typedef struct _ST_TYPE
{

	SN_TYPE type;
	SN_SIMPLE_TYPE st;
	SN_CONTAINER_TYPE ct;
	hpchar ot[MAX_STRING_LENGTH];
}ST_TYPE;

#ifndef MAX_PARAMETER_NUM
#define MAX_PARAMETER_NUM 1024
#endif//MAX_PARAMETER_NUM

typedef struct _ST_Parameter
{

	ST_TYPE type;
	hpchar identifier[MAX_STRING_LENGTH];
}ST_Parameter;

typedef struct _ST_Parameters
{

	hpuint32 par_list_num;
	ST_Parameter par_list[MAX_PARAMETER_NUM];
}ST_Parameters;

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
	hpchar op0[MAX_STRING_LENGTH];
	hpchar oper[MAX_STRING_LENGTH];
	hpchar op1[MAX_STRING_LENGTH];
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
	ST_UNIX_COMMENT_OR_NOT comment;
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
	ST_VALUE ta_lower_bound;
	ST_VALUE ta_upper_bound;
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

	hpchar package_name[MAX_STRING_LENGTH];
}ST_Import;

typedef struct _ST_Const
{

	ST_TYPE type;
	hpchar identifier[MAX_STRING_LENGTH];
	ST_VALUE val;
}ST_Const;

typedef struct _ST_UNIX_COMMENT
{

	hpuint32 len;
	hpchar text[MAX_COMMENT_LENGTH];
}ST_UNIX_COMMENT;

typedef struct _ST_ENUM_DEF
{

	hpchar identifier[MAX_STRING_LENGTH];
	ST_VALUE val;
	ST_UNIX_COMMENT_OR_NOT comment;
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

typedef enum _EN_DEFINITION_TYPE
{
	E_DT_IMPORT	= 0,
	E_DT_CONST = 1,
	E_DT_ENUM = 2,
	E_DT_STRUCT = 3,
	E_DT_UNION = 4,
	E_DT_TYPEDEF = 5,
	E_DT_UNIX_COMMENT = 6,
}EN_DEFINITION_TYPE;

typedef union _UN_DEFINITION
{
	ST_Import de_import;
	ST_Const de_const;
	ST_UNIX_COMMENT de_unix_comment;
	ST_ENUM de_enum;
	ST_STRUCT de_struct;
	ST_UNION de_union;
}UN_DEFINITION;

typedef struct _ST_DEFINITION
{
	EN_DEFINITION_TYPE type;
	UN_DEFINITION definition;
}ST_DEFINITION;

#endif//_H_syntactic_node_hd_types
