#ifndef _H_SCRIPT_H
#define _H_SCRIPT_H

#include "lua.h"

extern lua_State* g_ls;

int script_init(const char* script);

void script_fini();

#endif//_H_SCRIPT_H
