#ifndef _H_ERROR_CODE_READER
#define _H_ERROR_CODE_READER
#include "platform/tlibc_platform.h"
#include "error/error_code_types.h"
#include "protocol/tlibc_abstract_reader.h"
#include "error/error_code_types.h"
#include "lib/tlibc_error_code.h"
TLIBC_ERROR_CODE read_TD_ERROR_CODE_name(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data);
TLIBC_ERROR_CODE read_TD_ERROR_CODE_number(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data);
TLIBC_ERROR_CODE read_TD_ERROR_CODE(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data);
#endif//_H_ERROR_CODE_READER
