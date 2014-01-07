#include "parse/scanner.h"
#include "error/error_code.h"
#include "error/error_msg_reader.h"
#include "protocol/tlibc_xml_reader.h"
#include "error/error.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void scanner_init(SCANNER *self, char *yy_start, char *yy_limit, int state, const char *file_name)
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
}

void scanner_fini(SCANNER *self)
{
	self->yy_last = NULL;
}

void scanner_process(SCANNER *sp)
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
}

SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self)
{
	if(self->stack_num <= 0)
	{
		return NULL;
	}
	return &self->stack[self->stack_num - 1];
}

static tint32 path_repair(char* path, tuint32 *len)
{
	char file_path[TLIBC_MAX_FILE_PATH_LENGTH];
	char *p[TLIBC_MAX_FILE_PATH_LENGTH];
	tuint32 ptail = 0;
	tint32 first = 1;
	tuint32 i = 0;
	tuint32 j = 0;
	tuint32 file_path_len = 0;
	tuint32 tlen = 0;

	if((path == NULL) || (len == NULL))
	{
		return E_TD_ERROR;
	}
	file_path_len = (tuint32)strlen(path);

	snprintf(file_path, TLIBC_MAX_FILE_PATH_LENGTH, "%s", path);
	if(TLIBC_FILE_SEPARATOR == '/')
	{
		if(path[0] == '/')
		{
			p[0] = "/";
			ptail = 1;
		}
	}

	for(i = 0; i < file_path_len; ++i)
	{
		if((file_path[i] == '/') || (file_path[i] == '\\'))
		{
			file_path[i] = 0;
			first = 1;
		}
		else if(first)
		{
			p[ptail++] = &file_path[i];
			first = 0;
		}
	}

	for(i = 0; i < ptail; ++i)
	{
		if(p[i][0] == '.')
		{
			tlen = (tuint32)strlen(p[i]);			
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

	memset(path, 0, file_path_len);
	file_path_len = 0;

	for(i = 0; i < ptail; ++i)
	{
		if((file_path_len > 0) && (strlen(p[i]) > 0) && (path[file_path_len - 1] != TLIBC_FILE_SEPARATOR))
		{
			if(file_path_len < *len)
			{
				path[file_path_len++] += TLIBC_FILE_SEPARATOR;
			}
			else
			{
				*len = file_path_len;
				return E_TD_ERROR;
			}
		}

		for(j = 0; j < (tint32)strlen(p[i]); ++j)
		{
			if(file_path_len < *len)
			{
				path[file_path_len++] = p[i][j];
			}
			else
			{
				*len = file_path_len;
				return E_TD_ERROR;
			}
		}
	}

	if(file_path_len < *len)
	{
		path[file_path_len++] = 0;
		*len = file_path_len;
	}
	else
	{
		return E_TD_ERROR;
	}
	return E_TD_NOERROR;
}

tint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state)
{
	FILE* fin;
	char c;
	YYCTYPE* yy_start = self->buff_curr;
	tuint32 i = 0;
	tuint32 len = 0;
	char realPath[TLIBC_MAX_FILE_PATH_LENGTH];

	if(self->stack_num >= MAX_SCANNER_STACK_DEEP)
	{
		goto ERROR_RET;
	}

	//不允许递归include
	for(i = 0; i < self->stack_num; ++i)
		if(strcmp(self->stack[i].file_name, file_name) == 0)
		{
			goto ERROR_RET;
		}

	fin = fopen(file_name, "rb");
	if(fin == NULL)
	{
		for(i = 0; i < self->include_path_tail; ++i)
		{
			snprintf(realPath, TLIBC_MAX_FILE_PATH_LENGTH, "%s%c%s", self->include_path[i], TLIBC_FILE_SEPARATOR, file_name);
			len = TLIBC_MAX_FILE_PATH_LENGTH;
			if(path_repair(realPath, &len) == E_TD_NOERROR)
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
		goto ERROR_RET;
	}
	

	while((c = (char)fgetc(fin)) != EOF)
	{
		if(self->buff_curr == self->buff_limit)
		{
			//缓存大小不足， 解析失败
			goto F_ERROR_RET;
		}
		*self->buff_curr = c;
		++(self->buff_curr);
	}
	fclose(fin);

	//之前已经检查过self->stack_num是否出界了
	scanner_init(&self->stack[self->stack_num], yy_start, self->buff_curr, state, file_name);
	++(self->stack_num);

	return E_TD_NOERROR;
F_ERROR_RET:
	fclose(fin);
ERROR_RET:
	return E_TD_ERROR;
}
tint32 scanner_stack_push(SCANNER_STACK *self, char *yy_start, char *yy_limit, int state)
{
	if(self->stack_num >= MAX_SCANNER_STACK_DEEP)
	{
		goto ERROR_RET;
	}

	scanner_init(&self->stack[self->stack_num], yy_start, yy_limit, state, NULL);
	++(self->stack_num);

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}


tint32 scanner_stack_pop(SCANNER_STACK *self)
{
	SCANNER *scanner = scanner_stack_get_scanner(self);

	if(scanner == NULL)
	{
		goto ERROR_RET;
	}

	scanner_fini(scanner);
	--(self->stack_num);
	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

void scanner_stack_init(SCANNER_STACK *self)
{
	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_LEX_BUFF_SIZE;
	self->stack_num = 0;
	self->include_path_tail = 0;
	self->result_num = 0;
}

tint32 scanner_stack_add_path(SCANNER_STACK *self, const char* path)
{
	if(self->include_path_tail >= MAX_INCLUDE_PATH)
	{
		goto ERROR_RET;
	}
	strncpy(self->include_path[self->include_path_tail], path, TLIBC_MAX_FILE_PATH_LENGTH);
	++self->include_path_tail;

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

tuint32 scanner_stack_get_num(SCANNER_STACK *self)
{
	return self->stack_num;
}


void scanner_stack_errorap(SCANNER_STACK *self, const YYLTYPE *yylloc, TD_ERROR_CODE result, const char *s, va_list ap) 
{
	tuint32 len;

	if(self->result_num >= MAX_RESULT_NUM)
	{
		goto done;
	}

	if((yylloc) && (yylloc->file_name[0]))
	{
		snprintf(self->result_str[self->result_num], TD_MAX_ERROR_MSG_LENGTH, "%s", yylloc->file_name);
	}
	
	if(yylloc)
	{
		len = strlen(self->result_str[self->result_num]);
		snprintf(self->result_str[self->result_num] + len, TD_MAX_ERROR_MSG_LENGTH - len, "(%d): ", yylloc->first_line);
	}

	len = strlen(self->result_str[self->result_num]);
	snprintf(self->result_str[self->result_num] + len, TD_MAX_ERROR_MSG_LENGTH - len, "error %d: ", self->result[self->result_num]);
	
	len = strlen(self->result_str[self->result_num]);
	vsnprintf(self->result_str[self->result_num] + len, TD_MAX_ERROR_MSG_LENGTH - len, s, ap);

	self->result[self->result_num] = result;
	++(self->result_num);

	return;
done:
	return;
}



void scanner_stack_error(SCANNER_STACK *self, const YYLTYPE *yylloc, TD_ERROR_CODE result, ...) 
{
	va_list ap;
	const char *error_str = NULL;
	if(self->result_num >= MAX_RESULT_NUM)
	{
		goto done;
	}

	error_str = error_search_msg(result);
	self->result[self->result_num] = result;
	va_start(ap, result);
	scanner_stack_errorap(self, yylloc, result, error_str, ap);
	va_end(ap);

	return;
done:
	return;
}
