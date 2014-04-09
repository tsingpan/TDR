#ifndef _H_REDUCE
#define _H_REDUCE


#include "platform/tlibc_platform.h"

#include "parse/parser.h"

void reduce_Import(ST_Import* current, const char* str);


void reduce_Typedef(ST_TYPEDEF *current, syn_simple_type_t* type, const char *tok_identifier);

void reduce_Const(ST_Const* current, const syn_simple_type_t *type, const char *identifier, const syn_value_t *val);

void reduce_Enum(ST_ENUM *current, const char *identifier);

void reduce_EnumDef_Value(ST_ENUM_DEF *current, const char *identifier, const syn_value_t *st_value, const syn_unix_comment_t *comment);

void reduce_EnumDef(ST_ENUM_DEF *current, const char *identifier, const syn_value_t *st_last_value, const syn_unix_comment_t *comment);

void reduce_ContainerType_tok_t_vector(ST_TYPE *current, const syn_simple_type_t *simple_type, const char *tok_identifier);

void reduce_SimpleType_tok_t_string(syn_simple_type_t *current, const char *tok_identifier);

void reduce_SimpleType_tok_identifier(syn_simple_type_t* current, const char *tok_identifier);

void reduce_Type_SimpleType(ST_TYPE *current, const syn_simple_type_t *simple_type);

void reduce_Value_tok_identifier(syn_value_t* current, const char *sn_identifier);

void reduce_Value_tok_char(syn_value_t* current, const char pn_char);

void reduce_Value_tok_int64(syn_value_t* current, const int64_t i64);

void reduce_Value_tok_hex_int64(syn_value_t* current, const int64_t i64);

void reduce_Value_tok_uint64(syn_value_t* current, const uint64_t ui64);

void reduce_Value_tok_hex_uint64(syn_value_t* current, const uint64_t ui64);

void reduce_Value_tok_double(syn_value_t* current, const double d);

void reduce_Value_tok_string(syn_value_t* current, const char* str);

void reduce_ArgumentList_tok_identifier(ST_ARGUMENTS* current, const char *identifier);

void reduce_ArgumentList_ArgumentList_tok_identifier(ST_ARGUMENTS* current, const ST_ARGUMENTS* argument_list, const char *identifier);

void reduce_Value_tok_count(const symbols_t *symbols, syn_value_t *current, const char *identifier);

void reduce_Union(ST_UNION *current, const char *identifier, const ST_Parameters *parameters);

void reduce_UnionField(ST_UNION_FIELD *current, const char *key, const syn_simple_type_t *simple_type, const char *identifier, const syn_unix_comment_t *comment);

void reduce_Field(ST_FIELD *current, const ST_CONDITION *condition, const ST_TYPE *type
					 , const char *identifier, const ST_ARGUMENTS *args, const syn_unix_comment_t *comment);

#endif //_H_REDUCE
