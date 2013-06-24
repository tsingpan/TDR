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
			for(i = 0;i < op->op0.str_len; ++i)
			{
				fputc(op->op0.str[i], stdout);
			}
			++(self->current_op);
			break;
		}
	case HOT_PUSH:
		{
			if(hotobject_read_object_begin(&self->citer, op->op1.str) != E_HP_NOERROR)
			{
				self->current_op = op->op2.num;
			}
			else
			{
				++(self->current_op);
			}
			break;
		}
	case HOT_PUSH_INDEX:
		{
			char str[1024];
			const char* name = NULL;
			if(op->op0.num >= 0)
			{
				snprintf(str, 1024, "[%d]", op->op0.num);
				name = str;
			}

			if(hotobject_read_object_begin(&self->citer, name) != E_HP_NOERROR)
			{
				self->current_op = op->op1.num;
			}
			else
			{
				++(self->current_op);
			}
			break;
		}
	case HOT_POP:
		{
			hotobject_read_object_end(&self->citer, op->op1.str);
			++(self->current_op);
			break;
		}
	case HOT_ECHO_TRIE:
		{
			const char * str;
			hotobject_read(&self->citer, &str);
			printf("%s", str);
			++(self->current_op);
			break;
		}
	case HOT_JMP:
		{
			self->current_op = op->op0.num;			
			break;
		}
	default:
		{
			exit(1);
		}
	}

	return E_HP_NOERROR;
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, const HotObject *ho)
{
	hotobject_get_const_iterator(&self->citer, ho);

	self->hotoparr = hotoparr;
	self->current_op = 0;


	while(self->current_op < self->hotoparr->next_oparr)
	{
		hotvm_execute_once(self);		
	}

	return E_HP_NOERROR;
}
