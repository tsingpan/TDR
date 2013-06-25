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
#define GET_JSON_PARSER JSON_PARSER *jp = HP_CONTAINER_OF(arg, JSON_PARSER, scanner);
%}
%locations

%code requires
{
#include "globals.h"
#include "hotpot/hp_value.h"

#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM *(yyscan_t*)arg

#define MAX_STRING_LENGTH 124
#define MAX_NAME_LENGTH 64

#define YYSTYPE HPVar

}//code requires end

%define api.pure
%parse-param { void* arg }
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
		GET_JSON_PARSER;
		hp_writer_begin(jp->writer, &$1);
		hp_writer_write(jp->writer, &$3);
		hp_writer_end(jp->writer);
	}
|	
	tok_identifier ':' 
	{ GET_JSON_PARSER; hp_writer_begin(jp->writer, &$1); }
	Object
	{ GET_JSON_PARSER; hp_writer_end(jp->writer); }
|
	tok_identifier ':' 
	{ GET_JSON_PARSER; hp_writer_begin(jp->writer, &$1); } 
	Array
	{ GET_JSON_PARSER;	hp_writer_end(jp->writer); }
	
	
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
		GET_JSON_PARSER;
		hp_writer_begin(jp->writer, NULL);
		hp_writer_write(jp->writer, &$1);
		hp_writer_end(jp->writer);
	}
|	{ GET_JSON_PARSER; hp_writer_begin(jp->writer, NULL); }
	Object
	{ GET_JSON_PARSER; hp_writer_end(jp->writer);  }
	

	
%%

