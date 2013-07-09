#include "language/language_reader.h"

#include "language/language_types.h"
#include "hotprotocol/hp_abstract_reader.h"

hpint32 read_LanguageStringID(HPAbstractReader *self, LanguageStringID *sid)
{
	return E_HP_NOERROR;
}

hpint32 read_LanguageString(HPAbstractReader *self, LanguageString *data)
{
	read_LanguageStringID(self, &data->sid);
	read_hpstring(self, data->str, MAX_LANGUAGE_STRING_LENGTH);
	return E_HP_NOERROR;
}

hpint32 read_LanguageLib(HPAbstractReader *self, LanguageLib *data)
{
	hpuint32 i;

	read_hpuint32(self, &data->str_num);
	for(i = 0;i < MAX_LANGUAGE_STRING_NUM; ++i)
	{
		if(i >= data->str_num)
		{
			break;
		}
		read_LanguageString(self, &data->str[i]);
	}

	return E_HP_NOERROR;
}
