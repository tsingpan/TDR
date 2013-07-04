#include "hotscript/hotobject_reader.h"

#include "hotpot/hp_platform.h"

#include "hotpot/hp_error.h"




static const HotObject *hotobject_get(HotObjectReader *self)
{
	return self->stack[self->stack_num - 1].ho;
}

static hpint32 hotobject_push(HotObjectReader *self, const HotObject *ho)
{
	self->stack[self->stack_num].ho = ho;
	++(self->stack_num);

	return E_HP_NOERROR;
}

hpint32 hotobject_read_struct_begin(HPAbstractReader *super, const char *struct_name)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);

	return E_HP_NOERROR;
}

hpint32 hotobject_read_struct_end(HPAbstractReader *super, const char *struct_name)
{
	return E_HP_NOERROR;
}

hpint32 hotobject_read_field_begin(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = hotobject_get(self);
	const HotObject *new_ob;
	hpint32 ret;


	ret = trie_retrieve(ob->keys, var_name, &new_ob);
	if(!ret)
	{
		goto ERROR_RET;
	}
	hotobject_push(self, new_ob);

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotobject_read_field_end(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	--(self->stack_num);
	++self->stack[self->stack_num - 1].current_index;
	return E_HP_NOERROR;
}


hpint32 hotobject_read_vector_begin(HPAbstractReader *super)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = hotobject_get(self);
	self->stack[self->stack_num - 1].current_index = 0;

	return E_HP_NOERROR;
}

hpint32 hotobject_read_vector_end(HPAbstractReader *super)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);

	return E_HP_NOERROR;
}

static const HotObject* get_current_ob(HotObjectReader *self)
{
	const HotObject *ob = hotobject_get(self);
	//根据下标找一个位置
	if(ob->var.type ==  E_HP_VECTOR)
	{
		if(hotobject_reader_seek(&self->super, self->stack[self->stack_num - 1].current_index) != E_HP_NOERROR)
		{
			goto ERROR_RET;
		}
		return self->stack[self->stack_num - 1].current_ho;
	}
	//本身就是这个数据
	else
	{
		return ob;
	}

ERROR_RET:
	return NULL;
}

static hpint32 hotobject_seek(HPAbstractReader *super, hpuint32 index)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	self->stack[self->stack_num - 1].current_index = index;
	//这个id是否存在？
	return E_HP_NOERROR;
}

static hpint32 hotobject_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	*val = ob->var.val.i8;
	++self->stack[self->stack_num - 1].current_index;

	return E_HP_NOERROR;
}

static hpint32 hotobject_read_double(HPAbstractReader *super, hpdouble *val)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	*val = ob->var.val.d;
	++self->stack[self->stack_num - 1].current_index;

	return E_HP_NOERROR;
}

static hpint32 hotobject_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	*val = ob->var.val.i64;
	++self->stack[self->stack_num - 1].current_index;

	return E_HP_NOERROR;
}

hpint32 hotobject_read_bytes(HPAbstractReader *super, hpbytes *bytes)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	bytes->ptr = ob->var.val.bytes.ptr;
	bytes->len = ob->var.val.bytes.len;
	++self->stack[self->stack_num - 1].current_index;

	return E_HP_NOERROR;
}

hpint32 hotobject_read_type(HPAbstractReader *super, HPType *type)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	*type = ob->var.type;

	return E_HP_NOERROR;
}

hpint32 hotobject_read_hpbool(HPAbstractReader *super, hpbool *val)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = get_current_ob(self);
	*val = ob->var.val.b;

	return E_HP_NOERROR;
}

hpint32 hotobject_reader_seek(HPAbstractReader *super, hpuint32 index)
{
	HotObject *new_ob = NULL;
	char str[1024];
	hpuint32 str_len = 0;
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = hotobject_get(self);
	hpint32 count;
	if(ob->var.type != E_HP_VECTOR)
	{
		goto ERROR_RET;
	}

	self->stack[self->stack_num - 1].current_index = index;

	count = index;
	while(count > 0)
	{
		str[str_len++] = count % 10;
		count/=10;
	}
	str[str_len++] = TRIE_CHAR_TERM;


	if(!trie_retrieve(ob->keys, str, &new_ob))
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num - 1].current_ho = new_ob;

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}


hpint32 hotobject_reader_init(HotObjectReader* self, const HotObject *hotobject)
{
	self->stack_num = 0;
	self->stack[self->stack_num].ho = hotobject;
	++(self->stack_num);

	self->super.read_struct_begin = hotobject_read_struct_begin;
	self->super.read_struct_end = hotobject_read_struct_end;
	self->super.read_vector_begin = hotobject_read_vector_begin;
	self->super.read_vector_end = hotobject_read_vector_end;
	self->super.read_field_begin = hotobject_read_field_begin;
	self->super.read_field_end = hotobject_read_field_end;
	self->super.read_hpint8 = read_hpint8;

	return E_HP_NOERROR;
}
