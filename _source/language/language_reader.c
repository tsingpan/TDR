#include "language/language_reader.h"

#include "language/language_types.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>

hpint32 read_LanguageStringID(HPAbstractReader *self, LanguageStringID *sid)
{
	hpchar enum_name[1024];

	if(read_enum_name(self, enum_name, 1024) == E_HP_NOERROR)
	{
		//这里需要加速
		if(strcmp(enum_name, "E_SID_NOERROR") == 0)
		{
			*sid = E_SID_NOERROR;
		}
		else if(strcmp(enum_name, "E_SID_ERROR") == 0)
		{
			*sid = E_SID_ERROR;
		}
		else if(strcmp(enum_name, "E_SID_CONSTANT_REDEFINITION") == 0)
		{
			*sid = E_SID_CONSTANT_REDEFINITION;
		}
		else if(strcmp(enum_name, "E_SID_INTEGER_OVERFLOW") == 0)
		{
			*sid = E_SID_INTEGER_OVERFLOW;
		}
		else if(strcmp(enum_name, "E_SID_CAN_NOT_OPEN_FILE") == 0)
		{
			*sid = E_SID_CAN_NOT_OPEN_FILE;
		}
		else if(strcmp(enum_name, "E_SID_CAN_NOT_FIND_CONSTANCE") == 0)
		{
			*sid = E_SID_CAN_NOT_FIND_CONSTANCE;
		}		
	}

	read_enum(self, (hpint32*)sid);

	return E_HP_ERROR;
done:
	return E_HP_NOERROR;
}

hpint32 read_LanguageString(HPAbstractReader *self, LanguageString *data)
{
	read_field_begin(self, "sid", strlen("sid"));
	read_LanguageStringID(self, &data->sid);
	read_field_end(self, "sid", strlen("sid"));

	read_field_begin(self, "str", strlen("str"));
	read_hpstring(self, data->str, MAX_LANGUAGE_STRING_LENGTH);
	read_field_end(self, "str", strlen("str"));
	return E_HP_NOERROR;
}

hpint32 read_LanguageLib(HPAbstractReader *self, LanguageLib *data)
{
	hpuint32 i;
	read_struct_begin(self, "LanguageLib");

	read_field_begin(self, "str_num", strlen("str_num"));
	read_hpuint32(self, &data->str_num);
	read_field_end(self, "str_num", strlen("str_num"));

	read_field_begin(self, "str", strlen("str"));
	for(i = 0;i < MAX_LANGUAGE_STRING_NUM; ++i)
	{		
		if(i >= data->str_num)
		{
			break;
		}
		read_vector_item_begin(self, i);
		read_LanguageString(self, &data->str[i]);
		read_vector_item_end(self, i);
	}
	read_field_end(self, "str", strlen("str"));

	read_struct_end(self, "LanguageLib");
	return E_HP_NOERROR;
}
