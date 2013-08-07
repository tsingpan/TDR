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
#include "hotdata_check.h"
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
%type<sn_unix_comment> UnixComment UnixCommentOrNot

%type<sn_bool> tok_bool
%type<sn_type> Type SimpleType ObjectType ContainerType
%type<sn_value> Value
%type<sn_const> Const
%type<pn_tok_double> tok_double
%type<sn_string> tok_string
%type<sn_typedef> Typedef
%type<sn_type_annotation> TypeAnnotation
%type<sn_type_annotations> TypeAnnotations TypeAnnotationList

%type<sn_st> tok_t_char tok_t_bool tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64
%type<sn_ct> tok_t_vector tok_t_string

%type<sn_arguments> Arguments ArgumentList


%type<sn_enum_def> EnumDef

%type<sn_parameter> Parameter
%type<sn_parameters> Parameters ParameterList

%type<sn_expression> FieldExpression
%type<sn_condition> Condition FieldCondition



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
		write_hpuint32(GET_WRITER, GET_SELF->definition_list_num);
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
		dp_reduce_Definition_Typedef(GET_SELF, &yylloc, &GET_DEFINITION, &$1);
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
		GET_DEFINITION.definition.de_unix_comment = $1;
	};

Import :
	tok_import
	{
		dp_reduce_Import_tok_import(GET_SELF, &yylloc, &$$, &$1);
	};

Typedef :
	tok_typedef Type tok_identifier ';'
	{
		dp_reduce_Typedef_Type_Arguments_tok_identifier(GET_SELF, &yylloc, &$$, &$2, &$3);

		dp_check_tok_identifier(GET_SELF, &yylloc, &$3);
		dp_check_Typedef(GET_SELF, &yylloc, &$$);
	};

Const :
	tok_const Type tok_identifier
	{
		dp_check_Const_tok_identifier(GET_SELF, &yylloc, &$3);
	}
	'=' Value ';'
	{
		dp_reduce_Const(GET_SELF, &yylloc, &$$, &$2, &$3, &$6);
		
		dp_check_constant_value(GET_SELF, &yylloc, &$2, &$3, &$6);
				
		dp_check_Const_add_tok_identifier(GET_SELF, &yylloc, &$3, &$6);
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
	};


Enum :
	tok_enum TypeAnnotations
	{
		GET_DEFINITION.definition.de_enum.type_annotations = $2;
	}
	tok_identifier	
	{
		memcpy(GET_DEFINITION.definition.de_enum.name, $4.ptr, $4.len);
		GET_DEFINITION.definition.de_enum.name[$4.len] = 0;
		
		dp_check_tok_identifier(GET_SELF, &yylloc, &$4);
		
		
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 0;
	}
	'{' EnumDefList '}'
	{ }
	';'
	{
		dp_check_Enum_Add(GET_SELF, &yylloc, &$4);
	};
    
EnumDefList :
	EnumDefList EnumDef
	{
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num] = $2;
		++GET_DEFINITION.definition.de_enum.enum_def_list_num;
	}
|	
	EnumDef
	{
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 0;
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num] = $1;
		++GET_DEFINITION.definition.de_enum.enum_def_list_num;
	}
	
EnumDef : 
	tok_identifier 
	{
		dp_check_EnumDef_tok_identifier(GET_SELF, &yylloc, &$1);
	}
	'=' Value ',' UnixCommentOrNot
	{	
		dp_check_EnumDef_Value(GET_SELF, &yylloc, &$4);
		
		dp_check_Const_add_tok_identifier(GET_SELF, &yylloc, &$1, &$4);		
		
		memcpy($$.identifier, $1.ptr, $1.len);
		$$.identifier[$1.len] = 0;
		$$.val = $4;
		$$.comment = $6;
	};
    

