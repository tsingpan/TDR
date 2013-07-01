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
	ob->type = E_OBJECT;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_struct_end(HPAbstractWriter *self, const char *struct_name)
{
	return E_HP_NOERROR;
}

hpint32 hotobject_write_field_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	HotObject *new_ob = hotobject_new();

	new_ob->type = E_UNKNOW;
	hotobject_push(self, new_ob);
	if(!trie_store(ob->keys, var_name, new_ob))
	{
		goto ERROR_RET;
	}

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotobject_write_field_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	--(self->stack_num);

	return E_HP_NOERROR;
}


hpint32 hotobject_write_vector_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->type = E_ARRAY;
	self->stack[self->stack_num - 1].count = 0;

	return E_HP_NOERROR;
}

hpint32 hotobject_write_vector_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);

	return E_HP_NOERROR;
}

hpint32 write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);

	if(ob->type ==  E_ARRAY)
	{
		//根据下标找一个位置
	}
	else
	{

	}
	return E_HP_NOERROR;
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
