#ifndef _H_hp_error_msg_hd_TYPES
#define _H_hp_error_msg_hd_TYPES
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error_code.h"
#define	MAX_ERROR_LIST_NUM 65536
#define	MAX_ERROR_MSG_LENGTH 1024
typedef struct _HP_ERROR_MSG
{
	HP_ERROR_CODE error_code;
	hpchar error_msg[MAX_ERROR_MSG_LENGTH];
}HP_ERROR_MSG;
typedef struct _HP_ERROR_MSG_LIBRARY
{
	hpuint32 error_list_num;
	HP_ERROR_MSG error_list[MAX_ERROR_LIST_NUM];
}HP_ERROR_MSG_LIBRARY;
#endif//_H_hp_error_msg_hd_TYPES