Union :
	tok_union
	{
		dp_check_Union_begin(GET_SELF, &yylloc);
	}
	TypeAnnotations 
	{
		GET_DEFINITION.definition.de_union.ta = $3;
	}
	tok_identifier
	{
		memcpy(GET_DEFINITION.definition.de_union.name, $5.ptr, $5.len);
		GET_DEFINITION.definition.de_union.name[$5.len] = 0;
		
		dp_check_tok_identifier(GET_SELF, &yylloc, &$5);
		
		dp_check_domain_begin(GET_SELF, &yylloc, &$5);
	}
	Parameters
	{
		GET_DEFINITION.definition.de_union.parameters = $7;
		
		dp_check_Union_Parameters(GET_SELF, &yylloc, &GET_DEFINITION.definition.de_union);
		
		GET_SELF->pn_field_list.field_list_num = 0;
	}
	'{' FieldList
	{
		GET_DEFINITION.definition.de_union.field_list = GET_SELF->pn_field_list;
	}
	'}'
	{
		dp_check_domain_end(GET_SELF, &yylloc);
	}
	';'
	{
		dp_check_Union_end(GET_SELF, &yylloc);

		dp_check_Union_Add(GET_SELF, &yylloc, &$5);
	};
	
	
Struct : 
	tok_struct
	{
		dp_check_Struct_begin(GET_SELF, &yylloc);
	}
	TypeAnnotations tok_identifier
	{
		dp_check_domain_begin(GET_SELF, &yylloc, &$4);
	}
	Parameters
	{
		GET_SELF->pn_field_list.field_list_num = 0;
	}
	'{' FieldList '}' ';'
	{
		dp_check_domain_end(GET_SELF, &yylloc);

		GET_DEFINITION.definition.de_struct.ta = $3;
		memcpy(GET_DEFINITION.definition.de_struct.name, $4.ptr, $4.len);
		GET_DEFINITION.definition.de_struct.name[$4.len] = 0;
		GET_DEFINITION.definition.de_struct.parameters = $6;
		GET_DEFINITION.definition.de_struct.field_list = GET_SELF->pn_field_list;

		dp_check_Struct_end(GET_SELF, &yylloc);

		dp_check_Struct_Add(GET_SELF, &yylloc, &$4);
	};
	

	
FieldList: 
	FieldList Field
	{
		GET_SELF->pn_field_list.field_list[GET_SELF->pn_field_list.field_list_num] = GET_SELF->pn_field;
		++(GET_SELF->pn_field_list.field_list_num);
	}
|	
	Field
	{
		GET_SELF->pn_field_list.field_list_num = 0;
		GET_SELF->pn_field_list.field_list[GET_SELF->pn_field_list.field_list_num] = GET_SELF->pn_field;
		++(GET_SELF->pn_field_list.field_list_num);
	};
	

Field : 
	FieldCondition Type Arguments tok_identifier
	{
		dp_check_tok_identifier_local(GET_SELF, &yylloc, &$4);
	}
	';' UnixCommentOrNot
	{
		GET_SELF->pn_field.condition = $1;
		GET_SELF->pn_field.type = $2;
		GET_SELF->pn_field.args = $3;		
		memcpy(GET_SELF->pn_field.identifier, $4.ptr, $4.len);
		GET_SELF->pn_field.identifier[$4.len] = 0;		
		GET_SELF->pn_field.comment = $7;

		dp_check_Field(GET_SELF, &yylloc, &GET_SELF->pn_field);
		dp_check_Field_add(GET_SELF, &yylloc, &GET_SELF->pn_field);
	};

FieldCondition:	
	Condition
	{
		$$ = $1;
	}	
|
	{
		$$.empty = hptrue;
	};

Condition : 
	tok_if 	'(' FieldExpression	')'	
	{
		$$.empty = hpfalse;
		$$.exp = $3;
	}
|	tok_if '!' '(' FieldExpression ')'
	{
		$$.empty = hpfalse;
		$$.exp = $4;
		$$.exp.neg = hptrue;
	}
|	tok_if '(' Value tok_unequal Value ')'
	{
		$$.empty = hpfalse;
		$$.exp.neg = hptrue;
		$$.exp.op0 = $3;
		$$.exp.oper = E_EO_EQUAL;
		$$.exp.op1 = $5;

		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$3);
		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$5);
	}
