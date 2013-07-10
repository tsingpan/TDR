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
	for(i = sp->yy_last; i < sp->yy_cursor;++i)
	{
		if(*i == '\n')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
		}		
		else if(*i == '\r')
		{
			sp->yycolumn = 1;			
		}
		else
		{
			++(sp->yycolumn);
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
		self->file_name[0] = 0;
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

static hpint32 fix_path(char* _path, hpuint32 *_len)
{	
	char path[HP_MAX_FILE_PATH_LENGTH];
	char *p[HP_MAX_FILE_PATH_LENGTH];
	hpuint32 ptail = 0;
	hpint32 first = 1;
	hpuint32 i = 0;
	hpuint32 j = 0;
	hpuint32 len = 0;
	hpuint32 tlen = 0;

	if((_path == NULL) || (_len == NULL))
	{
		return E_HP_ERROR;
	}
	len = (hpuint32)strlen(_path);

	snprintf(path, HP_MAX_FILE_PATH_LENGTH, "%s", _path);
#ifndef WIN32
	if(_path[0] == '/')
	{
		p[0] = "/";
		ptail = 1;
	}
#endif //WIN32

	for(i = 0; i < len; ++i)
	{
		if((path[i] == '/') || (path[i] == '\\'))
		{
			path[i] = 0;
			first = 1;
		}
		else if(first)
		{
			p[ptail++] = &path[i];
			first = 0;
		}
	}

	for(i = 0; i < ptail; ++i)
	{
		if(p[i][0] == '.')
		{
			tlen = (hpuint32)strlen(p[i]);			
			for(j = i - 1; (tlen > 1) && (j >= 0); --j)
			{
				if(p[j][0] == '.')
				{
					break;
				}
				else if(p[j][0] != 0)
				{
					p[j][0] = 0;
					--tlen;
				}
			}

			for(j = 0; j < tlen; ++j)
			{
				p[i][j] = '.';
			}
			p[i][tlen] = 0;

			if(tlen == 1)
			{
				p[i][0] = 0;
			}
		}
	}

	memset(_path, 0, len);
	len = 0;

	for(i = 0; i < ptail; ++i)
	{
		if((len > 0) && (strlen(p[i]) > 0) && (_path[len - 1] != HP_FILE_SEPARATOR))
		{
			if(len < *_len)
			{
				_path[len++] += HP_FILE_SEPARATOR;
			}
			else
			{
				*_len = len;
				return E_HP_ERROR;
			}
		}

		for(j = 0; j < (hpint32)strlen(p[i]); ++j)
		{
			if(len < *_len)
			{
				_path[len++] = p[i][j];
			}
			else
			{
				*_len = len;
				return E_HP_ERROR;
			}
		}
	}

	if(len < *_len)
	{
		_path[len++] = 0;
		*_len = len;
	}
	return E_HP_NOERROR;
}

hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state)
{
	FILE* fin;
	char c;
	const YYCTYPE* yy_start = self->buff_curr;
	hpint32 i = 0;
	hpuint32 len = 0;
	char realPath[HP_MAX_FILE_PATH_LENGTH];

	for(i = 0; i < self->stack_num; ++i)
		if(strcmp(self->stack[i].file_name, file_name) == 0)
		{
			return E_HP_ERROR;
		}

	fin = fopen(file_name, "rb");
	if(fin == NULL)
	{
		for(i = 0; i < self->include_path_tail; ++i)
		{
			snprintf(realPath, HP_MAX_FILE_PATH_LENGTH, "%s%c%s", self->include_path[i], HP_FILE_SEPARATOR, file_name);
			len = HP_MAX_FILE_PATH_LENGTH;
			if(fix_path(realPath, &len) == E_HP_NOERROR)
			{
				fin = fopen(realPath, "r");
				if(fin != NULL)
				{
					break;
				}
			}
		}
	}
	if(fin == NULL)
	{
		return E_HP_ERROR;
	}
	

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

	scanner_fini(scanner);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 scanner_stack_init(SCANNER_STACK *self)
{
	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_BUFF_SIZE;
	self->stack_num = 0;
	self->include_path_tail = 0;

	return E_HP_NOERROR;
}

hpint32 scanner_stack_add_path(SCANNER_STACK *self, const char* path)
{
	strncpy(self->include_path[self->include_path_tail], path, HP_MAX_FILE_PATH_LENGTH);
	++self->include_path_tail;

	return E_HP_NOERROR;
}

hpuint32 scanner_stack_get_num(SCANNER_STACK *self)
{
	return self->stack_num;
}
