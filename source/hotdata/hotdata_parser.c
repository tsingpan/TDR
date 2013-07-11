#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error.h"
#include "data_description_l.h"
#include "hotscript/hotlex.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib)
{
	hpint32 ret;
	AlphaMap *alpha_map = NULL;
	hpuint32 i;

	self->language_lib = language_lib;
	self->writer = writer;	
	self->result_num = 0;

	

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->constant = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL);

	ret = yydataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);

	for(i = 0;i < self->result_num; ++i)
	{
		fprintf(stderr, self->result_str[i]);
		fputc('\n', stderr);
	}

	if(self->result_num == 0)
	{
		return E_HP_NOERROR;
	}
	else
	{
		return E_HP_ERROR;
	}
}


void _dp_error(DATA_PARSER *self, const YYLTYPE *yylloc, const char *s, va_list ap) 
{	
	hpuint32 len;

	if(yylloc->file_name[0])
	{
		snprintf(self->result_str[self->result_num], MAX_RESULT_STRING_LENGTH, "%s", yylloc->file_name);
	}
	len = strlen(self->result_str[self->result_num]);
	snprintf(self->result_str[self->result_num] + len, MAX_RESULT_STRING_LENGTH - len, "(%d): error %d: ", yylloc->first_line, self->result[self->result_num]);
	len = strlen(self->result_str[self->result_num]);
	vsnprintf(self->result_str[self->result_num] + len, MAX_RESULT_STRING_LENGTH - len, s, ap);
}

void dp_error(DATA_PARSER *self, const YYLTYPE *yylloc, hpint32 result, ...) 
{
	va_list ap;
	const char *error_str = get_string_by_sid(self->language_lib, (LanguageStringID)result);

	self->result[self->result_num] = result;

	va_start(ap, result);
	_dp_error(self, yylloc, error_str, ap);
	va_end(ap);
	++(self->result_num);
}
void yydataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...)
{
	va_list ap;
	DATA_PARSER *self = HP_CONTAINER_OF(jp, DATA_PARSER, scanner_stack);

	
	self->result[self->result_num] = E_HP_SYNTAX_ERROR;
	va_start(ap, s);
	_dp_error(self, yylloc, s, ap);
	va_end(ap);
	++(self->result_num);
}


#define HOTDATA_EXTENSION ".dd"

hpint32 get_token_yylval(DATA_PARSER *dp, int token, YYSTYPE * yylval, const YYLTYPE *yylloc)
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
				dp_error(dp, yylloc, (hpint32)E_HP_CAN_NOT_OPEN_FILE, file_name);
			}
			break;
		}
	case tok_int:
		{
			errno = 0;
			yylval->body.sn_value.var.type = E_HP_INT64;
			yylval->body.sn_value.var.val.i64 = strtoll(yytext, NULL, 10);
			if (errno == ERANGE)
			{
				errno = 0;
				yylval->body.sn_value.var.type = E_HP_UINT64;
				yylval->body.sn_value.var.val.ui64 = strtoull(yytext, NULL, 10);				
				if(errno == ERANGE)
				{
					dp_error(dp, yylloc, (hpint32)E_HP_INTEGER_OVERFLOW);
				}

				
			}
			break;
		}
	case tok_hex:
		{
			errno = 0;
			yylval->body.sn_value.var.type = E_HP_INT64;
			yylval->body.sn_value.var.val.i64 = strtoll(yytext+2, NULL, 16);
			if (errno == ERANGE)
			{
				errno = 0;
				yylval->body.sn_value.var.type = E_HP_UINT64;
				yylval->body.sn_value.var.val.ui64 = strtoull(yytext + 2, NULL, 16);				
				if(errno == ERANGE)
				{
					dp_error(dp, yylloc, (hpint32)E_HP_INTEGER_OVERFLOW);
				}
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
	case tok_true:
		{
			yylval->var.type = E_HP_BOOL;
			yylval->var.val.b = hptrue;
			break;
		}
	case tok_false:
		{
			yylval->var.type = E_HP_BOOL;
			yylval->var.val.b = hpfalse;
			break;
		}
	}

	return E_HP_NOERROR;
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
			if(get_token_yylval(jp, ret, yylval_param, yylloc_param) != E_HP_NOERROR)
			{
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
	void *data;
	for(i = 0; i < sn_identifier->var.val.bytes.len; ++i)
	{
		id[i] = sn_identifier->var.val.bytes.ptr[i];
	}
	id[i] = 0;

	if(trie_retrieve(self->constant, id, &data))
	{
		dp_error(self, yylloc, E_HP_CONSTANT_REDEFINITION, id);
	}
}

hpint32 data_parser_init(DATA_PARSER *self)
{
	scanner_stack_init(&self->scanner_stack);
	self->result_num = 0;

	return E_HP_NOERROR;
}


void dp_on_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier, const SyntacticNode* sn_value)
{
	char id[1024];
	hpuint32 i;
	const SyntacticNode* val = NULL;
	size_t size;


	if(sn_value->body.sn_value.is_identifier)
	{
		void *data;

		for(i = 0; i < sn_value->body.sn_value.var.val.bytes.len; ++i)
		{
			id[i] = sn_value->body.sn_value.var.val.bytes.ptr[i];
		}
		id[i] = 0;

		if(!trie_retrieve(self->constant, id, &data))
		{
			dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
			goto done;
		}
		val = (const SyntacticNode*)data;
	}
	else
	{
		val = sn_value;
	}

	for(i = 0; i < sn_identifier->var.val.bytes.len; ++i)
	{
		id[i] = sn_identifier->var.val.bytes.ptr[i];
	}
	id[i] = 0;

	switch(sn_type->body.sn_type.type)
	{
	case E_SNT_INT8:
		size = sizeof(hpint8);
		break;
	case E_SNT_INT16:
		size = sizeof(hpint16);		
		break;
	case E_SNT_INT32:
		size = sizeof(hpint32);
		break;
	case E_SNT_INT64:
		size = sizeof(hpint64);
		break;
	case E_SNT_UINT8:
		size = sizeof(hpuint8);
		break;
	case E_SNT_UINT16:
		size = sizeof(hpuint16);
		break;
	case E_SNT_UINT32:
		size = sizeof(hpuint32);
		break;
	case E_SNT_UINT64:
		size = sizeof(hpuint64);
		break;
	default:
		dp_error(self, yylloc, (hpint32)E_HP_INVALID_CONSTANCE_TYPE);
		goto done;
	}
	size *= 8;
	if(val->body.sn_value.var.type == E_HP_UINT64)
	{
		if(val->body.sn_value.var.val.ui64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else if(val->body.sn_value.var.type == E_HP_INT64)
	{
		if(val->body.sn_value.var.val.i64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else
	{
		dp_error(self, yylloc, (hpint32)E_HP_UNKNOW_CONSTANT_VALUE);
		goto done;
	}

	
	if(!trie_store_if_absent(self->constant, id, sn_value))
	{
		dp_error(self, yylloc, (hpint32)E_HP_ERROR, id);
		goto done;
	}

done:
	return;
}
