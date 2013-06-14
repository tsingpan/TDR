////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"hp_string_pot.dd"的协议文件所对应的C语言解析文件
//								请务必保证本文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hp_string_pot_read.h"
#include "hp_string_pot.h"
#include "hotpot/hp_platform.h"
#include "ddekit/encoding/ddekit_encoding_reader.h"



void HP_STRING_read(HP_STRING* data, DDEKIT_ENCODING_READER *dp)
{
	hpuint32 i;

	dp->read_struct_begin(dp, "HP_STRING");


	dp->read_var_begin(dp, "no", 98);
	dp->read_hpuint32(dp, &data->no);
	dp->read_var_end(dp, "no", 98);

	dp->read_repeat_begin(dp, "str", 64, TRUE);
	for(i = 0; i < HP_STRING_LENGTH; ++i)
	{
		hpchar *p = NULL;
		p = &data->str[i];

		dp->read_var_begin(dp, "str", 64);
		dp->read_hpchar(dp, p);
		dp->read_var_end(dp, "str", 64);
		if(*p == 0)
		{
			break;
		}
	}
	dp->read_repeat_end(dp, "str", 64, TRUE);

	dp->read_struct_end(dp, "HP_STRING");
}


void HP_STRING_POT_read(HP_STRING_POT* data, DDEKIT_ENCODING_READER *dp)
{
	hpuint32 i;

	dp->read_struct_begin(dp, "HP_STRING_POT");


	dp->read_var_begin(dp, "hp_string_num", 97);
	dp->read_hpuint16(dp, &data->hp_string_num);
	dp->read_var_end(dp, "hp_string_num", 97);

	dp->read_repeat_begin(dp, "hp_string", 1, FALSE);
	for(i = 0; i < HP_STRING_POT_SIZE; ++i)
	{
		void *p = NULL;
		if(i == data->hp_string_num)
		{
			break;
		}
		p = &data->hp_string[i];

		dp->read_var_begin(dp, "hp_string", 1);
		HP_STRING_read(p, dp);
		dp->read_var_end(dp, "hp_string", 1);
	}
	dp->read_repeat_end(dp, "hp_string", 1, FALSE);

	dp->read_struct_end(dp, "HP_STRING_POT");
}




