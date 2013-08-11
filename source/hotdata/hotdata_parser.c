#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error_code.h"
#include "data_description_l.h"
#include "hotscript/hp_script_lex.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "globals.h"

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer)
{
	hpint32 ret;
	AlphaMap *alpha_map = NULL;
	hpuint32 i;

	

	self->writer = writer;	
	self->scanner_stack.result_num = 0;
	

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->hotdata_symbols = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	if(file_name == NULL)
	{
		self->scanner_stack.result[0] = E_HP_ERROR;
		self->scanner_stack.result_num = 1;		
		goto done;
	}

	strncpy(self->file_name, file_name, MAX_FILE_NAME_LENGTH);
	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
	{
		self->scanner_stack.result[0] = E_HP_ERROR;
		self->scanner_stack.result_num = 1;		
		goto done;
	}

	ret = yydataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
done:
	for(i = 0;i < self->scanner_stack.result_num; ++i)
	{
		fprintf(stderr, self->scanner_stack.result_str[i]);
		fputc('\n', stderr);
	}

	if(self->scanner_stack.result_num == 0)
	{
		return E_HP_NOERROR;
	}
	else
	{
		return E_HP_ERROR;
	}
}


void yydataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...)
{
	DATA_PARSER *self = HP_CONTAINER_OF(jp, DATA_PARSER, scanner_stack);
	va_list ap;

	
	va_start(ap, s);
	scanner_stack_errorap(&self->scanner_stack, yylloc, E_HP_SYNTAX_ERROR, s, ap);
	va_end(ap);
}


hpint32 get_token_yylval(DATA_PARSER *dp, int *token, YYSTYPE * yylval, const YYLTYPE *yylloc)
{
	SCANNER *self = scanner_stack_get_scanner(&dp->scanner_stack);

	switch(*token)
	{
		/*
	case tok_import:
		{
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
		*/
	case tok_char:
		{
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_ERROR);
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
				scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_ERROR);
			}
			break;
		}
	case tok_string:
		{
			hpuint32 len = 0;
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_ERROR);
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
				scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_ERROR);
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
					scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_INTEGER_OVERFLOW);
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
					scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_INTEGER_OVERFLOW);
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
					scanner_stack_error(&dp->scanner_stack, yylloc, E_HP_INTEGER_OVERFLOW);
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



hpint32 data_parser_init(DATA_PARSER *self, const char* root_dir)
{
	scanner_stack_init(&self->scanner_stack, root_dir);
	self->scanner_stack.result_num = 0;
	self->definition_list_num = 0;
	self->domain[0] = 0;

	return E_HP_NOERROR;
}


//do
void dp_do_Definition(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_DEFINITION *pn_definition)
{
	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[HP_MAX_FILE_PATH_LENGTH];
		snprintf(file_name, HP_MAX_FILE_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_CAN_NOT_OPEN_FILE, file_name);
		}
	}
}

