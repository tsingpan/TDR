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

#ifndef _DEF_ST_STRING
#define _DEF_ST_STRING
typedef struct _ST_STRING
{
	char str[MAX_TOKEN_LENGTH];
	hpuint32 str_len;
}ST_STRING;
typedef struct _ST_STRING ST_STRING;
#endif//_DEF_ST_STRING
}//code requires end

%define api.pure
%parse-param { yyscan_t *arg }
%pure_parser

%token tok_include 
%token tok_file_name
%token tok_literal
%token tok_integer
%token tok_identifier
%token tok_text

%union
{
	ST_STRING literal;
	char file_name[MAX_TOKEN_LENGTH];
	char identifier[MAX_TOKEN_LENGTH];
	ST_STRING text;
	char prefix;
	hpuint32 ui32;
}


%type<literal>						tok_literal
%type<file_name>					tok_file_name
%type<ui32>							tok_integer
%type<identifier>					tok_identifier
%type<text>							tok_text
%type<prefix>						Prefix
%type<ui32>							ArrayIndex


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
|
	{
	}
	
Statement:
|	tok_literal
	{
		GET_SCRIPT_PARSER;
		
		hotscript_do_text(xp, &$1);
		printf("echo: %s", $1.str);
	}
|	Prefix tok_identifier {GET_SCRIPT_PARSER; hotscript_do_push(xp, $1, $2); }
	ArrayIndex {GET_SCRIPT_PARSER; hotscript_do_push_index(xp, $4); }
	'{' {printf("push %s\n", $2)}	
	StatementList '}' {printf("pop %s\n", $2)}
	{
		GET_SCRIPT_PARSER;
		hotscript_do_pop_index(xp, $4);
		hotscript_do_pop(xp);
		//printf("$%s[%d]\n", $2, $4);
	}
|	Prefix tok_identifier {GET_SCRIPT_PARSER; hotscript_do_push(xp, $1, $2); }
	ArrayIndex {GET_SCRIPT_PARSER; hotscript_do_push_index(xp, $4); }
	{
		GET_SCRIPT_PARSER;
		hotscript_do_echo_trie(xp);
		
		hotscript_do_pop_index(xp, $4);
		hotscript_do_pop(xp);
		//echo
		printf("$%s[?]\n", $2);
	}
|	tok_text
	{
		GET_SCRIPT_PARSER;
		
		hotscript_do_text(xp, &$1);
		printf("echo: %s", $1.str);
	}
	
ArrayIndex :
	'[' tok_integer ']'
	{
		$$ = $2;
	}
|	'[' '*' ']'
	{
		$$ = -1;
	}
|
	{
		$$ = 0;
	}
	
Prefix:
	'$'							
	{
		$$ = '$';
	}
|	'#'
	{
		$$ = '#';
	}




%%

