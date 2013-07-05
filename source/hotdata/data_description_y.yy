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
	{ write_vector_begin(GET_WRITER); }
	DefinitionList
	{ write_vector_end(GET_WRITER); };

DefinitionList :
	DefinitionList Definition 
	{
	}
|	{
	};

Definition :
	{write_struct_begin(GET_WRITER, NULL);}
	Import	
	{write_struct_end(GET_WRITER, NULL);}	 
|	{write_struct_begin(GET_WRITER, NULL);write_field_begin(GET_WRITER, "const", strlen("const")); }
	Const
	{write_field_end(GET_WRITER, "const", strlen("const"));write_struct_end(GET_WRITER, NULL); write_semicolon(GET_WRITER);} 
|	Typedef
	{
	}
|	Struct
	{
	}
|	Union
	{
	}
|	Enum
	{
	}
|   UnixComment
	{
	};

Import : 
	tok_import
	{
	};


Const : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_const Type tok_identifier 
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $4.var.val.bytes);
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
	}
|	tok_hex
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpint64(GET_WRITER, $1.var.val.i64);
		write_field_end(GET_WRITER, "value", strlen("value"));
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
	tok_typedef Type Arguments tok_identifier CommaOrSemicolonOptional
	{
	};
	
Enum :
	tok_enum TypeAnnotations tok_identifier '{' EnumDefList '}' CommaOrSemicolonOptional
	{
    };
    
EnumDefList : 
	EnumDefList EnumDef
	{
	}
|
	{
	};
	
EnumDef : 
	tok_identifier '=' Value CommaOrSemicolonOptional UnixComment
	{
	};
    

Union :
	tok_union TypeAnnotations tok_identifier  Parameters '{' FieldList '}'  CommaOrSemicolonOptional
	{
	};
	
	
Struct : 
	tok_struct TypeAnnotations tok_identifier Parameters '{' FieldList '}' CommaOrSemicolonOptional
	{
	};
	

	
FieldList: 
	FieldList Field
	{
	}
|
	{
	};
	

Field : 
	FieldCondition Type Arguments tok_identifier ';' UnixComment
	{
	};
	

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
	tok_t_bool | tok_t_char | tok_t_double | tok_t_string | tok_t_vector
	| tok_t_int8 | tok_t_int16 | tok_t_int32 | tok_t_int64
	| tok_t_uint8 | tok_t_uint16 | tok_t_uint32 | tok_t_uint64
	{
	}
|	tok_identifier
   	{
	};

Parameters :
	'<' ParameterList '>'
	{
	}
|
	{
	};
	
ParameterList:
	ParameterList Parameter 
	{
	}
|
	{
	};
	
	
Parameter:
	Type tok_identifier CommaOrSemicolonOptional
	{
	};




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
	tok_unixcomment
	{
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
