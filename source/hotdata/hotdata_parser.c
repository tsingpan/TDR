#include "hotdata_parser.h"
#include "data_description_y.h"
#include "hotpot/hp_error.h"
#include "data_description_l.h"
#include "hotscript/hotlex.h"


hpint32 data_parser(DATA_PARSER *self, const char* file_name)
{
	hpint32 ret;

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
		if(ret == 0)
		{
			if(scanner_stack_get_num(&jp->scanner_stack) <= 1)
			{
				break;
			}
			scanner_stack_pop(&jp->scanner_stack);
		}
		else if(ret == tok_import)
		{
			char file_name[1024];
			size_t len = 0;
			while(scanner->yy_cursor < scanner->yy_limit)
			{
				if(*scanner->yy_cursor == ';')
				{
					break;
				}
				else if((*scanner->yy_cursor == '\n') || (*scanner->yy_cursor == '\t') || (*scanner->yy_cursor == ' '))
				{
					++(scanner->yy_cursor);
				}
				else
				{
					file_name[len++] = *scanner->yy_cursor;
					++(scanner->yy_cursor);
				}
			}

			file_name[len] = 0;
			if(scanner_stack_push_file(&jp->scanner_stack, file_name, yycINITIAL) != E_HP_NOERROR)
			{
				jp->result = E_HP_ERROR;
				return 0;
			}
		}
		else
		{
			break;
		}		
	}

	return ret;
}
