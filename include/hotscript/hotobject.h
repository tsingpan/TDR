#ifndef _H_HOT_OBJECT
#define _H_HOT_OBJECT


#include "hotpot/hp_platform.h"

#include "hotpot/hp_value.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_writer.h"

#ifndef _DEC_HOTOBJECT
#define _DEC_HOTOBJECT
typedef struct _HotObject HotObject;
#endif//_DEC_HOTOBJECT

HotObject* hotobject_new();


void hotobject_free(HotObject* self);



#ifndef _DEC_HOTOBJECTITERATORA
#define _DEC_HOTOBJECTITERATORA
typedef struct _HotObjectWriter HotObjectWriter;
#endif//_DEC_HOTOBJECTITERATORA

#ifndef _DEC_HOTOBJECTCONSTITERATORA
#define _DEC_HOTOBJECTCONSTITERATORA
typedef struct _HotObjectReader HotObjectReader;
#endif//_DEC_HOTOBJECTCONSTITERATORA


#define HOTOBJECT_MAX_NAME_LENGTH 128
#define HOTOBJECT_MAX_STACK_DEEP 1024
typedef struct _HotObjectStackNode
{
	HotObject *ho;
	hpuint32 count;	
}HotObjectStackNode;

struct _HotObjectWriter
{
	HPAbstractWriter super;

	HotObjectStackNode stack[HOTOBJECT_MAX_STACK_DEEP];
	hpuint32 stack_num;

	char name[HOTOBJECT_MAX_NAME_LENGTH];	
};


typedef struct _HotObjectConstStackNode
{
	const HotObject *ho;
	hpuint32 count;	
}HotObjectConstStackNode;

struct _HotObjectReader
{
	HPAbstractReader super;
	HotObjectConstStackNode stack[HOTOBJECT_MAX_STACK_DEEP];
	hpuint32 stack_num;

	char name[HOTOBJECT_MAX_NAME_LENGTH];
};

hpint32 hotobject_get_writer(HotObjectWriter* self, HotObject *hotobject);

hpint32 hotobject_get_reader(HotObjectReader* self, const HotObject *hotobject);

#endif//_H_HOT_OBJECT

