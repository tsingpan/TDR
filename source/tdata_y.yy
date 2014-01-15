%define api.pure
%parse-param { SCANNER *self }
%pure_parser
%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>
#include <string.h>

#define YYERROR_VERBOSE
#define YYLEX_PARAM self


#define GET_SELF TLIBC_CONTAINER_OF(YYLEX_PARAM, PARSER, scanner)
#define GET_DEFINITION GET_SELF->pn_definition
#define GET_UNION_FIELD_LIST GET_DEFINITION.definition.de_union.union_field_list
#define GET_FIELD_LIST GET_DEFINITION.definition.de_struct.field_list
%}
%locations

%code requires
{
#include "parse/parser.h"
#include "parse/scanner.h"

#include "parse/check.h"
#include "parse/reduce.h"
#include "symbols.h"
}

%token tok_import 
%token tok_struct
%token tok_union 
%token tok_if 
%token tok_reserved_keyword

%token tok_int
%token tok_hex
%token tok_int64
%token tok_uint64
%token tok_hex_int64
%token tok_hex_uint64
%token tok_double
%token tok_string
%token tok_char


%token tok_identifier 
%token tok_const 
%token tok_enum 
%token tok_equal
%token tok_unequal
%token tok_count
%token tok_unixcomment
%token tok_typedef
%token tok_t_int8
%token tok_t_int16
%token tok_t_int32
%token tok_t_int64
%token tok_t_uint8
%token tok_t_uint16
%token tok_t_uint32
%token tok_t_uint64
%token tok_t_char
%token tok_t_double
%token tok_t_string
%token tok_t_vector


%type<sn_definition> Definition
%type<sn_import> Import

%type<sn_tok_identifier> tok_identifier
%type<sn_int64> tok_int64
%type<sn_hex_int64> tok_hex_int64
%type<sn_uint64> tok_uint64
%type<sn_hex_uint64> tok_hex_uint64
%type<sn_char> tok_char
%type<sn_tok_unixcomment> tok_unixcomment
%type<sn_unix_comment> UnixComment UnixCommentOrNot

%type<sn_simple_type> SimpleType
%type<sn_type> Type ContainerType
%type<sn_value> Value
%type<sn_const> Const
%type<pn_tok_double> tok_double
%type<sn_string> tok_string
%type<sn_typedef> Typedef

%type<sn_st> tok_t_char tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64 tok_t_string
%type<sn_ct> tok_t_vector

%type<sn_arguments> Arguments ArgumentList


%type<sn_enum_def> EnumDef
%type<sn_union_field> UnionField
%type<sn_field> Field

%type<sn_parameter> Parameter
%type<sn_parameters> Parameters ParameterList

%type<sn_condition> Condition



%left '='


%start Document

%%

Document : DefinitionList;

DefinitionList :
	DefinitionList Definition
	{
		parser_on_definition(GET_SELF, &yylloc, &GET_DEFINITION);
	}
|	{
	};

Definition :
	Import
	{
		GET_DEFINITION.type = E_DT_IMPORT;
		GET_DEFINITION.definition.de_import = $1;
	}
|	Const
	{
		GET_DEFINITION.type = E_DT_CONST;
		GET_DEFINITION.definition.de_const = $1;
	}
|	Typedef
	{
		GET_DEFINITION.type = E_DT_TYPEDEF;
		GET_DEFINITION.definition.de_typedef = $1;
	}
|	Struct
	{
		GET_DEFINITION.type = E_DT_STRUCT;
	}
|	Union
	{
		GET_DEFINITION.type = E_DT_UNION;
	}
|	Enum
	{
		GET_DEFINITION.type = E_DT_ENUM;
	}
|	UnixComment
	{
		GET_DEFINITION.type = E_DT_UNIX_COMMENT;
		GET_DEFINITION.definition.de_unix_comment = $1;
	};

