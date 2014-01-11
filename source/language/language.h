#ifndef _H_LANGUAGE_H
#define _H_LANGUAGE_H
#include "platform/tlibc_platform.h"

#include "error.h"

#include "language/language_types.h"


const char* language_string_library_search(const ST_TD_LANGUAGE_STRING_LIBRARY *self, EN_TD_LANGUAGE_STRING sid);

TD_ERROR_CODE language_string_library_init(ST_TD_LANGUAGE_STRING_LIBRARY *self);

#endif//_H_LANGUAGE_H
