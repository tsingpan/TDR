#ifndef _H_HP_WRITER
#define _H_HP_WRITER

#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _HPAbstractWriter HPAbstractWriter;
typedef hpint32 (*func_hp_writer_begin)(HPAbstractWriter* self, const HPVar *name);
typedef hpint32 (*func_hp_writer_write)(HPAbstractWriter* self, const HPVar *var);
typedef hpint32 (*func_hp_writer_end)(HPAbstractWriter* self);
struct _HPAbstractWriter
{
	func_hp_writer_begin begin;
	func_hp_writer_write write;
	func_hp_writer_end end;
};

hpint32 hp_writer_begin(HPAbstractWriter *self, const HPVar *name);

hpint32 hp_writer_write(HPAbstractWriter* self, const HPVar *var);

hpint32 hp_writer_end(HPAbstractWriter *self);

#endif //_H_HP_WRITER

