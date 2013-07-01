#include "hotscript/hotobject.h"
#include "hotpot/hp_platform.h"

#include "datrie/trie.h"
#include "datrie/alpha-map.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_value.h"

#include <string.h>




HotObject* hotobject_new()
{
	HotObject *self = (HotObject*)malloc(sizeof(HotObject));
	AlphaMap *alpha_map = NULL;

	alpha_map = alpha_map_new();
	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');

	self->keys = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	self->type = E_OBJECT;
	return self;
}

static Bool hotobject_visitor(const AlphaChar  *key, TrieData          key_data, void             *user_data)
{
	HotObject *next = key_data;


	hotobject_free(next);

	return TRUE;
}

void hotobject_free(HotObject* self)
{
	HotObject* next;

	if(self->type == E_VAR)
	{
	}
	else
	{
		while(trie_enumerate(self->keys, hotobject_visitor, self) == FALSE)
		{

		}
	}

	trie_free(self->keys);
}


static hpint32 hotobject_writer_begin(HPAbstractWriter* super, const HPVar *name)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	HotObject *new_ob = NULL;
	char str_name[1024];
	if(name == NULL)
	{
		strncpy(str_name, hotobject_get_normal_name(self), 1024);
	}
	else
	{
		strncpy(str_name, name->val.bytes.ptr, 1024);
		str_name[name->val.bytes.len] = TRIE_CHAR_TERM;
	}
	new_ob = hotobject_new();
	new_ob->type = E_OBJECT;
	hotobject_push(self, new_ob);
	if(!trie_store(ob->keys, str_name, new_ob))
	{
		goto ERROR_RET;
	}
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

static hpint32 hotobject_writer_write(HPAbstractWriter* super, const HPVar *var)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	HotObject *ob = hotobject_get(self);
	ob->var = *var;

	return E_HP_NOERROR;
}

static hpint32 hotobject_writer_end(HPAbstractWriter* super)
{
	HotObjectWriter* self = HP_CONTAINER_OF(super, HotObjectWriter, super);
	hotobject_pop(self);
	return E_HP_NOERROR;
}


static hpint32 hotobject_reader_begin(HPAbstractReader* super, const HPVar *name)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = hotobject_get_const(self);
	const HotObject *new_ob = NULL;
	int ret;
	char str_name[1024];
	if(name->type == E_HP_BYTES)
	{
		memcpy(str_name, name->val.bytes.ptr, name->val.bytes.len);
		str_name[name->val.bytes.len] = TRIE_CHAR_TERM;		
	}
	else if(name->type == E_HP_INT32)
	{
		snprintf(str_name, 1024, "[%d]", name->val.i32);
	}
	else
	{
		goto ERROR_RET;
	}

	ret = trie_retrieve(ob->keys, str_name, &new_ob);
	if(!ret)
	{
		goto ERROR_RET;
	}
	hotobject_push_const(self, new_ob);

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

static hpint32 hotobject_reader_read(HPAbstractReader* super, HPVar *var)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	const HotObject *ob = hotobject_get_const(self);
	*var = ob->var;
	return E_HP_NOERROR;
}

static hpint32 hotobject_reader_end(HPAbstractReader* super)
{
	HotObjectReader* self = HP_CONTAINER_OF(super, HotObjectReader, super);
	hotobject_pop_const(self);
	return E_HP_NOERROR;
}


hpint32 hotobject_get_writer(HotObjectWriter* self, HotObject *hotobject)
{
	self->super.begin = hotobject_writer_begin;
	self->super.write = hotobject_writer_write;
	self->super.end = hotobject_writer_end;
	self->stack_num = 0;
	hotobject_push(self, hotobject);

	return E_HP_NOERROR;
}

hpint32 hotobject_get_reader(HotObjectReader* self, const HotObject *hotobject)
{
	self->super.begin = hotobject_reader_begin;
	self->super.read = hotobject_reader_read;
	self->super.end = hotobject_reader_end;
	self->stack_num = 0;
	hotobject_push_const(self, hotobject);

	return E_HP_NOERROR;
}

