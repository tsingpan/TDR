%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"




#define YYERROR_VERBOSE

%}
%locations

%code requires
{
#include "hotscript/script_parser.h"
#include "hotscript/hot_vm.h"

#define YYSTYPE SP_NODE
#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM ss
}

%define api.pure
%parse-param { SCANNER_STACK *ss }
%pure_parser

%token tok_import
%token tok_literal
%token tok_integer
%token tok_identifier
%token tok_text

%start Script

%%

Script :
	StatementList

StatementList :
	StatementList Statement
	{
	}
|
	{
	}
	
Statement:
	tok_text 
	{
		hotscript_do_text(ss, &$1);
	}
|
	tok_literal 
	{
		hotscript_do_literal(ss, &$1);
	}
|	Prefix tok_identifier ArrayIndex
	{
		hotscript_do_field_begin(ss, &$$, &$1, &$2);
		hotscript_do_vector_begin(ss, &$$, &$3);
		hotscript_do_vector_seek(ss, &$$, &$3);

		hotscript_do_echo_field(ss, &$$);

		hotscript_do_jmp(ss, &$$);//如果echo失败了， 那么跳到jmp的下一行		
		hotscript_do_vector_end(ss, &$$, &$3);//如果vector begin失败了， 那么跳到下一行
		hotscript_do_field_end(ss, &$$, &$2);//如果field begin失败了， 那么跳到下一行
	}
|	Prefix tok_identifier ArrayIndex
	'{'
	{
		hotscript_do_field_begin(ss, &$$, &$1, &$2);
		hotscript_do_vector_begin(ss, &$$, &$3);
		hotscript_do_vector_seek(ss, &$$, &$3);
	}
	StatementList
	'}'
	{
		hotscript_do_jmp(ss, &$$);
		hotscript_do_vector_end(ss, &$$, &$3);
		hotscript_do_field_end(ss, &$$, &$2);
	}


ArrayIndex :
	'[' tok_integer ']'
	{
		$$.it = E_INDEX_GIVEN;
		$$.var.type = E_HP_UINT32;
		$$.var.val.ui32 = (hpuint32)$2.var.val.i64;	//指定下标， 超过int32肿么办？
	}
|	'[' '*' ']'
	{
		$$.it = E_INDEX_ALL;						//按次序展开元素， 直到不存在
	}
|
	{
		$$.it = E_INDEX_NULL;						//默认为第一个元素
	}

Prefix:
	'@'
	{
		$$.var.type = E_HP_CHAR;
		$$.var.val.c = '@';//局部变量
	}
|	'#'
	{
		$$.var.type = E_HP_CHAR;
		$$.var.val.c = '#';//全局变量
	}
|	'$'
	{
		$$.var.type = E_HP_CHAR;
		$$.var.val.c = '$';//搜索所有堆栈
	}


%%
