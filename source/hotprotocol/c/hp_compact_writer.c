#include "hotprotocol/hp_compact_writer.h"
#include "hotplatform/hp_platform.h"
#include "hotlib/hp_error_code.h"
#include "hotlib/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>


static HP_ERROR_CODE hp_compact_varint16_encode(hpuint16 n, char *buff_ptr, hpuint32 *buff_size)
{
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	buff_ptr[0] = (char)(n | 0x80);
	if (n >= (1 << 7))
	{
		if(*buff_size < 2)
		{
			goto not_enough_byte_size;
		}
		buff_ptr[1] = (char)((n >>  7) | 0x80);
		if (n >= (1 << 14))
		{
			if(*buff_size < 3)
			{
				goto not_enough_byte_size;
			}
			buff_ptr[2] = (char)(n >> 14);
			*buff_size = 3;
			goto done;
		}
		else
		{
			buff_ptr[1] &= 0x7F;
			*buff_size = 2;
			goto done;
		}
	}
	else
	{
		buff_ptr[0] &= 0x7F;
		*buff_size = 1;
		goto done;
	}

done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}



static HP_ERROR_CODE hp_compact_varint32_encode(hpuint32 n, char *buff_ptr, hpuint32 *buff_size)
{
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	buff_ptr[0] = (char)(n | 0x80);
	if (n >= (1 << 7))
	{
		if(*buff_size < 2)
		{
			goto not_enough_byte_size;
		}
		buff_ptr[1] = (char)((n >>  7) | 0x80);
		if (n >= (1 << 14))
		{
			if(*buff_size < 3)
			{
				goto not_enough_byte_size;
			}
			buff_ptr[2] = (char)((n >> 14) | 0x80);

			if (n >= (1 << 21))
			{
				if(*buff_size < 4)
				{
					goto not_enough_byte_size;
				}
				buff_ptr[3] = (char)((n >> 21) | 0x80);

				if (n >= (1 << 28))
				{
					if(*buff_size < 5)
					{
						goto not_enough_byte_size;
					}
					buff_ptr[4] = (char)(n >> 28);
					*buff_size = 5;
					goto done;
				}
				else
				{
					buff_ptr[3] &= 0x7F;
					*buff_size = 4;
					goto done;
				}
			}
			else
			{
				buff_ptr[2] &= 0x7F;
				*buff_size = 3;
				goto done;
			}
		}
		else
		{
			buff_ptr[1] &= 0x7F;
			*buff_size = 2;
			goto done;
		}
	}
	else
	{
		buff_ptr[0] &= 0x7F;
		*buff_size = 1;
		goto done;
	}

done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

static HP_ERROR_CODE hp_compact_varint64_encode(hpuint64 n, char *buff_ptr, hpuint32 *buff_size)
{
	hpuint32 part0 = (hpuint32)(n);
	hpuint32 part1 = (hpuint32)(n >> 28);
	hpuint32 part2 = (hpuint32)(n >> 56);

	hpint32 size;

	if (part2 == 0)
	{
		if (part1 == 0)
		{
			if (part0 < (1 << 14))
			{
				if (part0 < (1 << 7))
				{
					size = 1;
					if(*buff_size < 1)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 1;

					goto size1;
				}
				else
				{
					size = 2;
					if(*buff_size < 2)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 2;

					goto size2;
				}
			}
			else
			{
				if (part0 < (1 << 21))
				{
					size = 3;
					if(*buff_size < 3)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 3;

					goto size3;
				}
				else
				{
					size = 4;
					if(*buff_size < 4)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 4;

					goto size4;
				}
			}
		}
		else
		{
			if (part1 < (1 << 14))
			{
				if (part1 < (1 << 7))
				{
					size = 5;
					if(*buff_size < 5)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 5;
					goto size5;
				}
				else
				{
					size = 6;
					if(*buff_size < 6)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 6;
					goto size6;
				}
			}
			else
			{
				if (part1 < (1 << 21))
				{
					size = 7;
					if(*buff_size < 7)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 7;
					goto size7;
				}
				else
				{
					size = 8;
					if(*buff_size < 8)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 8;
					goto size8;
				}
			}
		}
	}
	else
	{
		if (part2 < (1 << 7))
		{
			size = 9;
			if(*buff_size < 9)
			{
				goto not_enough_byte_size;
			}
			*buff_size = 9;
			goto size9;
		}
		else
		{
			size = 10;
			if(*buff_size < 10)
			{
				goto not_enough_byte_size;
			}
			*buff_size = 10;
			goto size10;
		}
	}


size10:
	buff_ptr[9] = (char)((part2 >>  7) | 0x80);
size9:
	buff_ptr[8] = (char)((part2      ) | 0x80);
size8:
	buff_ptr[7] = (char)((part1 >> 21) | 0x80);
size7:
	buff_ptr[6] = (char)((part1 >> 14) | 0x80);
size6:
	buff_ptr[5] = (char)((part1 >>  7) | 0x80);
size5:
	buff_ptr[4] = (char)((part1      ) | 0x80);
size4:
	buff_ptr[3] = (char)((part0 >> 21) | 0x80);
size3:
	buff_ptr[2] = (char)((part0 >> 14) | 0x80);
size2:
	buff_ptr[1] = (char)((part0 >>  7) | 0x80);
size1:
	buff_ptr[0] = (char)((part0      ) | 0x80);

	buff_ptr[size-1] &= 0x7F;
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


void hp_compact_writer_init(HP_COMPACT_WRITER *self, void *addr, hpuint32 size)
{
	hp_abstract_writer_init(&self->super);

	self->super.write_enum_number = hp_compact_write_enum_number;
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
}

#define COMPACT_WRITER_CAPACITY(self) (self->size - self->offset)
#define COMPACT_WRITER_PTR(self) (self->addr + self->offset)

hpint32 hp_compact_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(hpint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(hpint8*)COMPACT_WRITER_PTR(self) = val;
	self->offset += sizeof(hpint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint16_encode(hp_host16_to_little(hp_zigzag_encode16(val)), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

hpint32 hp_compact_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint32_encode(hp_host32_to_little(hp_zigzag_encode16(val)), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

hpint32 hp_compact_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint64_encode(hp_host64_to_little(hp_zigzag_encode64(val)), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}


hpint32 hp_compact_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(hpuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(hpuint8*)COMPACT_WRITER_PTR(self) = val;
	self->offset += sizeof(hpuint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint16_encode(hp_host16_to_little(val), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

hpint32 hp_compact_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint32_encode(hp_host32_to_little(val), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

hpint32 hp_compact_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	hpint32 ret = hp_compact_varint64_encode(hp_host64_to_little(val), COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

hpint32 hp_compact_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)COMPACT_WRITER_PTR(self) = val;
	self->offset += sizeof(char);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_compact_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*(double*)COMPACT_WRITER_PTR(self) = val;
	self->offset += sizeof(double);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

HP_API hpint32 hp_compact_write_string(HPAbstractWriter *super, const hpchar* str)
{
	HP_COMPACT_WRITER *self = HP_CONTAINER_OF(super, HP_COMPACT_WRITER, super);
	hpuint32 str_len = 0;
	hpint32 ret= E_HP_NOERROR;

	for(; self->offset < self->size; ++(self->offset))
	{
		self->addr[self->offset] = str[str_len++];

		if(self->addr[self->offset] == 0)
		{
			goto done;
		}
	}
	ret = E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
done:
	return ret;
}

HP_API hpint32 hp_compact_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val)
{
	HP_UNUSED(name);
	return hp_compact_write_hpuint32(super, val);
}

HP_API hpint32 hp_compact_write_enum_number(HPAbstractWriter *super, const hpint32 val)
{
	return hp_compact_write_hpint32(super, val);
}
