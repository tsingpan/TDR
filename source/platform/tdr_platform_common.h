#ifndef _H_TDR_PLATFORM_COMMON
#define _H_TDR_PLATFORM_COMMON

#ifdef  __cplusplus
extern "C" {
#endif


#include <stdint.h>

#ifndef NULL
#define NULL				0
#endif

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif


#ifndef TDR_IN
#define TDR_IN
#endif

#ifndef TDR_OUT
#define TDR_OUT
#endif

#ifndef TDR_INOUT
#define TDR_INOUT
#endif

#define TDR_UNUSED(x) (void)(x)

#define TDR_FILE_SEPARATOR '/'


#define TDR_UINT64_MAX ((((uint64_t)0xffffffffUL) << 32) | 0xffffffffUL)
#define TDR_UINT32_MAX ((uint32_t)0xffffffffUL)
#define TDR_UINT16_MAX ((uint16_t)0xffffUL)
#define TDR_UINT8_MAX  255

#define TDR_INT64_MAX  ((((int64_t) 0x7fffffffL) << 32) | 0xffffffffL)
#define TDR_INT64_MIN  ((-TDR_INT64_MAX) - 1)
#define TDR_INT32_MAX  ((int32_t) 0x7fffffffL)
#define TDR_INT32_MIN  ((-TDR_INT32_MAX) - 1)
#define TDR_INT16_MAX  ((int16_t) 0x7fffL)
#define TDR_INT16_MIN  ((-TDR_INT16_MAX) - 1)
#define TDR_INT8_MAX   127
#define TDR_INT8_MIN   ((-TDR_INT8_MAX) - 1)

#define TDR_BYTE_ORDER (0x1234 & 0xff)
#define TDR_LITTLE_ENDIAN (0x34)
#define TDR_BIG_ENDIAN (0x12)

#define TDR_MAX_LENGTH_OF_IDENTIFIER 255


#define TDR_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define TDR_CONTAINER_OF(ptr, type, member) ((type *)((size_t)ptr - TDR_OFFSET_OF(type, member)))

#define TDR_MAX_PATH_LENGTH 1024


#ifdef  __cplusplus
}
#endif

#endif
