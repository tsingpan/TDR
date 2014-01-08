#ifndef _H_ERROR
#define _H_ERROR
#include "platform/tlibc_platform.h"

#include "error/error_msg_types.h"


const char* error_search_msg(TD_ERROR_CODE sid);

TD_ERROR_CODE error_init();

#endif//_H_ERROR
