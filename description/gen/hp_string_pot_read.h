#ifndef _H_HP_STRING_POT_READ
#define _H_HP_STRING_POT_READ

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								本文件是一个名叫"hp_string_pot.dd"的协议文件所对应的C语言解析文件
//								请务必保证本文件满足X协议的要求， 推荐使用XC程序编译X协议文件
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include "hotpot/hp_platform.h"

#ifndef _DEF_DDEKIT_ENCODING_READER
#define _DEF_DDEKIT_ENCODING_READER
typedef struct tagDDEKIT_ENCODING_READER DDEKIT_ENCODING_READER;
#endif//_DEF_DDEKIT_ENCODING_READER
#ifndef _DEF_HP_STRING
#define _DEF_HP_STRING
typedef struct tagHP_STRING HP_STRING;
#endif//_DEF_HP_STRING
void HP_STRING_read(HP_STRING* data, DDEKIT_ENCODING_READER *dp);

#ifndef _DEF_HP_STRING_POT
#define _DEF_HP_STRING_POT
typedef struct tagHP_STRING_POT HP_STRING_POT;
#endif//_DEF_HP_STRING_POT
void HP_STRING_POT_read(HP_STRING_POT* data, DDEKIT_ENCODING_READER *dp);

#endif //_H_HP_STRING_POT_READ

