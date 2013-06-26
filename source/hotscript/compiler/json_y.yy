%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"


#include "json_parser.h"
#include "hotscript/hotobject.h"

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
#define YYLEX_PARAM jp

#define MAX_STRING_LENGTH 124
#define MAX_NAME_LENGTH 64

#define YYSTYPE HPVar

}//code requires end

%define api.pure
%parse-param { JSON_PARSER *jp }
%pure_parser




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
		hp_writer_begin(jp->writer, &$1);
		hp_writer_write(jp->writer, &$3);
		hp_writer_end(jp->writer);
	}
|	
	tok_identifier ':' 
	{ hp_writer_begin(jp->writer, &$1); }
	Object
	{ hp_writer_end(jp->writer); }
|
	tok_identifier ':' 
	{ hp_writer_begin(jp->writer, &$1); } 
	Array
	{ hp_writer_end(jp->writer); }
	
	
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
		hp_writer_begin(jp->writer, NULL);
		hp_writer_write(jp->writer, &$1);
		hp_writer_end(jp->writer);
	}
|	{ hp_writer_begin(jp->writer, NULL); }
	Object
	{ hp_writer_end(jp->writer);  }
	

	
%%

