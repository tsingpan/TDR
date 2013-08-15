#ifndef _H_hp_error_hd_TYPES
#define _H_hp_error_hd_TYPES
#include "hotpot/hp_platform.h"

#include "hoterror/hp_error_msg.h"


extern HP_ERROR_MSG_LIBRARY hp_error_msg_library;
extern HP_ERROR_CODE hp_errno;
extern char hp_errmsg[MAX_ERROR_MSG_LENGTH];

const char* hp_error_search_msg(HP_ERROR_CODE sid);

void hp_error_load_if_first(const char *root_dir);

#endif//_H_hp_error_hd_TYPES
