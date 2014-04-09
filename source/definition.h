#ifndef _H_DEFINITION
#define _H_DEFINITION

#include "platform/tlibc_platform.h"
#include <stdint.h>


#define MAX_COMMENT_LENGTH 1024
typedef struct syn_unix_comment_s
{
    char text[MAX_COMMENT_LENGTH];
}syn_unix_comment_t;


typedef enum syn_value_type_e
{
    E_SNVT_IDENTIFIER = 0,
    E_SNVT_CHAR = 1,
    E_SNVT_DOUBLE = 2,
    E_SNVT_STRING = 3,
    E_SNVT_INT64 = 4,
    E_SNVT_UINT64 = 5,
    E_SNVT_HEX_INT64 = 6,
    E_SNVT_HEX_UINT64 = 7,
}syn_value_type_t;


typedef union syn_value_body_u
{
    int64_t i64;
    uint64_t ui64;
    char str[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
    double d;
    char c;
    char identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];//const, enumdef
}syn_value_body_t;

typedef struct  syn_value_s
{
    syn_value_type_t type;
    syn_value_body_t val;
}syn_value_t;

typedef enum syn_type_type_e
{
	E_SNT_SIMPLE = 0,
	E_SNT_CONTAINER = 1,
}syn_type_type_t;

typedef enum syn_simple_type_type_e
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
	E_ST_REFER = 11,//struct, union, enum, typedef
}syn_simple_type_type_t;
typedef struct syn_simple_type_s
{
	syn_simple_type_type_t st;
	
	char string_length[TLIBC_MAX_LENGTH_OF_IDENTIFIER];

	char st_refer[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}syn_simple_type_t;

typedef enum syn_container_type_type_e
{
	E_CT_VECTOR = 0,
}syn_container_type_type_t;
typedef struct syn_container_type_s
{
	syn_container_type_type_t ct;

	syn_simple_type_t vector_type;
	char vector_length[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}syn_container_type_t;

typedef struct  syn_type_s
{
    syn_type_type_t type;
    
	syn_simple_type_t st;

	syn_container_type_t ct;	
}syn_type_t;

typedef struct  syn_parameter_s
{
    syn_simple_type_t type;
    char identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}syn_parameter_t;
#define MAX_PARAMETER_NUM 1
typedef struct  syn_parameters_s
{
	uint32_t par_list_num;
    syn_parameter_t par_list[MAX_PARAMETER_NUM];
}syn_parameters_t;
#define MAX_ARGUMENT_NUM 1
typedef struct  syn_arguments_s
{
	uint32_t arg_list_num;
    char arg_list[MAX_ARGUMENT_NUM][TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}syn_arguments_t;
typedef enum syn_expression_oper_e
{
	E_EO_NON = 0,
    E_EO_AND = 2,
    E_EO_EQUAL = 3,
	E_EO_UNEQUAL = 4,
}syn_expression_oper_t;
typedef struct  syn_condition_s
{
	syn_expression_oper_t oper;
	char op0[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	syn_value_t op1;
}syn_condition_t;
typedef struct  syn_field_s
{
    syn_condition_t condition;
    syn_type_t type;    
    char identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	syn_arguments_t args;
    syn_unix_comment_t comment;
}syn_field_t;


typedef struct  syn_union_field_s
{
	char key[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	syn_simple_type_t simple_type;
	char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	syn_unix_comment_t comment;
}syn_union_field_t;
#define MAX_UNION_FIELD_LIST_NUM 65536
typedef struct  syn_union_field_list_s
{
	uint32_t union_field_list_num;
	syn_union_field_t union_field_list[MAX_UNION_FIELD_LIST_NUM];
}syn_union_field_list_t;


#define MAX_PACKAGE_NAME_LENGTH 1024
typedef struct  syn_import_s
{
    char package_name[MAX_PACKAGE_NAME_LENGTH];
}syn_import_t;
typedef struct  syn_const_s
{
    syn_simple_type_t type;
    char identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
    syn_value_t val;
}syn_const_t;
typedef struct  enum_def_s
{
    char identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
    syn_value_t val;
    syn_unix_comment_t comment;
}enum_def_t;
#define MAX_ENUM_DEF_LIST_NUM 65536
typedef struct  syn_enum_s
{
    char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	uint32_t enum_def_list_num;
    enum_def_t enum_def_list[MAX_ENUM_DEF_LIST_NUM];
}syn_enum_t;
#define MAX_FIELD_LIST_NUM 65536
typedef struct  syn_field_list_s
{
	uint32_t field_list_num;
    syn_field_t field_list[MAX_FIELD_LIST_NUM];
}syn_field_list_t;
typedef struct  syn_struct_s
{
    char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
    syn_field_list_t field_list;
}syn_struct_t;
typedef struct  syn_union_s
{
    char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
    syn_parameters_t parameters;
    syn_union_field_list_t union_field_list;
}syn_union_t;
typedef struct  syn_typedef_s
{
    syn_simple_type_t type;
    char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}syn_typedef_t;
typedef enum syn_definition_type_e
{
    E_DT_IMPORT = 0,
    E_DT_CONST = 1,
    E_DT_ENUM = 2,
    E_DT_STRUCT = 3,
    E_DT_UNION = 4,
    E_DT_TYPEDEF = 5,
    E_DT_UNIX_COMMENT = 6,
}syn_definition_type_t;
typedef union  syn_definition_body_u
{
    syn_import_t de_import;
    syn_const_t de_const;
    syn_unix_comment_t de_unix_comment;
    syn_enum_t de_enum;
    syn_struct_t de_struct;
    syn_union_t de_union;
    syn_typedef_t de_typedef;
}syn_definition_body_t;
typedef struct  syn_definition_s
{
    syn_definition_type_t type;
    syn_definition_body_t definition;
}syn_definition_t;
#endif//_H_DEFINITION
