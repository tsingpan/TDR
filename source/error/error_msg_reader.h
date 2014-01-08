#ifndef _H_ERROR_MSG_READER
#define _H_ERROR_MSG_READER
#include "platform/tlibc_platform.h"
#include "error/error_code_types.h"
#include "protocol/tlibc_abstract_reader.h"
#include "error/error_msg_types.h"
#include "error/error_code_reader.h"
TLIBC_ERROR_CODE read_TD_ERROR_MSG(TLIBC_ABSTRACT_READER *self, TD_ERROR_MSG *data);
TLIBC_ERROR_CODE read_TD_ERROR_MSG_LIBRARY(TLIBC_ABSTRACT_READER *self, TD_ERROR_MSG_LIBRARY *data);
#endif//_H_ERROR_MSG_READER
