#include "script/c_functions.h"
#include "script/script.h"
#include <stdlib.h>
#include "globals.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "parse/parser.h"



void cf_init(lua_State *ls)
{
	lua_register(ls, "error_halt", cf_error_halt);
	lua_register(ls, "make_rule", cf_make_rule);
}

int cf_error_halt(lua_State *ls)
{
	const char* error = luaL_checkstring(ls,1);
	scanner_error(&g_yyloc, E_LS_SCRIPT_HALT, error);
	exit(E_LS_SCRIPT_HALT);
}


int cf_make_rule(lua_State *ls)
{
	const char* target = luaL_checkstring(ls,1);
	parser_make_rule(&g_parser, target);
	return 0;
}