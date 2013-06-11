#include "hotpot/hp_number.h"
#include "hotpot/hp_error.h"

#include <string.h>


hpint32 hp_varint16_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint16 *result)
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

hpint32 hp_varint16_encode(hpuint16 n, char *buff_ptr, hpuint32 *buff_size)
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

	//unreachable here
	//return XERROR;

done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}

hpint32 hp_varint32_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint32 *result)
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


hpint32 hp_varint32_encode(hpuint32 n, char *buff_ptr, hpuint32 *buff_size)
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

	//unreachable here
	//return XERROR;

done:
	return E_HP_NOERROR;

not_enough_byte_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 hp_varint64_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint64 *result)
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

hpint32 hp_varint64_encode(hpuint64 n, char *buff_ptr, hpuint32 *buff_size)
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

	//unreachable here
	//return XERROR;

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
