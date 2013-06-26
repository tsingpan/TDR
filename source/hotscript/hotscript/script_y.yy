%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"


#include "hotscript/script_parser.h"

#define YYERROR_VERBOSE

%}
%locations

%code requires
{
#include "hotscript/hot_vm.h"

#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM sp
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
%parse-param { SCRIPT_PARSER *sp }
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
	tok_literal 
	{
		hotscript_do_literal(sp, &$1);
	}
|	Prefix tok_identifier ArrayIndex
	{
		hotscript_do_push(sp, &$1, &$2);
		hotscript_do_push_index(sp, &$3);
		
		hotscript_do_echo_trie(sp);
		
		hotscript_do_pop_index(sp, &$3);
		hotscript_do_pop(sp, &$2);
	}
|	Prefix tok_identifier ArrayIndex
	'{'
	{
		hotscript_do_push(sp, &$1, &$2);
		hotscript_do_push_index(sp, &$3);
	}
	StatementList
	'}'
	{
		hotscript_do_pop_index(sp, &$3);
		hotscript_do_pop(sp, &$2);
	}
|	tok_text 
	{
		hotscript_do_text(sp, &$1);
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
