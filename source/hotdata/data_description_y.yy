%{
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
	{
		write_struct_begin(GET_WRITER, "Document");
		write_field_begin(GET_WRITER, "DefinitionList", strlen("DefinitionList"));
		write_vector_begin(GET_WRITER);
	}
	DefinitionList
	{
		write_vector_end(GET_WRITER);
		write_field_end(GET_WRITER, "DefinitionList", strlen("DefinitionList"));
		write_struct_end(GET_WRITER, "Document");
	};

DefinitionList :
	DefinitionList 
	{
		write_semicolon(GET_WRITER);
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
	}
	Definition 
	{
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
	}
|
	{write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));}
	Definition 
	{write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));};

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
	tok_unixcomment
	{write_field_end(GET_WRITER, "comment", strlen("comment"));write_struct_end(GET_WRITER, NULL);}

Import : 
	tok_import
	{
		dp_on_import(GET_SELF, &yylloc, &$$, &$1);
	};


Const : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_const 
	Type
	{write_semicolon(GET_WRITER);}
	tok_identifier 
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $5.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));

		dp_on_constant_identifier(GET_SELF, &yylloc, &$3, &$5);
	}
	'='
	{
		write_semicolon(GET_WRITER);
	}
	Value
	';'
	{
		write_struct_end(GET_WRITER, NULL);		
		
		dp_on_constant_value(GET_SELF, &yylloc, &$3, &$5, &$9);
	}


Value :
	tok_int
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		if($1.var.type == E_HP_INT64)
		{
			write_hpint64(GET_WRITER, $1.body.sn_value.var.val.i64);
		}
		else
		{
			write_hpuint64(GET_WRITER, $1.body.sn_value.var.val.ui64);
		}
		write_field_end(GET_WRITER, "value", strlen("value"));

		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "base", strlen("base"));
		write_hpint64(GET_WRITER, 10);
		write_field_end(GET_WRITER, "base", strlen("base"));
		
		$$.type = NT_VALUE;
		$$.body.sn_value.is_identifier = hpfalse;
		$$.body.sn_value.var = $1.body.sn_value.var;
	}
|	tok_hex
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		if($1.var.type == E_HP_INT64)
		{
			write_hpint64(GET_WRITER, $1.body.sn_value.var.val.i64);
		}
		else
		{
			write_hpuint64(GET_WRITER, $1.body.sn_value.var.val.ui64);
		}
		write_field_end(GET_WRITER, "value", strlen("value"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "base", strlen("base"));
		write_hpint64(GET_WRITER, 16);
		write_field_end(GET_WRITER, "base", strlen("base"));
		
		$$.type = NT_VALUE;
		$$.body.sn_value.is_identifier = hpfalse;
		$$.body.sn_value.var = $1.body.sn_value.var;
	}
|	tok_true
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpstring(GET_WRITER, "true");
		write_field_end(GET_WRITER, "value", strlen("value"));
		
		$$.type = NT_VALUE;
		$$.body.sn_value.is_identifier = hpfalse;
		$$.body.sn_value.var = $1.body.sn_value.var;
	}
|	tok_false
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_hpstring(GET_WRITER, "false");
		write_field_end(GET_WRITER, "value", strlen("value"));
		
		$$.type = NT_VALUE;
		$$.body.sn_value.is_identifier = hpfalse;
		$$.body.sn_value.var = $1.body.sn_value.var;
	}
|	tok_identifier
	{
		write_field_begin(GET_WRITER, "value", strlen("value"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "value", strlen("value"));
		
		

		dp_on_value_identifier(GET_SELF, &yylloc, &$$, &$1);
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
	';'
	{write_struct_end(GET_WRITER, NULL);};
	
Enum :
	{write_struct_begin(GET_WRITER, NULL);}
	tok_enum
	{write_field_begin(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));}
	TypeAnnotations
	{write_field_end(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));write_semicolon(GET_WRITER);}
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $6.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
		write_semicolon(GET_WRITER);
	}
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	EnumDefList 
	'}' {write_vector_end(GET_WRITER); write_field_end(GET_WRITER, "list", strlen("list")); }
	';'
	{write_struct_end(GET_WRITER, NULL);};
    
