#include "hotscript/hstrie.h"

#include "hotpot/hp_error.h"

#include "hotpot/hp_platform.h"

#include <string.h>

static hpint32 get_token_index(const HOTSCRIPT_TRIE *self, char c)
{
	return self->char2index[(unsigned)c];
}


hpint32 hotscript_trie_init(HOTSCRIPT_TRIE *self, hpuint64 trie_size, const char *token_set)
{
	hpuint32 i;
	if((self == NULL ) || (token_set == NULL))
	{
		goto ERROR_RET;
	}
	if((trie_size < 0) || (trie_size < (hpuint32)HP_OFFSET_OF(HOTSCRIPT_TRIE, leafs)))
	{
		goto ERROR_RET;
	}
	self->total_alloc = 0;
	self->max_leaf_index = (hpuint32)strlen(token_set);
	if(self->max_leaf_index >= HOTSCRIPT_TRIE_WIDTH)
	{
		goto ERROR_RET;
	}
	for(i = 0; i < HOTSCRIPT_TRIE_WIDTH; ++i)
	{
		self->char2index[i] = HOTSCRIPT_TRIE_INVALID_INDEX;
	}

	for(i = 0; i < self->max_leaf_index; ++i)
	{
		self->char2index[token_set[i]] = i;
	}
	self->leafs_num = (hpuint32)((trie_size - HP_OFFSET_OF(HOTSCRIPT_TRIE, leafs)) / sizeof(HOTSCRIPT_TRIE_LEAF));
	if(self->leafs_num >= HOTSCRIPT_TRIE_LEAF_MAX)
	{
		goto ERROR_RET;
	}

	return hotscript_trie_clear(self);
ERROR_RET:
	return E_HP_ERROR;

	return E_HP_NOERROR;
}
hpint32 hotscript_trie_leaf_new(HOTSCRIPT_TRIE *self)
{
	hpuint32 i;
	hpuint32 mid = self->free_lesf_head;
	//分配内存
	if((mid == HOTSCRIPT_TRIE_INVALID_INDEX) || (mid >= HOTSCRIPT_TRIE_LEAF_MAX) || (mid >= self->leafs_num))
	{
		goto ERROR_RET;
	}
	self->free_lesf_head = self->leafs[mid].next_leaf_index;

	//构造函数

	for(i = 0; i < self->max_leaf_index; ++i)
	{
		self->leafs[mid].children_index[i] = HOTSCRIPT_TRIE_INVALID_INDEX;		
	}
	self->leafs[mid].children_index_size = self->max_leaf_index;
	self->leafs[mid].mark = FALSE;
	self->leafs[mid].count = 0;

	++(self->total_alloc);
	return mid;
ERROR_RET:
	return HOTSCRIPT_TRIE_INVALID_INDEX;
}

