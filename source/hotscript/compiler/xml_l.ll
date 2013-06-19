%{
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "xml_y.h"

#define YY_USER_ACTION																	\
	yylloc->first_line = yylloc->last_line = yylineno;									\
	yylloc->first_column = yycolumn;													\
	yylloc->last_column = yycolumn + yyleng - 1;										\
	yycolumn += yyleng;																	
		
		
static void hotscript_reserved_keyword(char* keyword)
{
	//yyerror(&yylloc, hotscript_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}
void yyerror(const YYLTYPE *yylloc, yyscan_t *yyscan, char *s, ...);
%}


%option noyywrap yylineno reentrant nounistd bison-bridge bison-locations
%x INTAG
%option extra-type = "yyscan_t*"


identifier		([a-zA-Z_][a-zA-Z_0-9]*)
content			([a-zA-Z_0-9]*)
multicomm		("<!--"[^-]*("-"[^-]+)*"-->")
newline			("\r"|"\n"|"\r\n")
whitespace		([ \t\r\n]*)
%%

"<"									{BEGIN INTAG; }
<INTAG>{identifier} {
	char c;
	for(c = input(*yyextra); c != '>'; c = input(*yyextra))
	{
		if(c == EOF)
		{
			yyterminate();
		}
	}
	strncpy(yylval->begin_tag, yytext, MAX_TOKEN_LENGTH);
	BEGIN INITIAL; 	
	return tok_begin_tag;
}

<INTAG>"/"{identifier} {
	char c;
	for(c = input(*yyextra); c != '>'; c = input(*yyextra))
	{
		if(c == EOF)
		{
			yyterminate();
		}
	}
	strncpy(yylval->end_tag, yytext + 1, MAX_TOKEN_LENGTH);
	BEGIN INITIAL; 	
	return tok_end_tag;
}


{content}							{strncpy(yylval->content, yytext, MAX_TOKEN_LENGTH); return tok_content;}


#Ê×ÏÈÌø¹ý×¢ÊÍ
<*>{multicomm}			{/* do nothing */																}
<*>{whitespace}			{/* do nothing */																}
<*>{newline}			{yycolumn = 1;																	}
<*>.					{}

%%
