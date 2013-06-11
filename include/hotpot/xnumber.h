#ifndef _H_XNUMBER
#define _H_XNUMBER

#include "typedef.h" 
#include "xbase.h"



#define MAX_XVAR_DECODE_LENGTH 10



#define _BYTE_ORDER (0x1234 & 0xff)
#define _LITTLE_ENDIAN (0x34)
#define _BIG_ENDIAN (0x12)

#define _swap16(x) (\
	((x & 0x00ff) << 8) | \
	((x & 0xff00) >> 8)  \
	)


#define _swap32(x) (\
	((x & 0x000000ff) << 24) | \
	((x & 0x0000ff00) << 8) | \
	((x & 0x00ff0000) >> 8) | \
	((x & 0xff000000) >> 24)  \
	)

#define _swap64(x) (\
	((x & (hpuint64)0x00000000000000ffLL) << 56) | \
	((x & (hpuint64)0x000000000000ff00LL) << 40) | \
	((x & (hpuint64)0x0000000000ff0000LL) << 24) | \
	((x & (hpuint64)0x00000000ff000000LL) << 8) | \
	((x & (hpuint64)0x000000ff00000000LL) >> 8) | \
	((x & (hpuint64)0x0000ff0000000000LL) >> 24) | \
	((x & (hpuint64)0x00ff000000000000LL) >> 40) | \
	((x & (hpuint64)0xff00000000000000LL) >> 56)  \
	)

#if _BYTE_ORDER == _LITTLE_ENDIAN 
#define ntoh16(x) _swap16(x)
#define ntoh32(x) _swap32(x)
#define ntoh64(x) _swap64(x)
#define hton16(x) _swap16(x)
#define hton32(x) _swap32(x)
#define hton64(x) _swap64(x)

#define little_to_host16(x) (x)
#define little_to_host32(x) (x)
#define little_to_host64(x) (x)
#define host16_to_little(x) (x)
#define host32_to_little(x) (x)
#define host64_to_little(x) (x)
#endif

#if _BYTE_ORDER == _BIG_ENDIAN
#define ntoh16(x) (x) 
#define ntoh32(x) (x)
#define ntoh64(x) (x)
#define hton16(x) (x) 
#define hton32(x) (x)
#define hton64(x) (x)

#define little_to_host16(x) _swap16(x)
#define little_to_host32(x) _swap32(x)
#define little_to_host64(x) _swap64(x)
#define host16_to_little(x) _swap16(x)
#define host32_to_little(x) _swap32(x)
#define host64_to_little(x) _swap64(x)

#endif



#define ZigZagEncode16(n) ((hpuint16)(((hpint16)n << 1) ^ ((hpint16)n >> 15)))
#define ZigZagEncode32(n) ((hpuint32)(((hpint32)n << 1) ^ ((hpint32)n >> 31)))
#define ZigZagEncode64(n) ((hpuint64)(((hpint64)n << 1) ^ ((hpint64)n >> 63)))

#define ZigZagDecode16(n) ((hpint16)((n >> 1) ^ -(hpint16)(n & 1)))
#define ZigZagDecode32(n) ((hpint32)((n >> 1) ^ -(hpint32)(n & 1)))
#define ZigZagDecode64(n) ((hpint64)((n >> 1) ^ -(hpint64)(n & 1)))

//3
XAPI int varint16_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint16 *result);
XAPI int varint16_encode(hpuint16 n, char *buff_ptr, hpuint32 *buff_size);

//5
XAPI int varint32_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint32 *result);
XAPI int varint32_encode(hpuint32 n, char *buff_ptr, hpuint32 *buff_size);

//10
XAPI int varint64_decode(const char *buff_ptr, hpuint32 *buff_size, hpuint64 *result);
XAPI int varint64_encode(hpuint64 n, char *buff_ptr, hpuint32 *buff_size);


#endif //_H_XNUMBER


