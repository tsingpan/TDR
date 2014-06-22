#include "platform/tlibc_platform.h"

#include "script/script.h"

#include "script/script_functions.h"
#include "script/c_functions.h"


#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "globals.h"
#include "parse/scanner.h"

#include <io.h>
lua_State* g_ls = NULL;

static int script_error(lua_State *L)
{
	scanner_error_halt(&g_yyloc, E_LS_SCRIPT_ERROR, lua_tostring(L, -1));
	return 1;
}

int script_init(const char* script)
{
	char path[TLIBC_MAX_PATH_LENGTH];
	

	if(access(script, 0) != 0)
	{
		char *tdr_root = getenv("TDR_ROOT");
		if(!tdr_root)
		{
			tdr_root = "/usr/local/tdr";			
		}

		snprintf(path, TLIBC_MAX_PATH_LENGTH, "%s%clua%c%s", tdr_root, TLIBC_FILE_SEPARATOR, TLIBC_FILE_SEPARATOR, script);
		if(access(path, 0) != 0)
		{
			fprintf(stderr, "File[%s] not exists , TDR_ROOT=%s\n", script, tdr_root);
			goto ERROR_RET;
		}
	}
	else
	{
		snprintf(path, TLIBC_MAX_PATH_LENGTH, script);
	}

	g_ls = luaL_newstate();
	if(g_ls == NULL)
	{
		fprintf(stderr, "luaL_newstate return NULL.\n");
		goto ERROR_RET;
	}
	luaL_openlibs(g_ls);
	if (luaL_dofile(g_ls, path))
	{
		fprintf(stderr, "%s\n", lua_tostring(g_ls, -1));
		goto ERROR_RET;
	}

	lua_atpanic(g_ls, script_error);

	cf_init(g_ls);
	return TRUE;
ERROR_RET:
	return FALSE;
}

void script_fini()
{
	lua_close(g_ls);
}