Import :
	tok_import tok_string
	{
		check_strlen_too_long(&GET_SELF->symbols, &yylloc, $2, "", MAX_PACKAGE_NAME_LENGTH);
		

		dp_reduce_Import(GET_SELF, &$$, $2);
	};

Typedef :
	tok_typedef SimpleType tok_identifier ';'
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $3);

		dp_reduce_Typedef(GET_SELF, &$$, &$2, $3);

		symbols_add_Typedef(&GET_SELF->symbols, &$$);
	};

Const :
	tok_const SimpleType tok_identifier '=' Value ';'
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $3);

		check_value_type(&GET_SELF->symbols, &yylloc, &$2, &$5);


		dp_reduce_Const(GET_SELF, &$$, &$2, $3, &$5);		
		symbols_add_Const(&GET_SELF->symbols, &$$);
	};

Enum :
	tok_enum tok_identifier	
	{
		GET_SELF->symbols.enum_name = $2;
	}
	'{' EnumDefList '}'	';'
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $2);

		dp_reduce_Enum(GET_SELF, &GET_DEFINITION.definition.de_enum, $2);		

		symbols_add_Enum(&GET_SELF->symbols, &GET_DEFINITION.definition.de_enum);
	};

EnumDefList :
	EnumDefList EnumDef
	{
		if(GET_DEFINITION.definition.de_enum.enum_def_list_num >= MAX_ENUM_DEF_LIST_NUM)
		{
			scanner_error(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_ENUM_DEF_LIST_NUM);
		}
		
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num++] = $2;
	}
|	
	EnumDef
	{
		GET_DEFINITION.definition.de_enum.enum_def_list[0] = $1;
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 1;
	};
	
EnumDef : 
	tok_identifier '=' Value ',' UnixCommentOrNot
	{
		ST_SIMPLE_TYPE enum_type;
		enum_type.st = E_ST_INT32;

		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $1);
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.enum_name, $1);

		
		check_value_type(&GET_SELF->symbols, &yylloc, &enum_type, &$3);

		dp_reduce_EnumDef(GET_SELF, &$$, $1, &$3, &$5);

		symbols_add_EnumDef(&GET_SELF->symbols, &$$);
	};

Union :
	tok_union tok_identifier
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $2);
		GET_SELF->symbols.union_name = $2;
	}
	Parameters
	{
		symbols_add_Parameters(&GET_SELF->symbols, &$4, $2);
	}
	'{' UnionFieldList '}' ';'
	{
		dp_reduce_Union(GET_SELF, &GET_DEFINITION.definition.de_union, $2, &$4);

		symbols_add_Union(&GET_SELF->symbols, &GET_DEFINITION.definition.de_union);
	};
	
UnionFieldList: 
	UnionFieldList UnionField
	{
		if(GET_UNION_FIELD_LIST.union_field_list_num >= MAX_UNION_FIELD_LIST_NUM)
		{
			scanner_error(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_UNION_FIELD_LIST_NUM);
		}

		GET_UNION_FIELD_LIST.union_field_list[GET_UNION_FIELD_LIST.union_field_list_num++] = $2;
	}
|	
	UnionField
	{
		GET_UNION_FIELD_LIST.union_field_list[0] = $1;
		GET_UNION_FIELD_LIST.union_field_list_num = 1;
	};
	
UnionField : 
	tok_identifier ':' SimpleType tok_identifier ';' UnixCommentOrNot
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.union_name, $1);

		check_string_length_defined(&GET_SELF->symbols, &yylloc, &$3);

		dp_reduce_UnionField(GET_SELF, &$$, $1, &$3, $4, &$6);

		symbols_add_UnionField(&GET_SELF->symbols, &$$);
	};

	Parameters :
	'(' ParameterList ')'
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
		if($1.par_list_num >= MAX_PARAMETER_NUM)
		{
			scanner_error(&yylloc, E_LS_TOO_MANY_PARAMETERS, MAX_PARAMETER_NUM);
		}
		$$ = $1;
		$$.par_list[$$.par_list_num++] = $3;
	}
