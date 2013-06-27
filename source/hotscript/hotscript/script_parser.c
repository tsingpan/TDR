#include "hotscript/script_parser.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_error.h"
#include "script_l.h"
#include <string.h>

hpint32 scanner_fini(SCANNER *self)
{
	return E_HP_NOERROR;
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

extern hpint32 script_lex_scan(SCANNER *sp, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyscriptlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCRIPT_PARSER *sp)
{
	int ret;
	

	for(;;)
	{
		SCANNER *scanner = &sp->stack[sp->stack_num - 1];
		ret = script_lex_scan(scanner, yylloc_param, yylval_param);
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		if(ret == 0)
		{
			if(sp->stack_num <= 1)
			{
				break;
			}
			scanner_fini(scanner);
			--(sp->stack_num);
		}
		else if(ret == tok_import)
		{
			char file_name[1024];
			size_t len = 0;
			for(;;)
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
			script_push_file(sp, file_name);
		}
		else
		{
			break;
		}
	}
	

	return ret;
}

hpint32 scanner_init(SCANNER *self, const char *str, const hpint32 str_size)
{
	self->buff = str;
	self->buff_size = str_size;

	self->yy_limit = self->buff + self->buff_size;
	self->yy_state = yycINITIAL;
	self->yy_marker = self->buff;
	self->yy_last = self->buff;
	self->yy_cursor = self->buff;	
	self->yylineno = 1;
	self->yycolumn = 1;

	return E_HP_NOERROR;
}

hpint32 script_push_file(SCRIPT_PARSER *self, const char *file_name)
{
	FILE* fin;
	char c;
	char *start_ptr = self->buff_ + self->buff_size;
	size_t size = 0;

	fin = fopen(file_name, "rb");
	while((c = fgetc(fin)) != EOF)
	{
		start_ptr[(size)++] = c;
	}
	fclose(fin);
	self->buff_size += size;

	scanner_init(&self->stack[self->stack_num], start_ptr, size);
	++(self->stack_num);

	return E_HP_NOERROR;
}

hpint32 script_push_str(SCRIPT_PARSER *self, const char *str, size_t str_size)
{
	scanner_init(&self->stack[self->stack_num], str, str_size);
	++(self->stack_num);

	return E_HP_NOERROR;
}


hpint32 script_pop_file(SCRIPT_PARSER *self)
{
	self->buff_size = self->stack[self->stack_num - 1].buff - self->buff_;
	scanner_fini(&self->stack[self->stack_num - 1]);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 script_pop_str(SCRIPT_PARSER *self)
{
	scanner_fini(&self->stack[self->stack_num - 1]);
	--(self->stack_num);
	return E_HP_NOERROR;
}

extern int yyscriptparse (SCRIPT_PARSER *sp);
hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	hpint32 ret;

	SNODE snode;
	YYLTYPE yylloc;

	self->buff_size = 0;
	self->stack_num = 0;

	script_push_file(self, file_name);
	

	hotoparr_init(&self->hotoparr);

	self->reader = reader;
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
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}
	script_pop_file(self);

	hotvm_execute(&self->hotvm, &self->hotoparr, self->reader, user_data, uputc);

	return self->result;
}
