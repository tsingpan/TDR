#include "hotpot/hp_writer.h"

hpint32 hp_writer_begin(HPAbstractWriter *self, const HPVar *name)
{
	return self->begin(self, name);
}

hpint32 hp_writer_write(HPAbstractWriter* self, const HPVar *var)
{
	return self->write(self, var);
}

hpint32 hp_writer_end(HPAbstractWriter *self)
{
	return self->end(self);
}
