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
	HotOp *ptr;
	if(self->next_oparr >= self->oparr_size)
	{
		self->oparr_size *= 2;
		self->oparr = realloc(self->oparr, self->oparr_size);
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



hpint32 hotvm_execute_once(HotVM *self)
{
	const HotOp *op = &self->hotoparr->oparr[self->current_op];
	switch(op->op)
	{
	case HOT_ECHO:
		{
			hpuint32 i;
			for(i = 0;i < op->op0.val.str.len; ++i)
			{
				fputc(op->op0.val.str.ptr[i], stdout);
			}
			++(self->current_op);
			break;
		}
	case HOT_PUSH:
		{
			if(hp_reader_begin(self->reader, &op->op1) != E_HP_NOERROR)
			{
				self->current_op = op->op2.val.ui32;
			}
			else
			{
				++(self->current_op);
			}
			break;
		}
	case HOT_PUSH_INDEX:
		{
			if(hp_reader_begin(self->reader, &op->op0) != E_HP_NOERROR)
			{
				self->current_op = op->op1.val.ui32;
			}
			else
			{
				++(self->current_op);
			}
			break;
		}
	case HOT_POP:
		{
			hp_reader_end(self->reader);
			++(self->current_op);
			break;
		}
	case HOT_ECHO_TRIE:
		{
			HPVar var;
			hpuint32 i;
			hp_reader_read(self->reader, &var);
			for(i = 0;i < var.val.str.len; ++i)
			{
				fputc(var.val.str.ptr[i], stdout);
			}
			++(self->current_op);
			break;
		}
	case HOT_JMP:
		{
			self->current_op = op->op0.val.i32;			
			break;
		}
	default:
		{
			exit(1);
		}
	}

	return E_HP_NOERROR;
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader)
{
	self->reader = reader;
	self->hotoparr = hotoparr;
	self->current_op = 0;


	while(self->current_op < self->hotoparr->next_oparr)
	{
		hotvm_execute_once(self);		
	}

	return E_HP_NOERROR;
}
