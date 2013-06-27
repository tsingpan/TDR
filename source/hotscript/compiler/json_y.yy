%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "json_parser.h"
#include "hotobject.h"

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
#define MAX_STRING_LENGTH 124
#define MAX_NAME_LENGTH 64

#define YYSTYPE HPVar


#define YYLEX_PARAM ss
#define GET_SELF JSON_PARSER *jp = HP_CONTAINER_OF(ss, JSON_PARSER, scanner_stack);
}//code requires end

%define api.pure
%parse-param { SCANNER_STACK *ss }
%pure_parser



%token tok_script_begin
%token tok_string
%token tok_identifier


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
	tok_identifier ':' tok_string
	{
		GET_SELF;

		hp_writer_begin(jp->writer, &$1);
		hp_writer_write(jp->writer, &$3);
		hp_writer_end(jp->writer);
	}
|	
	tok_identifier ':' 
	{ GET_SELF; hp_writer_begin(jp->writer, &$1); }
	Object
	{ GET_SELF; hp_writer_end(jp->writer); }
|
	tok_identifier ':' 
	{ GET_SELF; hp_writer_begin(jp->writer, &$1); } 
	Array
	{ GET_SELF; hp_writer_end(jp->writer); }
	
	
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
		GET_SELF;
		hp_writer_begin(jp->writer, NULL);
		hp_writer_write(jp->writer, &$1);
		hp_writer_end(jp->writer);
	}
|	{ GET_SELF; hp_writer_begin(jp->writer, NULL); }
	Object
	{ GET_SELF; hp_writer_end(jp->writer);  }
	

	
%%

