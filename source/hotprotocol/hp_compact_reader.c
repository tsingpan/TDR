#include "hotprotocol/hp_compact_reader.h"
#include "hotpot/hp_error_code.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"

#include <string.h>
#include <assert.h>

hpint32 hp_compact_reader_init(HP_COMPACT_READER *self, const void *addr, hpuint32 size)
{
	memset(&self->super, 0, sizeof(HPAbstractReader));

	self->super.read_enum_number = hp_compact_read_enum;
	self->super.read_hpchar = hp_compact_read_hpchar;
	self->super.read_hpdouble = hp_compact_read_hpdouble;
	self->super.read_hpint8 = hp_compact_read_hpint8;
	self->super.read_hpint16 = hp_compact_read_hpint16;
	self->super.read_hpint32 = hp_compact_read_hpint32;
	self->super.read_hpint64 = hp_compact_read_hpint64;
	self->super.read_hpuint8 = hp_compact_read_hpuint8;
	self->super.read_hpuint16 = hp_compact_read_hpuint16;
	self->super.read_hpuint32 = hp_compact_read_hpuint32;
	self->super.read_hpuint64 = hp_compact_read_hpuint64;
	self->super.read_string = hp_compact_read_string;
	self->super.read_counter = hp_compact_read_counter;



	self->addr = (const char*)addr;
	self->size = size;
	self->offset = 0;

	return E_HP_NOERROR;
}

hpint32 hp_compact_reader_fini(HP_COMPACT_READER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractReader, stack));

	self->addr = NULL;
	self->size = 0;
	self->offset = 0;
	return E_HP_NOERROR;
}

#define COMPACT_encoding_READER_CAPACITY(self) (self->size - self->offset)

#define COMPACT_encoding_READER_PTR(self) (self->addr + self->offset)
hpint32 hp_compact_read_enum(HPAbstractReader *super, hpint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode32(hp_little_to_host32(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}

hpint32 hp_compact_read_hpchar(HPAbstractReader *super, char *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(char*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(char);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(double*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(double);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(hpint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpint8*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(hpint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint16_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode16(hp_little_to_host16(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 hp_compact_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode32(hp_little_to_host32(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 hp_compact_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint64_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode64(hp_little_to_host64(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}



hpint32 hp_compact_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(hpuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpuint8*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(hpuint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}



hpint32 hp_compact_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint16_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host16(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 hp_compact_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host32(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}



hpint32 hp_compact_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint64_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host64(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}

hpint32 hp_compact_read_string(HPAbstractReader *super, hpchar* str, hpuint32 str_length)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 str_len = 0;

	for(; self->offset < self->size; ++(self->offset))
	{
		if(str_len >= str_length)
		{
			goto ERROR_RET;
		}
		str[str_len++] = self->addr[self->offset];

		if(self->addr[self->offset] == 0)
		{
			goto done;
		}
	}
	goto ERROR_RET;
done:
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

HP_API hpint32 hp_compact_read_counter(HPAbstractReader *super, const char *name, hpuint32 *val)
{
	return hp_compact_read_hpuint32(super, val);
}
