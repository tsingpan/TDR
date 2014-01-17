#ifndef _H_REDUCE
#define _H_REDUCE


#include "tlibc/platform/tlibc_platform.h"

#include "parse/parser.h"

void reduce_Import(ST_Import* current, const char* str);


void reduce_Typedef(ST_TYPEDEF *current, ST_SIMPLE_TYPE* type, const tchar *tok_identifier);

void reduce_Const(ST_Const* current, const ST_SIMPLE_TYPE *type, const tchar *identifier, const ST_VALUE *val);

void reduce_Enum(ST_ENUM *current, const tchar *identifier);

void reduce_EnumDef_Value(ST_ENUM_DEF *current, const tchar *identifier, const ST_VALUE *st_value, const ST_UNIX_COMMENT *comment);

void reduce_EnumDef(ST_ENUM_DEF *current, const tchar *identifier, const ST_VALUE *st_last_value, const ST_UNIX_COMMENT *comment);

void reduce_ContainerType_tok_t_vector(ST_TYPE *current, const ST_SIMPLE_TYPE *simple_type, const tchar *tok_identifier);

void reduce_SimpleType_tok_t_string(ST_SIMPLE_TYPE *current, const tchar *tok_identifier);

void reduce_SimpleType_tok_identifier(ST_SIMPLE_TYPE* current, const tchar *tok_identifier);

void reduce_Type_SimpleType(ST_TYPE *current, const ST_SIMPLE_TYPE *simple_type);

void reduce_Value_tok_identifier(ST_VALUE* current, const tchar *sn_identifier);

void reduce_Value_tok_char(ST_VALUE* current, const tchar pn_char);

void reduce_Value_tok_int64(ST_VALUE* current, const tint64 i64);

void reduce_Value_tok_hex_int64(ST_VALUE* current, const tint64 i64);

void reduce_Value_tok_uint64(ST_VALUE* current, const tuint64 ui64);

void reduce_Value_tok_hex_uint64(ST_VALUE* current, const tuint64 ui64);

void reduce_Value_tok_double(ST_VALUE* current, const double d);

void reduce_Value_tok_string(ST_VALUE* current, const char* str);

void reduce_ArgumentList_tok_identifier(ST_ARGUMENTS* current, const tchar *identifier);

void reduce_ArgumentList_ArgumentList_tok_identifier(ST_ARGUMENTS* current, const ST_ARGUMENTS* argument_list, const tchar *identifier);

void reduce_Value_tok_count(const SYMBOLS *symbols, ST_VALUE *current, const tchar *identifier);

void reduce_Union(ST_UNION *current, const tchar *identifier, const ST_Parameters *parameters);

void reduce_UnionField(ST_UNION_FIELD *current, const tchar *key, const ST_SIMPLE_TYPE *simple_type, const tchar *identifier, const ST_UNIX_COMMENT *comment);

void reduce_Field(ST_FIELD *current, const ST_CONDITION *condition, const ST_TYPE *type
					 , const tchar *identifier, const ST_ARGUMENTS *args, const ST_UNIX_COMMENT *comment);

#endif //_H_REDUCE
