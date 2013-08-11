#include "hotpot/hp_error.h"
#include <stdio.h>

HP_ERROR_MSG_LIBRARY hp_error_msg_library;


const char* hp_error_get_msg(HP_ERROR_CODE sid)
{
	hpuint32 i;
	for(i = 0; i < hp_error_msg_library.error_list_num; ++i)
		if(hp_error_msg_library.error_list[i].error_code == sid)
		{
			return hp_error_msg_library.error_list[i].error_msg;
		}

	return NULL;
}
