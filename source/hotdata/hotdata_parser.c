#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error.h"
#include "data_description_l.h"
#include "hotscript/hotlex.h"

#include <errno.h>
#include <string.h>

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer)
{
	hpint32 ret;
	self->writer = writer;

	self->result = E_HP_NOERROR;

	scanner_stack_init(&self->scanner_stack);
	scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL);

	ret = yydataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;
}


void yydataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, char *s, ...) 
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
			while(self->yy_cursor < self->yy_limit)
			{
				if(*self->yy_cursor == ';')
				{
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
			yylval->var.val.i64 = strtoll(yytext+2, NULL, 10);
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
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		ret = ddc_lex_scan(scanner, yylloc_param, yylval_param);
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

hpint32 dp_on_const(SCANNER_STACK *super, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier, const SyntacticNode* sn_value)
{
	DATA_PARSER *self = HP_CONTAINER_OF(super, DATA_PARSER, scanner_stack);


	return E_HP_NOERROR;
}
