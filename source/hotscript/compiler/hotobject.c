#include "hotobject.h"

#include "datrie/trie.h"
#include "datrie/alpha-map.h"
#include "hotpot/hp_error.h"

#include <string.h>

typedef enum _HotObjectType
{
	E_OBJECT,
	E_STRING,
}HotObjectType;

struct _HotObject
{
	HotObjectType type;

	const char *str;

	Trie* keys;
};



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
	self->str = NULL;
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

	if(self->type == E_STRING)
	{
		free(self->str);
	}
	else
	{
		while(trie_enumerate(self->keys, hotobject_visitor, self) == FALSE)
		{

		}
	}

	trie_free(self->keys);
}


hpint32 hotobject_get_iterator(HotObjectIterator* self, HotObject *hotobject)
{
	self->stack_num = 0;
	hotobject_push(self, hotobject);
}

hpint32 hotobject_get_const_iterator(HotObjectConstIterator* self, const HotObject *hotobject)
{
	self->stack_num = 0;
	hotobject_push_const(self, hotobject);
}

static HotObject *hotobject_get(HotObjectIterator* self)
{
	return self->stack[self->stack_num - 1].ho;
}

static hpint32 hotobject_push(HotObjectIterator* self, HotObject *ho)
{
	self->stack[self->stack_num].ho = ho;
	self->stack[self->stack_num].count = 0;
	++(self->stack_num);

	return E_HP_NOERROR;
}

static const HotObject *hotobject_get_const(HotObjectConstIterator* self)
{
	return self->stack[self->stack_num - 1].ho;
}

static hpint32 hotobject_push_const(HotObjectConstIterator* self, const HotObject *ho)
{
	self->stack[self->stack_num].ho = ho;
	self->stack[self->stack_num].count = 0;
	++(self->stack_num);

	return E_HP_NOERROR;
}

static const char* hotobject_get_normal_name(HotObjectIterator *self)
{
	snprintf(self->name, HOTOBJECT_MAX_NAME_LENGTH, "[%u]", self->stack[self->stack_num - 1].count);
	++(self->stack[self->stack_num - 1].count);

	return self->name;
}

static const char* hotobject_get_normal_name_const(HotObjectConstIterator *self)
{
	snprintf(self->name, HOTOBJECT_MAX_NAME_LENGTH, "[%u]", self->stack[self->stack_num - 1].count);
	++(self->stack[self->stack_num - 1].count);

	return self->name;
}

static hpint32 hotobject_pop(HotObjectIterator *self)
{
	--(self->stack_num);

	return E_HP_NOERROR;
}

static hpint32 hotobject_pop_const(HotObjectConstIterator *self)
{
	--(self->stack_num);

	return E_HP_NOERROR;
}



hpint32 hotobject_write_object_begin(HotObjectIterator* self, const char *name)
{
	HotObject *ob = hotobject_get(self);
	HotObject *new_ob = NULL;
	if(name == NULL)
	{
		name = hotobject_get_normal_name(self);
	}

	new_ob = hotobject_new();
	new_ob->type = E_OBJECT;
	hotobject_push(self, new_ob);
	trie_store_if_absent(ob->keys, name, new_ob);
}

hpint32 hotobject_write(HotObjectIterator* self, const char *string)
{
	HotObject *ob = hotobject_get(self);
	ob->type = E_STRING;
	ob->str = strdup(string);
}

hpint32 hotobject_write_object_end(HotObjectIterator* self, const char *name)
{
	hotobject_pop(self);
}


hpint32 hotobject_read_object_begin(HotObjectConstIterator* self, const char *name)
{
	const HotObject *ob = hotobject_get_const(self);
	const HotObject *new_ob = NULL;


	if(name == NULL)
	{
		name = hotobject_get_normal_name_const(self);
	}

	trie_retrieve(ob->keys, name, &new_ob);
	hotobject_push_const(self, new_ob);
}

hpint32 hotobject_read_object_end(HotObjectConstIterator* self, const char *name)
{
	hotobject_pop_const(self);
}

hpint32 hotobject_read(HotObjectConstIterator* self, const char ** string)
{
	const HotObject *ob = hotobject_get_const(self);
	*string = ob->str;
}
