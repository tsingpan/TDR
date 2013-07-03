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

hpint32 hotobject_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	

	return E_HP_NOERROR;
}

hpint32 hotobject_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	return E_HP_NOERROR;
}

hpint32 hotobject_write_field_begin(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	HotObject *new_ob = hotobject_new();

	hotobject_push(self, new_ob);
	if(!trie_store(ob->keys, var_name, new_ob))
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
	self->stack[self->stack_num - 1].current_index = 0;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_vector_end(HPAbstractWriter *super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);

	return E_HP_NOERROR;
}

static HotObject* get_current_ob(HotObjectWriter *self)
{
	HotObject *ob = hotobject_get(self);
	//根据下标找一个位置
	if(ob->var.type ==  E_HP_VECTOR)
	{
		HotObject *new_ob = hotobject_new();
		char str[1024];
		hpuint32 str_len = 0;
		hpint32 count = self->stack[self->stack_num - 1].current_index;
		while(count > 0)
		{
			str[str_len++] = count % 10;
			count/=10;
		}
		str[str_len++] = TRIE_CHAR_TERM;
		

		hotobject_push(self, new_ob);
		if(!trie_store(ob->keys, str, new_ob))
		{
			//free ob~~
			goto ERROR_RET;
		}
		return new_ob;
	}
	//本身就是这个数据
	else
	{
		return ob;
	}

ERROR_RET:
	return NULL;
}

static hpint32 hotobject_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = E_HP_INT8;
	ob->var.val.i8 = val;
	++self->stack[self->stack_num - 1].current_index;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_type(HPAbstractWriter *super, const HPType type)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = get_current_ob(self);
	ob->var.type = type;
}

hpint32 hotobject_writer_init(HotObjectWriter* self, HotObject *hotobject)
{
	self->stack_num = 0;
	self->stack[self->stack_num].ho = hotobject;
	++(self->stack_num);

	self->super.write_struct_begin = hotobject_write_struct_begin;
	self->super.write_struct_end = hotobject_write_struct_end;
	self->super.write_vector_begin = hotobject_write_vector_begin;
	self->super.write_vector_end = hotobject_write_vector_end;
	self->super.write_field_begin = hotobject_write_field_begin;
	self->super.write_field_end = hotobject_write_field_end;

	return E_HP_NOERROR;
}
