#include "hot_vm.h"
#include "stdio.h"

#include "hotpot/hp_error.h"
#define NORMAL_OP_SIZE 128


hpint32 hotoparr_init(HotOpArr *self)
{
	self->oparr_size = NORMAL_OP_SIZE;
	self->oparr = (HotOp*)malloc(sizeof(HotOp) * self->oparr_size);
	self->next_oparr = 0;

	return E_HP_NOERROR;
}

HotOp *hotoparr_get_next_op(HotOpArr *self)
{
	if(self->next_oparr >= self->oparr_size)
	{
		self->oparr_size *= 2;
		self->oparr = realloc(self->oparr, self->oparr_size);
	}

	return &self->oparr[(self->next_oparr)++];
}

hpuint32 hotoparr_get_next_op_number(HotOpArr *self)
{
	return self->next_oparr;
}


hpint32 hotvm_execute_once(HotVM *self)
{
	const HotOp *op = &self->hotoparr->oparr[self->current_op];
	switch(op->op)
	{
	case HOT_ECHO:
		{
			hpuint32 i;
			for(i = 0;i < op->op0.str_len; ++i)
			{
				fputc(op->op0.str[i], stdout);
			}
			break;
		}
	default:
		{
			exit(1);
		}
	}
	++(self->current_op);
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr)
{
	self->hotoparr = hotoparr;
	self->current_op = 0;


	while(self->current_op < self->hotoparr->next_oparr)
	{
		hotvm_execute_once(self);
	}

	return E_HP_NOERROR;
}