hpint32 hotscript_trie_leaf_delete(HOTSCRIPT_TRIE *self, hpuint32 index)
{
	//回收内存
	if((index >= HOTSCRIPT_TRIE_LEAF_MAX) || (index >= self->leafs_num))
	{
		goto ERROR_RET;
	}
	self->leafs[index].next_leaf_index = self->free_lesf_head;
	self->free_lesf_head = index;

	--self->total_alloc;
	//析构函数函数


	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

static hpint32 _insert(HOTSCRIPT_TRIE* self, hpuint32 index, const char* s, hpint64 data)
{
	hpuint32 i;
	hpint32 ret;

	if((index >= HOTSCRIPT_TRIE_LEAF_MAX) || (index >= self->leafs_num))
	{
		goto ERROR_RET;
	}

	if(*s == 0)
	{
		if(self->leafs[index].mark == TRUE)
		{
			goto ERROR_RET;			
		}
		self->leafs[index].mark = TRUE;
		self->leafs[index].data = data;
		++(self->leafs[index].count);
		return E_HP_NOERROR;
	}

	i = get_token_index(self, *s);
	if((i == HOTSCRIPT_TRIE_INVALID_INDEX) || (i >= HOTSCRIPT_TRIE_WIDTH))
	{
		return E_HP_ERROR;
	}
	if(self->leafs[index].children_index[i] == HOTSCRIPT_TRIE_INVALID_INDEX)
	{
		self->leafs[index].children_index[i] = hotscript_trie_leaf_new(self);
		if(self->leafs[index].children_index[i] == HOTSCRIPT_TRIE_INVALID_INDEX)
		{
			return E_HP_ERROR;
		}
	}


	ret = _insert(self, self->leafs[index].children_index[i], s + 1, data);
	if(ret == E_HP_NOERROR)
	{
		++(self->leafs[index].count);
	}
	return ret;
ERROR_RET:
	return E_HP_ERROR;
}


hpint32 hotscript_trie_insert(HOTSCRIPT_TRIE *self, const char *s, hpint64 data)
{
	return _insert(self, self->root_index, s, data);
}

static hpint32 _delete(HOTSCRIPT_TRIE* self, hpuint32 index, const char* s)
{
	hpint32 ret;
	hpuint32 i;

	if((index >= HOTSCRIPT_TRIE_LEAF_MAX) || (index >= self->leafs_num))
	{
		goto ERROR_RET;
	}
	if(*s == 0)
	{
		if(self->leafs[index].mark == TRUE)
		{
			self->leafs[index].mark = FALSE;
			--self->leafs[index].count;
			return E_HP_NOERROR;
		}
		else
		{
			return E_HP_ERROR;
		}
	}
	i = get_token_index(self, *s);
	if((i == HOTSCRIPT_TRIE_INVALID_INDEX) || (i >= HOTSCRIPT_TRIE_WIDTH))
	{
		return E_HP_ERROR;
	}
	if(self->leafs[index].children_index[i] == HOTSCRIPT_TRIE_INVALID_INDEX)
	{
		return E_HP_ERROR;
	}

	ret = _delete(self, self->leafs[index].children_index[i], s + 1);
	if(ret == E_HP_NOERROR)
	{
		if(self->leafs[self->leafs[index].children_index[i]].count <= 0)
		{
			hotscript_trie_leaf_delete(self, self->leafs[index].children_index[i]);
			self->leafs[index].children_index[i] = HOTSCRIPT_TRIE_INVALID_INDEX;
		}
		--self->leafs[index].count;
	}
	return ret;
ERROR_RET:
	return E_HP_ERROR;
}


hpint32 hotscript_trie_delete(HOTSCRIPT_TRIE* self, const char* s)
{
	return _delete(self, self->root_index, s);
}

static hpint32 _search(HOTSCRIPT_TRIE* self, hpuint32 index, const char* s, hpint64 *data)
{
	hpuint32 i;

	if((index >= HOTSCRIPT_TRIE_LEAF_MAX) || (index >= self->leafs_num))
	{
		goto ERROR_RET;
	}

	if(*s == 0)
	{
		if(self->leafs[index].mark != TRUE)
		{
			return E_HP_ERROR; 
		}
		*data = self->leafs[index].data;
		return E_HP_NOERROR;
	}
	i = get_token_index(self, *s);
	if((i == HOTSCRIPT_TRIE_INVALID_INDEX) || (i >= HOTSCRIPT_TRIE_WIDTH))
	{
		return E_HP_ERROR;
	}
	if(self->leafs[index].children_index[i] == HOTSCRIPT_TRIE_INVALID_INDEX)
	{
		return E_HP_ERROR;
	}
	return _search(self, self->leafs[index].children_index[i], s + 1, data);
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_trie_search(HOTSCRIPT_TRIE* self, const char* s, hpint64 *data)
{
	if((self == NULL) || (s == NULL) || (data == NULL))
	{
		return E_HP_ERROR;
	}
	return _search(self, self->root_index, s, data);
}


hpint32 hotscript_trie_fini(HOTSCRIPT_TRIE *self)
{
	return E_HP_NOERROR;
}


hpint32 hotscript_trie_clear(HOTSCRIPT_TRIE* self)
{
	hpuint32 i;

	self->free_lesf_head = 0;
	for(i = 0; i < self->leafs_num - 1; ++i)
	{
		self->leafs[i].next_leaf_index = i + 1;
	}
	self->leafs[self->leafs_num - 1].next_leaf_index = HOTSCRIPT_TRIE_INVALID_INDEX;


	self->root_index = hotscript_trie_leaf_new(self);
	if(self->root_index == HOTSCRIPT_TRIE_INVALID_INDEX)
	{
		goto ERROR_RET;
	}

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}
