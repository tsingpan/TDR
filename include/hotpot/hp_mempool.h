#ifndef _H_HP_MEMPOOL
#define _H_HP_MEMPOOL

#include "hotpot/hp_platform.h"

#pragma pack(1)
typedef hpuint64 HP_MEMPOOL_ID;

typedef struct tagHP_MEMPOOL_ID_DESC
{
	hpint32 code		: 16;
	hpint32 orderid		: 32;
}XMEMPOOL_ID_DESC;

typedef union tagHP_MEMPOOL_ID_HELPER
{
	HP_MEMPOOL_ID xmempool_id;
	XMEMPOOL_ID_DESC xmempool_id_desc;
}XMEMPOOL_ID_HELPER;

#pragma pack()

typedef struct tagHP_MEMPOOL_BLOCK HP_MEMPOOL_BLOCK;
struct tagHP_MEMPOOL_BLOCK
{
	hpint32 code;
	hpint32 next;
	char data[1];
};

#ifndef _DEF_HP_MEMPOOL
#define _DEF_HP_MEMPOOL
typedef struct tagHP_MEMPOOL HP_MEMPOOL;
#endif //_DEF_HP_MEMPOOL

struct tagHP_MEMPOOL
{
	hpint32 sn;
	hpint32 unit_size;
	hpint32 unit_num;
	hpint32 free_head;
	char data[1];
};

#define HP_MEMPOOL_INVALID_INDEX 0

#define HP_MEMPOOL_HEAD_SIZE (hpint32)&((HP_MEMPOOL*)0)->data

#define HP_MEMPOOL_REAL_UNIT_SIZE(unit_size) (unit_size + (hpint32)&((HP_MEMPOOL_BLOCK*)0)->data)

#define HP_MEMPOOL_SIZE(unit_size, unit_num) (HP_MEMPOOL_HEAD_SIZE + HP_MEMPOOL_REAL_UNIT_SIZE(unit_size) * unit_num)

#define HP_MEMPOOL_UNIT_NUM(size, unit_size) ((size - HP_MEMPOOL_HEAD_SIZE) / HP_MEMPOOL_REAL_UNIT_SIZE(unit_size))

HP_API hpint32 hp_mempool_init(HP_MEMPOOL* xmempool, hpint32 size, hpint32 _unit_size, hpint32 _unit_num);

HP_API HP_MEMPOOL_ID hp_mempool_alloc(HP_MEMPOOL* xmempool);

HP_API hpint32 hp_mempool_free(HP_MEMPOOL* xmempool, HP_MEMPOOL_ID mid);

HP_API void* hp_mempool_get(HP_MEMPOOL* xmempool, HP_MEMPOOL_ID mid);

#endif//_H_XMEMPOOL
