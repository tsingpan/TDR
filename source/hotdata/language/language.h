#ifndef _H_LANGUAGE
#define _H_LANGUAGE

#include "hotpot/hp_platform.h"

#include "hotpot/hp_error_msg.h"

hpint32 load_language(HP_ERROR_MSG_LIBRARY *language_lib, const char *file_name);

const char* get_string_by_sid(const HP_ERROR_MSG_LIBRARY *language_lib, HP_ERROR_CODE sid);

#endif//_H_LANGUAGE
