#ifndef _H_XBASE_COMMON
#define _H_XBASE_COMMON

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"XBASE_COMMON"的X协议的C语言头文件
//								请务必保证头文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma pack(push,1)

#include "typedef.h"


#define INVALID_INDEX                                                  (0xffffffff)
#define VALUE_MASK                                                     (0100)
#define INTEGER_MASK                                                   (040)
#define SIGN_MASK                                                      (020)

typedef enum tagXBOOL
{
	XTRUE                                                              = 1,
	XFALSE                                                             = 0,
}XBOOL;


typedef enum tagXTYPE
{
	X_STRUCT                                                           = 01,
	X_UNION                                                            = 02,
	X_CHAR                                                             = 0100,
	X_DOUBLE                                                           = 0102,
	X_UINT8                                                            = 0140,
	X_UINT16                                                           = 0141,
	X_UINT32                                                           = 0142,
	X_UINT64                                                           = 0143,
	X_VARUINT16                                                        = 0144,
	X_VARUINT32                                                        = 0145,
	X_VARUINT64                                                        = 0146,
	X_INT8                                                             = 0160,
	X_INT16                                                            = 0161,
	X_INT32                                                            = 0162,
	X_INT64                                                            = 0163,
	X_VARINT16                                                         = 0164,
	X_VARINT32                                                         = 0165,
	X_VARINT64                                                         = 0166,
	X_ENUM                                                             = 0167,
}XTYPE;


typedef union tagXVALUE
{
	char                                                               c;
	double                                                             d;
	uint8                                                              ui8;
	uint16                                                             ui16;
	uint32                                                             ui32;
	uint64                                                             ui64;
	varuint16                                                          varui16;
	varuint32                                                          varui32;
	varuint64                                                          varui64;
	int8                                                               i8;
	int16                                                              i16;
	int32                                                              i32;
	int64                                                              i64;
	varint16                                                           vari16;
	varint32                                                           vari32;
	varint64                                                           vari64;
}XVALUE;


typedef struct tagXVAR
{
	XTYPE                                                              xtype;
	XVALUE                                                             xvalue;
}XVAR;



#pragma pack(pop)

#endif //_H_XBASE_COMMON

