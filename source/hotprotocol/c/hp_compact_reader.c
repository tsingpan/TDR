#include "hotprotocol/hp_compact_reader.h"
#include "hotpot/hp_platform.h"
#include "hotlib/hp_error_code.h"
#include "hotlib/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"

#include <string.h>
#include <assert.h>

static HP_ERROR_CODE hp_compact_varint16_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint16 *result)
{
	hpuint8 b;

	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 0);
	*result = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 1);
	*result |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 2);
	*result |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	return E_HP_ERROR;
done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


static HP_ERROR_CODE hp_compact_varint32_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint32 *result)
{
	hpuint8 b;

	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 0);
	*result = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 1);
	*result |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 2);
	*result |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	if(*buff_size < 4)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 3);
	*result |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 4;
		goto done;
	}

	if(*buff_size < 5)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 4);
	*result |= (b & 0x7F) << 28;
	if (!(b & 0x80))
	{
		*buff_size = 5;
		goto done;
	}	

	return E_HP_ERROR;
done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


static HP_ERROR_CODE hp_compact_varint64_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint64 *result)
{
	hpuint8 b;

	hpuint32 par0 = 0;
	hpuint32 par1 = 0;
	hpuint32 par2 = 0;

	//par0
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 0);
	par0 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 1);
	par0 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 2);
	par0 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	if(*buff_size < 4)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 3);
	par0 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 4;
		goto done;
	}

	//par1
	if(*buff_size < 5)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 4);
	par1 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 5;
		goto done;
	}

	if(*buff_size < 6)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 5);
	par1 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 6;
		goto done;
	}

	if(*buff_size < 7)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 6);
	par1 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 7;
		goto done;
	}

	if(*buff_size < 8)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 7);
	par1 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 8;
		goto done;
	}

	//par2
	if(*buff_size < 9)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 8);
	par2 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 9;
		goto done;
	}

	if(*buff_size < 10)
	{
		goto not_enough_byte_size;
	}
	b = *(hpuint8*)(buff_ptr + 9);
	par2 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 10;
		goto done;
	}
	return E_HP_ERROR;
done:
	*result = ((hpuint64)par0) | ((hpuint64)par1 << 28 )| ((hpuint64)par2 << 56);
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

void hp_compact_reader_init(HP_COMPACT_READER *self, const void *addr, hpuint32 size)
{
	hp_abstract_reader_init(&self->super);

	self->super.read_enum_number = hp_compact_read_enum_number;
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
}

#define COMPACT_READER_CAPACITY(self) (self->size - self->offset)
#define COMPACT_READER_PTR(self) (self->addr + self->offset)


hpint32 hp_compact_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(hpint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpint8*)COMPACT_READER_PTR(self);
	self->offset += sizeof(hpint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint16_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_zigzag_decode16(hp_little_to_host16(res));
	self->offset +=buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint32_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_zigzag_decode32(hp_little_to_host32(res));
	self->offset +=buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint64_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_zigzag_decode64(hp_little_to_host64(res));
	self->offset +=buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(hpuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpuint8*)COMPACT_READER_PTR(self);
	self->offset += sizeof(hpuint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint16_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_little_to_host16(res);
	self->offset += buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint32_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_little_to_host32(res);
	self->offset += buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_READER_CAPACITY(self);
	int ret = hp_compact_varint64_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	*val = hp_little_to_host64(res);
	self->offset += buff_size;
done:
	return ret;
}

hpint32 hp_compact_read_hpchar(HPAbstractReader *super, char *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(char*)COMPACT_READER_PTR(self);
	self->offset += sizeof(char);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(double*)COMPACT_READER_PTR(self);
	self->offset += sizeof(double);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_compact_read_string(HPAbstractReader *super, hpchar* str, hpuint32 str_length)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, super);
	hpuint32 str_len = 0;
	hpint32 ret = E_HP_NOERROR;

	for(; self->offset < self->size; ++(self->offset))
	{
		if(str_len >= str_length)
		{
			ret = E_HP_NOT_ENOUGH_STRING_SIZE;
			goto done;
		}
		str[str_len++] = self->addr[self->offset];

		if(self->addr[self->offset] == 0)
		{
			goto done;
		}
	}
	ret = E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
done:
	return ret;
}

HP_API hpint32 hp_compact_read_counter(HPAbstractReader *super, const char *name, hpuint32 *val)
{
	HP_UNUSED(name);
	return hp_compact_read_hpuint32(super, val);
}

hpint32 hp_compact_read_enum_number(HPAbstractReader *super, hpint32 *val)
{
	return hp_compact_read_hpint32(super, val);
}