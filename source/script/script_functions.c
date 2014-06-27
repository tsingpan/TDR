#include "script/script_functions.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "script/script.h"
#include "definition.h"

#include <assert.h>

void sf_on_document_begin(const char* file)
{
	lua_getglobal(g_ls, "on_document_begin");
	lua_pushstring(g_ls, file);
	lua_call(g_ls, 1, 0);
}

void sf_on_document_error()
{
	lua_getglobal(g_ls, "on_document_error");
	lua_call(g_ls, 0, 0);
}

void sf_on_document_end()
{
	lua_getglobal(g_ls, "on_document_end");
	lua_call(g_ls, 0, 0);
}

void sf_on_import(const char* file)
{
	lua_getglobal(g_ls, "on_import");
	lua_pushstring(g_ls, file);
	lua_call(g_ls, 1, 0);
}

void sf_on_typedef(const char* type, const char* arg, const char* new_type)
{
	lua_getglobal(g_ls, "on_typedef");
	lua_pushstring(g_ls, type);
	lua_pushstring(g_ls, arg);
	lua_pushstring(g_ls, new_type);
	lua_call(g_ls, 3, 0);
}

static void push_value(const syn_value_t *val)
{
	switch(val->type)
	{
	case E_SNVT_IDENTIFIER:
		lua_pushstring(g_ls, val->val.identifier);
		lua_pushstring(g_ls, "identifier");
		break;
	case E_SNVT_CHAR:
		{
			char str[2];
			str[0] = val->val.c;
			str[1] = 0;
			lua_pushstring(g_ls, str);
			lua_pushstring(g_ls, "char");	
		}
		break;
	case E_SNVT_BOOL:
		lua_pushboolean(g_ls, val->val.b);
		lua_pushstring(g_ls, "bool");
		break;
	case E_SNVT_DOUBLE:
		lua_pushnumber(g_ls, val->val.d);
		lua_pushstring(g_ls, "f");
		break;
	case E_SNVT_STRING:
		lua_pushstring(g_ls, val->val.str);
		lua_pushstring(g_ls, "string");
		break;
	case E_SNVT_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		lua_pushstring(g_ls, "d");
		break;
	case E_SNVT_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		lua_pushstring(g_ls, "u");
		break;
	case E_SNVT_HEX_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		lua_pushstring(g_ls, "X");
		break;
	case E_SNVT_HEX_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		lua_pushstring(g_ls, "X");		
		break;	
	}
}

void sf_on_const(const char* type, const char* real_type, const syn_value_t *val)
{
	lua_getglobal(g_ls, "on_const");
	lua_pushstring(g_ls, type);
	lua_pushstring(g_ls, real_type);
	push_value(val);
	
	lua_call(g_ls, 4, 0);
}

void sf_on_enum_begin(const char* name)
{
	lua_getglobal(g_ls, "on_enum_begin");
	lua_pushstring(g_ls, name);
	lua_call(g_ls, 1, 0);
}

void sf_on_enum_field(const enum_def_t* enum_def)
{
	lua_getglobal(g_ls, "on_enum_field");
	lua_pushstring(g_ls, enum_def->identifier);
	assert((enum_def->val.type == E_SNVT_INT64) || (enum_def->val.type == E_SNVT_HEX_INT64));
	lua_pushinteger(g_ls, enum_def->val.val.i64);
	if(enum_def->comment.text[0])
	{
		lua_pushstring(g_ls, enum_def->comment.text);
	}
	else
	{
		lua_pushnil(g_ls);
	}
	
	lua_call(g_ls, 3, 0);
}

void sf_on_enum_end()
{
	lua_getglobal(g_ls, "on_enum_end");
	lua_call(g_ls, 0, 0);
}

void sf_on_union_begin(const char* name, const char *etype)
{
	lua_getglobal(g_ls, "on_union_begin");
	lua_pushstring(g_ls, name);
	lua_pushstring(g_ls, etype);
	lua_call(g_ls, 2, 0);
}

void sf_on_union_field(const char* key, const char* type
					   , const char* type_arg, const char* real_type, const char* real_type_arg
					   , const char* name, const char *comment)
{
	lua_getglobal(g_ls, "on_union_field");
	lua_pushstring(g_ls, key);
	lua_pushstring(g_ls, type);
	if(type_arg)
	{
		lua_pushstring(g_ls, type_arg);
	}
	else
	{
		lua_pushnil(g_ls);
	}

	lua_pushstring(g_ls, real_type);
	if(real_type_arg)
	{
		lua_pushstring(g_ls, real_type_arg);
	}
	else
	{
		lua_pushnil(g_ls);
	}	
	lua_pushstring(g_ls, name);
	if(comment)
	{
		lua_pushstring(g_ls, comment);
	}
	else
	{
		lua_pushnil(g_ls);
	}
	lua_call(g_ls, 7, 0);
}

void sf_on_union_end()
{
	lua_getglobal(g_ls, "on_union_end");
	lua_call(g_ls, 0, 0);
}

void sf_on_struct_begin(const char* name)
{
	lua_getglobal(g_ls, "on_struct_begin");
	lua_pushstring(g_ls, name);
	lua_call(g_ls, 1, 0);
}

void sf_on_struct_vector_field(const char* op, const char* op0, const syn_value_t* op1
							   , const char *type, const char *type_arg
							   , const char *real_type, const char *real_type_arg
							   , const char *vec_size, const char* name, const char *comment)
{
	lua_getglobal(g_ls, "on_struct_field");
	if(op != NULL)
	{
		lua_pushstring(g_ls, op);
		lua_pushstring(g_ls, op0);
		push_value(op1);
	}
	else
	{
		lua_pushnil(g_ls);
		lua_pushnil(g_ls);
		lua_pushnil(g_ls);
		lua_pushnil(g_ls);
	}
	lua_pushstring(g_ls, type);
	if(type_arg != NULL)
	{
		lua_pushstring(g_ls, type_arg);
	}
	else
	{
		lua_pushnil(g_ls);
	}

	lua_pushstring(g_ls, real_type);
	if(real_type_arg != NULL)
	{
		lua_pushstring(g_ls, real_type_arg);
	}
	else
	{
		lua_pushnil(g_ls);
	}

	if(vec_size)
	{
		lua_pushstring(g_ls, vec_size);
	}
	else
	{
		lua_pushnil(g_ls);
	}
	
	lua_pushstring(g_ls, name);
	if(comment)
	{
		lua_pushstring(g_ls, comment);
	}
	else
	{
		lua_pushnil(g_ls);
	}
	
	lua_call(g_ls, 11, 0);
}


void sf_on_struct_end()
{
	lua_getglobal(g_ls, "on_struct_end");
	lua_call(g_ls, 0, 0);
}

void sf_on_comment(const char *comment)
{
	lua_getglobal(g_ls, "on_comment");
	lua_pushstring(g_ls, comment);
	lua_call(g_ls, 1, 0);
}
