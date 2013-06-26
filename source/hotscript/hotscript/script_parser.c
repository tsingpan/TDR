#include "hotscript/script_parser.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_error.h"
#include "script_l.h"
#include <string.h>
extern int yyscriptparse (SCRIPT_PARSER *sp);
hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	hpint32 ret;
	FILE* fin;
	char c;

	SNODE snode;
	YYLTYPE yylloc;


	self->buff_size = 0;
	fin = fopen(file_name, "r");
	while((c = fgetc(fin)) != EOF)
	{
		self->buff[(self->buff_size)++] = c;
	}
	fclose(fin);
	self->yy_state = yycINITIAL;
	self->yy_marker = self->buff;
	self->yy_last = self->buff;
	self->yy_cursor = self->buff + 0;
	self->yy_limit = self->buff + self->buff_size;
	self->yylineno = 1;
	self->yycolumn = 1;

	hotoparr_init(&self->hotoparr);


	self->stack_num = 0;
	self->reader = reader;
	/*
	for(;;)
	{
		int ret = yyscriptlex(&snode, &yylloc, self);
		printf("%d ", ret);
		if(ret == 0)
		{
			break;
		}
		else
		{
			printf("%c\n", (char)ret);
		}
	}
	*/
	ret = yyscriptparse(self);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}

	hotvm_execute(&self->hotvm, &self->hotoparr, self->reader, user_data, uputc);
	
	return self->result;
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


void yyscripterror(const YYLTYPE *yylloc, SCRIPT_PARSER *sp, char *s, ...) 
{
	va_list ap;
	va_start(ap, s);

	if(yylloc->first_line)
	{
		fprintf(stderr, "%d.%d-%d.%d: error: ", yylloc->first_line, yylloc->first_column, yylloc->last_line, yylloc->last_column);
	}
	vfprintf(stderr, s, ap);
	printf("\n");
	va_end(ap);

	return;
}

extern hpint32 script_lex_scan(SCRIPT_PARSER *sp, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyscriptlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCRIPT_PARSER *sp)
{
	int ret = script_lex_scan(sp, yylloc_param, yylval_param);
	yylloc_param->last_line = sp->yylineno;
	yylloc_param->last_column = sp->yycolumn;

	return ret;
}
