#include "core/tdr_hash.h"
#include "core/tdr_list.h"
#include "core/tdr_error_code.h"

#include <memory.h>
#include <assert.h>
#include <stdint.h>

tdr_error_code_t tdr_hash_init(tdr_hash_t *self, tdr_hash_bucket_t *buckets, uint32_t size)
{
	uint32_t i;

	self->buckets = buckets;
	self->size = size;
	for(i = 0; i < self->size; ++i)
	{
		tdr_list_init(&self->buckets[i].data_list);
		tdr_list_init(&self->buckets[i].used_bucket_list);
		self->buckets[i].data_list_num = 0;
	}

	tdr_list_init(&self->used_bucket_list);
	self->used_bucket_list_num = 0;

	return E_TDR_NOERROR;
}

uint32_t tdr_hash_key(const char* key, uint32_t key_size)
{
	uint32_t i, key_hash;
	key_hash = 0;
	for(i = 0; i < key_size; ++i)
	{
		key_hash = key_hash * 31 + key[i];
	}
	return key_hash;
}

void tdr_hash_insert(tdr_hash_t *self, const char* key, uint32_t key_size, tdr_hash_head_t *val_head)
{
	uint32_t key_hash = tdr_hash_key(key, key_size);
	uint32_t key_index = key_hash % self->size;
	tdr_hash_bucket_t *bucket = &self->buckets[key_index];

	val_head->key = key;
	val_head->key_size = key_size;
	val_head->key_index = key_index;

	tdr_list_init(&val_head->data_list);
	tdr_list_add_tail(&val_head->data_list, &bucket->data_list);
	if(bucket->data_list_num == 0)
	{
		tdr_list_add_tail(&bucket->used_bucket_list, &self->used_bucket_list);
		++self->used_bucket_list_num;
	}	
	++bucket->data_list_num;
}

const tdr_hash_head_t* tdr_hash_find_const(const tdr_hash_t *self, const char *key, uint32_t key_size)
{
	uint32_t key_hash = tdr_hash_key(key, key_size);
	uint32_t key_index = key_hash % self->size;
	const tdr_hash_bucket_t *bucket = &self->buckets[key_index];
	tdr_list_head_t *iter = bucket->data_list.next;
	uint32_t i;
	for(i = 0; i < bucket->data_list_num; iter = iter->next, ++i)
	{
		tdr_hash_head_t *ele = TDR_CONTAINER_OF(iter, tdr_hash_head_t, data_list);
		if((ele->key_size == key_size ) && (memcmp(ele->key, key, key_size) == 0))
		{
			return ele;
		}
	}
	return NULL;
}

tdr_hash_head_t* tdr_hash_find(tdr_hash_t *self, const char *key, uint32_t key_size)
{
	uint32_t key_hash = tdr_hash_key(key, key_size);
	uint32_t key_index = key_hash % self->size;
	const tdr_hash_bucket_t *bucket = &self->buckets[key_index];
	tdr_list_head_t *iter = iter = bucket->data_list.next;
	uint32_t i;
	for(i = 0; i < bucket->data_list_num; iter = iter->next, ++i)
	{
		tdr_hash_head_t *ele = TDR_CONTAINER_OF(iter, tdr_hash_head_t, data_list);
		if((ele->key_size == key_size ) && (memcmp(ele->key, key, key_size) == 0))
		{
			return ele;
		}
	}
	return NULL;
}

void tdr_hash_remove(tdr_hash_t *self, tdr_hash_head_t *ele)
{
	if(ele->key_index < self->size)
	{
		tdr_hash_bucket_t		*bucket = &self->buckets[ele->key_index];
		tdr_list_del(&ele->data_list);
		--bucket->data_list_num;

		if(bucket->data_list_num == 0)
		{
			tdr_list_del(&bucket->used_bucket_list);
			--self->used_bucket_list_num;
		}
	}
}

void tdr_hash_clear(tdr_hash_t *self)
{
	tdr_list_head_t *iter = self->used_bucket_list.next;
	uint32_t i;
	for(i = 0; i < self->used_bucket_list_num; iter = iter->next, ++i)
	{
		tdr_hash_bucket_t *bucket = TDR_CONTAINER_OF(iter, tdr_hash_bucket_t, used_bucket_list);
		tdr_list_init(&bucket->data_list);
		bucket->data_list_num = 0;
	}
	tdr_list_init(&self->used_bucket_list);
	self->used_bucket_list_num = 0;
}
