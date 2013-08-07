#include "hotprotocol/hp_lua_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"

#include "lua.h"

#include <string.h>
#include <assert.h>

hpint32 lua_reader_init(HP_LUA_READER *self, lua_State *ls)
{
	memset(&self->super, 0, sizeof(HPAbstractReader));

	self->ls = ls;	

	self->super.read_struct_begin = lua_read_struct_begin;
	self->super.read_struct_end = lua_read_struct_end;
	self->super.read_vector_begin = lua_read_vector_begin;
	self->super.read_vector_end = lua_read_vector_end;
	self->super.read_field_begin = lua_read_field_begin;
	self->super.read_field_end = lua_read_field_end;
	self->super.read_enum_number = lua_read_enum;
	self->super.read_hpchar = lua_read_hpchar;
	self->super.read_hpdouble = lua_read_hpdouble;
	self->super.read_hpint8 = lua_read_hpint8;
	self->super.read_hpint16 = lua_read_hpint16;
	self->super.read_hpint32 = lua_read_hpint32;
	self->super.read_hpint64 = lua_read_hpint64;
	self->super.read_hpuint8 = lua_read_hpuint8;
	self->super.read_hpuint16 = lua_read_hpuint16;
	self->super.read_hpuint32 = lua_read_hpuint32;
	self->super.read_hpuint64 = lua_read_hpuint64;
	self->super.read_counter = lua_read_counter;

	return E_HP_NOERROR;
}

hpint32 lua_reader_fini(HP_LUA_READER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractReader, stack));


	return E_HP_NOERROR;
}


hpint32 lua_read_struct_begin(HPAbstractReader *super, const char *struct_name)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_struct_end(HPAbstractReader *super, const char *struct_name)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_vector_begin(HPAbstractReader *super)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_vector_end(HPAbstractReader *super)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_field_begin(HPAbstractReader *super, const char *var_name)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_field_end(HPAbstractReader *super, const char *var_name)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

HP_API hpint32 lua_read_enum(HPAbstractReader *super, hpint32 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

HP_API hpint32 lua_read_enum_name(HPAbstractReader *super, hpchar *enum_name, hpuint32 *enum_name_length)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_hpchar(HPAbstractReader *super, char *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	
	return E_HP_NOERROR;
}


hpint32 lua_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}


hpint32 lua_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}


hpint32 lua_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}


hpint32 lua_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}


hpint32 lua_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}



hpint32 lua_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}



hpint32 lua_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}


hpint32 lua_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}



hpint32 lua_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	HP_LUA_READER *self = HP_CONTAINER_OF(super, HP_LUA_READER, super);
	return E_HP_NOERROR;
}

hpint32 lua_read_counter(HPAbstractReader *super, const char *name, hpuint32 *val)
{
	lua_read_field_begin(super, name);
	lua_read_hpuint32(super, val);
	lua_read_field_end(super, name);
	return E_HP_NOERROR;
}
