#ifndef _H_HP_STRING_POT
#define _H_HP_STRING_POT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"hp_string_pot.dd"的X协议的C语言头文件
//								请务必保证头文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma pack(push,1)

#include "hotpot/hp_platform.h"


#define HP_STRING_POT_SIZE                                             (0x10000)
#define HP_STRING_LENGTH                                               (0x1000)

#ifndef _DEF_HP_STRING
#define _DEF_HP_STRING
typedef struct tagHP_STRING HP_STRING;
#endif// _DEF_HP_STRING
struct tagHP_STRING
{
		hpuint32	no;
		hpchar	str[HP_STRING_LENGTH];
};


#ifndef _DEF_HP_STRING_POT
#define _DEF_HP_STRING_POT
typedef struct tagHP_STRING_POT HP_STRING_POT;
#endif// _DEF_HP_STRING_POT
struct tagHP_STRING_POT
{
		hpuint16	hp_string_num;
		HP_STRING	hp_string[HP_STRING_POT_SIZE];
};



#pragma pack(pop)

#endif //_H_HP_STRING_POT

