#include "hotpot/hp_reader.h"
#include "hotpot/hp_error.h"

hpint32 hp_reader_init(HPAbstractReader *self)
{
	self->stack_num = 0;

	return E_HP_NOERROR;
}

hpint32 hp_reader_fini(HPAbstractReader *self)
{
	self->stack_num = 0;

	return E_HP_NOERROR;
}

hpint32 hp_reader_begin(HPAbstractReader *self, const HPVar *name)
{
	hpint32 ret;

	if(name == NULL)
	{
		HPVar _name;
		_name.type = E_HP_INT32;
		_name.val.i32 = self->stack[self->stack_num - 1].count;
		++(self->stack[self->stack_num - 1].count);

		self->stack[self->stack_num].count = 0;
		++(self->stack_num);
		ret = self->begin(self, &_name);
	}
	else
	{
		self->stack[self->stack_num].count = 0;
		++(self->stack_num);
		ret = self->begin(self, name);
	}
	
	return ret;
}

hpint32 hp_reader_read(HPAbstractReader* self, HPVar *var)
{
	return self->read(self, var);
}

hpint32 hp_reader_end(HPAbstractReader *self)
{
	hpint32 ret = self->end(self);
	--(self->stack_num);
	return ret;
}
