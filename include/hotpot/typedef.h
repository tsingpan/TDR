#ifndef _H_TYPEDEF
#define _H_TYPEDEF

typedef signed char hpint8;
typedef signed short int hpint16;
typedef signed int hpint32;
typedef signed long long hpint64;
typedef unsigned char hpuint8;
typedef unsigned short int hpuint16;
typedef unsigned int hpuint32;
typedef unsigned long long hpuint64;


#define HPUINT64_MAX ((((hpuint64)0xffffffffUL) << 32) | 0xffffffffUL)
#define HPINT64_MAX  ((((hpint64) 0x7fffffffL) << 32) | 0xffffffffL)
#define HPINT64_MIN  ((-HPINT64_MAX) - 1)
#define HPUINT32_MAX ((hpuint32)0xffffffffUL)
#define HPINT32_MAX  ((hpint32) 0x7fffffffL)
#define HPINT32_MIN  ((-HPINT32_MAX) - 1)
#define HPUINT16_MAX ((hpuint16)0xffffUL)
#define HPINT16_MAX  ((hpint16) 0x7fffL)
#define HPINT16_MIN  ((-HPINT16_MAX) - 1)
#define HPUINT8_MAX  255
#define HPINT8_MAX   127
#define HPINT8_MIN   ((-HPINT8_MAX) - 1)

#endif //_H_TYPEDEF

