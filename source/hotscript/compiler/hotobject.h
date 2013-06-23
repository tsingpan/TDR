#ifndef _H_HOT_OBJECT
#define _H_HOT_OBJECT


#include "hotpot/hp_platform.h"

#ifndef _DEC_HOTOBJECT
#define _DEC_HOTOBJECT
typedef struct _HotObject HotObject;
#endif//_DEC_HOTOBJECT

HotObject* hotobject_new();


void hotobject_free(HotObject* self);



#ifndef _DEC_HOTOBJECTITERATORA
#define _DEC_HOTOBJECTITERATORA
typedef struct _HotObjectIterator HotObjectIterator;
#endif//_DEC_HOTOBJECTITERATORA

#ifndef _DEC_HOTOBJECTCONSTITERATORA
#define _DEC_HOTOBJECTCONSTITERATORA
typedef struct _HotObjectConstIterator HotObjectConstIterator;
#endif//_DEC_HOTOBJECTCONSTITERATORA


#define HOTOBJECT_MAX_NAME_LENGTH 128
#define HOTOBJECT_MAX_STACK_DEEP 1024
typedef struct _HotObjectStackNode
{
	HotObject *ho;
	hpuint32 count;	
}HotObjectStackNode;

struct _HotObjectIterator
{
	HotObjectStackNode stack[HOTOBJECT_MAX_STACK_DEEP];
	hpuint32 stack_num;

	char name[HOTOBJECT_MAX_NAME_LENGTH];
};


typedef struct _HotObjectConstStackNode
{
	const HotObject *ho;
	hpuint32 count;	
}HotObjectConstStackNode;

struct _HotObjectConstIterator
{
	HotObjectConstStackNode stack[HOTOBJECT_MAX_STACK_DEEP];
	hpuint32 stack_num;

	char name[HOTOBJECT_MAX_NAME_LENGTH];
};

hpint32 hotobject_get_iterator(HotObjectIterator* self, HotObject *hotobject);
hpint32 hotobject_get_const_iterator(HotObjectConstIterator* self, const HotObject *hotobject);

void hotobject_write_object_begin(HotObjectIterator* self, const char *name);

void hotobject_write_object_end(HotObjectIterator* self, const char *name);

void hotobject_write_string(HotObjectIterator* self, const char *name, const char *string);



void hotobject_read_object_begin(HotObjectConstIterator* self, const char *name);

void hotobject_read_object_end(HotObjectConstIterator* self, const char *name);

void hotobject_read(HotObjectConstIterator* self, const char ** string);

void hotobject_read_string(HotObjectConstIterator* self, const char *name, const char ** string);

#endif//_H_HOT_OBJECT

