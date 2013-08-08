#include "language.h"
#include "hotpot/hp_error.h"

#include "hotpot/hp_error_msg_reader.h"
#include "hotprotocol/hp_xml_reader.h"

#include <stdio.h>

hpint32 load_language(HP_ERROR_MSG_LIBRARY *language_lib, const char *file_name)
{
	FILE *fin_xml;
	HP_XML_READER xml_reader;

	fin_xml = fopen(file_name, "r");

	xml_reader_init(&xml_reader, fin_xml);
	read_HP_ERROR_MSG_LIBRARY(&xml_reader.super, language_lib);
	fclose(fin_xml);

	return E_HP_NOERROR;
}

const char* get_string_by_sid(const HP_ERROR_MSG_LIBRARY *language_lib, HP_ERROR_CODE sid)
{
	hpuint32 i;
	for(i = 0; i < language_lib->error_list_num; ++i)
		if(language_lib->error_list[i].error_code == sid)
		{
			return language_lib->error_list[i].error_msg;
		}

	return NULL;
}
