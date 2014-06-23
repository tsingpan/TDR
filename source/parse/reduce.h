#ifndef _H_REDUCE
#define _H_REDUCE


#include "platform/tdr_platform.h"

#include "parse/parser.h"

void reduce_Import(syn_import_t* current, const char* str);


void reduce_Typedef(syn_typedef_t *current, syn_simple_type_t* type, const char *tok_identifier);

void reduce_Const(syn_const_t* current, const syn_simple_type_t *type, const char *identifier, const syn_value_t *val);

void reduce_Enum(syn_enum_t *current, const char *identifier);

void reduce_EnumDef_Value(enum_def_t *current, const char *identifier, const syn_value_t *st_value, const syn_unix_comment_t *comment);

void reduce_EnumDef(enum_def_t *current, const char *identifier, const syn_value_t *st_last_value, const syn_unix_comment_t *comment);

void reduce_ContainerType_tok_t_vector(syn_type_t *current, const syn_simple_type_t *simple_type, const char *tok_identifier);

void reduce_SimpleType_tok_t_string(syn_simple_type_t *current, const char *tok_identifier);

void reduce_SimpleType_tok_identifier(syn_simple_type_t* current, const char *tok_identifier);

void reduce_Type_SimpleType(syn_type_t *current, const syn_simple_type_t *simple_type);

void reduce_Value_tok_identifier(syn_value_t* current, const char *sn_identifier);

void reduce_Value_tok_char(syn_value_t* current, const char pn_char);

void reduce_Value_tok_int64(syn_value_t* current, const int64_t i64);

void reduce_Value_tok_hex_int64(syn_value_t* current, const int64_t i64);

void reduce_Value_tok_uint64(syn_value_t* current, const uint64_t ui64);

void reduce_Value_tok_hex_uint64(syn_value_t* current, const uint64_t ui64);

void reduce_Value_tok_double(syn_value_t* current, const double d);

void reduce_Value_tok_string(syn_value_t* current, const char* str);

void reduce_ArgumentList_tok_identifier(syn_arguments_t* current, const char *identifier);

void reduce_ArgumentList_ArgumentList_tok_identifier(syn_arguments_t* current, const syn_arguments_t* argument_list, const char *identifier);

void reduce_Value_tok_count(const symbols_t *symbols, syn_value_t *current, const char *identifier);

void reduce_Union(syn_union_t *current, const char *identifier, const syn_parameters_t *parameters);

void reduce_UnionField(syn_union_field_t *current, const char *key, const syn_simple_type_t *simple_type, const char *identifier, const syn_unix_comment_t *comment);

void reduce_Field(syn_field_t *current, const syn_condition_t *condition, const syn_type_t *type
					 , const char *identifier, const syn_arguments_t *args, const syn_unix_comment_t *comment);

#endif //_H_REDUCE
