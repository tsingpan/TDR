#include "json_parser.h"
#include "json_y.h"
#include "hotpot/hp_error.h"

hpint32 json_parser(JSON_PARSER *self, const char* file_name, HPAbstractWriter *writer)
{
	hpint32 ret;
	hpint64 data;
	FILE *fin;
	char c;
	YYSTYPE yystype;
	YYLTYPE yylloc;

	self->result = HP_INVALID_ERROR_CODE;

	self->writer = writer;

	self->buff_size = 0;
	self->yy_state = yycINITIAL;
	fin = fopen(file_name, "rb");
	while((c = fgetc(fin)) != EOF)
	{
		self->buff[(self->buff_size)++] = c;
	}
	self->yy_last = self->buff;
	self->yy_cursor = self->buff + 0;
	self->yy_limit = self->buff + self->buff_size;
	self->yylineno = 1;
	self->yycolumn = 1;
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
	ret = yyjsonparse(self);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;
}


extern hpint32 json_lex_scan(JSON_PARSER *jp, YYLTYPE *yylloc, YYSTYPE * yylval);

void yyjsonerror(const YYLTYPE *yylloc, JSON_PARSER *jp, char *s, ...) 
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

int yyjsonlex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , JSON_PARSER *jp)
{
	int ret = json_lex_scan(jp, yylloc_param, yylval_param);
	yylloc_param->last_line = jp->yylineno;
	yylloc_param->last_column = jp->yycolumn;

	return ret;
}
