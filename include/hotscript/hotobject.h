#ifndef _H_HOT_OBJECT
#define _H_HOT_OBJECT


#include "hotpot/hp_platform.h"
#include "datrie/trie.h"
#include "hotpot/hp_value.h"


typedef enum _HotObjectType
{
	E_UNKNOW,
	E_ARRAY,
	E_OBJECT,
	E_VAR,
}HotObjectType;

struct _HotObject
{
	HotObjectType type;

	HPVar var;

	Trie* keys;
};


#define HOTOBJECT_MAX_NAME_LENGTH 128
#define HOTOBJECT_MAX_STACK_DEEP 1024



#ifndef _DEC_HOTOBJECT
#define _DEC_HOTOBJECT
typedef struct _HotObject HotObject;
#endif//_DEC_HOTOBJECT

HotObject* hotobject_new();

void hotobject_free(HotObject* self);

#endif//_H_HOT_OBJECT
