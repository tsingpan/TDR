%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"


#include "json_parser.h"
#include "hotobject.h"

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
#define GET_JSON_PARSER JSON_PARSER *jp = HP_CONTAINER_OF(arg, JSON_PARSER, scanner);
%}
%locations

%code requires
{
#include "globals.h"

#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM *(yyscan_t*)arg

#define MAX_STRING_LENGTH 124
#define MAX_NAME_LENGTH 64


#ifndef _DEF_ZNODE
#define _DEF_ZNODE

typedef enum _ZNODE_TYPE
{
	E_ZN_STRING,
	E_ZN_OBJECT,
	E_ZN_MEMBERS,
	E_ZN_PAIR,
	E_ZN_ELEMENTS,
	E_ZN_ARRAY,
}ZNODE_TYPE;

typedef struct _znode znode;
struct _znode
{
	ZNODE_TYPE type;
	char name[MAX_NAME_LENGTH];
	char string[MAX_STRING_LENGTH];
	hpuint32 string_length;
};
#endif //_DEF_ZNODE
#define YYSTYPE znode

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
		hotobject_write_string(&jp->ho_iter, $1.name, $3.string);
	}
|	
	tok_identifier ':' 
	{ GET_JSON_PARSER; hotobject_write_object_begin(&jp->ho_iter, $1.name); }
	Object
	{ GET_JSON_PARSER; hotobject_write_object_end(&jp->ho_iter, $1.name); }
|
	tok_identifier ':' 
	{ GET_JSON_PARSER; hotobject_write_object_begin(&jp->ho_iter, $1.name); } 
	Array
	{ GET_JSON_PARSER;	hotobject_write_object_end(&jp->ho_iter, $1.name); }
	
	
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
		GET_JSON_PARSER; hotobject_write_string(&jp->ho_iter, NULL, $1.string);
	}
|	{ GET_JSON_PARSER; hotobject_write_object_begin(&jp->ho_iter, NULL); }
	Object
	{ GET_JSON_PARSER; hotobject_write_object_end(&jp->ho_iter, NULL);  }
	

	
%%

