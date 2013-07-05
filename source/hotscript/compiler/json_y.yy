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
#define YYSELF HP_CONTAINER_OF(ss, JSON_PARSER, scanner_stack)
#define YYJSON_WRITER YYSELF->writer
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
	'{' { write_struct_begin(YYJSON_WRITER, NULL); }
	'}' { write_struct_end(YYJSON_WRITER, NULL); }
|	
	'{' { write_struct_begin(YYJSON_WRITER, NULL); }
	Members
	'}' { write_struct_end(YYJSON_WRITER, NULL); }
;

Members:
	Pair
	{
	}
|
	Members
	',' { write_semicolon(YYJSON_WRITER); }
	Pair
;
	
	
Pair:
	tok_string { write_field_begin(YYJSON_WRITER, $1.val.bytes.ptr, $1.val.bytes.len); }
	':'
	Value { write_field_end(YYJSON_WRITER, $1.val.bytes.ptr, $1.val.bytes.len); }
	
	
Array:
	'[' { write_vector_begin(YYJSON_WRITER); }
	Elements
	']' { write_vector_end(YYJSON_WRITER); }

Elements:
	Elements
	',' { write_semicolon(YYJSON_WRITER); }
	{write_vector_item_begin(YYJSON_WRITER, writer_get_index(YYJSON_WRITER)); }
	Value
	{write_vector_item_end(YYJSON_WRITER, writer_get_index(YYJSON_WRITER)); }
|	
	{write_vector_item_begin(YYJSON_WRITER, writer_get_index(YYJSON_WRITER));}
	Value
	{write_vector_item_end(YYJSON_WRITER, writer_get_index(YYJSON_WRITER));}
	
Value:
	tok_string
	{
		write_type(YYJSON_WRITER, E_HP_BYTES);
		write_bytes(YYJSON_WRITER, $1.val.bytes);
	}
|	tok_integer
	{
		write_type(YYJSON_WRITER, E_HP_INT64);
		write_hpint64(YYJSON_WRITER, $1.val.i64);
	}
|	tok_double
	{
		write_type(YYJSON_WRITER, E_HP_DOUBLE);
		write_hpdouble(YYJSON_WRITER, $1.val.d);
	}
|	{ write_type(YYJSON_WRITER, E_HP_STRUCT); } Object
|	{ write_type(YYJSON_WRITER, E_HP_VECTOR); } Array	
|	tok_true
	{
		write_type(YYJSON_WRITER, E_HP_BOOL);
		write_hpbool(YYJSON_WRITER, hptrue);
	}
|	tok_false
	{
		write_type(YYJSON_WRITER, E_HP_BOOL);
		write_hpbool(YYJSON_WRITER, hpfalse);
	}
|	tok_null
	{
		write_type(YYJSON_WRITER, E_HP_NULL);
		write_null(YYJSON_WRITER);
	}
	

	
%%

