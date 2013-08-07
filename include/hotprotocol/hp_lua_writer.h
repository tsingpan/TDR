#ifndef _H_HP_LUA_WRITER
#define _H_HP_LUA_WRITER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <stdio.h>

#include "lua.h"

typedef struct _HP_LUA_WRITER
{
	HPAbstractWriter super;
	lua_State *ls;
}HP_LUA_WRITER;


HP_API hpint32 lua_writer_init(HP_LUA_WRITER *self, lua_State *lua_state);

HP_API hpint32 lua_writer_fini(HP_LUA_WRITER *self);

//virtual functions
HP_API hpint32 lua_write_struct_begin(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 lua_write_struct_end(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 lua_write_vector_begin(HPAbstractWriter *self);

HP_API hpint32 lua_write_vector_end(HPAbstractWriter *self);

HP_API hpint32 lua_write_field_begin(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 lua_write_field_end(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 lua_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 lua_write_vector_item_end(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 lua_write_enum_number(HPAbstractWriter *super, const int val);

HP_API hpint32 lua_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name);

HP_API hpint32 lua_write_hpchar(HPAbstractWriter *super, const char val);

HP_API hpint32 lua_write_hpdouble(HPAbstractWriter *super, const double val);

HP_API hpint32 lua_write_hpint8(HPAbstractWriter *super, const hpint8 val);

HP_API hpint32 lua_write_hpint16(HPAbstractWriter *super, const hpint16 val);

HP_API hpint32 lua_write_hpint32(HPAbstractWriter *super, const hpint32 val);

HP_API hpint32 lua_write_hpint64(HPAbstractWriter *super, const hpint64 val);

HP_API hpint32 lua_write_hpuint8(HPAbstractWriter *super, const hpuint8 val);

HP_API hpint32 lua_write_hpuint16(HPAbstractWriter *super, const hpuint16 val);

HP_API hpint32 lua_write_hpuint32(HPAbstractWriter *super, const hpuint32 val);

HP_API hpint32 lua_write_hpuint64(HPAbstractWriter *super, const hpuint64 val);

HP_API hpint32 lua_write_bytes(HPAbstractWriter *super, const hpbytes bytes);
HP_API hpint32 lua_write_string(HPAbstractWriter *super, const hpchar* str);

HP_API hpint32 lua_write_hpbool(HPAbstractWriter *super, const hpbool val);

HP_API hpint32 lua_write_null(HPAbstractWriter *super);

HP_API hpint32 lua_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val);

#endif//_H_DDEKIT_LUA_ENCODING_WRITER
