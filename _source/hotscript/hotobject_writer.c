#include "hotscript/hotobject_writer.h"
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "hotscript/hotobject.h"
#include "datrie/trie.h"

#include <string.h>

static HotObject *hotobject_get(HotObjectWriter *self)
{
	return self->stack[self->stack_num - 1].ho;
}

static hpint32 hotobject_push(HotObjectWriter *self, HotObject *ho)
{
	self->stack[self->stack_num].ho = ho;
	++(self->stack_num);

	return E_HP_NOERROR;
}

hpint32 hotobject_write_field_begin(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	HotObject *new_ob = hotobject_new();
	char name[1024];
	hpuint32 i;
	for(i = 0; i < len; ++i)
	{
		name[i] = var_name[i];
	}
	name[i] = 0;

	hotobject_push(self, new_ob);
	if(!trie_store(ob->keys, name, new_ob))
	{
		goto ERROR_RET;
	}

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotobject_write_field_end(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	--(self->stack_num);
	return E_HP_NOERROR;
}


hpint32 hotobject_write_vector_begin(HPAbstractWriter *super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var.type = E_HP_VECTOR;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_vector_end(HPAbstractWriter *super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);

	return E_HP_NOERROR;
}

static hpint32 hotobject_write_double(HPAbstractWriter *super, const hpdouble val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var.type = E_HP_DOUBLE;
	ob->var.val.d = val;
	return E_HP_NOERROR;
}

static hpint32 hotobject_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var.type = E_HP_INT64;
	ob->var.val.i64 = val;
	return E_HP_NOERROR;
}

hpint32 hotobject_write_bytes(HPAbstractWriter *super, const hpbytes bytes)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var.type = E_HP_BYTES;
	ob->var.val.bytes.ptr = (char*)malloc(bytes.len);
	memcpy(ob->var.val.bytes.ptr, bytes.ptr, bytes.len);
	ob->var.val.bytes.len = bytes.len;
	return E_HP_NOERROR;
}

hpint32 hotobject_write_hpstring(HPAbstractWriter *super, const hpstring str)
{
	hpbytes bytes;
	bytes.ptr = str;
	bytes.len = strlen(str);
	return hotobject_write_bytes(super, bytes);
}

hpint32 hotobject_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var.type = E_HP_BOOL;
	ob->var.val.b = val;
	return E_HP_NOERROR;
}

hpint32 hotobject_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index)
{
	char str[1024];
	hpuint32 str_len = 0;
	hpint32 count;

	count = index;
	do
	{
		str[str_len++] = '0' + count % 10;
		count/=10;
	}while(count > 0);

	return hotobject_write_field_begin(super, str, str_len);
}

hpint32 hotobject_write_vector_item_end(HPAbstractWriter *super, hpuint32 index)
{
	char str[1024];
	hpuint32 str_len = 0;
	hpint32 count;

	count = index;
	do
	{
		str[str_len++] = '0' + count % 10;
		count/=10;
	}while(count > 0);

	return hotobject_write_field_end(super, str, str_len);
}

hpint32 hotobject_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	return E_HP_NOERROR;
}

hpint32 hotobject_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	return E_HP_NOERROR;
}

hpint32 hotobject_writer_init(HotObjectWriter* self, HotObject *hotobject)
{
	memset(self, 0, sizeof(HotObjectWriter));
	self->stack_num = 0;
	self->stack[self->stack_num].ho = hotobject;
	++(self->stack_num);

	self->super.write_struct_begin = hotobject_write_struct_begin;
	self->super.write_struct_end = hotobject_write_struct_end;
	self->super.write_vector_begin = hotobject_write_vector_begin;
	self->super.write_vector_end = hotobject_write_vector_end;
	self->super.write_field_begin = hotobject_write_field_begin;
	self->super.write_field_end = hotobject_write_field_end;
	self->super.write_vector_item_begin = hotobject_write_vector_item_begin;
	self->super.write_vector_item_end = hotobject_write_vector_item_end;
	self->super.write_bytes = hotobject_write_bytes;
	self->super.write_string = hotobject_write_hpstring;
	self->super.write_double = hotobject_write_double;
	self->super.write_int64 = hotobject_write_hpint64;
	self->super.write_bool = hotobject_write_hpbool;

	return E_HP_NOERROR;
}
