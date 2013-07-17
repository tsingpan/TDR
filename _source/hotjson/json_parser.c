#include "hotjson/json_parser.h"
#include "json_y.h"
#include "hotpot/hp_error.h"
#include "json_l.h"
#include "hotscript/script_parser.h"
#include "hotscript/hotlex.h"
#include "hotscript/hotobject_reader.h"
#include "hotscript/hotobject_writer.h"

hpint32 json_parser(JSON_PARSER *self, const char* file_name, HotObject *obj, SCRIPT_PARSER *sp)
{
	hpint32 ret;
	hpint64 data;
	FILE *fin;
	char c;
	YYSTYPE yystype;
	YYLTYPE yylloc;
	HotObjectWriter writer;
	HotObjectReader reader;
	hotobject_writer_init(&writer, obj);
	hotobject_reader_init(&reader, obj);

	self->result = HP_INVALID_ERROR_CODE;
	self->stack_num = 0;
	self->sp = sp;
	self->writer = &writer.super;
	self->reader = &reader.super;
	scanner_stack_init(&self->scanner_stack);
	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
	{
		//todo 这里改成配置文件， 已支持多语言
		fprintf(stderr, "can not open file\n");
		self->result = E_HP_ERROR;
		goto ERROR_RET;
	}

	ret = yyjsonparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;

ERROR_RET:
	return self->result;
}

hpint32 json_parser_str(JSON_PARSER *self, const char* str, hpuint32 str_len, HotObject *obj, SCRIPT_PARSER *sp)
{
	hpint32 ret;
	hpint64 data;
	FILE *fin;
	char c;
	YYSTYPE yystype;
	YYLTYPE yylloc;
	HotObjectWriter writer;
	HotObjectReader reader;
	hotobject_writer_init(&writer, obj);
	hotobject_reader_init(&reader, obj);

	self->result = HP_INVALID_ERROR_CODE;

	self->sp = sp;
	self->writer = &writer.super;
	self->reader = &reader.super;
	scanner_stack_init(&self->scanner_stack);
	scanner_stack_push(&self->scanner_stack, str, str + str_len, yycINITIAL);

	ret = yyjsonparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;
}



void yyjsonerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, char *s, ...) 
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

static void json_putc(HotVM *self, char c)
{
	SCANNER_STACK *ss = (SCANNER_STACK*)self->user_data;

	if(ss->buff_curr < ss->buff_limit)
	{
		*ss->buff_curr = c;
		++(ss->buff_curr);
	}
}




static void hexToDigit(unsigned int * val, const char * hex)
{
	unsigned int i;
	for (i=0;i<4;i++) {
		unsigned char c = hex[i];
		if (c >= 'A') c = (c & ~0x20) - 7;
		c -= '0';
		*val = (*val << 4) | c;
	}
}

static size_t Utf32toUtf8(unsigned int codepoint, char * utf8Buf) 
{
	if (codepoint < 0x80) {
		utf8Buf[0] = (char) codepoint;
		utf8Buf[1] = 0;
		return 1;
	} else if (codepoint < 0x0800) {
		utf8Buf[0] = (char) ((codepoint >> 6) | 0xC0);
		utf8Buf[1] = (char) ((codepoint & 0x3F) | 0x80);
		utf8Buf[2] = 0;
		return 2;
	} else if (codepoint < 0x10000) {
		utf8Buf[0] = (char) ((codepoint >> 12) | 0xE0);
		utf8Buf[1] = (char) (((codepoint >> 6) & 0x3F) | 0x80);
		utf8Buf[2] = (char) ((codepoint & 0x3F) | 0x80);
		utf8Buf[3] = 0;
		return 3;
	} else if (codepoint < 0x200000) {
		utf8Buf[0] =(char)((codepoint >> 18) | 0xF0);
		utf8Buf[1] =(char)(((codepoint >> 12) & 0x3F) | 0x80);
		utf8Buf[2] =(char)(((codepoint >> 6) & 0x3F) | 0x80);
		utf8Buf[3] =(char)((codepoint & 0x3F) | 0x80);
		utf8Buf[4] = 0;
		return 4;
	} else {
		utf8Buf[0] = '?';
		utf8Buf[1] = 0;
		//error?
		return 2;
	}
}

static void hpstring_append(hpstring *str, size_t *len, size_t *size, char c)
{
	if(*len + 1 >= *size)
	{
		*size = (*len + 1) * 2;
		*str = (hpstring)realloc(*str, *size);
	}
	(*str)[(*len)++] = c;
}

