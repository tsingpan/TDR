#include "json_parser.h"
#include "json_y.h"
#include "hotpot/hp_error.h"

hpint32 json_parser(JSON_PARSER *self, const char* file_name, HPAbstractWriter *writer)
{
	hpint32 ret;
	hpint64 data;


	self->result = HP_INVALID_ERROR_CODE;

	self->writer = writer;

	ret = yyjsonparse(self);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;
}


extern int lex_scan(JSON_PARSER *jp);

void yyjsonerror(const YYLTYPE *yylloc, JSON_PARSER *jp, char *s, ...) 
{
	return;
}

int yyjsonlex(YYSTYPE * yylval_param,YYLTYPE * yylloc_param , JSON_PARSER *jp)
{
	return lex_scan(jp);
}
