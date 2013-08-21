#ifndef _H_HP_NUMBER
#define _H_HP_NUMBER

#include "hotplatform/hp_platform.h"



#define HP_MAX_VARIANT_DECODE_LENGTH 10


#define hp_swap16(x) (\
	((x & 0x00ff) << 8) | \
	((x & 0xff00) >> 8)  \
	)


#define hp_swap32(x) (\
	((x & 0x000000ff) << 24) | \
	((x & 0x0000ff00) << 8) | \
	((x & 0x00ff0000) >> 8) | \
	((x & 0xff000000) >> 24)  \
	)

#define hp_swap64(x) (\
	((x & (hpuint64)0x00000000000000ffLL) << 56) | \
	((x & (hpuint64)0x000000000000ff00LL) << 40) | \
	((x & (hpuint64)0x0000000000ff0000LL) << 24) | \
	((x & (hpuint64)0x00000000ff000000LL) << 8) | \
	((x & (hpuint64)0x000000ff00000000LL) >> 8) | \
	((x & (hpuint64)0x0000ff0000000000LL) >> 24) | \
	((x & (hpuint64)0x00ff000000000000LL) >> 40) | \
	((x & (hpuint64)0xff00000000000000LL) >> 56)  \
	)

#if HP_BYTE_ORDER == HP_LITTLE_ENDIAN 
	#define hp_ntoh16(x) hp_swap16(x)
	#define hp_ntoh32(x) hp_swap32(x)
	#define hp_ntoh64(x) hp_swap64(x)
	#define hp_hton16(x) hp_swap16(x)
	#define hp_hton32(x) hp_swap32(x)
	#define hp_hton64(x) hp_swap64(x)

	#define hp_little_to_host16(x) (x)
	#define hp_little_to_host32(x) (x)
	#define hp_little_to_host64(x) (x)
	#define hp_host16_to_little(x) (x)
	#define hp_host32_to_little(x) (x)
	#define hp_host64_to_little(x) (x)
#endif

#if HP_BYTE_ORDER == HP_BIG_ENDIAN
	#define hp_ntoh16(x) (x) 
	#define hp_ntoh32(x) (x)
	#define hp_ntoh64(x) (x)
	#define hp_hton16(x) (x) 
	#define hp_hton32(x) (x)
	#define hp_hton64(x) (x)

	#define hp_little_to_host16(x) hp_swap16(x)
	#define hp_little_to_host32(x) hp_swap32(x)
	#define hp_little_to_host64(x) hp_swap64(x)
	#define hp_host16_to_little(x) hp_swap16(x)
	#define hp_host32_to_little(x) hp_swap32(x)
	#define hp_host64_to_little(x) hp_swap64(x)
#endif



#define hp_zigzag_encode16(n) ((hpuint16)(((hpint16)n << 1) ^ ((hpint16)n >> 15)))
#define hp_zigzag_encode32(n) ((hpuint32)(((hpint32)n << 1) ^ ((hpint32)n >> 31)))
#define hp_zigzag_encode64(n) ((hpuint64)(((hpint64)n << 1) ^ ((hpint64)n >> 63)))

#define hp_zigzag_decode16(n) ((hpint16)((n >> 1) ^ -(hpint16)(n & 1)))
#define hp_zigzag_decode32(n) ((hpint32)((n >> 1) ^ -(hpint32)(n & 1)))
#define hp_zigzag_decode64(n) ((hpint64)((n >> 1) ^ -(hpint64)(n & 1)))

#endif //_H_XNUMBER


