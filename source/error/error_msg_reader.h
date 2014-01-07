#ifndef _H_hotlib_hp_error_msg_hd_READER_HEADER
#define _H_hotlib_hp_error_msg_hd_READER_HEADER
#include "platform/tlibc_platform.h"
#include "error/error_code.h"
#include "protocol/tlibc_abstract_reader.h"
#include "error/error_msg.h"
#include "error/error_code_reader.h"
TLIBC_ERROR_CODE read_TLIBC_ERROR_MSG(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_MSG *data);
TLIBC_ERROR_CODE read_TLIBC_ERROR_MSG_LIBRARY(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_MSG_LIBRARY *data);
#endif//_H_hotlib_hp_error_msg_hd_READER_HEADER
