#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error.h"
#include "data_description_l.h"
#include "hotscript/hotlex.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib)
{
	hpint32 ret;
	AlphaMap *alpha_map = NULL;
	hpuint32 i;

	self->language_lib = language_lib;
	self->writer = writer;	
	self->result_num = 0;
	//self->symbol_list_num = 0;

	

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->hotdata_symbols = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	strncpy(self->file_name, file_name, MAX_FILE_NAME_LENGTH);
	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
	{
		self->result[0] = E_HP_ERROR;
		self->result_num = 1;		
		goto done;
	}

	ret = yydataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
done:
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

//因为一次编译需要返回多个错误， 所以要确保错误码没有重复
static void dp_error_push_back(DATA_PARSER *self)
{
	/*
	hpuint32 i;
	for(i = 0;i < self->result_num; ++i)
	{
		if((self->result[i] == self->result[self->result_num]) 
			&& (strcmp(self->result_str[i], self->result_str[self->result_num]) == 0))
		{
			return;
		}
	}
	*/
	++(self->result_num);
}
static void _dp_error(DATA_PARSER *self, const YYLTYPE *yylloc, const char *s, va_list ap) 
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
	dp_error_push_back(self);
}
void yydataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...)
{
	va_list ap;
	DATA_PARSER *self = HP_CONTAINER_OF(jp, DATA_PARSER, scanner_stack);

	
	self->result[self->result_num] = E_HP_SYNTAX_ERROR;
	va_start(ap, s);
	_dp_error(self, yylloc, s, ap);
	va_end(ap);
	dp_error_push_back(self);
}


hpint32 get_token_yylval(DATA_PARSER *dp, int *token, YYSTYPE * yylval, const YYLTYPE *yylloc)
{
	SCANNER *self = scanner_stack_get_scanner(&dp->scanner_stack);
	SCANNER_STACK *ss = &dp->scanner_stack;


	switch(*token)
	{
	case tok_import:
		{
			const char *i;

			yylval->sn_tok_import = NULL;

			while(self->yy_cursor < self->yy_limit)
			{
				if(*self->yy_cursor == ';')
				{
					*self->yy_cursor = 0;
					++(self->yy_cursor);
					break;
				}
				else if((*self->yy_cursor == '\n') || (*self->yy_cursor == '\t') || (*self->yy_cursor == ' '))
				{
					*self->yy_cursor = 0;
					++(self->yy_cursor);
				}
				else
				{
					if(yylval->sn_tok_import == NULL)
					{
						yylval->sn_tok_import = self->yy_cursor;
					}

					++(self->yy_cursor);
				}
			}
			
			break;
		}
	case tok_char:
		{
			if(YYCURSOR >= YYLIMIT)
			{
				dp_error(dp, yylloc, (hpint32)E_HP_ERROR);
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
				dp_error(dp, yylloc, (hpint32)E_HP_ERROR);
			}
			break;
		}
	case tok_string:
		{
			hpuint32 len = 0;
			if(YYCURSOR >= YYLIMIT)
			{
				dp_error(dp, yylloc, (hpint32)E_HP_ERROR);
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
				dp_error(dp, yylloc, (hpint32)E_HP_ERROR);
			}
			else
			{				
				yyleng = YYCURSOR - yytext;
			}
			
			break;
		}
	case tok_bool:
		{
			if(yytext[0] == 't')
			{
				yylval->sn_bool = hptrue;
			}
			else
			{
				yylval->sn_bool = hpfalse;
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
					dp_error(dp, yylloc, (hpint32)E_HP_INTEGER_OVERFLOW);
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
					dp_error(dp, yylloc, (hpint32)E_HP_INTEGER_OVERFLOW);
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
					dp_error(dp, yylloc, (hpint32)E_HP_INTEGER_OVERFLOW);
				}
			}
			break;
		}
	case tok_unixcomment:
		{
			hpuint32 len = yyleng;
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
			if(get_token_yylval(jp, &ret, yylval_param, yylloc_param) != E_HP_NOERROR)
			{
				ret = -1;
				break;
			}			
			break;
		}		
	}

	return ret;
}



hpint32 data_parser_init(DATA_PARSER *self)
{
	scanner_stack_init(&self->scanner_stack);
	self->result_num = 0;
	self->definition_list_num = 0;
	self->domain[0] = 0;

	return E_HP_NOERROR;
}


//do
void dp_do_Definition(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_DEFINITION *pn_definition)
{
	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[1024];
		snprintf(file_name, sizeof(file_name), "%s%s", pn_definition->definition.de_import.package_name, DATA_DESCRIPTION_FILE_EXTENSION_NAME);
		file_name[sizeof(file_name) - 1] = 0;

		if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_OPEN_FILE, file_name);
		}
	}
}

