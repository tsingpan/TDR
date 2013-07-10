#ifndef _H_LANGUAGE
#define _H_LANGUAGE

#include "hotpot/hp_platform.h"

#include "language_types.h"

hpint32 load_language(LanguageLib *language_lib, const char *file_name);

const char* get_string_by_sid(const LanguageLib *language_lib, LanguageStringID sid);

#endif//_H_LANGUAGE
