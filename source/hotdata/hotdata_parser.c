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
	self->symbols = trie_new(alpha_map);
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

	for(i = 0;i < self->result_num; ++i)
	{
		fprintf(stderr, self->result_str[i]);
		fputc('\n', stderr);
	}
done:
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

	return E_HP_NOERROR;
}



//handler
void dp_on_document_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}

	write_struct_begin(self->writer, "ST_DOCUMENT");

	write_field_begin(self->writer, "file_name");
	write_string(self->writer, self->file_name);
	write_field_end(self->writer, "file_name");

	write_field_begin(self->writer, "definition_list");
	write_vector_begin(self->writer);
}

void dp_on_document_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_end(self->writer);
	write_field_end(self->writer, "definition_list");

	write_field_begin(self->writer, "definition_list_num");
	write_uint32(self->writer, self->definition_list_num);
	write_field_end(self->writer, "definition_list_num");
	write_struct_end(self->writer, "ST_DOCUMENT");
}

void dp_on_definition_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_item_begin(self->writer, writer_get_index(self->writer));
	write_struct_begin(self->writer, NULL);
}

void dp_on_definition_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_semicolon(self->writer);
}

void dp_on_definition_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_item_end(self->writer, writer_get_index(self->writer));
	write_struct_end(self->writer, NULL);
	++(self->definition_list_num);
}


void dp_on_const_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_semicolon(self->writer);
}

void dp_on_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_semicolon(self->writer);
}

void dp_on_field_begin(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, name);
}

void dp_on_field_end(DATA_PARSER *self, const YYLTYPE *yylloc, const char *name)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_end(self->writer, name);
}

void dp_on_vector_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_begin(self->writer);
}

void dp_on_vector_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_end(self->writer);
}

void dp_on_vector_item_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_item_begin(self->writer, writer_get_index(self->writer));
}

void dp_on_vector_item_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_item_end(self->writer, writer_get_index(self->writer));
}

void dp_on_null(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_null(self->writer);
}

void dp_on_bool(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbool b)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_bool(self->writer, b);
}

void dp_on_bytes(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes bytes)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_bytes(self->writer, bytes);
}

void dp_on_string(DATA_PARSER *self, const YYLTYPE *yylloc, const hpchar *str)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_string(self->writer, str);
}

void dp_on_const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "name");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "name");
}

void dp_on_typedef_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "typedef");
	write_struct_begin(self->writer, NULL);
}

void dp_on_typedef_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_end(self->writer, NULL);
	write_field_end(self->writer, "typedef");
}

void dp_on_struct_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_begin(self->writer, NULL);
}

void dp_on_struct_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_end(self->writer, NULL);
}

void dp_on_union_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "union");
	write_struct_begin(self->writer, NULL);
}

void dp_on_union_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_end(self->writer, NULL);
	write_field_end(self->writer, "union");
}


void dp_on_enum_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "enum");
	write_struct_begin(self->writer, NULL);
}

void dp_on_enum_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_end(self->writer, NULL);
	write_field_end(self->writer, "enum");
}


void dp_on_tok_unixcomment_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "comment");
	write_struct_begin(self->writer, NULL);
}

void dp_on_tok_unixcomment_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_end(self->writer, NULL);
	write_field_end(self->writer, "comment");
}

void dp_on_tok_import(DATA_PARSER *self, const YYLTYPE *yylloc, const hpstring sn_tok_import)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_struct_begin(self->writer, NULL);
	write_field_begin(self->writer, "file");
	write_string(self->writer, sn_tok_import);
	write_field_end(self->writer, "file");
	write_struct_end(self->writer, NULL);
}

void dp_on_value_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
}

void dp_on_value_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}

	write_field_end(self->writer, "value");
}

void dp_on_value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
	write_int64(self->writer, i64);
	write_field_end(self->writer, "value");

	write_semicolon(self->writer);
	write_field_begin(self->writer, "base");
	write_int64(self->writer, 10);
	write_field_end(self->writer, "base");
}

void dp_on_value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpint64 i64)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
	write_int64(self->writer, i64);
	write_field_end(self->writer, "value");

	write_semicolon(self->writer);
	write_field_begin(self->writer, "base");
	write_int64(self->writer, 16);
	write_field_end(self->writer, "base");
}

void dp_on_value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpuint64 ui64)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
	write_uint64(self->writer, ui64);
	write_field_end(self->writer, "value");

	write_semicolon(self->writer);
	write_field_begin(self->writer, "base");
	write_int64(self->writer, 10);
	write_field_end(self->writer, "base");
}

void dp_on_value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, const hpuint64 ui64)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
	write_uint64(self->writer, ui64);
	write_field_end(self->writer, "value");

	write_semicolon(self->writer);
	write_field_begin(self->writer, "base");
	write_int64(self->writer, 10);
	write_field_end(self->writer, "base");
}

void dp_on_value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "value");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "value");
}

void dp_on_typedef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_TYPE* type, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "new_type");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "new_type");
}

void dp_on_TypeAnnotations_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "TypeAnnotations");
	write_vector_begin(self->writer);
}

void dp_on_TypeAnnotations_switch(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes *sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_item_begin(self->writer, writer_get_index(self->writer));
	write_struct_begin(self->writer, NULL);
	write_field_begin(self->writer, "switch");
	if(sn_tok_identifier == NULL)
	{
		write_string(self->writer, "s");
	}
	else
	{
		write_bytes(self->writer, *sn_tok_identifier);
	}
	write_field_end(self->writer, "switch");
	write_vector_item_end(self->writer, writer_get_index(self->writer));
	write_struct_end(self->writer, NULL);
}

