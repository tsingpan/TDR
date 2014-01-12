#include "parse/scanner.h"
#include "error.h"
#include "protocol/tlibc_xml_reader.h"
#include "language/language.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tdata_y.h"
#include <errno.h>


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

	for(i = 1; i < ptail; ++i)
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




void scanner_process(SCANNER_STACK *_sp)
{
	const char *i;
	SCANNER *sp = scanner_get(_sp);
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

SCANNER *scanner_get(SCANNER_STACK *self)
{
	if(self->stack_num <= 0)
	{
		return NULL;
	}
	return &self->stack[self->stack_num - 1];
}


tint32 scanner_push(SCANNER_STACK *self, const char *file_name, int state)
{
	FILE* fin;
	char c;
	YYCTYPE* yy_start = self->buff_curr;
	tuint32 i = 0;
	tuint32 len = 0;
	char realPath[TLIBC_MAX_FILE_PATH_LENGTH];
	SCANNER *scanner = NULL;

	if(self->stack_num >= MAX_SCANNER_STACK_DEEP)
	{
		goto ERROR_RET;
	}

	//不允许递归include
	for(i = 0; i < self->stack_num; ++i)
	{
		if(strcmp(self->stack[i].file_name, file_name) == 0)
		{
			goto ERROR_RET;
		}
	}

	snprintf(realPath, TLIBC_MAX_FILE_PATH_LENGTH, "%s%c%s", g_source_dir, TLIBC_FILE_SEPARATOR, file_name);
	len = TLIBC_MAX_FILE_PATH_LENGTH;
	if(path_repair(realPath, &len) != E_TD_NOERROR)
	{
		goto ERROR_RET;		
	}
	fin = fopen(realPath, "r");
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
	
	scanner = &self->stack[self->stack_num];
	strncpy(scanner->file_name, file_name, TLIBC_MAX_FILE_PATH_LENGTH);
	scanner->file_name[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
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
	return E_TD_ERROR;
}

tint32 scanner_pop(SCANNER_STACK *self)
{
	--(self->stack_num);
	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

void scanner_init(SCANNER_STACK *self)
{
	self->buff_curr = self->buff;
	self->buff_limit = self->buff + MAX_LEX_BUFF_SIZE;
	self->stack_num = 0;
}

tuint32 scanner_get_stack_deep(SCANNER_STACK *self)
{
	return self->stack_num;
}

static void scanner_stack_errorap_halt(SCANNER_STACK *self, const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, const char *s, va_list ap) 
{
	if((yylloc) && (yylloc->file_name[0]))
	{
		fprintf(stderr, "%s", yylloc->file_name);
	}
	if(yylloc)
	{
		fprintf(stderr, "(%d): ", yylloc->first_line);
	}
	fprintf(stderr, "error %d: ", result);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");

	exit(1);
}



void scanner_error(SCANNER_STACK *self, const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, ...) 
{
	const char *error_str = language_string_library_search(g_language_string_library, result);
	va_list ap;

	va_start(ap, result);
	scanner_stack_errorap_halt(self, yylloc, result, error_str, ap);
	va_end(ap);
}

#define MAX_SCANNER_ERROR_MSG_LENGTH 256
void tdataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...)
{
	const char *error_str = language_string_library_search(g_language_string_library, E_LS_SYNTAX_ERROR);
	char bison_error_msg[MAX_SCANNER_ERROR_MSG_LENGTH];
	
	va_list ap;
	va_start(ap, s);
	vsnprintf(bison_error_msg, MAX_SCANNER_ERROR_MSG_LENGTH, s, ap);
	scanner_error(jp, yylloc, E_LS_SYNTAX_ERROR, bison_error_msg);
	va_end(ap);
}




static tint32 get_token_yylval(SCANNER_STACK *self, int *token, PARSER_VALUE * yylval, const YYLTYPE *yylloc)
{
	switch(*token)
	{
	case tok_char:
		{
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_error(self, yylloc, E_LS_UNKNOW);
				break;
			}
			if(*YYCURSOR == '\\')
			{
				++YYCURSOR;
				switch(*YYCURSOR)
				{
				case 'b':
					yylval->sn_char = '\b';
					break;
				case 'f':
					yylval->sn_char = '\f';
					break;
				case 'n':
					yylval->sn_char = '\n';
					break;
				case 'r':
					yylval->sn_char = '\r';
					break;
				case 't':
					yylval->sn_char = '\t';
					break;
				case '\'':
					yylval->sn_char = '\'';
					break;
				case '\"':
					yylval->sn_char = '\"';
					break;
				case '\\':
					yylval->sn_char = '\\';
					break;
				case '/':
					yylval->sn_char = '/';
					break;
				default:
					break;
				}
				++YYCURSOR;
			}
			else
			{
				yylval->sn_char = *YYCURSOR;
				++YYCURSOR;
			}
			if(*YYCURSOR == '\'')
			{
				++YYCURSOR;				
			}
			else
			{
				scanner_error(self, yylloc, E_LS_UNKNOW);
			}
			break;
		}
	case tok_string:
		{
			tuint32 len = 0;
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_error(self, yylloc, E_LS_UNKNOW);
				break;
			}
			yylval->sn_string = YYCURSOR;
			while(YYCURSOR < YYLIMIT)
			{
				if(*YYCURSOR == '\\')
				{
					++YYCURSOR;

					++YYCURSOR;
					yylval->sn_string[len++] = *YYCURSOR;
				}
				else if(*YYCURSOR == '\"')
				{
					++YYCURSOR;
					yylval->sn_string[len] = 0;
					break;
				}
				else
				{
					yylval->sn_string[len++] = *YYCURSOR;
					++YYCURSOR;
				}
			}
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_error(self, yylloc, E_LS_UNKNOW);
			}
			else
			{				
				yyleng = YYCURSOR - yytext;
			}

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
					scanner_error(self, yylloc, E_LS_UNKNOW);
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
				scanner_error(self, yylloc, E_LS_UNKNOW);
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
					scanner_error(self, yylloc, E_LS_UNKNOW);
				}
			}
			break;
		}
	case tok_unixcomment:
		{
			tuint32 len = yyleng;
			yylval->sn_tok_unixcomment = yytext + 1;
			while(len - 1 > 0)
			{
				if((yytext[len - 1] == '\n') || (yytext[len - 1] == '\r'))
				{
					yytext[len - 1] = 0;
				}
				--len;
			}
			break;
		}
	case tok_identifier:
		{
			yylval->sn_tok_identifier.ptr = yytext;
			yylval->sn_tok_identifier.len = yyleng;
			break;
		}
	case tok_t_bool:
		{
			yylval->sn_st = E_ST_BOOL;
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
	case tok_reserved_keyword:
		{
			yytext[yyleng] = 0;
			scanner_error(self, yylloc, E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD, yytext);
		}
	}

	return E_TD_NOERROR;
}


int tdatalex(PARSER_VALUE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	int ret = 0;

	for(;;)
	{
		SCANNER *scanner = scanner_get(ss);
		yylloc_param->file_name = scanner->file_name;
		ret = scanner_scan(ss, yylloc_param, yylval_param);
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		if(ret <= 0)
		{
			if(ss->stack_num <= 1)
			{
				break;
			}
			scanner_pop(ss);
		}
		else
		{
			if(get_token_yylval(ss, &ret, yylval_param, yylloc_param) != E_TD_NOERROR)
			{
				ret = -1;
				break;
			}			
			break;
		}		
	}

	return ret;
}