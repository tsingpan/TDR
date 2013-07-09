#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "json_y.h"
#include "json_parser.h"
#include "json_l.h"
#include "hotscript/hotlex.h"

static void hotscript_reserved_keyword(char* keyword)
{
	//yyerror(&yylloc, hotscript_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}


#include "hotscript/hotlex.h"

hpint32 json_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return 0;
	}
	yylloc->first_line = self->yylineno;
	yylloc->first_column = self->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;


newline			("\r"|"\n"|"\r\n")
whitespace		[ \t\r\n]*
symbols			[,:\[\]\{\}]
string_begin	['\"']
intconstant		([+-]?[0-9]+)
LNUM			[0-9]+
DNUM			([0-9]*"."[0-9]+)|([0-9]+"."[0-9]*)
EXPONENT_DNUM	(({LNUM}|{DNUM})[eE][+-]?{LNUM})


<!*> := yyleng = YYCURSOR - yytext; scanner_process(self);

<*> "<%" { return tok_script_begin; }

<INITIAL>{symbols} {
	return *yytext;
}

<INITIAL>{DNUM}|{EXPONENT_DNUM}
{
	return tok_double;
}

<INITIAL>{intconstant}
{
	return tok_integer;
}

<INITIAL>"true"
{
	return tok_true;
}

<INITIAL>"false"
{
	return tok_false;
}

<INITIAL>"null"
{
	return tok_null;
}
	
<INITIAL>{string_begin} {
	return tok_string;
}

<INITIAL>. | '\n' {
	goto restart;
}

*/
return 0;
}