|	
	Parameter
	{
		$$.par_list[0] = $1;
		$$.par_list_num = 1;
	};

Parameter:
	SimpleType tok_identifier
	{
		check_str_equal(&GET_SELF->symbols, &yylloc, $2, "selector");

		check_simpletype_is_enum_with_unique(&GET_SELF->symbols, &yylloc, &$1);

		$$.type = $1;
		strncpy($$.identifier, $2, TLIBC_MAX_IDENTIFIER_LENGTH - 1);
		$$.identifier[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	};






	
Struct : 
	tok_struct tok_identifier
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, "", $2);

		GET_SELF->symbols.struct_name = $2;
	}
	'{' FieldList '}' ';'
	{
		strncpy(GET_DEFINITION.definition.de_struct.name, $2, TLIBC_MAX_IDENTIFIER_LENGTH);
		GET_DEFINITION.definition.de_struct.name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;

		symbols_add_Struct(&GET_SELF->symbols, &GET_DEFINITION.definition.de_struct);
	};

FieldList: 
	FieldList Field
	{
		if(GET_FIELD_LIST.field_list_num >= MAX_FIELD_LIST_NUM)
		{
			scanner_error(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_FIELD_LIST_NUM);
		}

		GET_FIELD_LIST.field_list[GET_FIELD_LIST.field_list_num++] = $2;
	}
|	
	Field
	{
		GET_FIELD_LIST.field_list[0] = $1;
		GET_FIELD_LIST.field_list_num = 1;
	};

Field : 
	Condition Type tok_identifier Arguments	';' UnixCommentOrNot
	{
		check_identifier_not_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		if(($2.type == E_SNT_CONTAINER) && ($2.ct.ct == E_CT_VECTOR))
		{
			check_strlen_too_long(&GET_SELF->symbols, &yylloc, $3, "_num", TLIBC_MAX_IDENTIFIER_LENGTH);
		}
		

		if($2.type == E_SNT_SIMPLE)
		{
			check_string_length_defined(&GET_SELF->symbols, &yylloc, &$2.st);
		}

		check_arguments(&GET_SELF->symbols, &yylloc, &$2, &$4);

		dp_reduce_Field(GET_SELF, &$$, &$1, &$2, $3, &$4, &$6);		
		
		symbols_add_Field(&GET_SELF->symbols, &$$);
	};
	
Condition : 
	tok_if 	'(' tok_identifier '&' Value	')'	
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_integer_value(&GET_SELF->symbols, &yylloc, &$5);

		strncpy($$.op0, $3, TLIBC_MAX_IDENTIFIER_LENGTH);
		$$.op0[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;

		$$.oper = E_EO_AND;
		$$.op1 = $5;
	}
|	tok_if 	'(' tok_identifier tok_equal Value	')'	
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_integer_value(&GET_SELF->symbols, &yylloc, &$5);


		strncpy($$.op0, $3, TLIBC_MAX_IDENTIFIER_LENGTH);
		$$.op0[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;

		$$.oper = E_EO_EQUAL;
		$$.op1 = $5;
	}
|	tok_if '(' tok_identifier tok_unequal Value ')'
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $3);

		check_integer_value(&GET_SELF->symbols, &yylloc, &$5);


		strncpy($$.op0, $3, TLIBC_MAX_IDENTIFIER_LENGTH);
		$$.op0[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;

		$$.oper = E_EO_UNEQUAL;
		$$.op1 = $5;
	}
|
	{
		$$.oper = E_EO_NON;
	};




Type :
	SimpleType
	{
		dp_reduce_Type_SimpleType(GET_SELF, &$$, &$1);
	}
|	ContainerType
	{
		$$ = $1;
	}	

ContainerType:
	tok_t_vector '<' SimpleType ',' tok_identifier '>'
	{
		check_identifier_is_positive_integer(&GET_SELF->symbols, &yylloc, "", $5);

		check_string_length_defined(&GET_SELF->symbols, &yylloc, &$3);

		dp_reduce_ContainerType_tok_t_vector(GET_SELF, &$$, &$3, $5);
	};
	
