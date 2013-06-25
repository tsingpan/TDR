#include "hotpot/hp_reader.h"

hpint32 hp_reader_begin(HPAbstractReader *self, const HPVar *name)
{
	return self->begin(self, name);
}

hpint32 hp_reader_read(HPAbstractReader* self, HPVar *var)
{
	return self->read(self, var);
}

hpint32 hp_reader_end(HPAbstractReader *self)
{
	return self->end(self);
}
