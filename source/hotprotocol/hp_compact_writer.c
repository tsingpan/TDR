#include "hotprotocol/hp_compact_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>

hpint32 hp_compact_writer_init(HP_COMPACT_WRITER *self, void *addr, hpuint32 size)
{
	memset(&self->super, 0, sizeof(HPAbstractWriter));

	self->super.write_enum_number = hp_compact_write_enum;
	self->super.write_hpchar = hp_compact_write_hpchar;
	self->super.write_hpdouble = hp_compact_write_hpdouble;
	self->super.write_hpint8 = hp_compact_write_hpint8;
	self->super.write_hpint16 = hp_compact_write_hpint16;
	self->super.write_hpint32 = hp_compact_write_hpint32;
	self->super.write_hpint64 = hp_compact_write_hpint64;
	self->super.write_hpuint8 = hp_compact_write_hpuint8;
	self->super.write_hpuint16 = hp_compact_write_hpuint16;
	self->super.write_hpuint32 = hp_compact_write_hpuint32;
	self->super.write_hpuint64 = hp_compact_write_hpuint64;
	self->super.write_string = hp_compact_write_string;
	self->super.write_counter = hp_compact_write_counter;
	
	

	self->addr = addr;
	self->size = size;
	self->offset = 0;

	return E_HP_NOERROR;
}

hpint32 hp_compact_writer_fini(HP_COMPACT_WRITER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractWriter, stack));

	self->addr = NULL;
	self->size = 0;
	self->offset = 0;
	return E_HP_NOERROR;
}

#define DDEKIT_COMPACT_encoding_CAPACITY(self) (self->size - self->offset)

#define DDEKIT_COMPACT_encoding_PTR(self) (self->addr + self->offset)

HP_API hpint32 hp_compact_write_enum(HPAbstractWriter *super, const hpint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint32_encode(hp_host32_to_little(hp_zigzag_encode16(val)), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(DDEKIT_COMPACT_encoding_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)DDEKIT_COMPACT_encoding_PTR(self) = val;
	self->offset += sizeof(char);
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(DDEKIT_COMPACT_encoding_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*(double*)DDEKIT_COMPACT_encoding_PTR(self) = val;
	self->offset += sizeof(double);
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(DDEKIT_COMPACT_encoding_CAPACITY(self) < sizeof(hpint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(hpint8*)DDEKIT_COMPACT_encoding_PTR(self) = val;
	self->offset += sizeof(hpint8);
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint16_encode(hp_host16_to_little(hp_zigzag_encode16(val)), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint32_encode(hp_host32_to_little(hp_zigzag_encode16(val)), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint64_encode(hp_host64_to_little(hp_zigzag_encode64(val)), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(DDEKIT_COMPACT_encoding_CAPACITY(self) < sizeof(hpuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(hpuint8*)DDEKIT_COMPACT_encoding_PTR(self) = val;
	self->offset += sizeof(hpuint8);
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint16_encode(hp_host16_to_little(val), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint32_encode(hp_host32_to_little(val), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = DDEKIT_COMPACT_encoding_CAPACITY(self);
	hpint32 ret = hp_varint64_encode(hp_host64_to_little(val), DDEKIT_COMPACT_encoding_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	self->offset += buff_size;
	return E_HP_NOERROR;

not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

HP_API hpint32 hp_compact_write_string(HPAbstractWriter *super, const hpchar* str)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 str_len = 0;

	for(; self->offset < self->size; ++(self->offset))
	{
		self->addr[self->offset] = str[str_len++];

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

HP_API hpint32 hp_compact_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val)
{
	return hp_compact_write_hpuint32(super, val);
}
