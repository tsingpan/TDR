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
	op->instruct = HOT_ECHO;
	op->arg.echo_arg.bytes = text->var.val.bytes;
	return E_HP_NOERROR;
}

hpint32 hotscript_do_literal(SCANNER_STACK *super, const SP_NODE *text)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);

	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_ECHO_LITERAL;
	op->arg.echo_arg.bytes = text->var.val.bytes;

	return E_HP_NOERROR;
}



hpint32 hotscript_do_vector_begin(SCANNER_STACK *super, SP_NODE *current, const SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = NULL;
	if(index->it == E_INDEX_NULL)
	{
		goto ERROR_RET;
	}
	op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_VECTOR_BEGIN;

	current->vector_begin = op;

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_vector_seek(SCANNER_STACK *super, SP_NODE *current, const SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = NULL;

	if(index->it == E_INDEX_NULL)
	{
		goto ERROR_RET;
	}

	if(index->it == E_INDEX_GIVEN)
	{
		op = hotoparr_get_next_op(&self->hotoparr);
		op->instruct = HOT_VECTOR_SET_INDEX;
		op->arg.vector_set_index_arg.index = index->var.val.ui32;
	}
	else if(index->it == E_INDEX_ALL)
	{
		op = hotoparr_get_next_op(&self->hotoparr);
		op->instruct = HOT_VECTOR_SET_INDEX;
		op->arg.vector_set_index_arg.index = 0;		
	}

	op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_VECTOR_ITEM_BEGIN;
	current->vector_seek = op;

	op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_VECTOR_INC_INDEX;

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_vector_seek_jmp(SCANNER_STACK *super, SP_NODE *current, const SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = NULL;

	if(index->it != E_INDEX_ALL)
	{
		goto ERROR_RET;
	}
	op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_JMP;
	op->arg.jmp_arg.lineno = current->vector_seek->lineno;

	current->vector_seek->arg.vector_seek_arg.failed_jmp_lineno = hotoparr_get_next_op_number(&self->hotoparr);

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_vector_end(SCANNER_STACK *super, SP_NODE *current, const SP_NODE *index)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = NULL;
	if(index->it == E_INDEX_NULL)
	{
		goto ERROR_RET;
	}
	op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_VECTOR_END;

	current->vector_begin->arg.vector_begin_arg.failed_jmp_lineno = hotoparr_get_next_op_number(&self->hotoparr);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_field_begin(SCANNER_STACK *super, SP_NODE *current, const SP_NODE *prefix, const SP_NODE *name)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_FIELD_BEGIN;
	op->arg.field_begin_arg.filed_search_strategy = prefix->search_strategy;
	op->arg.field_begin_arg.name = name->var.val.bytes;

	current->field_begin = op;

	return E_HP_NOERROR;
}

hpint32 hotscript_do_field_end(SCANNER_STACK *super, SP_NODE *current)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);

	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_FIELD_END;

	current->field_begin->arg.field_begin_arg.failed_jmp_lineno = hotoparr_get_next_op_number(&self->hotoparr);
	return E_HP_NOERROR;
}


hpint32 hotscript_do_echo_field(SCANNER_STACK *super)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner_stack);
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->instruct = HOT_ECHO_FIELD;
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
	hpint32 ret;

	

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
