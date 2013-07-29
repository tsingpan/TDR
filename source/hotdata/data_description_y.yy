%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>


#define YYERROR_VERBOSE
#define GET_SELF HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack)
#define GET_WRITER HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack)->writer
#define GET_DEFINITION HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack)->pn_definition

%}
%locations

%code requires
{

#include "hotpot/hp_value.h"
#include "hotscript/hotlex.h"

#include "hotdata_parser.h"
#include "hotdata_reduce.h"

#define YYMALLOC
#define YYFREE

#define YYSTYPE ParserNode
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
%token tok_int64
%token tok_uint64
%token tok_hex_int64
%token tok_hex_uint64
%token tok_double
%token tok_bool
%token tok_string
%token tok_char


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


%type<sn_definition> Definition
%type<sn_import> Import
%type<sn_tok_import> tok_import

%type<sn_tok_identifier> tok_identifier
%type<sn_int64> tok_int64
%type<sn_hex_int64> tok_hex_int64
%type<sn_uint64> tok_uint64
%type<sn_hex_uint64> tok_hex_uint64
%type<sn_char> tok_char
%type<sn_tok_unixcomment> tok_unixcomment

%type<sn_bool> tok_bool
%type<sn_type> Type SimpleType ObjectType ContainerType
%type<sn_value> Value
%type<sn_const> Const
%type<pn_tok_double> tok_double
%type<sn_string> tok_string

%type<sn_st> tok_t_char tok_t_bool tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64
%type<sn_ct> tok_t_vector tok_t_string




%left '='


%start Document

%%

Document :
	{
		write_struct_begin(GET_WRITER, "ST_DOCUMENT");

		write_field_begin(GET_WRITER, "file_name");
		write_string(GET_WRITER, GET_SELF->file_name);
		write_field_end(GET_WRITER, "file_name");

		write_field_begin(GET_WRITER, "definition_list");
		write_vector_begin(GET_WRITER);
	}
	DefinitionList
	{
		write_vector_end(GET_WRITER);
		write_field_end(GET_WRITER, "definition_list");

		write_field_begin(GET_WRITER, "definition_list_num");
		write_uint32(GET_WRITER, GET_SELF->definition_list_num);
		write_field_end(GET_WRITER, "definition_list_num");
		write_struct_end(GET_WRITER, "ST_DOCUMENT");
	};

DefinitionList :
	DefinitionList Definition
	{
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_ST_DEFINITION(GET_WRITER, &GET_DEFINITION);
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));

		++GET_SELF->definition_list_num;

		dp_do_Definition(GET_SELF, &yylloc, &GET_DEFINITION);
	}
|	{
	};

Definition :
	Import
	{
		dp_reduce_Definition_Import(GET_SELF, &yylloc, &GET_DEFINITION, &$1);
	}
|	Const
	{
		dp_reduce_Definition_Const(GET_SELF, &yylloc, &GET_DEFINITION, &$1);
	}
| Typedef
	{
		GET_DEFINITION.type = E_DT_TYPEDEF;
	}
| Struct
	{
		GET_DEFINITION.type = E_DT_STRUCT;
	}
| Union
	{
		GET_DEFINITION.type = E_DT_UNION;
	}
| Enum
	{
		GET_DEFINITION.type = E_DT_ENUM;
	}
| UnixComment
	{
		GET_DEFINITION.type = E_DT_UNIX_COMMENT;
	}
;


Import :
	tok_import
	{
		dp_reduce_Import_tok_import(GET_SELF, &yylloc, &$$, &$1);
	};


Const :
	tok_const Type tok_identifier '=' Value ';'
	{
		dp_reduce_Const(GET_SELF, &yylloc, &$$, &$2, &$3, &$5);
	}



