#ifndef _H_LANGUAGE_TYPES
#define _H_LANGUAGE_TYPES

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"

#define MAX_LANGUAGE_STRING_NUM 10240
#define MAX_LANGUAGE_STRING_LENGTH 1024
typedef enum _LanguageStringID
{
	E_SID_NOERROR = E_HP_NOERROR,
}LanguageStringID;

typedef struct _LanguageString
{
	LanguageStringID sid;
	hpchar str[MAX_LANGUAGE_STRING_LENGTH];
}LanguageString;

typedef struct _LanguageLib
{
	hpuint32 str_num;
	LanguageString str[MAX_LANGUAGE_STRING_NUM];
}LanguageLib;

#endif//_H_LANGUAGE_TYPES
