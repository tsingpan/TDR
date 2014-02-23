#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/protocol/tlibc_xml_reader.h"
#include "error.h"
#include "language/language_reader.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>


const char* language_string_library_search(const ST_TD_LANGUAGE_STRING_LIBRARY *self, EN_TD_LANGUAGE_STRING sid)
{
	uint32_t l, r, m;

	l = 0;
	r = self->language_string_list_num;
	while(l < r)
	{
		m = (l + r - 1) >> 1;
		if(self->language_string_list[m].language_string_number < sid)
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

	return self->language_string_list[l].language_string;
ERROR_RET:
	return NULL;
}

static void sort_library(ST_TD_LANGUAGE_STRING_LIBRARY *self)
{
	uint32_t i,j;
	for(i = 0; i < self->language_string_list_num; ++i)
	{
		ST_TD_LANGUAGE_STRING tmp;
		uint32_t min_j = i;

		for(j = i + 1; j < self->language_string_list_num; ++j)
		{
			if(self->language_string_list[j].language_string_number < self->language_string_list[min_j].language_string_number)
			{
				min_j = j;
			}
		}
		if(i != min_j)
		{
			tmp = self->language_string_list[i];
			self->language_string_list[i] = self->language_string_list[min_j];
			self->language_string_list[min_j] = tmp;
		}
	}
}

TD_ERROR_CODE language_string_library_init(ST_TD_LANGUAGE_STRING_LIBRARY *self)
{
	TLIBC_XML_READER xml_reader;
	char language_path[TLIBC_MAX_PATH_LENGTH];
	
	TD_ERROR_CODE ret = E_TD_NOERROR;

	char root_dir[TLIBC_MAX_PATH_LENGTH];
	snprintf(root_dir, TLIBC_MAX_PATH_LENGTH, getenv(TDATA_HOME));
	if(root_dir[strlen(root_dir) - 1] != TLIBC_FILE_SEPARATOR)
	{
		root_dir[strlen(root_dir) + 1] = 0;
		root_dir[strlen(root_dir)] = TLIBC_FILE_SEPARATOR;
	}
	self->language_string_list_num = 0;
	snprintf(language_path, TLIBC_MAX_PATH_LENGTH, "%s%clanguage%cST_TD_LANGUAGE_STRING_LIBRARY.xml", root_dir, TLIBC_FILE_SEPARATOR, TLIBC_FILE_SEPARATOR);

	tlibc_xml_reader_init(&xml_reader);

	if(tlibc_xml_reader_push_file(&xml_reader, language_path) != E_TLIBC_NOERROR)
	{		
		ret = E_TD_ERROR;
		goto done;
	}
	
	if(tlibc_read_ST_TD_LANGUAGE_STRING_LIBRARY(&xml_reader.super, self) != E_TLIBC_NOERROR)
	{
		ret = E_TD_ERROR;
		goto fini;
	}
	
	sort_library(self);

fini:
	tlibc_xml_reader_pop_file(&xml_reader);
done:
	return ret;
}