EnumDefList : 
	EnumDefList {write_semicolon(GET_WRITER);} 
	{write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));}
	EnumDef
	{write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));}
|	
	{write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));}
	EnumDef
	{write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));};
	
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
	Value ',' UnixComment
	{write_struct_end(GET_WRITER, NULL);};
    

Union :
	{write_struct_begin(GET_WRITER, NULL);}
	tok_union 
	{write_field_begin(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));}
	TypeAnnotations	
	{write_field_end(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));write_semicolon(GET_WRITER);}
	tok_identifier 
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $6.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
		write_semicolon(GET_WRITER);

		write_field_begin(GET_WRITER, "Parameters", strlen("Parameters"));
	}
	Parameters
	{write_field_end(GET_WRITER, "Parameters", strlen("Parameters")); write_semicolon(GET_WRITER);}
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	FieldList 
	'}' {write_field_end(GET_WRITER, "list", strlen("list")); write_vector_end(GET_WRITER);}
	';'
	{write_struct_end(GET_WRITER, NULL);};
	
	
Struct : 
	{write_struct_begin(GET_WRITER, NULL);}
	tok_struct
	{write_field_begin(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));}
	TypeAnnotations
	{write_field_end(GET_WRITER, "TypeAnnotations", strlen("TypeAnnotations"));write_semicolon(GET_WRITER);}
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $6.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
		write_semicolon(GET_WRITER);

		write_field_begin(GET_WRITER, "Parameters", strlen("Parameters"));
	}
	Parameters
	{write_field_end(GET_WRITER, "Parameters", strlen("Parameters")); write_semicolon(GET_WRITER);}
	'{' {write_field_begin(GET_WRITER, "list", strlen("list")); write_vector_begin(GET_WRITER);}
	FieldList
	'}' {write_field_end(GET_WRITER, "list", strlen("list")); write_vector_end(GET_WRITER);}
	';'
	{write_struct_end(GET_WRITER, NULL);};
	

	
FieldList: 
	FieldList {write_semicolon(GET_WRITER);}
	Field	
|	
	Field
	

Field : 
	{
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_begin(GET_WRITER, NULL);
		
		write_field_begin(GET_WRITER, "condition", strlen("condition"));		
	}
	FieldCondition 
	{	write_field_end(GET_WRITER, "condition", strlen("condition"));	write_semicolon(GET_WRITER);}
	Type
	{write_semicolon(GET_WRITER); write_field_begin(GET_WRITER, "Arguments", strlen("Arguments"));}
	Arguments 
	{write_field_end(GET_WRITER, "Arguments", strlen("Arguments"));write_semicolon(GET_WRITER);}
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $8.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
	}
	';' UnixComment
	{
		write_struct_end(GET_WRITER, NULL);
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
	};

FieldCondition:
	{
		write_struct_begin(GET_WRITER, NULL);
	}
	Condition
	{
		write_struct_end(GET_WRITER, NULL);
	}
|
	{
		write_null(GET_WRITER);
	};

Condition : 
	tok_if 	'(' FieldExpression	')'	
|	tok_if
	'!'
	{
		write_field_begin(GET_WRITER, "negation", strlen("negation"));
		write_hpbool(GET_WRITER, hptrue);
		write_field_end(GET_WRITER, "negation", strlen("negation"));
		write_semicolon(GET_WRITER);
	}
	 '(' FieldExpression ')'
