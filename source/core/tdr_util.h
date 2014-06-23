#ifndef _H_TDR_UTIL
#define _H_TDR_UTIL

#ifdef  __cplusplus
extern "C" {
#endif


#include "platform/tdr_platform.h"

#include <stdint.h>

#define TDR_MAX_VARIANT_DECODE_LENGTH 10


#define tdr_swap16(x) (\
	((x & 0x00ff) << 8) | \
	((x & 0xff00) >> 8)  \
	)


#define tdr_swap32(x) (\
	((x & 0x000000ff) << 24) | \
	((x & 0x0000ff00) << 8) | \
	((x & 0x00ff0000) >> 8) | \
	((x & 0xff000000) >> 24)  \
	)

#define tdr_swap64(x) (\
	((x & (tuint64)0x00000000000000ffLL) << 56) | \
	((x & (tuint64)0x000000000000ff00LL) << 40) | \
	((x & (tuint64)0x0000000000ff0000LL) << 24) | \
	((x & (tuint64)0x00000000ff000000LL) << 8) | \
	((x & (tuint64)0x000000ff00000000LL) >> 8) | \
	((x & (tuint64)0x0000ff0000000000LL) >> 24) | \
	((x & (tuint64)0x00ff000000000000LL) >> 40) | \
	((x & (tuint64)0xff00000000000000LL) >> 56)  \
	)

#if TDR_BYTE_ORDER == TDR_LITTLE_ENDIAN 
	#define tdr_little_to_host16(x)
	#define tdr_little_to_host32(x)
	#define tdr_little_to_host64(x)
	#define tdr_host16_to_little(x)
	#define tdr_host32_to_little(x)
	#define tdr_host64_to_little(x)
	#define tdr_size_to_little(x)
	#define tdr_little_to_size(x)
#endif

#if TDR_BYTE_ORDER == TDR_BIG_ENDIAN
	#define tdr_little_to_host16(x) (x = tdr_swap16(x))
	#define tdr_little_to_host32(x) (x = tdr_swap32(x))
	#define tdr_little_to_host64(x) (x = tdr_swap64(x))
	#define tdr_host16_to_little(x) (x = tdr_swap16(x))
	#define tdr_host32_to_little(x) (x = tdr_swap32(x))
	#define tdr_host64_to_little(x) (x = tdr_swap64(x))

	#if TDR_WORDSIZE == 32
		#define tdr_size_to_little(x) tdr_host32_to_little(x)
		#define tdr_little_to_size(x) tdr_little_to_host32(x)
	#endif

	#if TDR_WORDSIZE == 64
		#define tdr_size_to_little(x) tdr_host64_to_little(x)
		#define tdr_little_to_size(x) tdr_little_to_host64(x)
	#endif

#endif



#define tdr_zigzag_encode16(n) ((uint16_t)(((uint16_t)n << 1) ^ ((uint16_t)n >> 15)))
#define tdr_zigzag_encode32(n) ((uint32_t)(((uint32_t)n << 1) ^ ((uint32_t)n >> 31)))
#define tdr_zigzag_encode64(n) ((uint64_t)(((uint64_t)n << 1) ^ ((uint64_t)n >> 63)))

#define tdr_zigzag_decode16(n) ((int16_t)((n >> 1) ^ -(int16_t)(n & 1)))
#define tdr_zigzag_decode32(n) ((int32_t)((n >> 1) ^ -(int32_t)(n & 1)))
#define tdr_zigzag_decode64(n) ((int64_t)((n >> 1) ^ -(int64_t)(n & 1)))


#ifdef  __cplusplus
}
#endif

#endif//_H_TDR_UTIL

