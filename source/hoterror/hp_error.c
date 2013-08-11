#include "hoterror/hp_error.h"
#include "hotprotocol/hp_xml_reader.h"
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

void hp_error_init(const char *root_dir)
{
	if(hp_error_msg_library.error_list_num == 0)
	{
		char language_path[HP_MAX_FILE_PATH_LENGTH];
		HP_XML_READER xml_reader;
		FILE* fin_xml;

		snprintf(language_path, HP_MAX_FILE_PATH_LENGTH, "%s%cresource%clanguage%csimplified_chinese.xml", root_dir, HP_FILE_SEPARATOR, HP_FILE_SEPARATOR, HP_FILE_SEPARATOR);

		fin_xml = fopen(language_path, "r");
		xml_reader_init(&xml_reader, fin_xml);
		read_HP_ERROR_MSG_LIBRARY(&xml_reader.super, &hp_error_msg_library);
		fclose(fin_xml);
	}	
}