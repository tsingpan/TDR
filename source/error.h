#ifndef _H_ERROR_H
#define _H_ERROR_H

#include "tlibc/platform/tlibc_platform.h"

typedef enum _TD_ERROR_CODE TD_ERROR_CODE;
enum _TD_ERROR_CODE
{
	E_TD_NOERROR = 0,
	E_TD_ERROR = -1,

	E_TD_SCANNER_STACK_SIZE_PROBLEM								= -1000,
	E_TD_SCANNER_READLY_IN_STACK								= -1001,
	E_TD_SCANNER_CAN_NOT_OPEN_FILE								= -1002,
	E_TD_SCANNER_OUT_OF_MEMORY									= -1003,
};

#endif //_H_ERROR_H
