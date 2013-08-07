#include "hotscript/hot_vm.h"
#include "hotpot/hp_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		self->oparr = (HotOp*)realloc(self->oparr, sizeof(HotOp) *self->oparr_size);
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
	++(self->eip);
	return E_HP_NOERROR;
}

hpint32 hotvm_echo_literal(HotVM *self, const HotOp* op)
{
	hpuint32 i;
	for(i = 0; i < op->arg.echo_arg.bytes.len; ++i)
	{
		if(op->arg.echo_arg.bytes.ptr[i] == '\\')
		{
			++i;
			if(!(i < op->arg.echo_arg.bytes.len))
			{
				break;
			}
			switch(op->arg.echo_arg.bytes.ptr[i])
			{
			case '"':
				self->uputc(self, '"');
				break;
			case '\'':
				self->uputc(self, '\'');
				break;
			case '\\':
				self->uputc(self, '\\');
				break;
			case '/':
				self->uputc(self, '/');
				break;
			case 'b':
				self->uputc(self, '\b');
				break;
			case 'f':
				self->uputc(self, '\f');
				break;
			case 'n':
				self->uputc(self, '\n');
				break;
			case 'r':
				self->uputc(self, '\r');
				break;
			case 't':
				self->uputc(self, '\t');
				break;
			default:
				goto ERROR_RET;
			}
		}
		else
		{
			self->uputc(self, op->arg.echo_arg.bytes.ptr[i]);
		}
	}
	++(self->eip);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}
hpint32 hotvm_field_begin(HotVM *self, const HotOp* op)
{
	hpchar name[HS_MAX_NAME_LENGTH];

	if(op->arg.field_begin_arg.name.len >= HS_MAX_NAME_LENGTH)
	{
		goto ERROR_RET;
	}
	memcpy(name, op->arg.field_begin_arg.name.ptr, HS_MAX_NAME_LENGTH);
	name[op->arg.field_begin_arg.name.len] = 0;	


	self->stack[self->stack_num].eax = self->eax;
	self->stack[self->stack_num].eip = self->eip;
	self->stack[self->stack_num].return_eip = op->arg.field_begin_arg.lineno_after_field_end;
	//todo filed_search_strategy
	if(read_field_begin(self->reader, name) != E_HP_NOERROR)
	{
		self->eip = op->arg.field_begin_arg.lineno_after_field_end;
	}
	else
	{
		++(self->eip);
		++(self->stack_num);
	}
	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_field_end(HotVM *self, const HotOp* op)
{
	if(self->stack_num <= 0 )
	{
		goto ERROR_RET;
	}

	self->eax = self->stack[self->stack_num - 1].eax;
	self->eip = self->stack[self->stack_num - 1].return_eip;
	--(self->stack_num);
	

	read_field_end(self->reader, NULL);

	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_begin(HotVM *self, const HotOp* op)
{
	self->eax = 0;
	if(read_vector_begin(self->reader) != E_HP_NOERROR)
	{
		self->eip = op->arg.vector_begin_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->eip);
	}

	
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_end(HotVM *self, const HotOp* op)
{
	read_vector_end(self->reader);
	++(self->eip);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_set_index(HotVM *self, const HotOp* op)
{
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}


	self->eax = op->arg.vector_set_index_arg.index;

	++(self->eip);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_inc_index(HotVM *self, const HotOp* op)
{
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}

	++(self->eax);

	++(self->eip);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_item_begin(HotVM *self, const HotOp* op)
{
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}

	if(read_vector_item_begin(self->reader, self->eax) != E_HP_NOERROR)
	{
		self->eip = op->arg.vector_item_begin_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->eip);
	}
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_item_end(HotVM *self, const HotOp* op)
{
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}

	if(read_vector_item_end(self->reader, self->eax) != E_HP_NOERROR)
	{
		self->eip = op->arg.vector_item_begin_arg.failed_jmp_lineno;
	}
	else
	{
		++(self->eip);
	}
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
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
	case E_HP_INT64:
		{
			const char *i;
			hpint64 i64;
			char number[1024];
			read_hpint64(self->reader, &i64);
			snprintf(number, 1024, "%lld", i64);

			for(i = number; *i; ++i)
			{
				self->uputc(self, *i);
			}
			break;
		}
	case E_HP_DOUBLE:
		{
			const char *i;
			hpdouble d;
			char number[1024];
			read_hpdouble(self->reader, &d);
			snprintf(number, 1024, "%lf", d);

			for(i = number; *i; ++i)
			{
				self->uputc(self, *i);
			}
			break;
		}
	case E_HP_BOOL:
		{
			const char *i;
			hpbool b;
			char number[1024];
			read_hpbool(self->reader, &b);
			if(b)
			{
				snprintf(number, 1024, "hptrue");
			}
			else
			{
				snprintf(number, 1024, "hpfalse");
			}
			

			for(i = number; *i; ++i)
			{
				self->uputc(self, *i);
			}
			break;
		}	
	}

	++(self->eip);
	return E_HP_NOERROR;
}

hpint32 hotvm_jmp(HotVM *self, const HotOp* op)
{
	self->eip = op->arg.jmp_arg.lineno;

	return E_HP_NOERROR;
}

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	self->reader = reader;
	self->hotoparr = hotoparr;
	self->eip = 0;
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
	self->op_handler[HOT_VECTOR_ITEM_BEGIN] = hotvm_vector_item_begin;
	self->op_handler[HOT_VECTOR_ITEM_END] = hotvm_vector_item_end;
	self->op_handler[HOT_ECHO_FIELD] = hotvm_echo_field;
	self->op_handler[HOT_JMP] = hotvm_jmp;
	self->op_handler[HOT_ECHO_LITERAL] = hotvm_echo_literal;
	self->op_handler[HOT_ECHO_LITERAL] = hotvm_echo_literal;
	self->op_handler[HOT_ECHO_LITERAL] = hotvm_echo_literal;

	while(self->eip < self->hotoparr->next_oparr)
	{
		hotvm_execute_func func = self->op_handler[self->hotoparr->oparr[self->eip].instruct];
		if(func == NULL)
		{
			++self->eip;
			continue;
		}

		if(func(self, &self->hotoparr->oparr[self->eip]) != E_HP_NOERROR)
		{
			goto ERROR_RET;
		}
	}

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}
