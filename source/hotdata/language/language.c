#include "language/language.h"
#include "hotpot/hp_error.h"

#include "language/language_reader.h"
#include "hotprotocol/hp_xml_reader.h"

#include <stdio.h>

hpint32 load_language(LanguageLib *language_lib, const char *file_name)
{
	FILE *fin_xml;
	HP_XML_READER xml_reader;
	hpint32 ret;

	fin_xml = fopen(file_name, "r");

	xml_reader_init(&xml_reader, fin_xml);
	ret = read_LanguageLib(&xml_reader.super, language_lib);
	fclose(fin_xml);

	return E_HP_NOERROR;
}

const char* get_string_by_sid(const LanguageLib *language_lib, LanguageStringID sid)
{
	hpuint32 i;
	for(i = 0; i < language_lib->str_num; ++i)
		if(language_lib->str[i].sid == sid)
		{
			return language_lib->str[i].str;
		}

	return NULL;
}
