#include "parse/scanner.h"
#include "tlibc/protocol/tlibc_xml_reader.h"

#include "error.h"
#include "language/language.h"
#include "tdata_y.h"
#include "globals.h"

#include <errno.h>
#include <string.h>
#include <stdarg.h>

static int32_t path_repair(char* path, uint32_t *len)
{
	char file_path[TLIBC_MAX_PATH_LENGTH];
	char *p[TLIBC_MAX_PATH_LENGTH];
	uint32_t ptail = 0;
	int32_t first = 1;
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t file_path_len = 0;
	uint32_t tlen = 0;

	if((path == NULL) || (len == NULL))
	{
		return E_TD_ERROR;
	}
	file_path_len = (uint32_t)strlen(path);

	snprintf(file_path, TLIBC_MAX_PATH_LENGTH, "%s", path);
	if(path[0] == '/')
	{
		p[0] = "/";
		ptail = 1;
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

	for(i = 1; i < ptail; ++i)
	{
		if(p[i][0] == '.')
		{
			tlen = (uint32_t)strlen(p[i]);			
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

		for(j = 0; j < (int32_t)strlen(p[i]); ++j)
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




void scanner_locate(SCANNER *self)
{
	const char *i;
	SCANNER_CONTEXT *sp = scanner_top(self);
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

SCANNER_CONTEXT *scanner_top(SCANNER *self)
{
	if((self->stack_num <= 0) || (self->stack_num > MAX_SCANNER_STACK_SIZE))
	{
		return NULL;
	}
	return &self->stack[self->stack_num - 1];
}

int32_t scanner_push(SCANNER *self, const char *file_name, int state)
{
	FILE* fin;
	char c;
	YYCTYPE* yy_start = self->buff_curr;
	uint32_t i = 0;
	uint32_t len = 0;
	char realPath[TLIBC_MAX_PATH_LENGTH];
	SCANNER_CONTEXT *scanner = NULL;
	TD_ERROR_CODE ret = E_TD_NOERROR;

	if(self->stack_num >= MAX_SCANNER_STACK_SIZE)
	{
		ret = E_TD_SCANNER_STACK_SIZE_PROBLEM;
		goto ERROR_RET;
	}

	for(i = 0; i < self->stack_num; ++i)
	{
		if(strcmp(self->stack[i].file_name, file_name) == 0)
		{
			ret = E_TD_SCANNER_READLY_IN_STACK;
			goto ERROR_RET;
		}
	}

	
	fin = fopen(file_name, "r");
	if(fin == NULL)
	{
		for(i = 0; i < g_include_dir_num; ++i)
		{
			snprintf(realPath, TLIBC_MAX_PATH_LENGTH, "%s%c%s", g_include_dir[i], TLIBC_FILE_SEPARATOR, file_name);
			len = TLIBC_MAX_PATH_LENGTH;
			if(path_repair(realPath, &len) != E_TD_NOERROR)
			{
				ret = E_TD_SCANNER_CAN_NOT_OPEN_FILE;
				goto ERROR_RET;
			}
			fin = fopen(realPath, "r");
			if(fin != NULL)
			{
				break;
			}
		}
		
		if(fin == NULL)
		{
			ret = E_TD_SCANNER_CAN_NOT_OPEN_FILE;
			goto ERROR_RET;
		}
	}

	while((c = (char)fgetc(fin)) != EOF)
	{
		if(self->buff_curr == self->buff_limit)
		{
			ret = E_TD_SCANNER_OUT_OF_MEMORY;
			goto F_ERROR_RET;
		}
		*self->buff_curr = c;
		++(self->buff_curr);
	}
	fclose(fin);

	scanner = &self->stack[self->stack_num];
	strncpy(scanner->file_name, file_name, TLIBC_MAX_PATH_LENGTH);
	scanner->file_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
	scanner->yy_start = yy_start;
	scanner->yy_limit = self->buff_curr;
	scanner->yy_state = state;
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->yy_cursor = scanner->yy_start;	
	scanner->yylineno = 1;
	scanner->yycolumn = 1;
	++(self->stack_num);

	return E_TD_NOERROR;
F_ERROR_RET:
	fclose(fin);
ERROR_RET:
	return ret;
}

void scanner_pop(SCANNER *self)
{
	--(self->stack_num);
}

void scanner_init(SCANNER *self)
{
	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_LEX_BUFF_SIZE;
	self->stack_num = 0;
}

uint32_t scanner_size(SCANNER *self)
{
	return self->stack_num;
}
static void scanner_error_ap(const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, va_list ap)
{
	const char *error_str = tdata_strerror(result);	
	if((yylloc) && (yylloc->file_name[0]))
	{
		fprintf(stderr, "%s", yylloc->file_name);
	}
	if(yylloc)
	{
		fprintf(stderr, "(%d): ", yylloc->last_line);
	}
	fprintf(stderr, "error %d: ", result);
	vfprintf(stderr, error_str, ap);
	fprintf(stderr, "\n");
}

void scanner_error(const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, ...)
{
	va_list ap;
	va_start(ap, result);
	scanner_error_ap(yylloc, result, ap);
	va_end(ap);
}


void scanner_error_halt(const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, ...) 
{
	va_list ap;
	va_start(ap, result);
	scanner_error_ap(yylloc, result, ap);
	va_end(ap);

	exit(result);
}

#define MAX_SCANNER_ERROR_MSG_LENGTH 256
void tdataerror(const YYLTYPE *yylloc, SCANNER *self, const char *s, ...)
{
	va_list ap;
	char bison_error_msg[MAX_SCANNER_ERROR_MSG_LENGTH];
	TLIBC_UNUSED(self);
	
	va_start(ap, s);
	vsnprintf(bison_error_msg, MAX_SCANNER_ERROR_MSG_LENGTH, s, ap);
	scanner_error_halt(yylloc, E_LS_SYNTAX_ERROR, bison_error_msg);
}

static int read_char(char c)
{
	switch(c)
	{
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case '\'':
		return '\'';
	case '\"':
		return '\"';
	case '\\':
		return '\\';
	default:
		return -1;
	}
}

static void get_yylval_tok_char(SCANNER *self, int *token, SCANNER_TOKEN_VALUE * yylval, const YYLTYPE *yylloc)
{
	*token = tok_char;
	if(YYCURSOR >= YYLIMIT)
	{
		goto ERROR_RET;
	}
	if(*YYCURSOR == '\\')
	{
		int c;
		++YYCURSOR;
		if(YYCURSOR >= YYLIMIT)
		{
			goto ERROR_RET;
		}
		c = read_char(*YYCURSOR);
		if(c == -1)
		{
			goto ERROR_RET;
		}
		yylval->sn_char = (char)c;
		++YYCURSOR;
	}
	else
	{
		yylval->sn_char = *YYCURSOR;
		++YYCURSOR;
	}

	if(YYCURSOR >= YYLIMIT)
	{
		goto ERROR_RET;
	}
	if(*YYCURSOR != '\'')
	{
		goto ERROR_RET;
	}
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	return;
ERROR_RET:
	scanner_error_halt(yylloc, E_LS_CHARACTER_CONSTANT_FORMAT_ERROR);
}

static void get_yylval_tok_string(SCANNER *self, int *token, SCANNER_TOKEN_VALUE * yylval, const YYLTYPE *yylloc)
{
	uint32_t len = 0;
	
	*token = tok_string;

	yylval->sn_string = YYCURSOR;
	while(YYCURSOR < YYLIMIT)
	{
		if(YYCURSOR >= YYLIMIT)
		{
			goto ERROR_RET;
		}
		if(*YYCURSOR == '\\')
		{
			int c;
			++YYCURSOR;
			if(YYCURSOR >= YYLIMIT)
			{
				goto ERROR_RET;
			}
			c = read_char(*YYCURSOR);
			if(c == -1)
			{
				goto ERROR_RET;
			}
			yylval->sn_string[len++] = (char)c;
			++YYCURSOR;
		}
		else if(*YYCURSOR == '\"')
		{
			yylval->sn_string[len] = 0;
			++YYCURSOR;
			goto done;
		}
		else
		{
			yylval->sn_string[len++] = *YYCURSOR;
			++YYCURSOR;
		}
	}
ERROR_RET:
	scanner_error_halt(yylloc, E_LS_STRING_CONSTANT_FORMAT_ERROR);
done:
	YYMARKER = YYCURSOR;
	yyleng = YYCURSOR - yytext;
}

static int32_t get_yylval(SCANNER *self, int *token, SCANNER_TOKEN_VALUE * yylval, const YYLTYPE *yylloc)
{
	switch(*token)
	{
	case tok_char:
		{
			get_yylval_tok_char(self, token, yylval, yylloc);
			break;
		}
	case tok_string:
		{
			get_yylval_tok_string(self, token, yylval, yylloc);
			break;
		}
	case tok_int:
		{
			errno = 0;
			*token = tok_int64;
			yylval->sn_int64 = strtoll(yytext, NULL, 10);			
			if (errno == ERANGE)
			{
				errno = 0;
				*token = tok_uint64;
				yylval->sn_uint64 = strtoull(yytext, NULL, 10);				
				if(errno == ERANGE)
				{
					scanner_error_halt(yylloc, E_LS_NUMBER_ERROR_RANGE);
				}
			}
			break;
		}
	case tok_hex:
		{
			errno = 0;
			*token = tok_hex_int64;
			yylval->sn_hex_int64 = strtoll(yytext+2, NULL, 16);
			if (errno == ERANGE)
			{
				errno = 0;
				*token = tok_hex_uint64;
				yylval->sn_hex_uint64 = strtoull(yytext + 2, NULL, 16);				
				if(errno == ERANGE)
				{
					scanner_error_halt(yylloc, E_LS_NUMBER_ERROR_RANGE);
				}
			}
			break;
		}
	case tok_double:
		{
			errno = 0;
			*token = tok_double;
			yylval->sn_d = strtod(yytext, NULL);
			if (errno == ERANGE)
			{
				scanner_error_halt(yylloc, E_LS_NUMBER_ERROR_RANGE);
			}
			break;
		}
	case tok_unixcomment:
		{
			uint32_t i;
			yylval->sn_tok_unixcomment = yytext + 1;
			for(i = yyleng - 1; i > 0; --i)
			{
				if((yytext[i] == '\n') || (yytext[i] == '\r'))
				{
					yytext[i] = 0;
					break;
				}
			}
			break;
		}
	case tok_identifier:
		{
			if(yyleng >= TLIBC_MAX_LENGTH_OF_IDENTIFIER)
			{
				scanner_error_halt(yylloc, E_LS_IDENTIFIER_LENGTH_ERROR, TLIBC_MAX_LENGTH_OF_IDENTIFIER);
			}
			memcpy(yylval->sn_tok_identifier, yytext, yyleng);
			yylval->sn_tok_identifier[yyleng] = 0;
			break;
		}
	case tok_reserved_keyword:
		{
			yytext[yyleng] = 0;
			scanner_error_halt(yylloc, E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD, yytext);
			break;
		}
	case tok_t_char:
		{
			yylval->sn_st = E_ST_CHAR;
			break;
		}
	case tok_t_double:
		{
			yylval->sn_st = E_ST_DOUBLE;
			break;
		}
	case tok_t_int8:
		{
			yylval->sn_st = E_ST_INT8;
			break;
		}
	case tok_t_int16:
		{
			yylval->sn_st = E_ST_INT16;
			break;
		}
	case tok_t_int32:
		{
			yylval->sn_st = E_ST_INT32;
			break;
		}
	case tok_t_int64:
		{
			yylval->sn_st = E_ST_INT64;
			break;
		}
	case tok_t_uint8:
		{
			yylval->sn_st = E_ST_UINT8;
			break;
		}
	case tok_t_uint16:
		{
			yylval->sn_st = E_ST_UINT16;
			break;
		}
	case tok_t_uint32:
		{
			yylval->sn_st = E_ST_UINT32;
			break;
		}
	case tok_t_uint64:
		{
			yylval->sn_st = E_ST_UINT64;
			break;
		}
	case tok_t_string:
		{
			yylval->sn_st = E_ST_STRING;
			break;
		}
	}

	return E_TD_NOERROR;
}


int tdatalex(SCANNER_TOKEN_VALUE * yylval_param, YYLTYPE * yylloc_param , SCANNER *self)
{
	int ret = 0;

	for(;;)
	{
		SCANNER_CONTEXT *scanner = scanner_top(self);
		strncpy(yylloc_param->file_name, scanner->file_name, TLIBC_MAX_PATH_LENGTH);
		yylloc_param->file_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
		ret = scanner_scan(self, yylloc_param);
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		if(ret == 0)
		{
			if(self->stack_num == 1)
			{
				goto done;
			}
			scanner_pop(self);
		}
		else
		{
			if(get_yylval(self, &ret, yylval_param, yylloc_param) != E_TD_NOERROR)
			{
				ret = -1;
				goto done;
			}			
			break;
		}		
	}
done:
	return ret;
}
