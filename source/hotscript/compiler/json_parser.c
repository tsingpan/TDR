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

	self->result = HP_INVALID_ERROR_CODE;

	self->writer = writer;

	self->buff_size = 0;
	self->yy_state = yycINITIAL;
	fin = fopen(file_name, "rb");
	while((c = fgetc(fin)) != EOF)
	{
		self->buff[(self->buff_size)++] = c;
	}
	self->yy_cursor = self->buff + 0;
	self->yy_limit = self->buff + self->buff_size;
	/*
	for(;;)
	{
		int ret = json_lex_scan(self, &yystype);
		
		
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


extern int json_lex_scan(JSON_PARSER *jp, YYSTYPE * yylval);

void yyjsonerror(const YYLTYPE *yylloc, JSON_PARSER *jp, char *s, ...) 
{
	return;
}

int yyjsonlex(YYSTYPE * yylval_param,YYLTYPE * yylloc_param , JSON_PARSER *jp)
{
	return json_lex_scan(jp, yylval_param);
}
