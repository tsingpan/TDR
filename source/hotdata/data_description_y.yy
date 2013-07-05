%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>

#define YYERROR_VERBOSE
#define GET_SELF DATA_PARSER *self = HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack);
#define GET_WRITER HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack)->writer

%}
%locations

%code requires
{

#include "hotpot/hp_value.h"
#include "hotscript/hotlex.h"

#include "hotdata_parser.h"

#define YYMALLOC
#define YYFREE

#define YYSTYPE SyntacticNode
#define YYLEX_PARAM ss
}

%define api.pure
%parse-param { SCANNER_STACK *ss }
%pure_parser



%token tok_import 
%token tok_struct
%token tok_union 
%token tok_if 
%token tok_int
%token tok_hex
%token tok_identifier 
%token tok_const 
%token tok_enum 
%token tok_equal
%token tok_unequal
%token tok_typename
%token tok_case
%token tok_unixcomment
%token tok_unique
%token tok_lower_bound
%token tok_upper_bound
%token tok_true
%token tok_false
%token tok_typedef
%token tok_switch
%token tok_t_int8
%token tok_t_int16
%token tok_t_int32
%token tok_t_int64
%token tok_t_uint8
%token tok_t_uint16
%token tok_t_uint32
%token tok_t_uint64
%token tok_t_bool
%token tok_t_char
%token tok_t_double
%token tok_t_string
%token tok_t_vector

%left '='


%start Document

%%

Document :
	{ write_struct_begin(GET_WRITER, NULL); write_field_begin(GET_WRITER, "DefinitionList", strlen("DefinitionList")); write_vector_begin(GET_WRITER); }
	DefinitionList
	{ write_vector_end(GET_WRITER); write_field_end(GET_WRITER, "DefinitionList", strlen("DefinitionList")); write_struct_end(GET_WRITER, NULL);};

DefinitionList :
	DefinitionList 
	{write_semicolon(GET_WRITER);}
	Definition 
|	Definition 
	{
	};

Definition :
	{write_struct_begin(GET_WRITER, NULL);}
	Import	
	{write_struct_end(GET_WRITER, NULL);}	 

|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "const", strlen("const")); }
	Const
	{write_field_end(GET_WRITER, "const", strlen("const"));write_struct_end(GET_WRITER, NULL);} 

|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "typedef", strlen("typedef")); }
	Typedef
	{write_field_end(GET_WRITER, "typedef", strlen("typedef"));write_struct_end(GET_WRITER, NULL); } 

|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "struct", strlen("struct")); }
	Struct
	{write_field_end(GET_WRITER, "struct", strlen("struct"));write_struct_end(GET_WRITER, NULL);} 

|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "union", strlen("union")); }
	Union
	{write_field_end(GET_WRITER, "union", strlen("union"));write_struct_end(GET_WRITER, NULL);} 

|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "enum", strlen("enum")); }
	Enum
	{write_field_end(GET_WRITER, "enum", strlen("enum"));write_struct_end(GET_WRITER, NULL);} 

|   {write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "comment", strlen("comment")); }
	UnixComment
	{write_field_end(GET_WRITER, "comment", strlen("comment"));write_struct_end(GET_WRITER, NULL);} ;

Import : 
	tok_import
	{
	};


Const : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_const 
	Type {write_semicolon(GET_WRITER);}
	tok_identifier 
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $5.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
	}
	'='
	{
		write_semicolon(GET_WRITER);
	}
	Value
	CommaOrSemicolonOptional
	{
		write_struct_end(GET_WRITER, NULL);
	}


Value :
	tok_int
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpint64(GET_WRITER, $1.var.val.i64);
		write_field_end(GET_WRITER, "value", strlen("value"));

		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "base", strlen("base"));
		write_hpint64(GET_WRITER, 10);
		write_field_end(GET_WRITER, "base", strlen("base"));
	}
|	tok_hex
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpint64(GET_WRITER, $1.var.val.i64);
		write_field_end(GET_WRITER, "value", strlen("value"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "base", strlen("base"));
		write_hpint64(GET_WRITER, 16);
		write_field_end(GET_WRITER, "base", strlen("base"));
	}
|	tok_identifier
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "value", strlen("value"));
	}
|	tok_true
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpstring(GET_WRITER, "true");
		write_field_end(GET_WRITER, "value", strlen("value"));
	}
|	tok_false
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpstring(GET_WRITER, "false");
		write_field_end(GET_WRITER, "value", strlen("value"));
	};

Typedef :
	{write_struct_begin(GET_WRITER, NULL);}
	tok_typedef Type 
	{write_semicolon(GET_WRITER);}
	Arguments
	tok_identifier
	{
		write_field_begin(GET_WRITER, "new_type", strlen("new_type"));
		write_bytes(GET_WRITER, $6.var.val.bytes);
		write_field_end(GET_WRITER, "new_type", strlen("new_type"));
	}
	CommaOrSemicolonOptional
	{write_struct_end(GET_WRITER, NULL);};
	
