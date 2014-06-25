#include "script/script_functions.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "script/script.h"

#include "definition.h"

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

void sf_on_const(const char* type, const syn_value_t *val)
{
	lua_getglobal(g_ls, "on_const");
	lua_pushstring(g_ls, type);
	switch(val->type)
	{
	case E_SNVT_IDENTIFIER:
		lua_pushstring(g_ls, val->val.identifier);
		lua_pushnil(g_ls);
		break;
	case E_SNVT_CHAR:
		{
			char str[2];
			str[0] = val->val.c;
			str[1] = 0;
			lua_pushstring(g_ls, str);
			lua_pushnil(g_ls);
		}
		break;
	case E_SNVT_BOOL:
		lua_pushboolean(g_ls, val->val.b);
		lua_pushnil(g_ls);
		break;
	case E_SNVT_DOUBLE:
		lua_pushnumber(g_ls, val->val.d);
		lua_pushnil(g_ls);
		break;
	case E_SNVT_STRING:
		lua_pushstring(g_ls, val->val.str);
		lua_pushnil(g_ls);
		break;
	case E_SNVT_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		lua_pushinteger(g_ls, 10);
		break;
	case E_SNVT_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		lua_pushinteger(g_ls, 10);
		break;
	case E_SNVT_HEX_INT64:
		lua_pushinteger(g_ls, val->val.i64);
		lua_pushinteger(g_ls, 16);
		break;
	case E_SNVT_HEX_UINT64:
		lua_pushunsigned(g_ls, val->val.ui64);
		lua_pushinteger(g_ls, 16);
		break;	
	}
	
	lua_call(g_ls, 3, 0);
}
