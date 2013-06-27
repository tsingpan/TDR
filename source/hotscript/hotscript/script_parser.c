#include "hotscript/script_parser.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_error.h"
#include "script_l.h"
#include "script_y.h"
#include <string.h>
#include "hotscript/hotlex.h"
#include "hotpot/hp_platform.h"

hpint32 hotscript_do_text(SCANNER_STACK *super, const SP_NODE *text)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);

	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO;
	op->op0.type = E_HP_STRING;
	op->op0.val.str.len = text->var.val.str.len;
	op->op0.val.str.ptr = text->var.val.str.ptr;
	return E_HP_NOERROR;
}

hpint32 hotscript_do_literal(SCANNER_STACK *super, const SP_NODE *text)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);

	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO_LITERAL;
	op->op0.type = E_HP_STRING;
	op->op0.val.str.len = text->var.val.str.len;
	op->op0.val.str.ptr = text->var.val.str.ptr;
	return E_HP_NOERROR;
}

hpint32 hotscript_do_push(SCANNER_STACK *super, const SP_NODE *prefix, SP_NODE *name)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_PUSH;
	op->op0.type = E_HP_CHAR;
	op->op0.val.c = prefix->var.val.c;
	op->op1.type = E_HP_STRING;
	op->op1.val.str.ptr = name->var.val.str.ptr;
	op->op1.val.str.len = name->var.val.str.len;

	name->op = op;
	return E_HP_NOERROR;
}

hpint32 hotscript_do_push_index(SCANNER_STACK *super, SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	if(index->var.val.i32 != -2)
	{
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_PUSH_INDEX;
		op->op0.type = E_HP_INT32;
		op->op0.val.i32 = index->var.val.i32;

		index->op = op;
	}
	return E_HP_NOERROR;
}

hpint32 hotscript_do_pop_index(SCANNER_STACK *super, SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	if(index->var.val.i32 != -2)
	{
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_POP;

		if(index->var.val.i32 == -1)
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

hpint32 hotscript_do_pop(SCANNER_STACK *super, SP_NODE *id)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);

	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_POP;
	id->op->op2.type = E_HP_UINT32;
	id->op->op2.val.ui32 = hotoparr_get_next_op_number(&self->hotoparr);
	return E_HP_NOERROR;
}


hpint32 hotscript_do_echo_trie(SCANNER_STACK *super)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO_TRIE;
	return E_HP_NOERROR;
}


void yyscripterror(const YYLTYPE *yylloc, SCANNER *sp, char *s, ...) 
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



extern hpint32 script_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyscriptlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	SCRIPT_PARSER *sp = HP_CONTAINER_OF(ss, SCRIPT_PARSER, scanner_stack);
	int ret;

	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(&sp->scanner_stack);		
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		ret = script_lex_scan(scanner, yylloc_param, yylval_param);
		if(ret == 0)
		{
			if(scanner_stack_get_num(&sp->scanner_stack) <= 1)
			{
				break;
			}
			scanner_stack_pop(&sp->scanner_stack);
		}
		else if(ret == tok_import)
		{
			char file_name[1024];
			size_t len = 0;
			while(scanner->yy_cursor < scanner->yy_limit)
			{
				if(*scanner->yy_cursor == ';')
				{
					break;
				}
				else if((*scanner->yy_cursor == '\n') || (*scanner->yy_cursor == '\t') || (*scanner->yy_cursor == ' '))
				{
					++(scanner->yy_cursor);
				}
				else
				{
					file_name[len++] = *scanner->yy_cursor;
					++(scanner->yy_cursor);
				}
			}

			file_name[len] = 0;
			if(scanner_stack_push_file(&sp->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
			{
				sp->result = E_HP_ERROR;
				return 0;
			}
		}
		else
		{
			break;
		}
	}


	return ret;
}

hpint32 script_parser_str(SCRIPT_PARSER *self, const char* script, const char *script_limit, 
						  HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	int ret;
	printf("\n--------------------------------------\n");
	while(script < script_limit)
	{
		putc(*script, stdout);
		++script;
	}
	printf("\n--------------------------------------\n");

	scanner_stack_init(&self->scanner_stack);


	scanner_stack_push(&self->scanner_stack, script, script_limit, yycINITIAL);


	hotoparr_init(&self->hotoparr);

	self->reader = reader;

	self->result = E_HP_NOERROR;

	ret = yyscriptparse(self);

	scanner_stack_pop(&self->scanner_stack);

	hotvm_execute(&self->hotvm, &self->hotoparr, self->reader, user_data, uputc);

	return self->result;
}

extern int yyscriptparse (SCRIPT_PARSER *sp);
hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	hpint32 ret;

	YYLTYPE yylloc;

	

	scanner_stack_init(&self->scanner_stack);


	scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL);
	

	hotoparr_init(&self->hotoparr);

	self->reader = reader;

	self->result = E_HP_NOERROR;
	/*
	for(;;)
	{
		int i;
		int ret = yyscriptlex(&snode, &yylloc, self);
		printf("%d ", ret);
		printf("\n---------------------------------------------------------\n");
		for(i = 0; i < snode.text.str_len; ++i)
		{
			putc(snode.text.str[i], stdout);
		}
		printf("\n---------------------------------------------------------\n");

		if(ret == 0)
		{
			break;
		}
		else
		{
			printf("%c\n", (char)ret);
		}
	}	
	exit(1);
	*/
	ret = yyscriptparse(self);

	scanner_stack_pop(&self->scanner_stack);

	hotvm_execute(&self->hotvm, &self->hotoparr, self->reader, user_data, uputc);

	return self->result;
}
