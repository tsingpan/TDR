#ifndef _H_XMEMPOOL
#define _H_XMEMPOOL

#include "xbase.h"
#include "typedef.h"

#pragma pack(1)
typedef uint64 XMEMPOOL_ID;

typedef struct tagXMEMPOOL_ID_DESC
{
	int code		: 16;
	int orderid		: 32;
}XMEMPOOL_ID_DESC;

typedef union tagXMEMPOOL_ID_HELPER
{
	XMEMPOOL_ID xmempool_id;
	XMEMPOOL_ID_DESC xmempool_id_desc;
}XMEMPOOL_ID_HELPER;

#pragma pack()

typedef struct tagXBLOCK XBLOCK;
struct tagXBLOCK
{
	int code;
	int next;
	char data[1];
};

#ifndef _XMEMPOOL_DEFINED
#define _XMEMPOOL_DEFINED
typedef struct tagXMEMPOOL XMEMPOOL;
#endif //_XMEMPOOL_DEFINED

struct tagXMEMPOOL
{
	int sn;
	int unit_size;
	int unit_num;
	int free_head;
	char data[1];
};

#define XMEMPOOL_INVALID_INDEX 0

#define XMEMPOOL_HEAD_SIZE (int)&((XMEMPOOL*)0)->data

#define XMEMPOOL_REAL_UNIT_SIZE(unit_size) (unit_size + (int)&((XBLOCK*)0)->data)

#define XMEMPOOL_SIZE(unit_size, unit_num) (XMEMPOOL_HEAD_SIZE + XMEMPOOL_REAL_UNIT_SIZE(unit_size) * unit_num)

#define XMEMPOOL_UNIT_NUM(size, unit_size) ((size - XMEMPOOL_HEAD_SIZE) / XMEMPOOL_REAL_UNIT_SIZE(unit_size))

XAPI int xmempool_init(XMEMPOOL* xmempool, int size, int _unit_size, int _unit_num);

XAPI XMEMPOOL_ID xmempool_alloc(XMEMPOOL* xmempool);

XAPI int xmempool_free(XMEMPOOL* xmempool, XMEMPOOL_ID mid);

XAPI void* xmempool_get(XMEMPOOL* xmempool, XMEMPOOL_ID mid);

#endif//_H_XMEMPOOL