|	tok_if '(' tok_identifier tok_unequal tok_identifier ')'
	{
		write_field_begin(GET_WRITER, "negation", strlen("negation"));
		write_hpbool(GET_WRITER, hptrue);
		write_field_end(GET_WRITER, "negation", strlen("negation"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "expression", strlen("expression"));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "op0", strlen("op0"));
		write_bytes(GET_WRITER, $3.var.val.bytes);
		write_field_end(GET_WRITER, "op0", strlen("op0"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "operator", strlen("operator"));
		write_hpstring(GET_WRITER, "==");
		write_field_end(GET_WRITER, "operator", strlen("operator"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "op1", strlen("op1"));
		write_bytes(GET_WRITER, $5.var.val.bytes);
		write_field_end(GET_WRITER, "op1", strlen("op1"));
		write_struct_end(GET_WRITER, NULL);
		write_field_end(GET_WRITER, "expression", strlen("expression"));		
	}
|	tok_case tok_identifier ':'
	{
		write_field_begin(GET_WRITER, "expression", strlen("expression"));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "op0", strlen("op0"));
		write_hpstring(GET_WRITER, "switch");
		write_field_end(GET_WRITER, "op0", strlen("op0"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "operator", strlen("operator"));
		write_hpstring(GET_WRITER, "==");
		write_field_end(GET_WRITER, "operator", strlen("operator"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "op1", strlen("op1"));
		write_bytes(GET_WRITER, $2.var.val.bytes);
		write_field_end(GET_WRITER, "op1", strlen("op1"));
		write_struct_end(GET_WRITER, NULL);
		write_field_end(GET_WRITER, "expression", strlen("expression"));	
	};


FieldExpression :
	tok_identifier tok_equal tok_identifier
	{
		write_field_begin(GET_WRITER, "expression", strlen("expression"));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "op0", strlen("op0"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "op0", strlen("op0"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "operator", strlen("operator"));
		write_hpstring(GET_WRITER, "==");
		write_field_end(GET_WRITER, "operator", strlen("operator"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "op1", strlen("op1"));
		write_bytes(GET_WRITER, $3.var.val.bytes);
		write_field_end(GET_WRITER, "op1", strlen("op1"));
		write_struct_end(GET_WRITER, NULL);
		write_field_end(GET_WRITER, "expression", strlen("expression"));		
	}

|	tok_identifier '&' tok_identifier
	{
		write_field_begin(GET_WRITER, "expression", strlen("expression"));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "op0", strlen("op0"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "op0", strlen("op0"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "operator", strlen("operator"));
		write_hpstring(GET_WRITER, "&");
		write_field_end(GET_WRITER, "operator", strlen("operator"));
		write_semicolon(GET_WRITER);
		write_field_begin(GET_WRITER, "op1", strlen("op1"));
		write_bytes(GET_WRITER, $3.var.val.bytes);
		write_field_end(GET_WRITER, "op1", strlen("op1"));
		write_struct_end(GET_WRITER, NULL);
		write_field_end(GET_WRITER, "expression", strlen("expression"));		
	};



Type :
	{write_field_begin(GET_WRITER, "Type", strlen("Type")); write_struct_begin(GET_WRITER, NULL); write_field_begin(GET_WRITER, "SimpleType", strlen("SimpleType")); write_struct_begin(GET_WRITER, NULL);}
	SimpleType
	{write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "SimpleType", strlen("SimpleType")); write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "Type", strlen("Type"));
	$$.body = $2.body;
	$$.type = NT_TYPE;
	}
	
|	{write_field_begin(GET_WRITER, "Type", strlen("Type")); write_struct_begin(GET_WRITER, NULL); write_field_begin(GET_WRITER, "ContainerType", strlen("ContainerType")); write_struct_begin(GET_WRITER, NULL);}
	ContainerType
	{write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "ContainerType", strlen("ContainerType")); write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "Type", strlen("Type"));
	$$.body = $2.body;
	$$.type = NT_TYPE;
	}
	
|	{write_field_begin(GET_WRITER, "Type", strlen("Type")); write_struct_begin(GET_WRITER, NULL); write_field_begin(GET_WRITER, "ObjectType", strlen("ObjectType")); write_struct_begin(GET_WRITER, NULL);}
	ObjectType
	{write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "ObjectType", strlen("ObjectType")); write_struct_end(GET_WRITER, NULL); write_field_end(GET_WRITER, "Type", strlen("Type"));
	$$.body = $2.body;
	$$.type = NT_TYPE;};

ObjectType:
	tok_identifier
   	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_OBJECT;
		memcpy($$.body.sn_type.identifier, $1.var.val.bytes.ptr, $1.var.val.bytes.len);
		$$.body.sn_type.identifier[$1.var.val.bytes.len] = 0;
	};

