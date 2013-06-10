#include "hotpot/xmempool.h"
#include "hotpot/xerror.h"
#include "hotpot/typedef.h"
#include "hotpot/xbase.h"

#define XMEMPOOL_GET_XBLOCK_BY_IDX(p, idx) ((XBLOCK*)((char*)p->data + (XMEMPOOL_REAL_UNIT_SIZE(p->unit_size) * (idx))))


#define XMEMPOOL_INVALID_SN 0
#define XMEMPOOL_INVALID_ORDERID -1

static int xmempool_gen_sn(XMEMPOOL* xmempool)
{
	while(xmempool->sn == XMEMPOOL_INVALID_SN)
	{
		++xmempool->sn;
	}
	return XNOERROR;
}

int xmempool_init(XMEMPOOL* xmempool, int size, int _unit_size, int _unit_num)
{
	XBLOCK *b = NULL;
	int i;

	if((_unit_size < 0) 
		|| (_unit_num <= 0)
		|| ((size - XMEMPOOL_HEAD_SIZE) / _unit_num < XMEMPOOL_REAL_UNIT_SIZE(_unit_size)) 
		)
	{
		return XERROR;
	}

	xmempool->unit_size = _unit_size;
	xmempool->unit_num = _unit_num;
	xmempool->free_head = 0;
	xmempool_gen_sn(xmempool);

	for(i = 0; i < _unit_num; ++i)
	{
		b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, i);
		b->next = i + 1;
		b->code = XMEMPOOL_INVALID_SN;
	}
	if(b != NULL)
	{
		b->next = XMEMPOOL_INVALID_ORDERID;
	}
	
	return XNOERROR;
}

XMEMPOOL_ID xmempool_alloc(XMEMPOOL* xmempool)
{
	XMEMPOOL_ID_HELPER idh;
	XBLOCK *b;

	idh.xmempool_id = 0;

	if(xmempool->free_head == XMEMPOOL_INVALID_ORDERID)
	{
		return XMEMPOOL_INVALID_INDEX;
	}

	
	b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, xmempool->free_head);
	

	idh.xmempool_id_desc.orderid = xmempool->free_head;
	xmempool_gen_sn(xmempool);
	idh.xmempool_id_desc.code = xmempool->sn;
	b->code = idh.xmempool_id_desc.code;

	xmempool->free_head = b->next;
	return idh.xmempool_id;
}

int xmempool_free(XMEMPOOL* xmempool, XMEMPOOL_ID mid)
{
	XBLOCK *b;
	XMEMPOOL_ID_HELPER idh;
	idh.xmempool_id = mid;

	if((idh.xmempool_id_desc.orderid < 0) || (idh.xmempool_id_desc.orderid >= xmempool->unit_num))
	{
		return XERROR;
	}
	if(idh.xmempool_id_desc.code == XMEMPOOL_INVALID_SN)
	{
		return XERROR;
	}

	b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, idh.xmempool_id_desc.orderid);
	if(b->code != idh.xmempool_id_desc.code)
	{
		return XERROR;
	}

	

	b->code = XMEMPOOL_INVALID_SN;
	b->next = xmempool->free_head;
	xmempool->free_head = idh.xmempool_id_desc.orderid;
	xmempool_gen_sn(xmempool);
	return XNOERROR;
}

void* xmempool_get(XMEMPOOL* xmempool, XMEMPOOL_ID mid)
{
	XBLOCK *b = NULL;
	XMEMPOOL_ID_HELPER idh;
	idh.xmempool_id = mid;

	if((idh.xmempool_id_desc.orderid < 0) || (idh.xmempool_id_desc.orderid >= xmempool->unit_num))
	{
		return NULL;
	}

	if(idh.xmempool_id_desc.code == XMEMPOOL_INVALID_SN)
	{
		return NULL;
	}

	b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, idh.xmempool_id_desc.orderid);
	if(b->code != idh.xmempool_id_desc.code)
	{
		return NULL;
	}

	return b->data;
}
