#ifndef _H_hp_error_hd_TYPES
#define _H_hp_error_hd_TYPES
#include "hotpot/hp_platform.h"

#include "hoterror/hp_error_msg.h"

extern HP_ERROR_MSG_LIBRARY hp_error_msg_library;

const char* hp_error_get_msg(HP_ERROR_CODE sid);

void hp_error_init(const char *root_dir);

#endif//_H_hp_error_hd_TYPES
