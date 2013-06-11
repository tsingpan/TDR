#ifndef _H_HP_TRIE
#define _H_HP_TRIE

#include "hotpot/hp_platform.h"
#include "hotpot/hp_mempool.h"

#define HP_TRIE_MAX_ASCII (1 << (8 + sizeof(char)))

typedef struct tagHP_TRIE
{
	hpuint32 token2index[HP_TRIE_MAX_ASCII];
	hpuint32 children_num;
	hpuint32 unit_size;
	hpuint32 unit_num;
	HP_MEMPOOL_ID root_mid;
 	char xmempool[1];
}HP_TRIE;


HP_API hpint32 hp_trie_init(HP_TRIE* xtrie, hpuint32 xtrie_size, const char* token_set);

HP_API hpint32 hp_trie_insert(HP_TRIE* xtrie, const char* s, void* data);

HP_API hpint32 hp_trie_delete(HP_TRIE* xtrie, const char* s);

HP_API hpint32 hp_trie_search(HP_TRIE* xtrie, const char* s, void** data);

HP_API hpint32 hp_trie_clear(HP_TRIE* xtrie);

#endif//_H_XTRIE

