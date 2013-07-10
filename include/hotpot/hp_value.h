#ifndef _H_HP_READ
#define _H_HP_READ

#include "hotpot/hp_platform.h"

typedef enum _HPType
{
	E_HP_INT8			= 0,
	E_HP_INT16			= 1,
	E_HP_INT32			= 2,
	E_HP_INT64			= 3,
	E_HP_UINT8			= 4,
	E_HP_UINT16			= 5,
	E_HP_UINT32			= 6,
	E_HP_UINT64			= 7,
	E_HP_CHAR			= 8,
	E_HP_BYTES			= 9,
	E_HP_STRING			= 10,
	E_HP_DOUBLE			= 11,
	E_HP_BOOL			= 12,
	E_HP_NULL			= 13,
	E_HP_VECTOR			= 14,
	E_HP_OBJECT			= 15,
}HPType;

typedef union _HPValue
{
	hpint8 i8;
	hpint16 i16;
	hpint32 i32;
	hpint64 i64;
	hpuint8 ui8;
	hpuint16 ui16;
	hpuint32 ui32;
	hpuint64 ui64;
	hpchar c;	
	hpdouble d;
	hpbool b;
	hpbytes bytes;
	hpstring str;
}HPValue;

typedef struct _HPVar
{
	HPType type;
	HPValue val;
}HPVar;

#endif //_H_HP_READ

