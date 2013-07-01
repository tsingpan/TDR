#ifndef _H_HP_READR
#define _H_HP_READR

#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _HPAbstractReader HPAbstractReader;
typedef hpint32 (*func_hp_reader_begin)(HPAbstractReader* self, const HPVar *name);
typedef hpint32 (*func_hp_reader_read)(HPAbstractReader* self, HPVar *var);
typedef hpint32 (*func_hp_reader_end)(HPAbstractReader* self);

typedef struct _HPReaderStackNode
{
	int count;
}HPReaderStackNode;

#define READER_STACK_DEEP 1024
struct _HPAbstractReader
{
	func_hp_reader_begin begin;
	func_hp_reader_read read;
	func_hp_reader_end end;

	HPReaderStackNode stack[READER_STACK_DEEP];
	hpuint32 stack_num;

	//

	hpint32 (*read_object_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_object_end)(HPAbstractReader *self, const hpstring name);

	hpint32 (*read_field_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_field_end)(HPAbstractReader *self, const hpstring name);

	hpint32 (*read_vector_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_vector_end)(HPAbstractReader *self, const hpstring name);
};

hpint32 hp_reader_init(HPAbstractReader *self);

hpint32 hp_reader_begin(HPAbstractReader *self, const HPVar *name);

hpint32 hp_reader_read(HPAbstractReader* self, HPVar *var);

hpint32 hp_reader_end(HPAbstractReader *self);

#endif //_H_HP_READR

