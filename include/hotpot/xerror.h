#ifndef _H_XERROR
#define _H_XERROR

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"XERROR"的X协议的C语言头文件
//								请务必保证头文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma pack(push,1)

#include "typedef.h"



typedef enum tagXE_XBASE
{
	XNOERROR                                                           = -10000,
	XERROR                                                             = -10001,
	XBAD_PARAMETER                                                     = -10002,
	XBYTEBUFF_CAN_NOT_DECODE_THE_TYPE                                  = -10003,
	XNOT_ENOUGH_BYTEBUFF_SIZE                                          = -10004,
}XE_XBASE;


typedef enum tagXE_OS
{
	XWOULDBLOCK                                                        = -20000,
	XINPROCESS                                                         = -20001,
}XE_OS;


typedef enum tagXE_XSN
{
	XSTACK_EMPTY                                                       = -30000,
	XSTACK_OVERFOLLOW                                                  = -30001,
}XE_XSN;



#pragma pack(pop)

#endif //_H_XERROR

