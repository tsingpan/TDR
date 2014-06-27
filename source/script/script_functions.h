#ifndef _H_SCRIPT_FUNCTIONS_H
#define _H_SCRIPT_FUNCTIONS_H

#include "definition.h"

/*
* Lua提供给C调用的函数
*/

void sf_on_document_begin(const char* file);

void sf_on_document_error();

void sf_on_document_end();

void sf_on_import(const char* file);

void sf_on_typedef(const syn_simple_type_t* type, const char* name);

void sf_on_const(const syn_simple_type_t *type, const syn_value_t *val);

void sf_on_enum_begin(const char* name);

void sf_on_enum_field(const enum_def_t* enum_def);

void sf_on_enum_end();

void sf_on_union_begin(const char* name, const char *etype);

void sf_on_union_field(const char* key, const syn_simple_type_t *type, const char* name, const char *comment);

void sf_on_union_end();

void sf_on_struct_begin(const char* name);

void sf_on_struct_field(const syn_condition_t *condition, const syn_simple_type_t *type
							   , const char *vec_size, const char* name, const char *comment);

void sf_on_struct_end();

void sf_on_comment(const char *comment);

#endif//_H_SCRIPT_FUNCTIONS_H
