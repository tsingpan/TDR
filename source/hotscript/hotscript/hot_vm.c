#include "hotscript/hot_vm.h"
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
	HotOp *ptr;
	if(self->next_oparr >= self->oparr_size)
	{
		self->oparr_size *= 2;
		self->oparr = (HotOp*)realloc(self->oparr, self->oparr_size);
	}

	ptr = &self->oparr[self->next_oparr];
	++(self->next_oparr);
	return ptr;
}

hpuint32 hotoparr_get_next_op_number(HotOpArr *self)
{
	return self->next_oparr;
}



hpint32 hotvm_execute_once(HotVM *self)
{
	const HotOp *op = &self->hotoparr->oparr[self->current_op];
	switch(op->instruct)
	{
	case HOT_ECHO:
		{
			hpuint32 i;
			for(i = 0;i < op->arg.echo_arg.bytes.len; ++i)
			{
				self->uputc(self, op->arg.echo_arg.bytes.ptr[i]);
			}
			++(self->current_op);
			break;
		}
	case HOT_FIELD_BEGIN:
		{
			++(self->current_op);
			break;
		}
	case HOT_VECTOR_BEGIN:
		{
			++(self->current_op);
			break;
		}
	case HOT_VECTOR_END:
		{
			++(self->current_op);
			break;
		}
	case HOT_ECHO_FIELD:
		{
			++(self->current_op);
			break;
		}
	case HOT_JMP:
		{
			//£¤%¡­¡­&*£¨£©¡£¡£
			++(self->current_op);
			break;
		}
	default:
		{
			exit(1);
		}
	}

	return E_HP_NOERROR;
}

static void normal_putc(HotVM *self, char c)
{
	fputc(c, stdout);
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	self->reader = reader;
	self->hotoparr = hotoparr;
	self->current_op = 0;
	self->user_data = user_data;

	if(uputc == NULL)
	{
		self->uputc = normal_putc;
	}
	else
	{
		self->uputc = uputc;
	}

	while(self->current_op < self->hotoparr->next_oparr)
	{
		hotvm_execute_once(self);		
	}

	return E_HP_NOERROR;
}
