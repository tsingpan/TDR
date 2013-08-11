#ifndef _H_HP_PLATFORM_COMMON
#define _H_HP_PLATFORM_COMMON

#include "hotpot/hp_config.h"

typedef signed char hpint8;
typedef signed short int hpint16;
typedef signed int hpint32;
typedef signed long long hpint64;
typedef unsigned char hpuint8;
typedef unsigned short int hpuint16;
typedef unsigned int hpuint32;
typedef unsigned long long hpuint64;
typedef char hpchar;
typedef double hpdouble;
typedef struct _hpbytes
{
	char *ptr;
	hpuint32 len;
}hpbytes;

typedef hpchar* hpstring;


#ifndef NULL
#define NULL				0
#endif

#define HP_UINT64_MAX ((((hpuint64)0xffffffffUL) << 32) | 0xffffffffUL)
#define HP_INT64_MAX  ((((hpint64) 0x7fffffffL) << 32) | 0xffffffffL)
#define HP_INT64_MIN  ((-HP_INT64_MAX) - 1)
#define HP_UINT32_MAX ((hpuint32)0xffffffffUL)
#define HP_INT32_MAX  ((hpint32) 0x7fffffffL)
#define HP_INT32_MIN  ((-HP_INT32_MAX) - 1)
#define HP_UINT16_MAX ((hpuint16)0xffffUL)
#define HP_INT16_MAX  ((hpint16) 0x7fffL)
#define HP_INT16_MIN  ((-HP_INT16_MAX) - 1)
#define HP_UINT8_MAX  255
#define HP_INT8_MAX   127
#define HP_INT8_MIN   ((-HP_INT8_MAX) - 1)

#define HP_BYTE_ORDER (0x1234 & 0xff)
#define HP_LITTLE_ENDIAN (0x34)
#define HP_BIG_ENDIAN (0x12)


#define HP_MAX_ENUM_LENGTH 512
#define HP_MAX_FILE_PATH_LENGTH 1024

#define HP_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define HP_CONTAINER_OF(ptr, type, member) ((type *)((size_t)ptr - HP_OFFSET_OF(type, member)))


#ifndef HP_IN
#define HP_IN
#endif

#ifndef HP_OUT
#define HP_OUT
#endif

#ifndef HP_INOUT
#define HP_INOUT
#endif

#if (defined (SHARED_EXPORTS))
#define HP_API __declspec(dllexport)
#else
#define HP_API extern
#endif


#endif//_H_HP_PLATFORM_COMMON
