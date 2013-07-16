#include "hotprotocol/hp_compact_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>

hpint32 hp_compact_writer_init(HP_COMPACT_WRITER *self, void *addr, hpuint32 size)
{
	memset(&self->super, 0, sizeof(self->super));

	self->super.write_struct_begin = hp_compact_write_struct_begin;
	self->super.write_struct_end = hp_compact_write_struct_end;
	self->super.write_vector_begin = hp_compact_write_vector_begin;
	self->super.write_vector_end = hp_compact_write_vector_end;
	self->super.write_field_begin = hp_compact_write_field_begin;
	self->super.write_field_end = hp_compact_write_field_end;
	self->super.write_enum = hp_compact_write_enum;
	self->super.write_char = hp_compact_write_hpchar;
	self->super.write_double = hp_compact_write_hpdouble;
	self->super.write_int8 = hp_compact_write_hpint8;
	self->super.write_int16 = hp_compact_write_hpint16;
	self->super.write_int32 = hp_compact_write_hpint32;
	self->super.write_int64 = hp_compact_write_hpint64;
	self->super.write_uint8 = hp_compact_write_hpuint8;
	self->super.write_uint16 = hp_compact_write_hpuint16;
	self->super.write_uint32 = hp_compact_write_hpuint32;
	self->super.write_uint64 = hp_compact_write_hpuint64;
	self->super.write_string = hp_compact_write_string;
	
	

	self->addr = addr;
	self->size = size;
	self->offset = 0;

	return E_HP_NOERROR;
}

hpint32 hp_compact_writer_fini(HP_COMPACT_WRITER *self)
{
	self->super.write_struct_begin = NULL;
	self->super.write_struct_end = NULL;
	self->super.write_field_begin = NULL;
	self->super.write_field_end = NULL;
	self->super.write_vector_begin = NULL;
	self->super.write_vector_end = NULL;
	self->super.write_enum = NULL;
	self->super.write_char = NULL;
	self->super.write_double = NULL;
	self->super.write_int8 = NULL;
	self->super.write_int16 = NULL;
	self->super.write_int32 = NULL;
	self->super.write_int64 = NULL;
	self->super.write_uint8 = NULL;
	self->super.write_uint16 = NULL;
	self->super.write_uint32 = NULL;
	self->super.write_uint64 = NULL;

	self->addr = NULL;
	self->size = 0;
	self->offset = 0;
	return E_HP_NOERROR;
}

#define DDEKIT_COMPACT_encoding_CAPACITY(self) (self->size - self->offset)

#define DDEKIT_COMPACT_encoding_PTR(self) (self->addr + self->offset)

hpint32 hp_compact_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 hp_compact_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 hp_compact_write_vector_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 hp_compact_write_vector_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 hp_compact_write_field_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 hp_compact_write_field_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	return E_HP_NOERROR;
}

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
