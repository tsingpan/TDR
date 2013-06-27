#include "hotscript/hotlex.h"
#include "hotpot/hp_error.h"

#include <stdio.h>
#include <string.h>


hpint32 scanner_fini(SCANNER *self)
{
	self->yy_last = NULL;

	return E_HP_NOERROR;
}

hpint32 scanner_process(SCANNER *sp)
{
	const char *i;
	for(i = sp->yy_last; i < sp->yy_cursor;)
	{
		if(*i == '\n')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
			++i;
		}
		else if(*i == '\r')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
			++i;
			if((i < sp->yy_cursor) && (*i == '\n'))
			{
				++i;
			}
		}
		else
		{
			++(sp->yycolumn);
			++i;
		}
	}
	sp->yy_last = sp->yy_cursor;

	return E_HP_NOERROR;
}

hpint32 scanner_init(SCANNER *self, const char *yy_start, const char *yy_limit, int state, const char *file_name)
{
	if(file_name)
	{
		strncpy(self->file_name, file_name, MAX_FILE_NAME_LENGTH);
	}
	else
	{
		self->file_name[0];
	}
	self->yy_start = yy_start;
	self->yy_limit = yy_limit;
	self->yy_state = state;
	self->yy_marker = self->yy_start;
	self->yy_last = self->yy_start;
	self->yy_cursor = self->yy_start;	
	self->yylineno = 1;
	self->yycolumn = 1;
	return E_HP_NOERROR;
}

SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self)
{
	return &self->stack[self->stack_num - 1];
}

hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state)
{
	FILE* fin;
	char c;
	const YYCTYPE* yy_start = self->buff_curr;
	hpuint32 i;

	for(i = 0; i < self->stack_num; ++i)
		if(strcmp(self->stack[i].file_name, file_name) == 0)
		{
			return E_HP_ERROR;
		}

	fin = fopen(file_name, "rb");
	while((c = (char)fgetc(fin)) != EOF)
	{
		if(self->buff_curr == self->buff_limit)
		{
			return E_HP_ERROR;
		}
		*self->buff_curr = c;
		++(self->buff_curr);
	}
	fclose(fin);

	scanner_init(&self->stack[self->stack_num], yy_start, self->buff_curr, state, file_name);
	++(self->stack_num);

	return E_HP_NOERROR;
}
hpint32 scanner_stack_push(SCANNER_STACK *self, const char *yy_start, const char *yy_limit, int state)
{
	scanner_init(&self->stack[self->stack_num], yy_start, yy_limit, state, NULL);
	++(self->stack_num);

	return E_HP_NOERROR;
}


hpint32 scanner_stack_pop(SCANNER_STACK *self)
{
	SCANNER *scanner = scanner_stack_get_scanner(self);
	
	//如果这个缓存在自身缓存的末尾， 那么移除这个缓存
	if(scanner->yy_limit == self->buff_curr)
	{
		self->buff_curr -= scanner->yy_limit - scanner->yy_start;		
	}

	scanner_fini(scanner);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 scanner_stack_init(SCANNER_STACK *self)
{
	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_BUFF_SIZE;
	self->stack_num = 0;

	return E_HP_NOERROR;
}

hpuint32 scanner_stack_get_num(SCANNER_STACK *self)
{
	return self->stack_num;
}
