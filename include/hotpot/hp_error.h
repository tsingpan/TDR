#ifndef _H_hp_error_hd_TYPES
#define _H_hp_error_hd_TYPES
#include "hotpot/hp_platform.h"

#include "hotpot/hp_error_msg.h"

extern HP_ERROR_MSG_LIBRARY hp_error_msg_library;

const char* hp_error_get_msg(HP_ERROR_CODE sid);

#endif//_H_hp_error_hd_TYPES
