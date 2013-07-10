#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error.h"
#include "data_description_l.h"
#include "hotscript/hotlex.h"

#include <errno.h>
#include <string.h>

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib)
{
	hpint32 ret;
	AlphaMap *alpha_map = NULL;

	self->language_lib = language_lib;
	self->writer = writer;	
	self->result = E_HP_NOERROR;

	

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->constant = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL);
	self->result = E_HP_NOERROR;

	ret = yydataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
	if(ret != 0)
	{
		self->result = E_HP_SYNTAX_ERROR;
	}
	if(self->result != E_HP_NOERROR)
	{
		fprintf(stderr, self->result_str);
		fputc('\n', stderr);
	}


	return self->result;
}


void yydataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...) 
{
	DATA_PARSER *self = HP_CONTAINER_OF(jp, DATA_PARSER, scanner_stack);
	va_list ap;
	hpuint32 len;

	self->result_str[0] = 0;
	va_start(ap, s);
	if(yylloc->file_name[0])
	{
		snprintf(self->result_str, MAX_RESULT_STRING_LENGTH, "%s", yylloc->file_name);
	}
	len = strlen(self->result_str);
	if(self->result == E_HP_NOERROR)
	{
		self->result = E_HP_SYNTAX_ERROR;
	}
	snprintf(self->result_str + len, MAX_RESULT_STRING_LENGTH - len, "(%d): error %d: ", yylloc->first_line, self->result);
	len = strlen(self->result_str);
	vsnprintf(self->result_str + len, MAX_RESULT_STRING_LENGTH - len, s, ap);
	va_end(ap);
}
#define HOTDATA_EXTENSION ".dd"

hpint32 get_token_yylval(DATA_PARSER *dp, int token, YYSTYPE * yylval)
{
	SCANNER *self = scanner_stack_get_scanner(&dp->scanner_stack);
	SCANNER_STACK *ss = &dp->scanner_stack;


	switch(token)
	{
	case tok_import:
		{
			char file_name[1024];
			size_t len = 0;
			const char *i;

			while(self->yy_cursor < self->yy_limit)
			{
				if(*self->yy_cursor == ';')
				{
					++(self->yy_cursor);
					break;
				}
				else if((*self->yy_cursor == '\n') || (*self->yy_cursor == '\t') || (*self->yy_cursor == ' '))
				{
					++(self->yy_cursor);
				}
				else
				{
					file_name[len++] = *self->yy_cursor;
					++(self->yy_cursor);
				}
			}
			for(i = HOTDATA_EXTENSION; *i; ++i)
			{
				file_name[len++] = *i;
			}
			file_name[len] = 0;
			if(scanner_stack_push_file(ss, file_name, yycINITIAL) != E_HP_NOERROR)
			{
				dp->result = E_HP_ERROR;
				goto ERROR_RET;
			}
			break;
		}
	case tok_int:
		{
			errno = 0;
			yylval->var.type = E_HP_INT64;
			yylval->var.val.i64 = strtoll(yytext, NULL, 10);
			if (errno == ERANGE)
			{
				dp->result = E_HP_ERROR;
				goto ERROR_RET;
			}
			break;
		}
	case tok_hex:
		{
			errno = 0;
			yylval->var.type = E_HP_INT64;
			yylval->var.val.i64 = strtoll(yytext+2, NULL, 16);
			if (errno == ERANGE)
			{
				dp->result = E_HP_ERROR;
				goto ERROR_RET;
			}
			break;
		}
	case tok_unixcomment:
		{
			yylval->var.type = E_HP_BYTES;
			yylval->var.val.bytes.ptr = yytext + 1;
			yylval->var.val.bytes.len = yyleng - 1;
			while(yylval->var.val.bytes.len > 0 )
			{
				if(yylval->var.val.bytes.ptr[yylval->var.val.bytes.len - 1] == '\r')
				{
					--yylval->var.val.bytes.len;
				}
				else if(yylval->var.val.bytes.ptr[yylval->var.val.bytes.len - 1] == '\n')
				{
					--yylval->var.val.bytes.len;
				}
				else
				{
					break;
				}
			}
			break;
		}
	case tok_identifier:
		{
			yylval->var.type = E_HP_BYTES;
			yylval->var.val.bytes.ptr = yytext;
			yylval->var.val.bytes.len = yyleng;
			break;
		}
	}


	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

extern hpint32 ddc_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval);
int yydatalex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	DATA_PARSER *jp = HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack);
	int ret = 0;
	
	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(ss);
		yylloc_param->file_name = scanner->file_name;
		ret = ddc_lex_scan(scanner, yylloc_param, yylval_param);
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

void dp_on_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier)
{
	char id[1024];
	hpuint32 i;
	for(i = 0; i < sn_identifier->var.val.bytes.len; ++i)
	{
		id[i] = sn_identifier->var.val.bytes.ptr[i];
	}
	id[i] = 0;

	if(!trie_store_if_absent(self->constant, id, NULL))
	{
		const char *error_str = get_string_by_sid(self->language_lib, (LanguageStringID)E_HP_CONSTANT_REDEFINITION);
		self->result = E_HP_CONSTANT_REDEFINITION;
		yydataerror(yylloc, &self->scanner_stack, error_str, id);
	}
}

hpint32 data_parser_init(DATA_PARSER *self)
{
	scanner_stack_init(&self->scanner_stack);
	
	return E_HP_NOERROR;
}
