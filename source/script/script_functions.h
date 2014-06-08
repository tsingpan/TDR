#ifndef _H_SCRIPT_FUNCTIONS_H
#define _H_SCRIPT_FUNCTIONS_H

#include "definition.h"

/*
* Lua提供给C调用的函数
*/

void sf_on_import(const char* file);

void sf_on_typedef(const char* type, const char* arg, const char* new_type);

void sf_on_const(const char* type, const syn_value_t *val);

#endif//_H_SCRIPT_FUNCTIONS_H
