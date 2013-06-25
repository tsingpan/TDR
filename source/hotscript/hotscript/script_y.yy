%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"


#include "hotscript/script_parser.h"

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
}
//这里的代码生成在自身的文件中
#define GET_SCRIPT_PARSER SCRIPT_PARSER *xp = HP_CONTAINER_OF(arg, SCRIPT_PARSER, scanner);



%}
%locations

%code requires
{
#include "hotscript/hot_vm.h"

#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM *arg
#define MAX_TOKEN_LENGTH 1024
#ifndef _DEF_SNODE
#define _DEF_SNODE
typedef struct _ST_STRING
{
	char str[MAX_TOKEN_LENGTH];
	hpuint32 str_len;
}ST_STRING;
typedef struct _ST_STRING ST_STRING;

typedef struct _SNODE
{
	ST_STRING literal;
	char file_name[MAX_TOKEN_LENGTH];
	char identifier[MAX_TOKEN_LENGTH];
	ST_STRING text;
	char prefix;
	hpint32 i32;
	HotOp *op;
}SNODE;
#endif//_DEF_SNODE

#define YYSTYPE SNODE

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
		
		hotscript_do_literal(xp, &$1);
	}
|	Prefix tok_identifier ArrayIndex
	{
		GET_SCRIPT_PARSER;
		hotscript_do_push(xp, &$1, &$2);
		hotscript_do_push_index(xp, &$3);
		
		hotscript_do_echo_trie(xp);
		
		hotscript_do_pop_index(xp, &$3);
		hotscript_do_pop(xp, &$2);
	}
|	Prefix tok_identifier ArrayIndex
	'{'
	{
		GET_SCRIPT_PARSER;
		hotscript_do_push(xp, &$1, &$2);
		hotscript_do_push_index(xp, &$3);
	}
	StatementList
	'}'
	{
		GET_SCRIPT_PARSER;
		hotscript_do_pop_index(xp, &$3);
		hotscript_do_pop(xp, &$2);
	}
|	tok_text 
	{
		GET_SCRIPT_PARSER;
		
		hotscript_do_text(xp, &$1);
	}


ArrayIndex :
	'[' tok_integer ']'
	{
		$$.i32 = $2.i32;	//指定下表
	}
|	'[' '*' ']'
	{
		$$.i32 = -1;		//按次序展开元素， 直到不存在
	}
|
	{
		$$.i32 = -2;		//默认为第一个元素
	}

Prefix:
	'@'
	{
		$$.prefix = '@';//局部变量
	}
|	'#'
	{
		$$.prefix = '#';//全局变量
	}
|	'$'
	{
		$$.prefix = '$';//搜索所有堆栈
	}


%%
