#include "hotscript/hotlex.h"
#include "hotpot/hp_error.h"

#include <stdio.h>
#include <string.h>


hpint32 scanner_fini(SCANNER *self)
{
	return E_HP_NOERROR;
}

hpint32 scanner_init(SCANNER *self, const char *str, const hpint32 str_size, int state)
{
	self->buff = str;
	self->buff_size = str_size;

	self->yy_limit = self->buff + self->buff_size;
	self->yy_state = state;
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

hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state)
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

	scanner_stack_push(self, str, str_size, state);
	return E_HP_NOERROR;
}

hpint32 scanner_stack_push(SCANNER_STACK *self, const char *str, size_t str_size, int state)
{
	scanner_init(&self->stack[self->stack_num], str, str_size, state);
	++(self->stack_num);

	return E_HP_NOERROR;
}


hpint32 scanner_stack_pop(SCANNER_STACK *self)
{
	SCANNER *scanner = scanner_stack_get_scanner(self);
	
	if(scanner->buff + scanner->buff_size == self->buff + self->buff_size)
	{		
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
