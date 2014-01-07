#ifndef _H_hp_error_hd_TYPES
#define _H_hp_error_hd_TYPES
#include "platform/tlibc_platform.h"

#include "error/error_msg.h"


const char* hp_error_search_msg(const TD_ERROR_MSG_LIBRARY *hp_error_msg_library, TD_ERROR_CODE sid);

void hp_error_init(TD_ERROR_MSG_LIBRARY *hp_error_msg_library);

TD_ERROR_CODE hp_error_load_if_first(TD_ERROR_MSG_LIBRARY *hp_error_msg_library, const char *root_dir);

#endif//_H_hp_error_hd_TYPES
