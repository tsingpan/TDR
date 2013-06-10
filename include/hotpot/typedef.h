#ifndef _H_TYPEDEF
#define _H_TYPEDEF

typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
typedef signed long long int64;
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

//varint
typedef int16 varint16;
typedef int32 varint32;
typedef int64 varint64;
typedef uint16 varuint16;
typedef uint32 varuint32;
typedef uint64 varuint64;


#define X_UINT64_MAX ((((uint64)0xffffffffUL) << 32) | 0xffffffffUL)
#define X_INT64_MAX  ((((int64) 0x7fffffffL) << 32) | 0xffffffffL)
#define X_INT64_MIN  ((-X_INT64_MAX) - 1)
#define X_UINT32_MAX ((uint32)0xffffffffUL)
#define X_INT32_MAX  ((int32) 0x7fffffffL)
#define X_INT32_MIN  ((-X_INT32_MAX) - 1)
#define X_UINT16_MAX ((uint16)0xffffUL)
#define X_INT16_MAX  ((int16) 0x7fffL)
#define X_INT16_MIN  ((-X_INT16_MAX) - 1)
#define X_UINT8_MAX  255
#define X_INT8_MAX   127
#define X_INT8_MIN   ((-X_INT8_MAX) - 1)

#endif //_H_TYPEDEF

