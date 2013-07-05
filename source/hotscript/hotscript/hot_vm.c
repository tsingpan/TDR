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
	ptr->lineno = self->next_oparr;
	++(self->next_oparr);
	return ptr;
}

hpuint32 hotoparr_get_next_op_number(HotOpArr *self)
{
	return self->next_oparr;
}

static void normal_putc(HotVM *self, char c)
{
	fputc(c, stdout);
}


hpint32 hotvm_echo(HotVM *self, const HotOp* op)
{
	hpuint32 i;
	for(i = 0; i < op->arg.echo_arg.bytes.len; ++i)
	{
		self->uputc(self, op->arg.echo_arg.bytes.ptr[i]);
	}
	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_field_begin(HotVM *self, const HotOp* op)
{
	//todo filed_search_strategy
	if(read_field_begin(self->reader, op->arg.field_begin_arg.name.ptr, op->arg.field_begin_arg.name.len) != E_HP_NOERROR)
	{
		self->current_op = op->arg.field_begin_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->current_op);
	}

	
	return E_HP_NOERROR;
}

hpint32 hotvm_field_end(HotVM *self, const HotOp* op)
{
	read_field_end(self->reader, NULL, 0);

	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_begin(HotVM *self, const HotOp* op)
{
	self->stack[self->stack_num] = 0;
	++(self->stack_num);

	if(read_vector_begin(self->reader) != E_HP_NOERROR)
	{
		self->current_op = op->arg.vector_begin_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->current_op);
	}

	
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_end(HotVM *self, const HotOp* op)
{
	read_vector_end(self->reader);
	--(self->stack_num);

	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_set_index(HotVM *self, const HotOp* op)
{
	self->stack[self->stack_num - 1] = op->arg.vector_set_index_arg.index;

	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_inc_index(HotVM *self, const HotOp* op)
{
	++(self->stack[self->stack_num - 1]);

	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_seek(HotVM *self, const HotOp* op)
{
	if(reader_seek(self->reader, self->stack[self->stack_num - 1]) != E_HP_NOERROR)
	{
		self->current_op = op->arg.vector_seek_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->current_op);
	}
	return E_HP_NOERROR;
}

hpint32 hotvm_echo_field(HotVM *self, const HotOp* op)
{
	HPType type;
	read_type(self->reader, &type);
	switch(type)
	{
	case E_HP_BYTES:
		{
			hpuint32 i;
			hpbytes bytes;
			read_bytes(self->reader, &bytes);
			for(i = 0;i <bytes.len; ++i)
			{
				self->uputc(self, bytes.ptr[i]);
			}
			break;
		}
	}

	++(self->current_op);
	return E_HP_NOERROR;
}

hpint32 hotvm_jmp(HotVM *self, const HotOp* op)
{
	self->current_op = op->arg.jmp_arg.lineno;

	return E_HP_NOERROR;
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	self->reader = reader;
	self->hotoparr = hotoparr;
	self->current_op = 0;
	self->user_data = user_data;
	self->stack_num = 0;

	if(uputc == NULL)
	{
		self->uputc = normal_putc;
	}
	else
	{
		self->uputc = uputc;
	}

	self->op_handler[HOT_ECHO] = hotvm_echo;
	self->op_handler[HOT_FIELD_BEGIN] = hotvm_field_begin;
	self->op_handler[HOT_FIELD_END] = hotvm_field_end;
	self->op_handler[HOT_VECTOR_BEGIN] = hotvm_vector_begin;
	self->op_handler[HOT_VECTOR_END] = hotvm_vector_end;
	self->op_handler[HOT_VECTOR_SET_INDEX] = hotvm_vector_set_index;
	self->op_handler[HOT_VECTOR_INC_INDEX] = hotvm_vector_inc_index;
	self->op_handler[HOT_VECTOR_SEEK] = hotvm_vector_seek;
	self->op_handler[HOT_ECHO_FIELD] = hotvm_echo_field;
	self->op_handler[HOT_JMP] = hotvm_jmp;

	while(self->current_op < self->hotoparr->next_oparr)
	{
		self->op_handler[self->hotoparr->oparr[self->current_op].instruct](self, &self->hotoparr->oparr[self->current_op]);
	}

	return E_HP_NOERROR;
}
