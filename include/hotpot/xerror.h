#ifndef _H_XERROR
#define _H_XERROR


typedef enum tagXE_XBASE
{
	XNOERROR                                                           = -10000,
	XERROR                                                             = -10001,
	XBAD_PARAMETER                                                     = -10002,
	XBYTEBUFF_CAN_NOT_DECODE_THE_TYPE                                  = -10003,
	XNOT_ENOUGH_BYTEBUFF_SIZE                                          = -10004,
}XE_XBASE;

#endif //_H_XERROR

