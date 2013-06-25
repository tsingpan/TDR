#include "hotscript/script_parser.h"

#include "hotpot/hp_error.h"

hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	hpint32 ret;

	hotoparr_init(&self->hotoparr);

	yyscriptlex_init_extra(&self->scanner, &self->scanner);

	self->stack_num = 0;
	script_open_file(&self->scanner, file_name);
	self->reader = reader;

	ret = yyscriptparse(&self->scanner);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}

	hotvm_execute(&self->hotvm, &self->hotoparr, self->reader, user_data, uputc);
	
	return self->result;
}

hpint32 script_open_file(yyscan_t *super, const char *file_name)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner);
	
	self->stack[self->stack_num].f = fopen(file_name, "r");
	if(self->stack[self->stack_num].f == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs = yyscript_create_buffer(self->stack[self->stack_num].f, YY_BUF_SIZE, self->scanner);
	if(self->stack[self->stack_num].bs == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs->yy_bs_column = 1;
	++self->stack_num;
	yyscript_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);
	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 script_close_file(yyscan_t *super)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner);

	fclose(self->stack[self->stack_num - 1].f);
	yyscript_delete_buffer(self->stack[self->stack_num - 1].bs, self->scanner);

	--self->stack_num;
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}
	yyscript_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_text(SCRIPT_PARSER *self, const SNODE *text)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO;
	op->op0.type = E_HP_STRING;
	op->op0.val.str.len = text->text.str_len;
	op->op0.val.str.ptr = (char*)malloc(op->op0.val.str.len);
	memcpy(op->op0.val.str.ptr, text->text.str, op->op0.val.str.len);
	
	return E_HP_NOERROR;
}

hpint32 hotscript_do_literal(SCRIPT_PARSER *self, const SNODE *text)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO;
	op->op0.type = E_HP_STRING;
	op->op0.val.str.len = text->literal.str_len;
	op->op0.val.str.ptr = (char*)malloc(op->op0.val.str.len);
	memcpy(op->op0.val.str.ptr, text->literal.str, op->op0.val.str.len);
	return E_HP_NOERROR;
}

hpint32 hotscript_do_push(SCRIPT_PARSER *self, const SNODE *prefix, SNODE *name)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_PUSH;
	op->op0.type = E_HP_CHAR;
	op->op0.val.c = prefix->prefix;
	op->op1.type = E_HP_STRING;
	op->op1.val.str.ptr = strdup(name->identifier);
	op->op1.val.str.len = strlen(name->identifier) + 1;
	name->op = op;
	return E_HP_NOERROR;
}

hpint32 hotscript_do_push_index(SCRIPT_PARSER *self, SNODE *index)
{
	if(index->i32 != -2)
	{
		char *str = (char*)malloc(sizeof(char));
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_PUSH_INDEX;
		op->op0.type = E_HP_INT32;
		op->op0.val.i32 = index->i32;

		index->op = op;
	}
	return E_HP_NOERROR;
}

hpint32 hotscript_do_pop_index(SCRIPT_PARSER *self, SNODE *index)
{
	if(index->i32 != -2)
	{
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_POP;

		if(index->i32 == -1)
		{
			HotOp *op = hotoparr_get_next_op(&self->hotoparr);
			op->op = HOT_JMP;
			op->op0.type = E_HP_UINT32;
			op->op0.val.ui32 = index->op->lineno;
		}

		op->op1.type = E_HP_UINT32;
		index->op->op1.val.ui32 = hotoparr_get_next_op_number(&self->hotoparr);
	}

	
	

	return E_HP_NOERROR;
}

hpint32 hotscript_do_pop(SCRIPT_PARSER *self, SNODE *id)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_POP;
	id->op->op2.type = E_HP_UINT32;
	id->op->op2.val.ui32 = hotoparr_get_next_op_number(&self->hotoparr);
	return E_HP_NOERROR;
}


hpint32 hotscript_do_echo_trie(SCRIPT_PARSER *self)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO_TRIE;
	return E_HP_NOERROR;
}
