#ifndef _H_TDR_HASH
#define _H_TDR_HASH

#ifdef  __cplusplus
extern "C" {
#endif


#include "platform/tdr_platform.h"
#include "core/tdr_list.h"
#include "core/tdr_error_code.h"
#include <stdint.h>
typedef struct _tdr_hash_head_t tdr_hash_head_t;
struct _tdr_hash_head_t
{
	tdr_list_head_t data_list;
	const char* key;
	uint32_t key_size;
	uint32_t key_index;
};

typedef struct _tdr_hash_bucket_t tdr_hash_bucket_t;
struct _tdr_hash_bucket_t
{
	tdr_list_head_t data_list;
	tdr_list_head_t used_bucket_list;
	uint32_t data_list_num;
};

typedef struct _tdr_hash_t tdr_hash_t;
struct _tdr_hash_t
{
	tdr_hash_bucket_t		*buckets;
	uint32_t					size;

	tdr_list_head_t			used_bucket_list;
	uint32_t					used_bucket_list_num;
};

tdr_error_code_t tdr_hash_init(tdr_hash_t *self, tdr_hash_bucket_t *buckets, uint32_t size);

uint32_t tdr_hash_key(const char *key, uint32_t key_size);

void tdr_hash_insert(tdr_hash_t *self, const char *key, uint32_t key_size, tdr_hash_head_t *val);

const tdr_hash_head_t* tdr_hash_find_const(const tdr_hash_t *self, const char *key, uint32_t key_size);

tdr_hash_head_t* tdr_hash_find(tdr_hash_t *self, const char *key, uint32_t key_size);

void tdr_hash_remove(tdr_hash_t *self, tdr_hash_head_t *ele);

void tdr_hash_clear(tdr_hash_t *self);

#ifdef  __cplusplus
}
#endif

#endif//_H_TDR_HASH
