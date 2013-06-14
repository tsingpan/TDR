////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"hp_string_pot.dd"的协议文件所对应的C语言解析文件
//								请务必保证本文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hp_string_pot_write.h"
#include "hp_string_pot.h"
#include "hotpot/hp_platform.h"
#include "ddekit/encoding/ddekit_encoding_writer.h"



void HP_STRING_write(const HP_STRING* data, DDEKIT_ENCODING_WRITER *dp)
{
	hpuint32 i;

	dp->write_struct_begin(dp, "HP_STRING");


	dp->write_var_begin(dp, "no", 98);
	dp->write_hpuint32(dp, data->no);
	dp->write_var_end(dp, "no", 98);

	dp->write_repeat_begin(dp, "str", 64, TRUE);
	for(i = 0; i < HP_STRING_LENGTH; ++i)
	{
		const hpchar *p = NULL;
		p = &data->str[i];

		dp->write_var_begin(dp, "str", 64);
		dp->write_hpchar(dp, *p);
		dp->write_var_end(dp, "str", 64);
		if(*p == 0)
		{
			break;
		}
	}
	dp->write_repeat_end(dp, "str", 64, TRUE);

	dp->write_struct_end(dp, "HP_STRING");
}


void HP_STRING_POT_write(const HP_STRING_POT* data, DDEKIT_ENCODING_WRITER *dp)
{
	hpuint32 i;

	dp->write_struct_begin(dp, "HP_STRING_POT");


	dp->write_var_begin(dp, "hp_string_num", 97);
	dp->write_hpuint16(dp, data->hp_string_num);
	dp->write_var_end(dp, "hp_string_num", 97);

	dp->write_repeat_begin(dp, "hp_string", 1, FALSE);
	for(i = 0; i < HP_STRING_POT_SIZE; ++i)
	{
		const void *p = NULL;
		if(i == data->hp_string_num)
		{
			break;
		}
		p = &data->hp_string[i];

		dp->write_var_begin(dp, "hp_string", 1);
		HP_STRING_write(p, dp);
		dp->write_var_end(dp, "hp_string", 1);
	}
	dp->write_repeat_end(dp, "hp_string", 1, FALSE);

	dp->write_struct_end(dp, "HP_STRING_POT");
}




