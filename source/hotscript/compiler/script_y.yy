%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"


#include "script_parser.h"

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
#define GET_SCRIPT_PARSER SCRIPT_PARSER *xp = HP_CONTAINER_OF(arg, SCRIPT_PARSER, scanner);
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

%token tok_include 
%token tok_file_name
%token tok_literal
%token tok_integer
%token tok_identifier

%union
{
	char literal[MAX_TOKEN_LENGTH];
	char file_name[MAX_TOKEN_LENGTH];
	hpuint64 ui64;
	char identifier[MAX_TOKEN_LENGTH];
}


%type<literal>						tok_literal
%type<file_name>					tok_file_name
%type<ui64>							tok_integer
%type<identifier>					tok_identifier


%start Script

%%

Script :
	StatementList
	{
	}

StatementList :
	StatementList Statement
	{
	}
|	Statement
	{
	}
	
Statement :
	tok_include tok_file_name
	{
		printf("include %s", $2);
	}
|	'$' tok_identifier '[' tok_integer ']'
	{
		printf("$%s[%d]\n", $2, $4);
	}
|	'$' tok_identifier '[' '*' ']' '{' '}'
	{
		printf("$%s[*]\n", $2);
	}
|	tok_literal
	{
		printf("%s\n", $1);
	}

%%

