#include "hotpot/xtrie.h"
#include "hotpot/xnumber.h"
#include "hotpot/xerror.h"
#include "hotpot/xmempool.h"
#include "hotpot/xbase.h"
#include "hotpot/xerror.h"

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
    int count;
	int has_data;
    XMEMPOOL_ID children[0];
};

static int get_token_index(const struct tagXTRIE *xtrie, char c)
{
	return xtrie->token2index[(unsigned)c];
}

static void xtnode_init(XTRIE *xtrie, XTNODE *xtnode)
{
	int i;

	xtnode->count = 0;
	for(i = 0; i < xtrie->children_num; ++i)
	{
		xtnode->children[i] = XMEMPOOL_INVALID_INDEX;
	}	
	xtnode->has_data = FALSE;
}

int xtrie_init(XTRIE* xtrie, int xtrie_size, const char* token_set)
{
	size_t i;
	if((xtrie == NULL ) || (token_set == NULL))
	{
		goto ERROR_RET;
	}
	if((xtrie_size < 0) || (xtrie_size < (int)OFFSET_OF(XTRIE, xmempool)))
	{
		goto ERROR_RET;
	}

	xtrie->children_num = strlen(token_set);
	if((xtrie->children_num > MAX_ASCII) || (xtrie->children_num <= 0))
	{
		goto ERROR_RET;
	}
	for(i = 0; i < MAX_ASCII; ++i)
	{
		xtrie->token2index[i] = INVALID_CHILDREN_INDEX;
	}

	for(i = 0; i < (unsigned)xtrie->children_num; ++i)
	{
		xtrie->token2index[(unsigned)token_set[i]] = i;
	}
	xtrie->unit_size = OFFSET_OF(XTNODE, children) + sizeof(XMEMPOOL_ID) * xtrie->children_num;
	xtrie->unit_num = XMEMPOOL_UNIT_NUM((unsigned)xtrie_size - OFFSET_OF(XTRIE, xmempool), xtrie->unit_size);

	return xtrie_clear(xtrie);
ERROR_RET:
	return XERROR;
}

static int _xtrie_insert(XTRIE* xtrie, XMEMPOOL_ID xtn_mid, const char* s, void* data)
{
	int ret;
	int i;
	XTNODE *xtnode = xmempool_get((XMEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return XERROR;
	}

	if(*s == 0)
	{
		if(xtnode->has_data == TRUE)
		{
			return XERROR;
		}
		xtnode->has_data = TRUE;
		xtnode->data = data;
		++xtnode->count;
		return XNOERROR;
	}

	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= MAX_ASCII))
	{
		return XERROR;
	}
	if(xtnode->children[i] == XMEMPOOL_INVALID_INDEX)
	{
		xtnode->children[i] = xmempool_alloc((XMEMPOOL*)xtrie->xmempool);
		if(xtnode->children[i] == XMEMPOOL_INVALID_INDEX)
		{
			return XERROR;
		}
		xtnode_init(xtrie, xmempool_get((XMEMPOOL*)xtrie->xmempool, xtnode->children[i]));
	}
	ret = _xtrie_insert(xtrie, xtnode->children[i], s + 1, data);
	if(ret == XNOERROR)
	{
		++xtnode->count;
	}
	return ret;
}

int xtrie_insert(XTRIE* xtrie, const char* s, void* data)
{
	if((xtrie == NULL) || (s == NULL))
	{
		return XERROR;
	}
	return _xtrie_insert(xtrie, xtrie->root_mid, s, data);
}

static int _xtrie_delete(XTRIE* xtrie, XMEMPOOL_ID xtn_mid, const char* s)
{
	int ret;
	int i;
	XTNODE *xtnode = xmempool_get((XMEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return XERROR;
	}
	if(*s == 0)
	{
		if(xtnode->has_data == TRUE)
		{
			xtnode->has_data = FALSE;
			--xtnode->count;
			return XNOERROR;
		}
		else
		{
			return XERROR;
		}
	}
	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= MAX_ASCII))
	{
		return XERROR;
	}
	if(xtnode->children[i] == XMEMPOOL_INVALID_INDEX)
	{
		return XERROR;
	}
	ret = _xtrie_delete(xtrie, xtnode->children[i], s + 1);
	if(ret == XNOERROR)
	{
		XTNODE *xc = xmempool_get((XMEMPOOL*)xtrie->xmempool, xtnode->children[i]);
		if(xc == NULL)
		{
			return XERROR;
		}
		if(xc->count <= 0)
		{
			ret = xmempool_free((XMEMPOOL*)xtrie->xmempool, xtnode->children[i]);
			if(ret != XNOERROR)
			{
				return XERROR;
			}
			xtnode->children[i] = XMEMPOOL_INVALID_INDEX;
		}
		--xtnode->count;
	}
	return ret;
}

int xtrie_delete(XTRIE* xtrie, const char* s)
{
	if((xtrie == NULL) || (s == NULL))
	{
		return XERROR;
	}
	return _xtrie_delete(xtrie, xtrie->root_mid, s);
}

static int _xtrie_search(XTRIE* xtrie, XMEMPOOL_ID xtn_mid, const char* s, void** data)
{
	int i;
	XTNODE *xtnode = xmempool_get((XMEMPOOL*)xtrie->xmempool, xtn_mid);
	if(xtnode == NULL)
	{
		return XERROR;
	}	
	if(*s == 0)
	{
		if(xtnode->has_data != TRUE)
		{
			return XERROR; 
		}
		*data = xtnode->data;
		return XNOERROR;
	}
	i = get_token_index(xtrie, *s);
	if((i == INVALID_CHILDREN_INDEX) || (i < 0) || (i >= MAX_ASCII))
	{
		return XERROR;
	}
	if(xtnode->children[i] == XMEMPOOL_INVALID_INDEX)
	{
		return XERROR;
	}
	return _xtrie_search(xtrie, xtnode->children[i], s + 1, data);
}

int xtrie_search(XTRIE* xtrie, const char* s, void** data)
{
	if((xtrie == NULL) || (s == NULL) || (data == NULL))
	{
		return XERROR;
	}
	return _xtrie_search(xtrie, xtrie->root_mid, s, data);
}

int xtrie_clear(XTRIE* xtrie)
{
	int ret;
	ret = xmempool_init((XMEMPOOL*)xtrie->xmempool, XMEMPOOL_SIZE(xtrie->unit_size, xtrie->unit_num), xtrie->unit_size, xtrie->unit_num);
	if(ret != XNOERROR)
	{
		return XERROR;
	}
	xtrie->root_mid = xmempool_alloc((XMEMPOOL*)xtrie->xmempool);
	if(xtrie->root_mid == XMEMPOOL_INVALID_INDEX)
	{
		return XERROR;
	}
	xtnode_init(xtrie, xmempool_get((XMEMPOOL*)xtrie->xmempool, xtrie->root_mid));

	return XNOERROR;
}