#ifndef _H_hotlib_hp_error_msg_hd_TYPES
#define _H_hotlib_hp_error_msg_hd_TYPES
#include "platform/tlibc_platform.h"
#include "error/error_code.h"
#define MAX_ERROR_MSG_LENGTH 1024
typedef struct  _TLIBC_ERROR_MSG
{
    TLIBC_ERROR_CODE error_code;
    tchar error_msg[MAX_ERROR_MSG_LENGTH];
}TLIBC_ERROR_MSG;
typedef struct  _TLIBC_ERROR_MSG_LIBRARY
{
	tuint32 error_list_num;
    TLIBC_ERROR_MSG error_list[TLIBC_ERROR_CODE_NUM];
}TLIBC_ERROR_MSG_LIBRARY;
#endif//_H_hotlib_hp_error_msg_hd_TYPES
