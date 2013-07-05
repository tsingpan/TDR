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
	
	//alpha_map_add_range(alpha_map, 0, 0xff);
	self->keys = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	self->var.type = E_HP_NULL;
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

	while(trie_enumerate(self->keys, hotobject_visitor, self) == FALSE)
	{

	}

	trie_free(self->keys);
}

