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
	if(lua_isnil(g_ls, -1))
	{
		goto done;
	}

	lua_pushstring(g_ls, file);
	lua_call(g_ls, 1, 0);
done:
	return;
}

void sf_on_document_error()
{
	lua_getglobal(g_ls, "on_document_error");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_call(g_ls, 0, 0);
}

void sf_on_document_end()
{
	lua_getglobal(g_ls, "on_document_end");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_call(g_ls, 0, 0);
}

void sf_on_import(const char* file)
{
	lua_getglobal(g_ls, "on_import");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, file);
	lua_call(g_ls, 1, 0);
}

static void push_type(const syn_simple_type_t *st)
{
	lua_newtable(g_ls);
	lua_pushstring(g_ls, "type");	
	switch(st->st)
	{
	case E_ST_INT8:
		lua_pushstring(g_ls, "int8");
		break;
	case E_ST_INT16:
		lua_pushstring(g_ls, "int16");
		break;
	case E_ST_INT32:
		lua_pushstring(g_ls, "int32");
		break;
	case E_ST_INT64:
		lua_pushstring(g_ls, "int64");
		break;
	case E_ST_UINT8:
		lua_pushstring(g_ls, "uint8");
		break;
	case E_ST_UINT16:
		lua_pushstring(g_ls, "uint16");
		break;
	case E_ST_UINT32:
		lua_pushstring(g_ls, "uint32");
		break;
	case E_ST_UINT64:
		lua_pushstring(g_ls, "uint64");
		break;
	case E_ST_CHAR:
		lua_pushstring(g_ls, "char");
		break;
	case E_ST_BOOL:
		lua_pushstring(g_ls, "bool");
		break;
	case E_ST_DOUBLE:
		lua_pushstring(g_ls, "double");
		break;
	case E_ST_STRING:
		lua_pushstring(g_ls, "string");
		break;
	case E_ST_REFER:
		lua_pushstring(g_ls, "refer");//enum typedef struct union
		break;
	}
	lua_settable(g_ls,-3);

	if(st->st == E_ST_STRING)
	{
		lua_pushstring(g_ls, "length");
		if(st->string_length[0])
		{
			lua_pushstring(g_ls, st->string_length);//const enum
		}
		else
		{
			lua_pushnil(g_ls);
		}		
		lua_settable(g_ls,-3);
	}
	else if(st->st == E_ST_REFER)
	{
		lua_pushstring(g_ls, "refer");
		lua_pushstring(g_ls, st->st_refer);//enum typedef struct union
		lua_settable(g_ls,-3);
	}
}

static void push_value(const syn_value_t *val)
{
	lua_newtable(g_ls);
	lua_pushstring(g_ls, "type");
	switch(val->type)
	{
	case E_SNVT_REFER:			
		lua_pushstring(g_ls, "refer");
		break;
	case E_SNVT_CHAR:
		lua_pushstring(g_ls, "char");	
		break;
	case E_SNVT_BOOL:
		lua_pushstring(g_ls, "bool");
		break;
	case E_SNVT_DOUBLE:
		lua_pushstring(g_ls, "double");
		break;
	case E_SNVT_STRING:
		lua_pushstring(g_ls, "string");
		break;
	case E_SNVT_INT64:
		lua_pushstring(g_ls, "int64");
		break;
	case E_SNVT_UINT64:
		lua_pushstring(g_ls, "uint64");
		break;
	case E_SNVT_HEX_INT64:
		lua_pushstring(g_ls, "hex_int64");
		break;
	case E_SNVT_HEX_UINT64:
		lua_pushstring(g_ls, "hex_uint64");
		break;	
	}
	lua_settable(g_ls,-3);
	lua_pushstring(g_ls, "value");
	switch(val->type)
	{
	case E_SNVT_REFER:			
		lua_pushstring(g_ls, val->val.refer);
		break;
	case E_SNVT_CHAR:
		{
			char str[2];
			str[0] = val->val.c;
			str[1] = 0;
			lua_pushstring(g_ls, str);
		}		
		break;
	case E_SNVT_BOOL:
		lua_pushboolean(g_ls, val->val.b);
		break;
	case E_SNVT_DOUBLE:
		lua_pushnumber(g_ls, val->val.d);
		break;
	case E_SNVT_STRING:
		lua_pushstring(g_ls, val->val.str);
		break;
	case E_SNVT_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		break;
	case E_SNVT_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		break;
	case E_SNVT_HEX_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		break;
	case E_SNVT_HEX_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		break;	
	}
	lua_settable(g_ls,-3);
}

