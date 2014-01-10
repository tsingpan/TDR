#include "parse/parser.h"
#include "error/error_code_types.h"

#include "tdata_y.h"
#include "tdata_l.h"
#include "parse/scanner.h"

#include "symbols.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

tint32 parser_add_generator(PARSER *self, GENERATOR *generator)
{
	if(self->generator_num >= TD_MAX_GENERATOR)
	{
		goto ERROR_RET;
	}
	self->generator_list[self->generator_num] = generator;
	++self->generator_num;

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

tint32 parser_parse(PARSER *self, const char* file_name)
{
	tint32 ret;	
	tuint32 i;

	symbols_init(&self->symbols);
	

	if(file_name == NULL)
	{
		scanner_stack_error_halt(&self->scanner_stack, NULL, E_TD_ERROR);
	}

	strncpy(self->file_name, file_name, MAX_FILE_NAME_LENGTH);
	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
	{
		scanner_stack_error_halt(&self->scanner_stack, NULL, E_TD_ERROR);
	}

	ret = tdataparse(&self->scanner_stack);
	scanner_stack_pop(&self->scanner_stack);
done:
	symbols_fini(&self->symbols);
	return E_TD_NOERROR;
}


void tdataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...)
{
	PARSER *self = TLIBC_CONTAINER_OF(jp, PARSER, scanner_stack);
	va_list ap;

	va_start(ap, s);
	scanner_stack_errorap(&self->scanner_stack, yylloc, E_TD_SYNTAX_ERROR, s, ap);
	va_end(ap);
}


tint32 get_token_yylval(PARSER *dp, int *token, YYSTYPE * yylval, const YYLTYPE *yylloc)
{
	SCANNER *self = scanner_stack_get_scanner(&dp->scanner_stack);

	switch(*token)
	{
	case tok_char:
		{
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_ERROR);
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
				scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_ERROR);
			}
			break;
		}
	case tok_string:
		{
			tuint32 len = 0;
			if(YYCURSOR >= YYLIMIT)
			{
				scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_ERROR);
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
				scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_ERROR);
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
					scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_INTEGER_OVERFLOW);
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
					scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_INTEGER_OVERFLOW);
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
					scanner_stack_error_halt(&dp->scanner_stack, yylloc, E_TD_INTEGER_OVERFLOW);
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
	}

	return E_TD_NOERROR;
}

extern tint32 tdata_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval);
int tdatalex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	PARSER *jp = TLIBC_CONTAINER_OF(ss, PARSER, scanner_stack);
	int ret = 0;
	
	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(ss);
		yylloc_param->file_name = scanner->file_name;
		ret = tdata_lex_scan(scanner, yylloc_param, yylval_param);
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
			if(get_token_yylval(jp, &ret, yylval_param, yylloc_param) != E_TD_NOERROR)
			{
				ret = -1;
				break;
			}			
			break;
		}		
	}

	return ret;
}



void parser_init(PARSER *self)
{
	scanner_stack_init(&self->scanner_stack);
	self->generator_num = 0;
	symbols_init(&self->symbols);
}


//do
void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const ST_DEFINITION *pn_definition)
{
	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TLIBC_MAX_FILE_PATH_LENGTH];
		snprintf(file_name, TLIBC_MAX_FILE_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_TD_NOERROR)
		{
			scanner_stack_error_halt(&self->scanner_stack, yylloc, E_TD_CAN_NOT_OPEN_FILE, file_name);
		}
	}

	if(scanner_stack_get_num(&self->scanner_stack) == 1)
	{
		tuint32 i;
		for(i = 0; i < self->generator_num; ++i)
		{
			generator_on_definition(self->generator_list[i], &self->pn_definition);
		}
	}
}

void parser_on_document_begin(PARSER *self)
{
	tuint32 i;
	if(scanner_stack_get_num(&self->scanner_stack) != 1)
	{
		goto SKIP;
	}

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_begin(self->generator_list[i], self->file_name);
	}

SKIP:
	return;
}

void parser_on_document_end(PARSER *self)
{
	tuint32 i;
	if(scanner_stack_get_num(&self->scanner_stack) != 1)
	{
		goto SKIP;
	}

	for(i = 0; i < self->generator_num; ++i)
	{
		generator_on_document_end(self->generator_list[i], self->file_name);
	}

SKIP:
	return;
}