ContainerType:
	tok_t_vector
	{	
		write_field_begin(GET_WRITER, "vector", strlen("vector"));
		write_struct_begin(GET_WRITER, NULL);
		write_struct_end(GET_WRITER, NULL);
		write_field_end(GET_WRITER, "vector", strlen("vector"));

		$$.body.sn_type.type = E_SNT_VECTOR;
	}
	
SimpleType:
	tok_t_bool
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "bool");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_BOOL;
	}
|	tok_t_char
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "char");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_CHAR;
	}
|	tok_t_double
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "double");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_DOUBLE;
	}
|	tok_t_string
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "string");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_STRING;
	}
|	tok_t_int8
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int8");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_INT8;
	}
|	tok_t_int16
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int16");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_INT16;
	}
|	tok_t_int32
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int32");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_INT32;
	}
|	tok_t_int64
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "int64");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_INT64;
	}
|	tok_t_uint8 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint8");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_UINT8;
	}
|	tok_t_uint16 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint16");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_UINT16;
	}
|	tok_t_uint32 
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint32");
		write_field_end(GET_WRITER, "type", strlen("type"));
		$$.body.sn_type.type = E_SNT_UINT32;
	}
|	tok_t_uint64
	{
		write_field_begin(GET_WRITER, "type", strlen("type"));
		write_hpstring(GET_WRITER, "uint64");
		write_field_end(GET_WRITER, "type", strlen("type"));

		$$.body.sn_type.type = E_SNT_UINT64;
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
	ParameterList ',' {write_semicolon(GET_WRITER);} Parameter 
|	
	Parameter
	
	
Parameter:
	{
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_begin(GET_WRITER, NULL);
	}
	Type {write_semicolon(GET_WRITER);}
	tok_identifier
	{
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $4.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
		write_struct_end(GET_WRITER, NULL);
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
	};




Arguments:
	'<'
	{write_vector_begin(GET_WRITER);}
	ArgumentList
	{write_vector_end(GET_WRITER);}
	'>'
|
	{
		write_null(GET_WRITER);
	};
	
ArgumentList:
	ArgumentList ',' {write_semicolon(GET_WRITER);} Argument
|	Argument;
	
Argument:
	tok_identifier
	{
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "name", strlen("name"));
		write_bytes(GET_WRITER, $1.var.val.bytes);
		write_field_end(GET_WRITER, "name", strlen("name"));
		write_struct_end(GET_WRITER, NULL);
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
	};

UnixComment:
	{write_struct_begin(GET_WRITER, NULL);}
	tok_unixcomment
	{
		write_field_begin(GET_WRITER, "text", strlen("text"));
		write_bytes(GET_WRITER, $2.var.val.bytes);
		write_field_end(GET_WRITER, "text", strlen("text"));
		write_struct_end(GET_WRITER, NULL);
	}
|
	{
		
	};

TypeAnnotations:
  '(' {write_vector_begin(GET_WRITER);}
	 TypeAnnotationList ')'
    {write_vector_end(GET_WRITER);}
|
    {
		//这里放一个默认值
		write_vector_begin(GET_WRITER);
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "switch", strlen("switch"));
		write_hpstring(GET_WRITER, "selector");
		write_field_end(GET_WRITER, "switch", strlen("switch"));		
		write_struct_end(GET_WRITER, NULL);
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
		write_vector_end(GET_WRITER);
    };

TypeAnnotationList:
  TypeAnnotationList ',' TypeAnnotation
    {
    }
| TypeAnnotation
    {
    };

Bool:
	tok_true | tok_false
	{
	};

TypeAnnotation:
	tok_unique '=' Bool
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
		write_vector_item_begin(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_begin(GET_WRITER, NULL);
		write_field_begin(GET_WRITER, "switch", strlen("switch"));
		write_bytes(GET_WRITER, $3.var.val.bytes);
		write_field_end(GET_WRITER, "switch", strlen("switch"));		
		write_vector_item_end(GET_WRITER, writer_get_index(GET_WRITER));
		write_struct_end(GET_WRITER, NULL);
	}
|	;

    
%%