Value :
	tok_uint64
	{
		dp_reduce_Value_tok_uint64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_hex_uint64
	{
		dp_reduce_Value_tok_hex_uint64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_int64
	{
		dp_reduce_Value_tok_int64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_hex_int64
	{
		dp_reduce_Value_tok_hex_int64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_bool
	{
		dp_reduce_Value_tok_bool(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_double
	{
		dp_reduce_Value_tok_double(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_string
	{
		dp_reduce_Value_tok_string(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_char
	{
		dp_reduce_Value_tok_char(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_identifier
	{
		dp_reduce_Value_tok_identifier(GET_SELF, &yylloc, &$$, $1);
	}
;

Typedef :
	tok_typedef Type Arguments tok_identifier ';'
	{
	};
	
Enum :
	tok_enum TypeAnnotations tok_identifier	'{' EnumDefList '}' ';'
	{
	
	};
    
EnumDefList : 
	EnumDefList EnumDef
|	
	EnumDef
	
EnumDef : 
	tok_identifier '=' Value ',' UnixCommentOrNot
	{

	};
    

Union :
	tok_union TypeAnnotations tok_identifier Parameters '{' FieldList '}' ';'
	{
	};
	
	
Struct : 
	tok_struct TypeAnnotations tok_identifier Parameters '{' FieldList '}' ';'
	{
	};
	

	
FieldList: 
	FieldList Field	
|	
	Field
	

Field : 
	FieldCondition Type Arguments tok_identifier ';' UnixCommentOrNot;

FieldCondition:
	{
	}
	Condition
	{
	}
|
	{
	};

Condition : 
	tok_if 	'(' FieldExpression	')'	
|	tok_if '!' '(' FieldExpression ')'
|	tok_if '(' tok_identifier tok_unequal tok_identifier ')'
	{
	}
|	tok_case tok_identifier ':'
	{
	};


FieldExpression :
	tok_identifier tok_equal tok_identifier
	{
	}
|	tok_identifier '&' tok_identifier
	{
	}
	;



Type :
	SimpleType
	{
		$$ = $1;
	}
|	ContainerType
	{
		$$ = $1;
	}
	
|	ObjectType
	{
		$$ = $1;
	};

ObjectType:
	tok_identifier
   	{
		dp_reduce_ObjectType_tok_identifier(GET_SELF, &yylloc, &$$, &$1);
	};

ContainerType:
	tok_t_vector
	{
		dp_reduce_ContainerType_tok_t_vector(GET_SELF, &yylloc, &$$);
	}
|	tok_t_string
	{
		dp_reduce_ContainerType_tok_t_string(GET_SELF, &yylloc, &$$);
	};

	
SimpleType:
	tok_t_bool
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_char
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_double
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int8
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int16
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int32
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int64
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint8 
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint16 
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint32 
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint64
	{
		dp_reduce_SimpleType(GET_SELF, &yylloc, &$$, $1);
	};

Parameters :
	'<' ParameterList '>'
	{
	}
|
	{
	};
	
ParameterList:
	ParameterList ',' Parameter 
|	
	Parameter
	
	
Parameter:
	Type
	tok_identifier
	{
	};




Arguments:
	'<' ArgumentList '>'
|
	{
	};
	
ArgumentList:
	ArgumentList ',' Argument
|	Argument;
	
Argument:
	tok_identifier
	{
	}
|	{
	}
	SimpleType
	{
	};

UnixComment:
	tok_unixcomment
	{
		
	};

UnixCommentOrNot:
	tok_unixcomment
	{
	}
|
	{
		
	};

TypeAnnotations:
	'('	TypeAnnotationList	')'
|
    {
    };

TypeAnnotationList:
  TypeAnnotationList ',' TypeAnnotation
    {
    }
| TypeAnnotation
    {
    };

TypeAnnotation:
	tok_unique '=' tok_bool
    {
    }
|	tok_lower_bound '=' tok_identifier
	{
		
	}
|	tok_upper_bound '=' tok_identifier
	{
		
	}
|	tok_switch '=' tok_identifier
	{
	}
|	;

    
%%
