#ifndef _H_hp_error_hd_TYPES
#define _H_hp_error_hd_TYPES
#include "hotpot/hp_platform.h"

#include "hoterror/hp_error_msg.h"


const char* hp_error_search_msg(const HP_ERROR_MSG_LIBRARY *hp_error_msg_library, HP_ERROR_CODE sid);

void hp_error_init(HP_ERROR_MSG_LIBRARY *hp_error_msg_library);

HP_ERROR_CODE hp_error_load_if_first(HP_ERROR_MSG_LIBRARY *hp_error_msg_library, const char *root_dir);

#endif//_H_hp_error_hd_TYPES
