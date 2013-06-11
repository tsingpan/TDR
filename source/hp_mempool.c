#include "hotpot/hp_mempool.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_platform.h"

#define XMEMPOOL_GET_XBLOCK_BY_IDX(p, idx) ((HP_MEMPOOL_BLOCK*)((char*)p->data + (HP_MEMPOOL_REAL_UNIT_SIZE(p->unit_size) * (idx))))


#define XMEMPOOL_INVALID_SN 0
#define XMEMPOOL_INVALID_ORDERID -1

static hpint32 xmempool_gen_sn(HP_MEMPOOL* xmempool)
{
	while(xmempool->sn == XMEMPOOL_INVALID_SN)
	{
		++xmempool->sn;
	}
	return E_HP_NOERROR;
}

hpint32 hp_mempool_init(HP_MEMPOOL* xmempool, hpuint32 size, hpuint32 _unit_size, hpint32 _unit_num)
{
	HP_MEMPOOL_BLOCK *b = NULL;
	hpint32 i;

	if((_unit_size < 0) 
		|| (_unit_num <= 0)
		|| ((size - HP_MEMPOOL_HEAD_SIZE) / _unit_num < HP_MEMPOOL_REAL_UNIT_SIZE(_unit_size)) 
		)
	{
		return E_HP_ERROR;
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
	
	return E_HP_NOERROR;
}

HP_MEMPOOL_ID hp_mempool_alloc(HP_MEMPOOL* xmempool)
{
	XMEMPOOL_ID_HELPER idh;
	HP_MEMPOOL_BLOCK *b;

	idh.xmempool_id = 0;

	if(xmempool->free_head == XMEMPOOL_INVALID_ORDERID)
	{
		return HP_MEMPOOL_INVALID_INDEX;
	}

	
	b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, xmempool->free_head);
	

	idh.xmempool_id_desc.orderid = xmempool->free_head;
	xmempool_gen_sn(xmempool);
	idh.xmempool_id_desc.code = xmempool->sn;
	b->code = idh.xmempool_id_desc.code;

	xmempool->free_head = b->next;
	return idh.xmempool_id;
}

hpint32 hp_mempool_free(HP_MEMPOOL* xmempool, HP_MEMPOOL_ID mid)
{
	HP_MEMPOOL_BLOCK *b;
	XMEMPOOL_ID_HELPER idh;
	idh.xmempool_id = mid;

	if((idh.xmempool_id_desc.orderid < 0) || (idh.xmempool_id_desc.orderid >= xmempool->unit_num))
	{
		return E_HP_ERROR;
	}
	if(idh.xmempool_id_desc.code == XMEMPOOL_INVALID_SN)
	{
		return E_HP_ERROR;
	}

	b = XMEMPOOL_GET_XBLOCK_BY_IDX(xmempool, idh.xmempool_id_desc.orderid);
	if(b->code != idh.xmempool_id_desc.code)
	{
		return E_HP_ERROR;
	}

	

	b->code = XMEMPOOL_INVALID_SN;
	b->next = xmempool->free_head;
	xmempool->free_head = idh.xmempool_id_desc.orderid;
	xmempool_gen_sn(xmempool);
	return E_HP_NOERROR;
}

void* hp_mempool_get(HP_MEMPOOL* xmempool, HP_MEMPOOL_ID mid)
{
	HP_MEMPOOL_BLOCK *b = NULL;
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
