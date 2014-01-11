#include "platform/tlibc_platform.h"
#include "error.h"
#include "protocol/tlibc_xml_reader.h"
#include "error_msg_reader.h"
#include "lib/tlibc_error_code.h"

#include <stdio.h>
#include <string.h>

const char* error_search_msg(const TD_ERROR_MSG_LIBRARY *self, TD_ERROR_CODE sid)
{
	tuint32 l, r, m;

	l = 0;
	r = self->error_list_num;
	while(l < r)
	{
		m = (l + r - 1) >> 1;
		if(self->error_list[m].error_code < sid)
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

	return self->error_list[l].error_msg;
ERROR_RET:
	return NULL;
}

static void sort_library(TD_ERROR_MSG_LIBRARY *self)
{
	tuint32 i,j;
	for(i = 0; i < self->error_list_num; ++i)
	{
		TD_ERROR_MSG tmp;
		tuint32 min_j = i;

		for(j = i + 1; j < self->error_list_num; ++j)
		{
			if(self->error_list[j].error_code < self->error_list[min_j].error_code)
			{
				min_j = j;
			}
		}
		if(i != min_j)
		{
			tmp = self->error_list[i];
			self->error_list[i] = self->error_list[min_j];
			self->error_list[min_j] = tmp;
		}
	}
}

TD_ERROR_CODE error_init(TD_ERROR_MSG_LIBRARY *self)
{
	char language_path[TLIBC_MAX_FILE_PATH_LENGTH];
	TLIBC_XML_READER xml_reader;
	FILE* fin_xml;
	TD_ERROR_CODE ret = E_TD_NOERROR;

	char root_dir[TLIBC_MAX_FILE_PATH_LENGTH];
	snprintf(root_dir, TLIBC_MAX_FILE_PATH_LENGTH, getenv("TDATA_HOME"));
	if(root_dir[strlen(root_dir) - 1] != TLIBC_FILE_SEPARATOR)
	{
		root_dir[strlen(root_dir) + 1] = 0;
		root_dir[strlen(root_dir)] = TLIBC_FILE_SEPARATOR;
	}
	self->error_list_num = 0;
	snprintf(language_path, TLIBC_MAX_FILE_PATH_LENGTH, "%s%cerror%cTD_ERROR_MSG_LIBRARY.xml", root_dir, TLIBC_FILE_SEPARATOR, TLIBC_FILE_SEPARATOR, TLIBC_FILE_SEPARATOR);
	fin_xml = fopen(language_path, "r");
	if(fin_xml == NULL)
	{
		ret = E_TD_CAN_NOT_OPEN_FILE;
		goto done;
	}

	xml_reader_init(&xml_reader, fin_xml);
	if(read_TD_ERROR_MSG_LIBRARY(&xml_reader.super, self) != E_TLIBC_NOERROR)
	{
		ret = E_TD_CAN_NOT_OPEN_FILE;
		goto f_done;
	}	
	sort_library(self);
f_done:
	fclose(fin_xml);
done:
	return ret;
}
