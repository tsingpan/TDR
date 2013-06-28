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
			for(i = 0;i < op->op0.val.bytes.len; ++i)
			{
				self->uputc(self, op->op0.val.bytes.ptr[i]);
			}
			++(self->current_op);
			break;
		}
	case HOT_ECHO_LITERAL:
		{
			hpuint32 i;			
			for(i = 0;i < op->op0.val.bytes.len; ++i)
			{
				if(op->op0.val.bytes.ptr[i] == '\\')
				{
					++i;
					if(i >= op->op0.val.bytes.len)
					{
						break;
					}					
					switch (op->op0.val.bytes.ptr[i])
					{
					case 'r':
						self->uputc(self, '\r');
						break;
					case 'n':
						self->uputc(self, '\n');
						break;
					case 't':
						self->uputc(self, '\t');
						break;
					case '"':
						self->uputc(self, '\"');
						break;
					case '\'':
						self->uputc(self, '\'');
						break;
					case '\\':
						self->uputc(self, '\\');
						break;
					default:
						break;
					}
				}
				else
				{
					self->uputc(self, op->op0.val.bytes.ptr[i]);
				}
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
			//这里需要转义
			for(i = 0;i < var.val.bytes.len; ++i)
			{
				self->uputc(self, var.val.bytes.ptr[i]);
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
