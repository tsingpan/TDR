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


#define HOTDATA_EXTENSION ".dd"

hpint32 get_token_yylval(DATA_PARSER *dp, int *token, YYSTYPE * yylval, const YYLTYPE *yylloc)
{
	SCANNER *self = scanner_stack_get_scanner(&dp->scanner_stack);
	SCANNER_STACK *ss = &dp->scanner_stack;


	switch(*token)
	{
	case tok_import:
		{
			char file_name[1024];
			size_t len = 0;
			const char *i;

			yylval->sn_tok_import.ptr = NULL;
			yylval->sn_tok_import.len = 0;

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
					if(yylval->sn_tok_import.ptr != NULL)
					{
						++(yylval->sn_tok_import.len);
					}
				}
				else
				{
					if(yylval->sn_tok_import.ptr == NULL)
					{
						yylval->sn_tok_import.ptr = self->yy_cursor;
					}

					++(yylval->sn_tok_import.len);

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
			yylval->sn_tok_unixcomment.ptr = yytext + 1;
			yylval->sn_tok_unixcomment.len = yyleng - 1;
			while(yylval->sn_tok_unixcomment.len > 0 )
			{
				if(yylval->sn_tok_unixcomment.ptr[yylval->sn_tok_unixcomment.len - 1] == '\r')
				{
					--yylval->sn_tok_unixcomment.len;
				}
				else if(yylval->sn_tok_unixcomment.ptr[yylval->sn_tok_unixcomment.len - 1] == '\n')
				{
					--yylval->sn_tok_unixcomment.len;
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
			yylval->sn_tok_identifier.ptr = yytext;
			yylval->sn_tok_identifier.len = yyleng;
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

void dp_on_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier)
{
	char id[1024];
	hpuint32 i;
	void *data;
	for(i = 0; i < sn_identifier->sn_tok_identifier.len; ++i)
	{
		id[i] = sn_identifier->sn_tok_identifier.ptr[i];
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


	if(sn_value->sn_value.is_identifier)
	{		
		val = sn_value->sn_value.sn;
	}
	else
	{
		val = sn_value;
	}

	if(val == NULL)
	{
		goto done;
	}

	for(i = 0; i < sn_identifier->sn_tok_identifier.len; ++i)
	{
		id[i] = sn_identifier->sn_tok_identifier.ptr[i];
	}
	id[i] = 0;

	switch(sn_type->sn_type.type)
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
	if(val->sn_value.var.type == E_HP_UINT64)
	{
		if(val->sn_value.var.val.ui64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else if(val->sn_value.var.type == E_HP_INT64)
	{
		if(val->sn_value.var.val.i64 >> size)
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


void dp_on_value_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, SyntacticNode* current, const SyntacticNode* sn_identifier)
{
	void *data;
	hpuint32 i;
	char id[1024];

	for(i = 0; i < sn_identifier->sn_tok_identifier.len; ++i)
	{
		id[i] = sn_identifier->sn_tok_identifier.ptr[i];
	}
	id[i] = 0;

	if(!trie_retrieve(self->constant, id, &data))
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
		goto error_ret;
	}

	current->sn_value.is_identifier = hptrue;
	current->sn_value.sn = (const SyntacticNode*)data;
done:
	return;
error_ret:
	current->sn_value.is_identifier = hptrue;
	current->sn_value.sn = NULL;
	return;
}

void dp_on_import(DATA_PARSER *self, const YYLTYPE *yylloc, SyntacticNode* current, const SyntacticNode* sn_import)
{
	current->sn_import = sn_import->sn_tok_import;
}

void dp_on_document_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	write_struct_begin(self->writer, "Document");
	write_field_begin(self->writer, "DefinitionList", strlen("DefinitionList"));
	write_vector_begin(self->writer);
}

void dp_on_document_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	write_vector_end(self->writer);
	write_field_end(self->writer, "DefinitionList", strlen("DefinitionList"));
	write_struct_end(self->writer, "Document");
}

void dp_on_definition_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	write_vector_item_begin(self->writer, writer_get_index(self->writer));
	write_struct_begin(self->writer, NULL);
}

void dp_on_definition_semicolon(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	write_semicolon(self->writer);
}

void dp_on_definition_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	write_vector_item_end(self->writer, writer_get_index(self->writer));
	write_struct_end(self->writer, NULL);
}

//handler
