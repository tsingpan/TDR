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

SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self)
{
	return &self->stack[self->stack_num - 1];
}

hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name)
{
	FILE* fin;
	char c;
	char *str = self->buff + self->buff_size;
	size_t str_size = 0;
	hpuint32 i;


	for(i = 0; i < self->file_name_list_num; ++i)
		if(strcmp(self->file_name_list[i], file_name) == 0)
		{
			return E_HP_ERROR;
		}

	strncpy(self->file_name_list[self->file_name_list_num], file_name, MAX_FILE_NAME_LENGTH);
	++(self->file_name_list_num);

	fin = fopen(file_name, "rb");
	while((c = fgetc(fin)) != EOF)
	{
		str[(str_size)++] = c;
	}
	fclose(fin);
	self->buff_size += str_size;

	scanner_stack_push(self, str, str_size);
	return E_HP_NOERROR;
}

hpint32 scanner_stack_push(SCANNER_STACK *self, const char *str, size_t str_size)
{
	scanner_init(&self->stack[self->stack_num], str, str_size);
	++(self->stack_num);

	return E_HP_NOERROR;
}


hpint32 scanner_stack_pop(SCANNER_STACK *self)
{
	SCANNER *scanner = scanner_stack_get_scanner(self);
	
	if(scanner->buff + scanner->buff_size == self->buff + self->buff_size)
	{
		/*
		int i;
		printf("---------------------------------------------------------------------\n");		
		for(i = 0;i < self->buff_size; ++i)
		{
			putc(self->buff[i], stdout);
		}
		printf("---------------------------------------------------------------------\n");
		*/
		self->buff_size -= scanner->buff_size;
		--(self->file_name_list_num);
	}

	scanner_fini(scanner);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 scanner_stack_init(SCANNER_STACK *self)
{
	self->buff_size = 0;
	self->stack_num = 0;
	self->file_name_list_num = 0;
}

hpuint32 scanner_stack_get_num(SCANNER_STACK *self)
{
	return self->stack_num;
}

extern hpint32 script_lex_scan(SCANNER *sp, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyscriptlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCRIPT_PARSER *sp)
{
	int ret;

	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(&sp->scanner_stack);
		ret = script_lex_scan(scanner, yylloc_param, yylval_param);
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
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
			if(scanner_stack_push_file(&sp->scanner_stack, file_name) != E_HP_NOERROR)
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



extern int yyscriptparse (SCRIPT_PARSER *sp);
hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc)
{
	hpint32 ret;

	SNODE snode;
	YYLTYPE yylloc;

	

	scanner_stack_init(&self->scanner_stack);


	scanner_stack_push_file(&self->scanner_stack, file_name);
	

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
