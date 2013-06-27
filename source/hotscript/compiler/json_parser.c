#include "json_parser.h"
#include "json_y.h"
#include "hotpot/hp_error.h"
#include "json_l.h"

#include "hotscript/hotlex.h"
hpint32 json_parser(JSON_PARSER *self, const char* file_name, HPAbstractWriter *writer, HPAbstractReader *reader)
{
	hpint32 ret;
	hpint64 data;
	FILE *fin;
	char c;
	YYSTYPE yystype;
	YYLTYPE yylloc;

	self->result = HP_INVALID_ERROR_CODE;

	self->writer = writer;
	self->reader = reader;
	scanner_stack_init(&self->scanner_stack);
	scanner_stack_push_file(&self->scanner_stack, file_name, yycINITIAL);


	
	/*
	for(;;)
	{
		int ret = json_lex_scan(self, &yylloc, &yystype);
		
		
		if((ret == tok_string) || (ret == tok_identifier))
		{
			hpuint32 i;
			printf("%d: ", ret);
			for(i = 0;i < yystype.val.str.len; ++i)
			{
				putc(yystype.val.str.ptr[i], stdout);
			}			
			printf("\n");
		}
		else
		{
			char c = (char)ret;
			printf("%d: %c\n", ret, c);
		}
		
		if(ret <= 0)
		{
			exit(1);
		}
	}
	*/
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

extern hpint32 json_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval);
int yyjsonlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss)
{
	JSON_PARSER *jp = HP_CONTAINER_OF(jp, JSON_PARSER, scanner_stack);
	int ret = 0;

	for(;;)
	{
		SCANNER *scanner = scanner_stack_get_scanner(ss);

		ret = json_lex_scan(scanner, yylloc_param, yylval_param);
		if(ret == tok_script_begin)
		{

		}
		yylloc_param->last_line = scanner->yylineno;
		yylloc_param->last_column = scanner->yycolumn;
		break;
	}

	return ret;
}
