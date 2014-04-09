#ifndef _H_ERROR_H
#define _H_ERROR_H

#include "platform/tlibc_platform.h"

typedef enum td_error_code_e td_error_code_t;
enum td_error_code_e
{
	E_TD_NOERROR = 0,
	E_TD_ERROR = -1,

	E_TD_SCANNER_STACK_SIZE_PROBLEM								= -1000,
	E_TD_SCANNER_READLY_IN_STACK								= -1001,
	E_TD_SCANNER_CAN_NOT_OPEN_FILE								= -1002,
	E_TD_SCANNER_OUT_OF_MEMORY									= -1003,
};

#endif //_H_ERROR_H