void dp_on_TypeAnnotations_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_vector_end(self->writer);
	write_field_end(self->writer, "TypeAnnotations");
}

void dp_on_enum_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "name");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "name");
	write_semicolon(self->writer);
}

void dp_on_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	dp_on_field_begin(self, yylloc, "name");
	write_bytes(self->writer, sn_tok_identifier);
	dp_on_field_end(self, yylloc, "name");
}

void dp_on_union_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "name");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "name");
}

void dp_on_struct_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "name");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "name");
}

void dp_on_field_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const hpbytes sn_tok_identifier)
{
	if(self->scanner_stack.stack_num > 1)
	{
		return;
	}
	write_field_begin(self->writer, "name");
	write_bytes(self->writer, sn_tok_identifier);
	write_field_end(self->writer, "name");
}


//do
void dp_do_import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_IMPORT* current, const hpstring sn_tok_import)
{
	snprintf(current->package_name, sizeof(current->package_name), sn_tok_import);
}

void dp_dodo_import(DATA_PARSER *self, const YYLTYPE *yylloc, const hpstring sn_tok_import)
{
	char file_name[1024];
	snprintf(file_name, sizeof(file_name), "%s%s", sn_tok_import, DATA_DESCRIPTION_FILE_EXTENSION_NAME);
	file_name[sizeof(file_name) - 1] = 0;

	if(scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_OPEN_FILE, file_name);
	}
}

void dp_do_value_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbytes sn_identifier)
{
	hpuint32 data;
	hpuint32 i;
	char id[1024];

	for(i = 0; i < sn_identifier.len; ++i)
	{
		id[i] = sn_identifier.ptr[i];
	}
	id[i] = 0;

	if(!trie_retrieve(self->symbols, id, &data))
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
		goto error_ret;
	}
/*
	if((self->symbol_list[data].type != E_SST_Const) && (self->symbol_list[data].type != E_SST_EnumDef))
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
		goto error_ret;
	}
*/
	current->type = E_SNVT_IDENTIFIER;
done:
	return;
error_ret:
	current->type = E_SNVT_IDENTIFIER;
	return;
}

void dp_do_value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64)
{
	current->type = E_SNVT_INT64;
}

void dp_do_value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64)
{
	current->type = E_SNVT_HEX_INT64;
}

void dp_do_value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64)
{
	current->type = E_SNVT_UINT64;
}

void dp_do_value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64)
{
	current->type = E_SNVT_HEX_UINT64;
}

void dp_do_simple_type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_SIMPLE_TYPE type)
{
	current->type = E_SNT_SIMPLE;
	current->st = type;
}

void dp_do_container_type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_CONTAINER_TYPE type)
{
	current->type = E_SNT_CONTAINER;
	current->ct = type;
}

void dp_do_type_object(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const hpbytes sn_tok_identifier)
{
	hpuint32 i;
	char id[1024];
	PN_TYPE *type;

	current->type = E_SNT_OBJECT;

	for(i = 0; i < sn_tok_identifier.len; ++i)
	{
		id[i] = sn_tok_identifier.ptr[i];
	}
	id[i] = 0;
	strncpy(current->ot, id, MAX_STRING_LENGTH);
/*
	if(!trie_retrieve(self->typedef_identifier, id, type))
	{
		dp_error(self, yylloc, (hpint32)E_HP_ERROR, id);
		goto done;
	}
	*/
done:
	return;
}

/*
//check
void dp_check_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc,const SyntacticNode* sn_type, const hpbytes sn_tok_identifier)
{
	char id[1024];
	hpuint32 i;
	hpuint32 data;

	for(i = 0; i < sn_tok_identifier.len; ++i)
	{
		id[i] = sn_tok_identifier.ptr[i];
	}
	id[i] = 0;

	if(trie_retrieve(self->symbols, id, &data))
	{
		dp_error(self, yylloc, E_SID_SYMBOL_REDEFINITION, id);
	}
}

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const SN_TYPE* sn_type, const hpbytes* sn_identifier, const SN_VALUE* sn_value)
{
	char id[1024];
	hpuint32 i;
	const SN_VALUE* val = NULL;
	size_t size;

	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		if(self->symbol_list[sn_value->index].type == E_SST_Const)
		{
			val = &self->symbol_list[sn_value->index].sn.sn_value;
		}
	}
	else
	{
		val = sn_value;
	}

	if(val == NULL)
	{
		goto done;
	}

	for(i = 0; i < sn_identifier->len; ++i)
	{
		id[i] = sn_identifier->ptr[i];
	}
	id[i] = 0;

	switch(sn_type->type)
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
	if((val->type == E_SNVT_UINT64) || (val->type == E_SNVT_HEX_UINT64))
	{
		if(val->ui64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else if((val->type == E_SNVT_INT64) || (val->type == E_SNVT_HEX_INT64))
	{
		if(val->i64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else if(val->type == E_SNVT_IDENTIFIER)
	{

	}
	else
	{
		dp_error(self, yylloc, (hpint32)E_HP_UNKNOW_CONSTANT_VALUE);
		goto done;
	}

	self->symbol_list[self->symbol_list_num].type = E_SST_Const;
	self->symbol_list[self->symbol_list_num].sn.sn_value = *sn_value;


	if(!trie_store_if_absent(self->symbols, id, self->symbol_list_num))
	{
		dp_error(self, yylloc, (hpint32)E_HP_ERROR, id);
		goto done;
	}

	++(self->symbol_list_num);

done:
	return;
}
*/