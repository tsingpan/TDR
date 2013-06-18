%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"


#include "xml_parser.h"

#define YYERROR_VERBOSE

void yyerror(const YYLTYPE *yylloc, yyscan_t *yyscan, char *s, ...) 
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
ERROR_RET:
	return;
}
//这里的代码生成在自身的文件中
#define GET_XML_PARSER XML_PARSER *xp = HP_CONTAINER_OF(arg, XML_PARSER, scanner);
%}
%locations

%code requires
{
#include "globals.h"

#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM *arg

}//code requires end

%define api.pure
%parse-param { yyscan_t *arg }
%pure_parser

%token tok_begin_tag 
%token tok_end_tag
%token tok_content


%union
{
	char content[MAX_TOKEN_LENGTH];
}

%type <content>							tok_content


%start XML

%%

XML : Element


Content :
	Element
	{
	}
|	Content Element
	{
	}


Element :
	tok_begin_tag tok_content tok_end_tag
	{
		printf("%s\n", $2);
	}
|
	tok_begin_tag Content tok_end_tag
	{
	}
%%