|	tok_case Value ':'
	{
		dp_reduce_Condition_tok_case(GET_SELF, &yylloc, &$$, &$2);

		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$$.exp.op0);
		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$2);
	};


FieldExpression :
	Value
	{
		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$1);
	}
	tok_equal
	Value
	{
		$$.neg = hpfalse;
		$$.op0 = $1;
		$$.oper = E_EO_EQUAL;
		$$.op1 = $4;

		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$4);
	}
|	Value
	{
		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$1);
	}
	'&'
	Value
	{
		$$.neg = hpfalse;
		$$.op0 = $1;
		$$.oper = E_EO_AND;
		$$.op1 = $4;

		dp_check_FieldExpression_Value(GET_SELF, &yylloc, &$4);
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
		$$ = $2;
	}
|
	{
		$$.par_list_num = 0;
	};
	
ParameterList:
	ParameterList ',' Parameter 
	{
		$$ = $1;
		$$.par_list[$$.par_list_num] = $3;
		++$$.par_list_num;
	}
|	
	Parameter
	{
		$$.par_list_num = 0;
		$$.par_list[$$.par_list_num] = $1;
		++$$.par_list_num;
	};
	
	
Parameter:
	Type tok_identifier
	{
		$$.type = $1;
		memcpy($$.identifier, $2.ptr, $2.len);
		$$.identifier[$2.len] = 0;
		
		dp_check_tok_identifier_local(GET_SELF, &yylloc, &$2);
		
		dp_check_Parameter_add(GET_SELF, &yylloc, &$$);
	};




Arguments:
	'<' ArgumentList '>'
	{
		$$ = $2;
	}
|
	{
		$$.arg_list_num = 0;
	};
	
ArgumentList:
	ArgumentList ',' Type
	{
		dp_reduce_ArgumentList_ArgumentList_Type(GET_SELF, &yylloc, &$$, &$1, &$3);
	}
|	Type
	{
		dp_reduce_ArgumentList_Type(GET_SELF, &yylloc, &$$, &$1);
	};

UnixComment:
	tok_unixcomment
	{
		$$.empty = hpfalse;
		strncpy($$.text, $1, MAX_COMMENT_LENGTH);
	};

UnixCommentOrNot:
	tok_unixcomment
	{
		$$.empty = hpfalse;
		strncpy($$.text, $1, MAX_COMMENT_LENGTH);
	}
|
	{
		$$.empty = hptrue;
	};

TypeAnnotations:
	'('	TypeAnnotationList	')'
	{
		$$ = $2;
	}
|
    {
		$$.ta_list_num = 0;
    };

TypeAnnotationList:
  TypeAnnotationList ',' TypeAnnotation
    {
		$$ = $1;
		$$.ta_list[$$.ta_list_num] = $3;
		++$$.ta_list_num;
    }
| TypeAnnotation
    {
		$$.ta_list_num = 0;
		$$.ta_list[$$.ta_list_num] = $1;
		++$$.ta_list_num;
    };

TypeAnnotation:
	tok_unique '=' Value
    {
		dp_check_TypeAnnotation_tok_unique_Value(GET_SELF, &yylloc, &$3);
    
		$$.type = E_TA_UNIQUE;
		$$.val = $3;
    }
|	tok_lower_bound '=' Value
	{
		dp_check_TypeAnnotation_bound_Value(GET_SELF, &yylloc, &$3);
		
		$$.type = E_TA_LOWER_BOUND;
		$$.val = $3;
	}
|	tok_upper_bound '=' Value
	{
		dp_check_TypeAnnotation_bound_Value(GET_SELF, &yylloc, &$3);
		
		$$.type = E_TA_UPPER_BOUND;
		$$.val = $3;
	}
|	tok_switch '=' Value
	{
		dp_check_TypeAnnotation_tok_switch_Value(GET_SELF, &yylloc, &$3);
	
		$$.type = E_TA_SWITCH;
		$$.val = $3;
	};

    
%%
