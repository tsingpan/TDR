#include "hotscript/hp_script_op.h"


void hotoparr_init(HotOpArr *self)
{
	self->oparr_size = NORMAL_OP_SIZE;
//	self->oparr = (HotOp*)malloc(sizeof(HotOp) * self->oparr_size);
	self->next_oparr = 0;
}

void hotoparr_fini(HotOpArr *self)
{
	//free(self->oparr);
}

HotOp *hotoparr_get_next_op(HotOpArr *self)
{
	HotOp *ptr;
	if(self->next_oparr >= self->oparr_size)
	{
		return NULL;
		//self->oparr_size *= 2;
		//self->oparr = (HotOp*)realloc(self->oparr, sizeof(HotOp) *self->oparr_size);
	}

	ptr = &self->oparr[self->next_oparr];
	ptr->lineno = self->next_oparr;
	++(self->next_oparr);
	return ptr;
}

hpuint32 hotoparr_get_next_op_number(HotOpArr *self)
{
	return self->next_oparr;
}