Enum :
	{write_struct_begin(GET_WRITER, NULL);}
	tok_enum TypeAnnotations tok_identifier 
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	EnumDefList 
	'}' {write_field_end(GET_WRITER, "list", strlen("list")); write_vector_end(GET_WRITER);}
	CommaOrSemicolonOptional
	{write_struct_end(GET_WRITER, NULL);};
    
EnumDefList : 
	EnumDefList {write_semicolon(GET_WRITER);} EnumDef
	{
	}
|	EnumDef
	{
	};
	
EnumDef : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_identifier 
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $2.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
	}
	'='
	{
		write_semicolon(GET_WRITER);
	}
	Value CommaOrSemicolonOptional UnixComment
	{write_struct_end(GET_WRITER, NULL);};
    

Union :
	{write_struct_begin(GET_WRITER, NULL);}
	tok_union 
	TypeAnnotations	
	tok_identifier 
	{write_field_begin(GET_WRITER, "Parameters", strlen("Parameters")); }
	Parameters
	{write_field_end(GET_WRITER, "Parameters", strlen("Parameters")); write_semicolon(GET_WRITER);}
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	FieldList 
	'}' {write_field_end(GET_WRITER, "list", strlen("list")); write_vector_end(GET_WRITER);}
	CommaOrSemicolonOptional
	{write_struct_end(GET_WRITER, NULL);};
	
	
Struct : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_struct TypeAnnotations tok_identifier
	{write_field_begin(GET_WRITER, "Parameters", strlen("Parameters")); }
	Parameters
	{write_field_end(GET_WRITER, "Parameters", strlen("Parameters")); write_semicolon(GET_WRITER);}
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	FieldList
	'}' {write_field_end(GET_WRITER, "list", strlen("list")); write_vector_end(GET_WRITER);}
	CommaOrSemicolonOptional
	{write_struct_end(GET_WRITER, NULL);};
	

	
FieldList: 
	FieldList {write_semicolon(GET_WRITER);}
	Field
	{
	}
|	Field
	{
	};
	

Field : 
	{write_struct_begin(GET_WRITER, NULL);}
	FieldCondition 
	Type {write_semicolon(GET_WRITER);}
	Arguments 
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $6.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
	}
	';' UnixComment
	{write_struct_end(GET_WRITER, NULL);};
	

FieldCondition : 
	tok_if '(' FieldExpression ')'
	{
		
	}
|	tok_if '!' '(' FieldExpression ')'
	{
		
	}
|	tok_case tok_identifier ':'
	{
		
	}
|	tok_if '(' tok_identifier tok_unequal tok_identifier ')'
	{
		
	}
|
	{
	};

FieldExpression :
	tok_identifier tok_equal tok_identifier
	{
	}

|	tok_identifier '&' tok_identifier
	{
	};



Type :
	tok_t_bool
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "bool");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_char
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "char");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_double
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "double");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_string
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "string");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_vector
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "vector");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_int8
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int8");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_int16
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int16");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_int32
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int32");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_int64
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int64");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_uint8 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint8");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_uint16 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint16");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_uint32 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint32");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_t_uint64
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint64");
		write_field_end(GET_WRITER, "type", strlen("type"));
	}
|	tok_identifier
   	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "type", strlen("type"));
	};

Parameters :
	'<' 
	{write_vector_begin(GET_WRITER);}
	ParameterList
	{write_vector_end(GET_WRITER);}
	'>'
	{
	}
|
	{
		write_null(GET_WRITER);
	};
	
ParameterList:
	ParameterList {write_semicolon(GET_WRITER);} Parameter 
	{
	}
|	Parameter
	{
	};
	
	
Parameter:
	{write_struct_begin(GET_WRITER, NULL);}
	Type {write_semicolon(GET_WRITER);}
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $4.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
	}
	CommaOrSemicolonOptional
	{write_struct_end(GET_WRITER, NULL);};




Arguments:
	'<' ArgumentList '>'
	{
	}
|
	{
	};
	
ArgumentList:
	ArgumentList Argument
	{
	}
|
	{
	};
	
Argument:
	tok_identifier CommaOrSemicolonOptional
	{
	}

UnixComment:
	{write_struct_begin(GET_WRITER, NULL)}
	tok_unixcomment
	{
		write_field_begin(GET_WRITER, "text", strlen("text"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "text", strlen("text"));
		write_struct_end(GET_WRITER, NULL);
	}
|
	{
		
	};

TypeAnnotations:
  '(' TypeAnnotationList ')'
    {
    }
|
    {
    };

TypeAnnotationList:
  TypeAnnotationList TypeAnnotation
    {
    }
|
    {
    };

Bool:
	tok_true | tok_false
	{
	};

TypeAnnotation:
	tok_unique '=' Bool CommaOrSemicolonOptional
    {
    }
|	tok_lower_bound '=' tok_identifier CommaOrSemicolonOptional
	{
		
	}
|	tok_upper_bound '=' tok_identifier CommaOrSemicolonOptional
	{
		
	}
|	tok_switch '=' tok_identifier CommaOrSemicolonOptional
	{
		
	};




CommaOrSemicolonOptional:
  ','
    {
    
    }
| ';'
    {
    
    }
|
    {
    
    };
    
%%
