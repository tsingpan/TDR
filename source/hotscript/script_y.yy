%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error_code.h"




#define YYERROR_VERBOSE
#define GET_SELF HP_CONTAINER_OF(ss, SCRIPT_PARSER, scanner_stack)

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
%token tok_auto_integer
%token tok_identifier
%token tok_call_identifier
%token tok_text

%start Script
%%

Script :
	StatementList

StatementList :
	StatementList Statement
|	;
	
Statement:
	tok_text {hotscript_do_text(GET_SELF, &yylloc, &$1);}
|	tok_literal {hotscript_do_literal(GET_SELF, &yylloc, &$1);}
|	Identifier {hotscript_do_field_begin(GET_SELF, &yylloc, &$1); hotscript_do_echo_field(GET_SELF, &yylloc, &$1); hotscript_do_field_end(GET_SELF, &yylloc, &$1);}
|	Identifier {hotscript_do_field_begin(GET_SELF, &yylloc, &$1);}
	'[' {hotscript_do_vector_begin(GET_SELF, &yylloc, &$1);}
	StatementList
	']' {hotscript_do_vector_end(GET_SELF, &yylloc, &$1);}
	{hotscript_do_field_end(GET_SELF, &yylloc, &$1);}
|	Identifier {hotscript_do_field_begin(GET_SELF, &yylloc, &$1);}
	'{' StatementList '}'
	{hotscript_do_field_end(GET_SELF, &yylloc, &$1);};

Identifier :
	tok_identifier {$$ = $1;$$.token = tok_identifier; }
|	tok_call_identifier {$$ = $1;$$.token = tok_call_identifier; }
|	tok_integer{$$ = $1;$$.token = tok_integer; }
|	tok_auto_integer{$$ = $1;$$.token = tok_auto_integer; }


%%