void sf_on_typedef(const syn_simple_type_t* type, const char* name)
{
	lua_getglobal(g_ls, "on_typedef");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	push_type(type);
	lua_pushstring(g_ls, name);
	lua_call(g_ls, 2, 0);
}

void sf_on_const(const syn_simple_type_t *type, const syn_value_t *val)
{
	lua_getglobal(g_ls, "on_const");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	push_type(type);
	push_value(val);	
	lua_call(g_ls, 2, 0);
}

void sf_on_enum_begin(const char* name)
{
	lua_getglobal(g_ls, "on_enum_begin");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, name);
	lua_call(g_ls, 1, 0);
}

void sf_on_enum_field(const enum_def_t* enum_def)
{
	lua_getglobal(g_ls, "on_enum_field");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, enum_def->identifier);
	push_value(&enum_def->val);
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
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_call(g_ls, 0, 0);
}

void sf_on_union_begin(const char* name, const char *etype)
{
	lua_getglobal(g_ls, "on_union_begin");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, name);
	lua_pushstring(g_ls, etype);
	lua_call(g_ls, 2, 0);
}

void sf_on_union_field(const char* key, const syn_simple_type_t *type, const char* name, const char *comment)
{
	lua_getglobal(g_ls, "on_union_field");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, key);
	push_type(type);
	lua_pushstring(g_ls, name);
	if(comment)
	{
		lua_pushstring(g_ls, comment);
	}
	else
	{
		lua_pushnil(g_ls);
	}
	lua_call(g_ls, 4, 0);
}

void sf_on_union_end()
{
	lua_getglobal(g_ls, "on_union_end");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_call(g_ls, 0, 0);
}

void sf_on_struct_begin(const char* name)
{
	lua_getglobal(g_ls, "on_struct_begin");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, name);
	lua_call(g_ls, 1, 0);
}

static void push_condition(const syn_condition_t *condition)
{
	if(condition->oper == E_EO_NON)
	{
		lua_pushnil(g_ls);
		goto done;
	}
	lua_newtable(g_ls);

	lua_pushstring(g_ls, "op0");
	lua_pushstring(g_ls, condition->op0);
	lua_settable(g_ls, -3);

	lua_pushstring(g_ls, "oper");
	switch (condition->oper)
	{
	case E_EO_AND:
		lua_pushstring(g_ls, "&");
		break;
	case E_EO_EQUAL:
		lua_pushstring(g_ls, "==");
		break;
	case E_EO_UNEQUAL:
		lua_pushstring(g_ls, "!=");
		break;
	case E_EO_BOOL:
		lua_pushnil(g_ls);
		break;
	default:
		assert(0);
	}
	lua_settable(g_ls, -3);

	lua_pushstring(g_ls, "op1");
	push_value(&condition->op1);
	lua_settable(g_ls, -3);

done:
	return;
}
void sf_on_struct_field(const syn_condition_t *condition, const syn_simple_type_t *type
						, const char *vec_size, const char* name, const char *comment)
{
	lua_getglobal(g_ls, "on_struct_field");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	if((condition != NULL) && (condition->oper != E_EO_NON))
	{
		push_condition(condition);
	}
	else
	{
		lua_pushnil(g_ls);
	}

	push_type(type);

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
	
	lua_call(g_ls, 5, 0);
}


void sf_on_struct_end()
{
	lua_getglobal(g_ls, "on_struct_end");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_call(g_ls, 0, 0);
}

void sf_on_comment(const char *comment)
{
	lua_getglobal(g_ls, "on_comment");
	if(lua_isnil(g_ls, -1))
	{
		return;
	}

	lua_pushstring(g_ls, comment);
	lua_call(g_ls, 1, 0);
}
