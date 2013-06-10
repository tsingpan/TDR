#ifndef _H_XTRIE
#define _H_XTRIE

#include "xbase.h"
#include "xmempool.h"

#define MAX_ASCII (1 << (8 + sizeof(char)))

typedef struct tagXTRIE
{
	int token2index[MAX_ASCII];
	int children_num;
	int unit_size;
	int unit_num;
	XMEMPOOL_ID root_mid;
 	char xmempool[0];
}XTRIE;


XAPI int xtrie_init(XTRIE* xtrie, int xtrie_size, const char* token_set);

XAPI int xtrie_insert(XTRIE* xtrie, const char* s, void* data);

XAPI int xtrie_delete(XTRIE* xtrie, const char* s);

XAPI int xtrie_search(XTRIE* xtrie, const char* s, void** data);

XAPI int xtrie_clear(XTRIE* xtrie);

#endif//_H_XTRIE

