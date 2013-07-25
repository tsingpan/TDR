%{
//todo这里生成的json， 最好也用hotdata格式描述， 不过目前有些难度， 主要是因为没有树形容器
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>


#define YYERROR_VERBOSE
#define GET_SELF HP_CONTAINER_OF(ss, DATA_PARSER, scanner_stack)
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


%type<sn_import> Import
%type<sn_tok_import> tok_import

%type<sn_tok_identifier> tok_identifier
%type<sn_int64> tok_int64
%type<sn_hex_int64> tok_hex_int64
%type<sn_uint64> tok_uint64
%type<sn_hex_uint64> tok_hex_uint64
%type<sn_tok_unixcomment> tok_unixcomment
%type<sn_bool> tok_bool
%type<sn_type> Type SimpleType ObjectType ContainerType
%type<sn_value> Value

%type<sn_st> tok_t_char tok_t_bool tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64
%type<sn_ct> tok_t_vector tok_t_string




%left '='


%start Document

%%

Document :
	{
		dp_on_document_begin(GET_SELF, &yylloc);		
	}
	DefinitionList
	{
		dp_on_document_end(GET_SELF, &yylloc);
	};

DefinitionList :
	DefinitionList 
	{
		dp_on_definition_semicolon(GET_SELF, &yylloc);
	}
	Definition 
	{
	}
|
	{}
	Definition 
	{};

Definition :
	Import
|	Const
|	Typedef
|	Struct
|	Union
|	Enum
|   UnixComment;


Import :
	{dp_on_vector_item_begin(GET_SELF, &yylloc); dp_on_struct_begin(GET_SELF, &yylloc); dp_on_field_begin(GET_SELF, &yylloc, "import");  }
	tok_import
	{
		//首先规约这个语法节点
		dp_do_import(GET_SELF, &yylloc, &$$, $2);

		write_ST_Import(GET_WRITER, &$$);
		//dp_on_tok_import(GET_SELF, &yylloc, $2);
		
		dp_on_field_end(GET_SELF, &yylloc, "import");
		
		dp_on_struct_end(GET_SELF, &yylloc);

		dp_on_vector_item_end(GET_SELF, &yylloc);
		
		
	};


Const : 
	{dp_on_definition_begin(GET_SELF, &yylloc); dp_on_const_begin(GET_SELF, &yylloc);}
	tok_const 
	Type
	{dp_on_const_semicolon(GET_SELF, &yylloc); }
	tok_identifier 
	{
		dp_on_const_tok_identifier(GET_SELF, &yylloc, $5);


		//dp_check_constant_identifier(GET_SELF, &yylloc, &$3, $5);
	}
	'='
	{
		dp_on_const_semicolon(GET_SELF, &yylloc); 		
	}
	Value
	';'
	{	
		dp_on_const_end(GET_SELF, &yylloc); 

		//dp_check_constant_value(GET_SELF, &yylloc, &$3, &$5, &$9);
		
		dp_on_definition_end(GET_SELF, &yylloc);
	}


Value :
	tok_uint64
	{
		dp_on_value_tok_uint64(GET_SELF, &yylloc, $1);

		dp_do_value_tok_uint64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_hex_uint64
	{
		dp_on_value_tok_hex_uint64(GET_SELF, &yylloc, $1);

		dp_do_value_tok_hex_uint64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_int64
	{
		dp_on_value_tok_int64(GET_SELF, &yylloc, $1);

		dp_do_value_tok_int64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_hex_int64
	{
		dp_on_value_tok_hex_int64(GET_SELF, &yylloc, $1);

		dp_do_value_tok_hex_int64(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_bool
	{
	
	}
|	tok_double
	{
	}
|	tok_string
	{
	}
|	tok_char
	{
	}
|	tok_identifier
	{
		dp_on_value_tok_identifier(GET_SELF, &yylloc, $1);	
		

		dp_do_value_identifier(GET_SELF, &yylloc, &$$, $1);
	}
;

Typedef :
	{dp_on_definition_begin(GET_SELF, &yylloc);dp_on_typedef_begin(GET_SELF, &yylloc);}
	tok_typedef	
	Type 
	{dp_on_semicolon(GET_SELF, &yylloc);dp_on_field_begin(GET_SELF, &yylloc, "Arguments");}
	Arguments
	{dp_on_field_end(GET_SELF, &yylloc, "Arguments");dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier
	{
		dp_on_typedef_tok_identifier(GET_SELF, &yylloc, &$3, $7);
	}
	';'
	{
		dp_on_typedef_end(GET_SELF, &yylloc);
		dp_on_definition_end(GET_SELF, &yylloc);
	};
	
Enum :
	{dp_on_definition_begin(GET_SELF, &yylloc);dp_on_enum_begin(GET_SELF, &yylloc);}
	tok_enum
	TypeAnnotations
	{dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier
	{
		dp_on_enum_tok_identifier(GET_SELF, &yylloc, $5);
	}
	'{' {dp_on_field_begin(GET_SELF, &yylloc, "list");  dp_on_vector_begin(GET_SELF, &yylloc);}
	EnumDefList 
	'}' {dp_on_vector_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "list"); }
	';'
	{dp_on_enum_end(GET_SELF, &yylloc);
	dp_on_definition_end(GET_SELF, &yylloc);
	};
    
EnumDefList : 
	EnumDefList {dp_on_semicolon(GET_SELF, &yylloc);} 
	{dp_on_vector_item_begin(GET_SELF, &yylloc);}
	EnumDef
	{dp_on_vector_item_end(GET_SELF, &yylloc);}
|	
	{dp_on_vector_item_begin(GET_SELF, &yylloc);}
	EnumDef
	{dp_on_vector_item_end(GET_SELF, &yylloc);};
	
EnumDef : 
	{dp_on_struct_begin(GET_SELF, &yylloc);}
	tok_identifier 
	{
		dp_on_EnumDef_tok_identifier(GET_SELF, &yylloc, $2);
	}
	'='
	{
		dp_on_semicolon(GET_SELF, &yylloc);
	}
	Value
	','
	{
		dp_on_semicolon(GET_SELF, &yylloc);
	}
	UnixCommentOrNot
	{dp_on_struct_end(GET_SELF, &yylloc);};
    

Union :
	{ dp_on_definition_begin(GET_SELF, &yylloc);dp_on_union_begin(GET_SELF, &yylloc);}
	tok_union 
	TypeAnnotations	
	{dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier 
	{
		dp_on_union_tok_identifier(GET_SELF, &yylloc, $5);

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "Parameters");
	}
	Parameters
	{dp_on_field_end(GET_SELF, &yylloc, "Parameters");dp_on_semicolon(GET_SELF, &yylloc);}
	'{' {dp_on_field_begin(GET_SELF, &yylloc, "list");dp_on_vector_begin(GET_SELF, &yylloc);}
	FieldList 
	'}' {dp_on_field_end(GET_SELF, &yylloc, "list");dp_on_vector_end(GET_SELF, &yylloc);}
	';'
	{dp_on_union_end(GET_SELF, &yylloc);dp_on_definition_end(GET_SELF, &yylloc);};
	
	
Struct : 
	{dp_on_definition_begin(GET_SELF, &yylloc);dp_on_field_begin(GET_SELF, &yylloc, "struct"); dp_on_struct_begin(GET_SELF, &yylloc);  }
	tok_struct
	TypeAnnotations
	{dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier
	{
		dp_on_struct_tok_identifier(GET_SELF, &yylloc, $5);;
		dp_on_semicolon(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "Parameters");
	}
	Parameters
	{dp_on_field_end(GET_SELF, &yylloc, "Parameters"); dp_on_semicolon(GET_SELF, &yylloc);}
	'{' {dp_on_field_begin(GET_SELF, &yylloc, "list"); dp_on_vector_begin(GET_SELF, &yylloc);}
	FieldList
	'}' {dp_on_field_end(GET_SELF, &yylloc, "list"); dp_on_vector_end(GET_SELF, &yylloc);}
	';'
	{dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "struct");dp_on_definition_end(GET_SELF, &yylloc);};
	

	
FieldList: 
	FieldList {dp_on_semicolon(GET_SELF, &yylloc);}
	Field	
|	
	Field
	

Field : 
	{
		dp_on_vector_item_begin(GET_SELF, &yylloc);

		dp_on_struct_begin(GET_SELF, &yylloc);
		
		dp_on_field_begin(GET_SELF, &yylloc, "condition");
	}
	FieldCondition 
	{	dp_on_field_end(GET_SELF, &yylloc, "condition"); dp_on_semicolon(GET_SELF, &yylloc);}
	Type
	{dp_on_semicolon(GET_SELF, &yylloc); dp_on_field_begin(GET_SELF, &yylloc, "Arguments");}
	Arguments 
	{dp_on_field_end(GET_SELF, &yylloc, "Arguments");dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier
	{
		dp_on_field_tok_identifier(GET_SELF, &yylloc, $8);
	}
	';'
	{
		dp_on_semicolon(GET_SELF, &yylloc);
	}
	UnixCommentOrNot
	{
		dp_on_struct_end(GET_SELF, &yylloc);

		dp_on_vector_item_end(GET_SELF, &yylloc);
	};

FieldCondition:
	{
		dp_on_struct_begin(GET_SELF, &yylloc);;
	}
	Condition
	{
		dp_on_struct_end(GET_SELF, &yylloc);
	}
|
	{
		dp_on_null(GET_SELF, &yylloc);
	};

Condition : 
	tok_if 	'(' FieldExpression	')'	
|	tok_if
	'!'
	{
		dp_on_field_begin(GET_SELF, &yylloc, "negation"); 
		dp_on_bool(GET_SELF, &yylloc, hptrue);
		dp_on_field_end(GET_SELF, &yylloc, "negation"); 

		dp_on_semicolon(GET_SELF, &yylloc);
	}
	 '(' FieldExpression ')'
|	tok_if '(' tok_identifier tok_unequal tok_identifier ')'
	{
		dp_on_field_begin(GET_SELF, &yylloc, "negation"); 
		dp_on_bool(GET_SELF, &yylloc, hptrue);
		dp_on_field_end(GET_SELF, &yylloc, "negation"); 


		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "expression"); 
		dp_on_struct_begin(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op0"); 
		dp_on_bytes(GET_SELF, &yylloc, $3);
		dp_on_field_end(GET_SELF, &yylloc, "op0"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "operator"); 
		dp_on_string(GET_SELF, &yylloc, "==");
		dp_on_field_end(GET_SELF, &yylloc, "operator"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op1"); 
		dp_on_bytes(GET_SELF, &yylloc, $5);
		dp_on_field_end(GET_SELF, &yylloc, "op1"); 

		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "expression"); 
	}
|	tok_case tok_identifier ':'
	{
		dp_on_field_begin(GET_SELF, &yylloc, "expression"); 
		dp_on_struct_begin(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op0"); 
		dp_on_string(GET_SELF, &yylloc, "switch");
		dp_on_field_end(GET_SELF, &yylloc, "op0"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "operator"); 
		dp_on_string(GET_SELF, &yylloc, "==");
		dp_on_field_end(GET_SELF, &yylloc, "operator"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op1"); 
		dp_on_bytes(GET_SELF, &yylloc, $2);
		dp_on_field_end(GET_SELF, &yylloc, "op1"); 

		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "expression"); 
	};


FieldExpression :
	tok_identifier tok_equal tok_identifier
	{
		dp_on_field_begin(GET_SELF, &yylloc, "expression"); 
		dp_on_struct_begin(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op0"); 
		dp_on_bytes(GET_SELF, &yylloc, $1);
		dp_on_field_end(GET_SELF, &yylloc, "op0"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "operator"); 
		dp_on_string(GET_SELF, &yylloc, "==");
		dp_on_field_end(GET_SELF, &yylloc, "operator"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op1"); 
		dp_on_bytes(GET_SELF, &yylloc, $3);
		dp_on_field_end(GET_SELF, &yylloc, "op1"); 

		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "expression"); 
	}

|	tok_identifier '&' tok_identifier
	{
		dp_on_field_begin(GET_SELF, &yylloc, "expression"); 
		dp_on_struct_begin(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op0"); 
		dp_on_bytes(GET_SELF, &yylloc, $1);
		dp_on_field_end(GET_SELF, &yylloc, "op0"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "operator"); 
		dp_on_string(GET_SELF, &yylloc, "&");
		dp_on_field_end(GET_SELF, &yylloc, "operator"); 

		dp_on_semicolon(GET_SELF, &yylloc);

		dp_on_field_begin(GET_SELF, &yylloc, "op1"); 
		dp_on_bytes(GET_SELF, &yylloc, $3);
		dp_on_field_end(GET_SELF, &yylloc, "op1"); 

		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "expression"); 	
	}
	;



Type :
	{dp_on_field_begin(GET_SELF, &yylloc, "Type"); dp_on_struct_begin(GET_SELF, &yylloc); dp_on_field_begin(GET_SELF, &yylloc, "SimpleType"); dp_on_struct_begin(GET_SELF, &yylloc);dp_on_field_begin(GET_SELF, &yylloc, "type");}
	SimpleType
	{dp_on_field_end(GET_SELF, &yylloc, "type");dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "SimpleType"); dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "Type"); $$ = $2;
	}
	
|	{dp_on_field_begin(GET_SELF, &yylloc, "Type"); dp_on_struct_begin(GET_SELF, &yylloc); dp_on_field_begin(GET_SELF, &yylloc, "ContainerType"); dp_on_struct_begin(GET_SELF, &yylloc);}
	ContainerType
	{dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "ContainerType"); dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "Type"); $$ = $2;
	}
	
|	{dp_on_field_begin(GET_SELF, &yylloc, "Type"); dp_on_struct_begin(GET_SELF, &yylloc); dp_on_field_begin(GET_SELF, &yylloc, "ObjectType"); dp_on_struct_begin(GET_SELF, &yylloc);}
	ObjectType
	{dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "ObjectType"); dp_on_struct_end(GET_SELF, &yylloc); dp_on_field_end(GET_SELF, &yylloc, "Type"); $$ = $2;
	};

ObjectType:
	tok_identifier
   	{
		dp_on_field_begin(GET_SELF, &yylloc, "type");
		dp_on_bytes(GET_SELF, &yylloc, $1);
		dp_on_field_end(GET_SELF, &yylloc, "type");
		
		dp_do_type_object(GET_SELF, &yylloc, &$$, $1);
	};

ContainerType:
	tok_t_vector
	{
		dp_on_field_begin(GET_SELF, &yylloc, "vector");
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "vector");

		dp_do_container_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_string
	{
		dp_on_field_begin(GET_SELF, &yylloc, "string");
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "string");
		
		dp_do_container_type(GET_SELF, &yylloc, &$$, $1);
	};

	
SimpleType:
	tok_t_bool
	{
		dp_on_string(GET_SELF, &yylloc, "bool");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_char
	{
		dp_on_string(GET_SELF, &yylloc, "char");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_double
	{
		dp_on_string(GET_SELF, &yylloc, "double");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int8
	{
		dp_on_string(GET_SELF, &yylloc, "int8");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int16
	{
		dp_on_string(GET_SELF, &yylloc, "int16");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int32
	{
		dp_on_string(GET_SELF, &yylloc, "int32");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_int64
	{
		dp_on_string(GET_SELF, &yylloc, "int64");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint8 
	{
		dp_on_string(GET_SELF, &yylloc, "uint8");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint16 
	{
		dp_on_string(GET_SELF, &yylloc, "uint16");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint32 
	{
		dp_on_string(GET_SELF, &yylloc, "uint32");
		
		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	}
|	tok_t_uint64
	{
		dp_on_string(GET_SELF, &yylloc, "uint64");

		dp_do_simple_type(GET_SELF, &yylloc, &$$, $1);
	};

Parameters :
	'<' 
	{dp_on_vector_begin(GET_SELF, &yylloc);}
	ParameterList
	{dp_on_vector_end(GET_SELF, &yylloc);}
	'>'
	{
	}
|
	{
		dp_on_null(GET_SELF, &yylloc);
	};
	
ParameterList:
	ParameterList ',' {dp_on_semicolon(GET_SELF, &yylloc);} Parameter 
|	
	Parameter
	
	
Parameter:
	{
		dp_on_vector_item_begin(GET_SELF, &yylloc);
		dp_on_struct_begin(GET_SELF, &yylloc);
	}
	Type {dp_on_semicolon(GET_SELF, &yylloc);}
	tok_identifier
	{
		dp_on_field_begin(GET_SELF, &yylloc, "name");
		dp_on_bytes(GET_SELF, &yylloc, $4);
		dp_on_field_end(GET_SELF, &yylloc, "name");

		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_vector_item_end(GET_SELF, &yylloc);
	};




Arguments:
	'<'
	{dp_on_vector_begin(GET_SELF, &yylloc);}
	ArgumentList
	{dp_on_vector_end(GET_SELF, &yylloc);}
	'>'
|
	{
		dp_on_null(GET_SELF, &yylloc);
	};
	
ArgumentList:
	ArgumentList ',' {dp_on_semicolon(GET_SELF, &yylloc);} Argument
|	Argument;
	
Argument:
	tok_identifier
	{
		dp_on_vector_item_begin(GET_SELF, &yylloc);
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "Identifier");
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "id");
		dp_on_bytes(GET_SELF, &yylloc, $1);
		dp_on_field_end(GET_SELF, &yylloc, "id");
		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "Identifier");
		dp_on_struct_end(GET_SELF, &yylloc);		
		dp_on_vector_item_end(GET_SELF, &yylloc);
	}
|	{
		dp_on_vector_item_begin(GET_SELF, &yylloc);
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "SimpleType");
		dp_on_struct_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "type");
	}
	SimpleType
	{
		dp_on_field_end(GET_SELF, &yylloc, "type");
		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "SimpleType");
		dp_on_struct_end(GET_SELF, &yylloc);
		dp_on_vector_item_end(GET_SELF, &yylloc);
	};

UnixComment:
	{dp_on_definition_begin(GET_SELF, &yylloc);dp_on_tok_unixcomment_begin(GET_SELF, &yylloc);}
	tok_unixcomment
	{
		dp_on_field_begin(GET_SELF, &yylloc, "text");
		dp_on_bytes(GET_SELF, &yylloc, $2);
		dp_on_field_end(GET_SELF, &yylloc, "text");

		dp_on_tok_unixcomment_end(GET_SELF, &yylloc);
		dp_on_definition_end(GET_SELF, &yylloc);
	};

UnixCommentOrNot:
	tok_unixcomment
	{
		dp_on_tok_unixcomment_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "text");
		dp_on_bytes(GET_SELF, &yylloc, $1);
		dp_on_field_end(GET_SELF, &yylloc, "text");
		dp_on_tok_unixcomment_end(GET_SELF, &yylloc);
	}
|
	{
		dp_on_tok_unixcomment_begin(GET_SELF, &yylloc);
		dp_on_field_begin(GET_SELF, &yylloc, "text");
		dp_on_null(GET_SELF, &yylloc);
		dp_on_field_end(GET_SELF, &yylloc, "text");
		dp_on_tok_unixcomment_end(GET_SELF, &yylloc);
	};

TypeAnnotations:
	'('
	{dp_on_TypeAnnotations_begin(GET_SELF, &yylloc);}
	TypeAnnotationList
	{dp_on_TypeAnnotations_end(GET_SELF, &yylloc);}
	')'
|
    {
		dp_on_TypeAnnotations_begin(GET_SELF, &yylloc);
		dp_on_TypeAnnotations_switch(GET_SELF, &yylloc, NULL);
		dp_on_TypeAnnotations_end(GET_SELF, &yylloc);
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
		dp_on_TypeAnnotations_switch(GET_SELF, &yylloc, &$3);
	}
|	;

    
%%
