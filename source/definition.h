#ifndef _H_DEFINITION
#define _H_DEFINITION

#include "platform/tlibc_platform.h"
#include "tdata/tdata_types.h"


#define TD_MAX_COMMENT_LENGTH 1024
typedef struct  _ST_UNIX_COMMENT
{
    tchar text[TD_MAX_COMMENT_LENGTH];
}ST_UNIX_COMMENT;


typedef enum _SN_VALUE_TYPE
{
    E_SNVT_IDENTIFIER = 0,
    E_SNVT_CHAR = 1,
    E_SNVT_DOUBLE = 2,
    E_SNVT_STRING = 3,
    E_SNVT_INT64 = 4,
    E_SNVT_UINT64 = 5,
    E_SNVT_HEX_INT64 = 6,
    E_SNVT_HEX_UINT64 = 7,
}SN_VALUE_TYPE;
typedef union  _UN_VALUE
{
    tint64 i64;
    tuint64 ui64;
    tchar str[TLIBC_MAX_IDENTIFIER_LENGTH];
    tdouble d;
    tchar c;
    tchar identifier[TLIBC_MAX_IDENTIFIER_LENGTH];//const, enumdef
}UN_VALUE;
typedef struct  _ST_VALUE
{
    SN_VALUE_TYPE type;
    UN_VALUE val;
}ST_VALUE;

typedef enum _SN_SIMPLE_TYPE
{
    E_ST_INT8 = 0,
    E_ST_INT16 = 1,
    E_ST_INT32 = 2,
    E_ST_INT64 = 3,
    E_ST_UINT8 = 4,
    E_ST_UINT16 = 5,
    E_ST_UINT32 = 6,
    E_ST_UINT64 = 7,    
    E_ST_CHAR = 8,
    E_ST_DOUBLE = 9,
	E_ST_STRING = 10,
	E_ST_REFER = 11,//struct, enum, typedef
}SN_SIMPLE_TYPE;
typedef enum _SN_TYPE
{
    E_SNT_SIMPLE = 0,
    E_SNT_CONTAINER = 1,
}SN_TYPE;
typedef struct _ST_SIMPLE_TYPE
{
	SN_SIMPLE_TYPE st;
	
	tchar string_length[TLIBC_MAX_IDENTIFIER_LENGTH];

	tchar st_refer[TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_SIMPLE_TYPE;

typedef enum _SN_CONTAINER_TYPE
{
	E_CT_VECTOR = 0,
}SN_CONTAINER_TYPE;
typedef struct _ST_CONTAINER_TYPE
{
	SN_CONTAINER_TYPE ct;

	ST_SIMPLE_TYPE vector_type;
	tchar vector_length[TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_CONTAINER_TYPE;

typedef struct  _ST_TYPE
{
    SN_TYPE type;
    
	ST_SIMPLE_TYPE st;

	ST_CONTAINER_TYPE ct;	
}ST_TYPE;
typedef struct  _ST_Parameter
{
    ST_SIMPLE_TYPE type;
    tchar identifier[TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_Parameter;
#define MAX_PARAMETER_NUM 16
typedef struct  _ST_Parameters
{
	tuint32 par_list_num;
    ST_Parameter par_list[MAX_PARAMETER_NUM];
}ST_Parameters;
#define MAX_ARGUMENT_NUM 16
typedef struct  _ST_ARGUMENTS
{
	tuint32 arg_list_num;
    tchar arg_list[MAX_ARGUMENT_NUM][TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_ARGUMENTS;
typedef enum _ST_EXPRESSION_OPER
{
	E_EO_NON = 0,
    E_EO_AND = 2,
    E_EO_EQUAL = 3,
	E_EO_UNEQUAL = 4,
}ST_EXPRESSION_OPER;
typedef struct  _ST_CONDITION
{
	ST_EXPRESSION_OPER oper;
	ST_VALUE op0;
	ST_VALUE op1;
}ST_CONDITION;
typedef struct  _ST_FIELD
{
    ST_CONDITION condition;
    ST_TYPE type;    
    tchar identifier[TLIBC_MAX_IDENTIFIER_LENGTH];
	ST_ARGUMENTS args;
    ST_UNIX_COMMENT comment;
}ST_FIELD;


typedef struct  _ST_UNION_FIELD
{
	tchar key[TLIBC_MAX_IDENTIFIER_LENGTH];
	ST_SIMPLE_TYPE simple_type;
	tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
	ST_UNIX_COMMENT comment;
}ST_UNION_FIELD;
#define MAX_UNION_FIELD_LIST_NUM 65536
typedef struct  _ST_UNION_FIELD_LIST
{
	tuint32 union_field_list_num;
	ST_UNION_FIELD union_field_list[MAX_UNION_FIELD_LIST_NUM];
}ST_UNION_FIELD_LIST;


#define MAX_PACKAGE_NAME_LENGTH 1024
typedef struct  _ST_Import
{
    tchar package_name[MAX_PACKAGE_NAME_LENGTH];
}ST_Import;
typedef struct  _ST_Const
{
    ST_SIMPLE_TYPE type;
    tchar identifier[TLIBC_MAX_IDENTIFIER_LENGTH];
    ST_VALUE val;
}ST_Const;
typedef struct  _ST_ENUM_DEF
{
    tchar identifier[TLIBC_MAX_IDENTIFIER_LENGTH];
    ST_VALUE val;
    ST_UNIX_COMMENT comment;
}ST_ENUM_DEF;
#define MAX_ENUM_DEF_LIST_NUM 65536
typedef struct  _ST_ENUM
{
    tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
	tuint32 enum_def_list_num;
    ST_ENUM_DEF enum_def_list[MAX_ENUM_DEF_LIST_NUM];
}ST_ENUM;
#define MAX_FIELD_LIST_NUM 65536
typedef struct  _ST_FIELD_LIST
{
	tuint32 field_list_num;
    ST_FIELD field_list[MAX_FIELD_LIST_NUM];
}ST_FIELD_LIST;
typedef struct  _ST_STRUCT
{
    tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
    ST_FIELD_LIST field_list;
}ST_STRUCT;
typedef struct  _ST_UNION
{
    tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
    ST_Parameters parameters;
    ST_UNION_FIELD_LIST union_field_list;
}ST_UNION;
typedef struct  _ST_TYPEDEF
{
    ST_SIMPLE_TYPE type;
    tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_TYPEDEF;
typedef enum _EN_DEFINITION_TYPE
{
    E_DT_IMPORT = 0,
    E_DT_CONST = 1,
    E_DT_ENUM = 2,
    E_DT_STRUCT = 3,
    E_DT_UNION = 4,
    E_DT_TYPEDEF = 5,
    E_DT_UNIX_COMMENT = 6,
}EN_DEFINITION_TYPE;
typedef union  _UN_DEFINITION
{
    ST_Import de_import;
    ST_Const de_const;
    ST_UNIX_COMMENT de_unix_comment;
    ST_ENUM de_enum;
    ST_STRUCT de_struct;
    ST_UNION de_union;
    ST_TYPEDEF de_typedef;
}UN_DEFINITION;
typedef struct  _ST_DEFINITION
{
    EN_DEFINITION_TYPE type;
    UN_DEFINITION definition;
}ST_DEFINITION;
#endif//_H_DEFINITION
