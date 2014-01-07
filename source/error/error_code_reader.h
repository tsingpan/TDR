#ifndef _H_hotlib_hp_error_code_hd_READER_HEADER
#define _H_hotlib_hp_error_code_hd_READER_HEADER
#include "platform/tlibc_platform.h"
#include "error/error_code.h"
#include "protocol/tlibc_abstract_reader.h"
#include "error/error_code.h"
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE_name(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data);
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE_number(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data);
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data);
#endif//_H_hotlib_hp_error_code_hd_READER_HEADER
