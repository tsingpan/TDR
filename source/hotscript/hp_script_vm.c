#include "hotscript/hp_script_vm.h"
#include "hotpot/hp_error_code.h"
#include "hotscript/hp_script_parser.h"

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

hpint32 hotvm_echo(HotVM *self, const HotOp* op)
{
	hpuint32 i;
	for(i = 0; i < op->arg.echo_arg.bytes.len; ++i)
	{
		self->uputc(self, op->arg.echo_arg.bytes.ptr[i]);
	}
	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	return E_HP_NOERROR;
}

hpint32 hotvm_import(HotVM *self, const HotOp* op)
{
	SCRIPT_PARSER sp;

	if(script_parser(&sp, op->arg.import_arg.file_name, self->root_dir, self->working_dir) != E_HP_NOERROR)
	{
		self->result = sp.scanner_stack.result[0];
		strncpy(self->result_str, sp.scanner_stack.result_str[0], MAX_ERROR_MSG_LENGTH);
		goto ERROR_RET;
	}
	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	hotvm_push(self, 0, sp.hotoparr);

	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
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
	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
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


	if(read_field_begin(self->reader, name) != E_HP_NOERROR)
	{
		hotvm_set_eip(self, op->arg.field_begin_arg.lineno_after_field_end);
	}
	else
	{
		hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	}
	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_field_end(HotVM *self, const HotOp* op)
{
	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	read_field_end(self->reader, NULL);
	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_begin(HotVM *self, const HotOp* op)
{	
	if(read_vector_begin(self->reader) != E_HP_NOERROR)
	{
		hotvm_set_eip(self, op->arg.vector_begin_arg.failed_jmp_lineno);
	}
	else
	{
		self->vector_stack[self->vector_stack_num++] = 0;
		hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	}

	
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_end(HotVM *self, const HotOp* op)
{
	read_vector_end(self->reader);
	--(self->vector_stack_num);
	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	return E_HP_NOERROR;
}

hpint32 hotvm_vector_set_index(HotVM *self, const HotOp* op)
{
	if(self->vector_stack_num <= 0)
	{
		goto ERROR_RET;
	}


	self->vector_stack[self->vector_stack_num] = op->arg.vector_set_index_arg.index;

	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_inc_index(HotVM *self, const HotOp* op)
{
	if(self->vector_stack_num <= 0)
	{
		goto ERROR_RET;
	}

	++(self->vector_stack[self->vector_stack_num]);

	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_item_begin(HotVM *self, const HotOp* op)
{
	if(self->vector_stack_num <= 0)
	{
		goto ERROR_RET;
	}

	if(read_vector_item_begin(self->reader, self->vector_stack[self->vector_stack_num]) != E_HP_NOERROR)
	{
		hotvm_set_eip(self, op->arg.vector_item_begin_arg.failed_jmp_lineno);
	}
	else
	{
		hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	}
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotvm_vector_item_end(HotVM *self, const HotOp* op)
{
	if(self->vector_stack_num <= 0)
	{
		goto ERROR_RET;
	}

	if(read_vector_item_end(self->reader, self->vector_stack[self->vector_stack_num]) != E_HP_NOERROR)
	{
		hotvm_set_eip(self, op->arg.vector_item_begin_arg.failed_jmp_lineno);
	}
	else
	{
		hotvm_set_eip(self, hotvm_get_eip(self) + 1);
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

	hotvm_set_eip(self, hotvm_get_eip(self) + 1);
	return E_HP_NOERROR;
}

hpint32 hotvm_jmp(HotVM *self, const HotOp* op)
{
	hotvm_set_eip(self, op->arg.jmp_arg.lineno);

	return E_HP_NOERROR;
}

hpint32 hotvm_execute(HotVM *self, const char* file_name, const char* root_dir, HPAbstractReader *reader, void *user_data, vm_user_putc uputc, const char *working_dir)
{
	SCRIPT_PARSER sp;

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
	self->op_handler[HOT_IMPORT] = hotvm_import;

	self->root_dir = root_dir;
	self->working_dir = working_dir;
	self->reader = reader;	
	self->user_data = user_data;
	self->vector_stack_num = 0;
	self->uputc = uputc;


	self->stack_num = 0;

	if(script_parser(&sp, file_name, root_dir, working_dir) != E_HP_NOERROR)
	{
		self->result = sp.scanner_stack.result[0];
		strncpy(self->result_str, sp.scanner_stack.result_str[0], MAX_ERROR_MSG_LENGTH);
		goto ERROR_RET;
	}
	hotvm_push(self, 0, sp.hotoparr);
	while(self->stack_num > 0)
	{
		while(hotvm_get_eip(self) < hotvm_get_op(self)->next_oparr)
		{
			hotvm_execute_func func = self->op_handler[hotvm_get_op(self)->oparr[hotvm_get_eip(self)].instruct];
			if(func(self, &hotvm_get_op(self)->oparr[hotvm_get_eip(self)]) != E_HP_NOERROR)
			{
				goto ERROR_RET;
			}
		}
		hotvm_pop(self);
	}
	

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpuint32 hotvm_get_eip(HotVM *self)
{
	return self->stack[self->stack_num - 1].eip;
}

void hotvm_set_eip(HotVM *self, hpuint32 eip)
{
	self->stack[self->stack_num - 1].eip = eip;
}

void hotvm_push(HotVM *self, hpuint32 eip, const HotOpArr hotoparr)
{
	self->stack[self->stack_num].eip = eip;
	self->stack[self->stack_num].hotoparr = hotoparr;
	++(self->stack_num);
}

const HotOpArr *hotvm_get_op(HotVM *self)
{
	return &self->stack[self->stack_num - 1].hotoparr;
}

void hotvm_pop(HotVM *self)
{
	--(self->stack_num);
}
