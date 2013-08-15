#include "hotpot/hp_platform.h"
#include "hoterror/hp_error.h"
#include "hotprotocol/hp_xml_reader.h"
#include "hoterror/hp_error_msg_reader.h"

#include <stdio.h>

HP_ERROR_MSG_LIBRARY hp_error_msg_library;
HP_ERROR_CODE hp_errno;
char hp_errmsg[MAX_ERROR_MSG_LENGTH];

const char* hp_error_search_msg(HP_ERROR_CODE sid)
{
	hpuint32 l, r, m;

	l = 0;
	r = hp_error_msg_library.error_list_num;
	while(l < r)
	{
		m = (l + r - 1) >> 1;
		if(hp_error_msg_library.error_list[m].error_code < sid)
		{
			l = m + 1;
		}
		else
		{
			r = m;
		}
	}
	if(l != r)
	{
		goto ERROR_RET;
	}

	return hp_error_msg_library.error_list[l].error_msg;
ERROR_RET:
	return NULL;
}

static void sort_library()
{
	hpuint32 i,j;
	for(i = 0; i < hp_error_msg_library.error_list_num; ++i)
	{
		HP_ERROR_MSG tmp;
		hpuint32 min_j = i;

		for(j = i + 1; j < hp_error_msg_library.error_list_num; ++j)
		{
			if(hp_error_msg_library.error_list[j].error_code < hp_error_msg_library.error_list[min_j].error_code)
			{
				min_j = j;
			}
		}
		if(i != min_j)
		{
			tmp = hp_error_msg_library.error_list[i];
			hp_error_msg_library.error_list[i] = hp_error_msg_library.error_list[min_j];
			hp_error_msg_library.error_list[min_j] = tmp;
		}
	}
}

HP_ERROR_CODE hp_error_load_if_first(const char *root_dir)
{
	char language_path[HP_MAX_FILE_PATH_LENGTH];
	HP_XML_READER xml_reader;
	FILE* fin_xml;
	HP_ERROR_CODE ret = E_HP_NOERROR;

	if(hp_error_msg_library.error_list_num != 0)
	{
		goto done;
	}
	snprintf(language_path, HP_MAX_FILE_PATH_LENGTH, "%s%clanguage%csimplified_chinese.xml", root_dir, HP_FILE_SEPARATOR, HP_FILE_SEPARATOR, HP_FILE_SEPARATOR);
	fin_xml = fopen(language_path, "r");
	if(fin_xml == NULL)
	{
		ret = E_HP_CAN_NOT_OPEN_FILE;
		goto done;
	}

	xml_reader_init(&xml_reader, fin_xml);
	ret = read_HP_ERROR_MSG_LIBRARY(&xml_reader.super, &hp_error_msg_library);
	if(ret != E_HP_NOERROR)
	{
		goto f_done;
	}	
	sort_library();
f_done:
	fclose(fin_xml);
done:
	hp_errno = ret;
	return ret;
}
