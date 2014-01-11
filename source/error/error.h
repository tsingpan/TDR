#ifndef _H_ERROR
#define _H_ERROR
#include "platform/tlibc_platform.h"

#include "error/error_msg_types.h"


const char* error_search_msg(const TD_ERROR_MSG_LIBRARY *self, TD_ERROR_CODE sid);

TD_ERROR_CODE error_init(TD_ERROR_MSG_LIBRARY *self);

#endif//_H_ERROR