SimpleType:
	tok_t_char
	{
		$$.st = $1;
	}
|	tok_t_double
	{
		$$.st = $1;
	}
|	tok_t_int8
	{
		$$.st = $1;
	}
|	tok_t_int16
	{
		$$.st = $1;
	}
|	tok_t_int32
	{
		$$.st = $1;
	}
|	tok_t_int64
	{
		$$.st = $1;
	}
|	tok_t_uint8 
	{
		$$.st = $1;
	}
|	tok_t_uint16 
	{
		$$.st = $1;
	}
|	tok_t_uint32 
	{
		$$.st = $1;
	}
|	tok_t_uint64
	{
		$$.st = $1;
	}
|	tok_identifier
   	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, "", $1);

		check_identifier_is_type(&GET_SELF->symbols, &yylloc, "", $1);

		dp_reduce_SimpleType_tok_identifier(GET_SELF, &$$, $1);
	}
|	tok_t_string '<' tok_identifier '>'
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, "", $3);

		check_identifier_is_positive_integer(&GET_SELF->symbols, &yylloc, "", $3);

		dp_reduce_SimpleType_tok_t_string(GET_SELF, &$$, $3);
	}
|	tok_t_string
	{
		dp_reduce_SimpleType_tok_t_string(GET_SELF, &$$, NULL);
	};


Arguments:
	'(' ArgumentList ')'
	{
		$$ = $2;
	}
|
	{
		$$.arg_list_num = 0;
	};
	
ArgumentList:
	ArgumentList ',' tok_identifier
	{
		if($1.arg_list_num >= MAX_ARGUMENT_NUM)
		{
			scanner_error(&yylloc, E_LS_TOO_MANY_ARGUMENTS, MAX_ARGUMENT_NUM);
		}

		dp_reduce_ArgumentList_ArgumentList_tok_identifier(GET_SELF, &$$, &$1, $3);
	}
|	tok_identifier
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, GET_SELF->symbols.struct_name, $1);

		dp_reduce_ArgumentList_tok_identifier(GET_SELF, &$$, $1);
	};

Value :
	tok_uint64
	{
		dp_reduce_Value_tok_uint64(GET_SELF, &$$, $1);
	}
|	tok_hex_uint64
	{
		dp_reduce_Value_tok_hex_uint64(GET_SELF, &$$, $1);
	}
|	tok_int64
	{
		dp_reduce_Value_tok_int64(GET_SELF, &$$, $1);
	}
|	tok_hex_int64
	{
		dp_reduce_Value_tok_hex_int64(GET_SELF, &$$, $1);
	}
|	tok_double
	{
		dp_reduce_Value_tok_double(GET_SELF, &$$, $1);
	}
|	tok_string
	{
		dp_reduce_Value_tok_string(GET_SELF, &$$, $1);
	}
|	tok_char
	{
		dp_reduce_Value_tok_char(GET_SELF, &$$, $1);
	}
|	tok_identifier
	{
		check_identifier_defined(&GET_SELF->symbols, &yylloc, "", $1);

		check_identifier_is_value(&GET_SELF->symbols, &yylloc, "", $1);

		dp_reduce_Value_tok_identifier(GET_SELF, &$$, $1);
	}
|	tok_count '(' tok_identifier ')'
	{
		dp_reduce_Value_tok_count(GET_SELF, &$$, $3);
	};

UnixComment:
	tok_unixcomment
	{
		strncpy($$.text, $1, TD_MAX_COMMENT_LENGTH);
		$$.text[TD_MAX_COMMENT_LENGTH - 1] = 0;
	};

UnixCommentOrNot:
	tok_unixcomment
	{
		strncpy($$.text, $1, TD_MAX_COMMENT_LENGTH);
		$$.text[TD_MAX_COMMENT_LENGTH - 1] = 0;
	}
|
	{
		$$.text[0] = 0;
	};
    
%%
