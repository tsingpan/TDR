#ifndef _H_HOT_OBJECT_WRITER
#define _H_HOT_OBJECT_WRITER

#include "hotscript/hotobject.h"

#include "hotpot/hp_writer.h"


#ifndef _DEC_HOTOBJECTITERATORA
#define _DEC_HOTOBJECTITERATORA
typedef struct _HotObjectWriter HotObjectWriter;
#endif//_DEC_HOTOBJECTITERATORA


typedef struct _HotObjectStackNode
{
	HotObject *ho;
}HotObjectStackNode;

struct _HotObjectWriter
{
	HPAbstractWriter super;

	HotObjectStackNode stack[HOTOBJECT_MAX_STACK_DEEP];
	hpuint32 stack_num;
};

hpint32 hotobject_writer_init(HotObjectWriter* self, HotObject *hotobject);

hpint32 hotobject_write_vector_item_begin(HPAbstractWriter *self, hpuint32 index);

#endif//_H_HOT_OBJECT_WRITER

