#include "hotscript/hotobject_writer.h"
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "hotscript/hotobject.h"
#include "datrie/trie.h"

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

static HotObject* get_current_ob(HotObjectWriter *self)
{
	HotObject *ob = hotobject_get(self);
	if(ob->var.type ==  E_HP_VECTOR)
	{
		if(hotobject_writer_seek(&self->super, self->stack[self->stack_num - 1].current_index) != E_HP_NOERROR)
		{
			goto ERROR_RET;
		}
		return self->stack[self->stack_num - 1].current_ho;
	}
	else
	{
		return ob;
	}

ERROR_RET:
	return NULL;
}

hpint32 hotobject_write_field_begin(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
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
	HotObject *ob = NULL;
	--(self->stack_num);
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}


hpint32 hotobject_write_vector_begin(HPAbstractWriter *super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	self->stack[self->stack_num - 1].current_index = 0;
	ob->var.type = E_HP_VECTOR;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_vector_end(HPAbstractWriter *super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);

	return E_HP_NOERROR;
}

static hpint32 hotobject_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_INT8;
	ob->var.val.i8 = val;
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}

static hpint32 hotobject_write_double(HPAbstractWriter *super, const hpdouble val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_DOUBLE;
	ob->var.val.d = val;
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}

static hpint32 hotobject_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_INT64;
	ob->var.val.i64 = val;
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}

hpint32 hotobject_write_bytes(HPAbstractWriter *super, const hpchar* buff, const hpuint32 buff_size)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_BYTES;
	ob->var.val.bytes.ptr = (char*)malloc(buff_size);
	memcpy(ob->var.val.bytes.ptr, buff, buff_size);
	ob->var.val.bytes.len = buff_size;
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}

hpint32 hotobject_write_type(HPAbstractWriter *super, const HPType type)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = type;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_BOOL;
	ob->var.val.b = val;
	++(self->stack[self->stack_num - 1].current_index);
	return E_HP_NOERROR;
}

hpint32 hotobject_writer_seek(HPAbstractWriter *super, hpuint32 index)
{
	HotObject *new_ob = NULL;
	char str[1024];
	hpuint32 str_len = 0;
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	hpint32 count;
	if(ob->var.type != E_HP_VECTOR)
	{
		goto ERROR_RET;
	}

	self->stack[self->stack_num - 1].current_index = index;

	count = index;
	do
	{
		str[str_len++] = '0' + count % 10;
		count/=10;
	}while(count > 0);
	str[str_len++] = TRIE_CHAR_TERM;

	//fix bug~
	/*
	trie_store(ob->keys, "[0]", 1);
	trie_retrieve(ob->keys, "[1]", &new_ob);
*/
	if(!trie_retrieve(ob->keys, str, &new_ob))
	{
		new_ob = hotobject_new();
		if(!trie_store(ob->keys, str, new_ob))
		{
			//free ob~~
			goto ERROR_RET;
		}
	}
	self->stack[self->stack_num - 1].current_ho = new_ob;

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotobject_writer_init(HotObjectWriter* self, HotObject *hotobject)
{
	self->stack_num = 0;
	self->stack[self->stack_num].ho = hotobject;
	++(self->stack_num);

	self->super.write_vector_begin = hotobject_write_vector_begin;
	self->super.write_vector_end = hotobject_write_vector_end;
	self->super.write_field_begin = hotobject_write_field_begin;
	self->super.write_field_end = hotobject_write_field_end;
	self->super.write_type = hotobject_write_type;
	self->super.writer_seek = hotobject_writer_seek;
	self->super.write_bytes = hotobject_write_bytes;
	self->super.write_hpdouble = hotobject_write_double;
	self->super.write_hpint64 = hotobject_write_hpint64;
	self->super.write_hpbool = hotobject_write_hpbool;

	return E_HP_NOERROR;
}
