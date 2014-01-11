#ifndef _H_LANGUAGE_H
#define _H_LANGUAGE_H
#include "platform/tlibc_platform.h"

#include "error.h"

#include "language/language_types.h"


const char* error_search_msg(const ST_TD_LANGUAGE_STRING_LIBRARY *self, TD_ERROR_CODE sid);

TD_ERROR_CODE error_init(ST_TD_LANGUAGE_STRING_LIBRARY *self);

#endif//_H_LANGUAGE_H
