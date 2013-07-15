#include "hotprotocol/hp_compact_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>

hpint32 ddekit_compact_encoding_writer_init(HP_COMPACT_WRITER *self, void *addr, hpuint32 size)
{
	self->dpw.write_struct_begin = ddekit_compact_encoding_write_struct_begin;
	self->dpw.write_struct_end = ddekit_compact_encoding_write_struct_end;
	self->dpw.write_vector_begin = ddekit_compact_encoding_write_vector_begin;
	self->dpw.write_vector_end = ddekit_compact_encoding_write_vector_end;
	self->dpw.write_field_begin = ddekit_compact_encoding_write_field_begin;
	self->dpw.write_field_end = ddekit_compact_encoding_write_field_end;
	self->dpw.write_enum = ddekit_compact_encoding_write_enum;
	self->dpw.write_char = ddekit_compact_encoding_write_hpchar;
	self->dpw.write_double = ddekit_compact_encoding_write_hpdouble;
	self->dpw.write_int8 = ddekit_compact_encoding_write_hpint8;
	self->dpw.write_int16 = ddekit_compact_encoding_write_hpint16;
	self->dpw.write_int32 = ddekit_compact_encoding_write_hpint32;
	self->dpw.write_int64 = ddekit_compact_encoding_write_hpint64;
	self->dpw.write_uint8 = ddekit_compact_encoding_write_hpuint8;
	self->dpw.write_uint16 = ddekit_compact_encoding_write_hpuint16;
	self->dpw.write_uint32 = ddekit_compact_encoding_write_hpuint32;
	self->dpw.write_uint64 = ddekit_compact_encoding_write_hpuint64;
	

	self->addr = addr;
	self->size = size;
	self->offset = 0;

	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_writer_fini(HP_COMPACT_WRITER *self)
{
	self->dpw.write_struct_begin = NULL;
	self->dpw.write_struct_end = NULL;
	self->dpw.write_field_begin = NULL;
	self->dpw.write_field_end = NULL;
	self->dpw.write_vector_begin = NULL;
	self->dpw.write_vector_end = NULL;
	self->dpw.write_enum = NULL;
	self->dpw.write_char = NULL;
	self->dpw.write_double = NULL;
	self->dpw.write_int8 = NULL;
	self->dpw.write_int16 = NULL;
	self->dpw.write_int32 = NULL;
	self->dpw.write_int64 = NULL;
	self->dpw.write_uint8 = NULL;
	self->dpw.write_uint16 = NULL;
	self->dpw.write_uint32 = NULL;
	self->dpw.write_uint64 = NULL;

	self->addr = NULL;
	self->size = 0;
	self->offset = 0;
	return E_HP_NOERROR;
}

#define DDEKIT_COMPACT_encoding_CAPACITY(self) (self->size - self->offset)

#define DDEKIT_COMPACT_encoding_PTR(self) (self->addr + self->offset)

hpint32 ddekit_compact_encoding_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_write_vector_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_write_vector_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_write_field_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_write_field_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_compact_encoding_write_enum(HPAbstractWriter *super, const hpint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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


hpint32 ddekit_compact_encoding_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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

hpint32 ddekit_compact_encoding_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, dpw);
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
