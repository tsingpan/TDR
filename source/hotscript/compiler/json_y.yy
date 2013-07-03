%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "json_parser.h"
#include "hotscript/hotobject.h"

#include "hotscript/hotlex.h"

#define YYERROR_VERBOSE


%}
%locations

%code requires
{
#include "globals.h"
#include "hotpot/hp_value.h"
#include "json_parser.h"

#define YYMALLOC
#define YYFREE
#define YYSTYPE HPVar
#define YYLEX_PARAM ss
#define GET_SELF JSON_PARSER *jp = HP_CONTAINER_OF(ss, JSON_PARSER, scanner_stack);
}

%define api.pure
%parse-param { SCANNER_STACK *ss }
%pure_parser



%token tok_script_begin
%token tok_string
%token tok_double
%token tok_integer
%token tok_true
%token tok_false
%token tok_null


%start Object

%%

Object:
	'{' '}'
	{
	}
|	'{' Members '}'
	{
	};


Members:
	Pair
	{
	}
|	Pair ',' Members
	{
	};
	
	
Pair:
	tok_string ':' Value
	{
	}
	
	
Array:
	'[' Elements ']'
	{
	}

Elements:
	Value
	{
	
	}
|
	Value ',' Elements
	{
			
	}
	
	
Value:
	tok_string
	{
	}
|	tok_integer
	{
	}
|	tok_double
	{
	}
|	Object
	{
	}
|	Array
	{
	}
|	tok_true
|	tok_false
|	tok_null
	

	
%%

