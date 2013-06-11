#include "hotpot/hp_trie.h"
#include "hotpot/hp_number.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_mempool.h"
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>


#define INVALID_CHILDREN_INDEX -1

#ifndef _XTNODE_DEFINED
#define _XTNODE_DEFINED
typedef struct tagXTNODE XTNODE;
#endif //_XTNODE_DEFINED

struct tagXTNODE
{
    void* data;
    hpint32 count;
	hpint32 has_data;
    HP_MEMPOOL_ID children[1];
};

static hpint32 get_token_index(const struct tagHP_TRIE *xtrie, char c)
{
	return xtrie->token2index[(unsigned)c];
}

static void xtnode_init(HP_TRIE *xtrie, XTNODE *xtnode)
{
	hpint32 i;

	xtnode->count = 0;
	for(i = 0; i < xtrie->children_num; ++i)
	{
		xtnode->children[i] = HP_MEMPOOL_INVALID_INDEX;
	}	
	xtnode->has_data = FALSE;
}

hpint32 hp_trie_init(HP_TRIE* xtrie, hpint32 xtrie_size, const char* token_set)
{
	size_t i;
	if((xtrie == NULL ) || (token_set == NULL))
	{
		goto ERROR_RET;
	}
	if((xtrie_size < 0) || (xtrie_size < (hpint32)HP_OFFSET_OF(HP_TRIE, xmempool)))
	{
		goto ERROR_RET;
	}

	xtrie->children_num = strlen(token_set);
	if((xtrie->children_num > HP_TRIE_MAX_ASCII) || (xtrie->children_num <= 0))
	{
		goto ERROR_RET;
	}
	for(i = 0; i < HP_TRIE_MAX_ASCII; ++i)
	{
		xtrie->token2index[i] = INVALID_CHILDREN_INDEX;
	}

	for(i = 0; i < (unsigned)xtrie->children_num; ++i)
	{
		xtrie->token2index[(unsigned)token_set[i]] = i;
	}
	xtrie->unit_size = HP_OFFSET_OF(XTNODE, children) + sizeof(HP_MEMPOOL_ID) * xtrie->children_num;
	xtrie->unit_num = HP_MEMPOOL_UNIT_NUM((unsigned)xtrie_size - HP_OFFSET_OF(HP_TRIE, xmempool), xtrie->unit_size);

	return hp_trie_clear(xtrie);
ERROR_RET:
	return E_HP_ERROR;
}

static hpint32 _xtrie_insert(HP_TRIE* xtrie, HP_MEMPOOL_ID xtn_mid, const char* s, void* data)
{
	hpint32 ret;
	hpint32 i;
	XTNODE *xtnode = hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return E_HP_ERROR;
	}

	if(*s == 0)
	{
		if(xtnode->has_data == TRUE)
		{
			return E_HP_ERROR;
		}
		xtnode->has_data = TRUE;
		xtnode->data = data;
		++xtnode->count;
		return E_HP_NOERROR;
	}

	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= HP_TRIE_MAX_ASCII))
	{
		return E_HP_ERROR;
	}
	if(xtnode->children[i] == HP_MEMPOOL_INVALID_INDEX)
	{
		xtnode->children[i] = hp_mempool_alloc((HP_MEMPOOL*)xtrie->xmempool);
		if(xtnode->children[i] == HP_MEMPOOL_INVALID_INDEX)
		{
			return E_HP_ERROR;
		}
		xtnode_init(xtrie, hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtnode->children[i]));
	}
	ret = _xtrie_insert(xtrie, xtnode->children[i], s + 1, data);
	if(ret == E_HP_NOERROR)
	{
		++xtnode->count;
	}
	return ret;
}

hpint32 hp_trie_insert(HP_TRIE* xtrie, const char* s, void* data)
{
	if((xtrie == NULL) || (s == NULL))
	{
		return E_HP_ERROR;
	}
	return _xtrie_insert(xtrie, xtrie->root_mid, s, data);
}

static hpint32 _xtrie_delete(HP_TRIE* xtrie, HP_MEMPOOL_ID xtn_mid, const char* s)
{
	hpint32 ret;
	hpint32 i;
	XTNODE *xtnode = hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return E_HP_ERROR;
	}
	if(*s == 0)
	{
		if(xtnode->has_data == TRUE)
		{
			xtnode->has_data = FALSE;
			--xtnode->count;
			return E_HP_NOERROR;
		}
		else
		{
			return E_HP_ERROR;
		}
	}
	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= HP_TRIE_MAX_ASCII))
	{
		return E_HP_ERROR;
	}
	if(xtnode->children[i] == HP_MEMPOOL_INVALID_INDEX)
	{
		return E_HP_ERROR;
	}
	ret = _xtrie_delete(xtrie, xtnode->children[i], s + 1);
	if(ret == E_HP_NOERROR)
	{
		XTNODE *xc = hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtnode->children[i]);
		if(xc == NULL)
		{
			return E_HP_ERROR;
		}
		if(xc->count <= 0)
		{
			ret = hp_mempool_free((HP_MEMPOOL*)xtrie->xmempool, xtnode->children[i]);
			if(ret != E_HP_NOERROR)
			{
				return E_HP_ERROR;
			}
			xtnode->children[i] = HP_MEMPOOL_INVALID_INDEX;
		}
		--xtnode->count;
	}
	return ret;
}

hpint32 hp_trie_delete(HP_TRIE* xtrie, const char* s)
{
	if((xtrie == NULL) || (s == NULL))
	{
		return E_HP_ERROR;
	}
	return _xtrie_delete(xtrie, xtrie->root_mid, s);
}

static hpint32 _xtrie_search(HP_TRIE* xtrie, HP_MEMPOOL_ID xtn_mid, const char* s, void** data)
{
	hpint32 i;
	XTNODE *xtnode = hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return E_HP_ERROR;
	}	
	if(*s == 0)
	{
		if(xtnode->has_data != TRUE)
		{
			return E_HP_ERROR; 
		}
		*data = xtnode->data;
		return E_HP_NOERROR;
	}
	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= HP_TRIE_MAX_ASCII))
	{
		return E_HP_ERROR;
	}
	if(xtnode->children[i] == HP_MEMPOOL_INVALID_INDEX)
	{
		return E_HP_ERROR;
	}
	return _xtrie_search(xtrie, xtnode->children[i], s + 1, data);
}

hpint32 hp_trie_search(HP_TRIE* xtrie, const char* s, void** data)
{
	if((xtrie == NULL) || (s == NULL) || (data == NULL))
	{
		return E_HP_ERROR;
	}
	return _xtrie_search(xtrie, xtrie->root_mid, s, data);
}

hpint32 hp_trie_clear(HP_TRIE* xtrie)
{
	hpint32 ret;
	ret = hp_mempool_init((HP_MEMPOOL*)xtrie->xmempool, HP_MEMPOOL_SIZE(xtrie->unit_size, xtrie->unit_num), xtrie->unit_size, xtrie->unit_num);
	if(ret != E_HP_NOERROR)
	{
		return E_HP_ERROR;
	}
	xtrie->root_mid = hp_mempool_alloc((HP_MEMPOOL*)xtrie->xmempool);
	if(xtrie->root_mid == HP_MEMPOOL_INVALID_INDEX)
	{
		return E_HP_ERROR;
	}
	xtnode_init(xtrie, hp_mempool_get((HP_MEMPOOL*)xtrie->xmempool, xtrie->root_mid));

	return E_HP_NOERROR;
}