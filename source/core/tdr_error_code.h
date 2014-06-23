#ifndef _H_TDR_ERROR_CODE
#define _H_TDR_ERROR_CODE

#include "platform/tdr_platform.h"

typedef enum tdr_error_code_e
{
    E_TDR_NOERROR = 0,
    E_TDR_ERROR = -1,	
    E_TDR_OUT_OF_MEMORY = -2,
	E_TDR_NOT_FOUND = -3,
	E_TDR_SYNTAX = -4,
	E_TDR_MISMATCH = -5,
	E_TDR_ERRNO = -6,
	E_TDR_WOULD_BLOCK = -7,
	E_TDR_BAD_FILE = -8,
	E_TDR_EOF = -9,	
	E_TDR_EMPTY = -10,
	E_TDR_INTEGER_OVERFLOW = -11,
	E_TDR_IGNORE = -12,
	E_TDR_PLEASE_READ_ENUM_NAME = -13,
	E_TDR_FILE_IS_ALREADY_ON_THE_STACK = -14,
}tdr_error_code_t;

#define TDR_ERROR_CODE_NUM 15


#endif