#define NORMAL_STRING_LENGTH 1
static hpint32 get_token_yylval(JSON_PARSER *jp, int token, YYSTYPE * yylval)
{
	SCANNER *self = scanner_stack_get_scanner(&jp->scanner_stack);

	switch(token)
	{
	case tok_double:
		yylval->type = E_HP_DOUBLE;
		yylval->val.d = atof(yytext);
		break;
	case tok_integer:
		yylval->type = E_HP_INT64;
		yylval->val.i64 = strtoll(yytext, NULL, 10);
		break;
	case tok_true:
		yylval->type = E_HP_BOOL;
		yylval->val.b = hptrue;
		break;
	case tok_false:
		yylval->type = E_HP_BOOL;
		yylval->val.b = hpfalse;
		break;
	case tok_null:
		yylval->type = E_HP_NULL;
		break;
	case tok_string:
		{
			char mark = *yytext;
			char *str = (char*)malloc(NORMAL_STRING_LENGTH);
			size_t str_size = NORMAL_STRING_LENGTH;
			size_t str_len = 0;

			while (YYCURSOR < YYLIMIT)
			{
				unsigned char ch = *YYCURSOR;
				++YYCURSOR;
				if(ch == '\\')
				{
					if (YYCURSOR >= YYLIMIT)
					{
						goto ERROR_RET;
					}
					ch = *YYCURSOR;
					++YYCURSOR;
					switch (ch)
					{
					case '"':
						hpstring_append(&str, &str_len, &str_size, '"');
						continue;
					case '\'':
						hpstring_append(&str, &str_len, &str_size, '\'');
						continue;
					case '\\':
						hpstring_append(&str, &str_len, &str_size, '\\');
						continue;
					case '/':
						hpstring_append(&str, &str_len, &str_size, '/');
						continue;
					case 'b':
						hpstring_append(&str, &str_len, &str_size, '\b');
						continue;
					case 'f':
						hpstring_append(&str, &str_len, &str_size, '\f');
						continue;
					case 'n':
						hpstring_append(&str, &str_len, &str_size, '\n');
						continue;
					case 'r':
						hpstring_append(&str, &str_len, &str_size, '\r');
						continue;
					case 't':
						hpstring_append(&str, &str_len, &str_size, '\t');
						continue;					
					default:
						{
							//printf("error\n");
							//yyerror("bad escape character\n");							
							goto ERROR_RET;
						}
					}
				}
				else
				{
					if (ch == mark)
					{
						yylval->type = E_HP_BYTES;
						yylval->val.bytes.ptr = str;
						yylval->val.bytes.len = str_len;
						goto done;
					}
					else
					{
						hpstring_append(&str, &str_len, &str_size, ch);
					}
				}
			}
		}
	}

done:
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

extern hpint32 json_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyjsonlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	JSON_PARSER *jp = HP_CONTAINER_OF(ss, JSON_PARSER, scanner_stack);
	int ret = 0;

	
	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(ss);
		yylloc_param->file_name = scanner->file_name;
		ret = json_lex_scan(scanner, yylloc_param, yylval_param);
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		if(ret <= 0)
		{
			if(scanner_stack_get_num(&jp->scanner_stack) <= 1)
			{
				break;
			}
			scanner_stack_pop(&jp->scanner_stack);
		}
		else if(ret == tok_script_begin)
		{
			//这里好玩了~
			const YYCTYPE * script_start = scanner->yy_cursor - 2;
			const char *yy_start;

			while(scanner->yy_cursor < scanner->yy_limit)
			{
				if((scanner->yy_cursor - script_start >= 4)
					&& (*(scanner->yy_cursor - 1) == '%') && (*scanner->yy_cursor == '>'))
				{
					++(scanner->yy_cursor);
					break;
				}
				else
				{
					++(scanner->yy_cursor);
				}
			}

			//脚本执行的结果放到ss缓存的最后面
			yy_start = ss->buff_curr;
			
			script_parser_str(jp->sp, script_start, scanner->yy_cursor, jp->reader, ss, json_putc);
			if(scanner_stack_push(ss, yy_start, ss->buff_curr, yycINITIAL) != E_HP_NOERROR)
			{
				break;
			}			
		}
		else
		{
			if(get_token_yylval(jp, ret, yylval_param) != E_HP_NOERROR)
			{
				jp->result = E_HP_ERROR;
				ret = -1;
				break;
			}
			break;
		}		
	}

	return ret;
}
