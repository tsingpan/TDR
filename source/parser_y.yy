%define api.pure
%parse-param { scanner_t *self }
%pure_parser
%{
//bison生成的代码会有几个warning, 在这里屏蔽掉
#ifdef _WIN32
#pragma warning (disable: 4244)
#pragma warning (disable: 4702)
#pragma warning (disable: 4127)
#endif

//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>
#include <string.h>

#define YYERROR_VERBOSE
#define YYLEX_PARAM self

#define GET_PARSER TDR_CONTAINER_OF(YYLEX_PARAM, PARSER, scanner)
#define GET_SYMBOLS GET_PARSER->symbols
#define GET_DEFINITION GET_PARSER->pn_definition


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
%token tok_true
%token tok_false
%token tok_bool


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
%token tok_t_bool

%type<sn_definition> Definition
%type<sn_import> Import

%type<sn_tok_identifier> tok_identifier
%type<sn_int64> tok_int64
%type<sn_hex_int64> tok_hex_int64
%type<sn_uint64> tok_uint64
%type<sn_hex_uint64> tok_hex_uint64
%type<sn_char> tok_char
%type<sn_bool> tok_true tok_false tok_bool
%type<sn_tok_unixcomment> tok_unixcomment
%type<sn_unix_comment> UnixComment UnixCommentOrNot

%type<sn_simple_type> SimpleType
%type<sn_type> Type ContainerType
%type<sn_value> Value
%type<sn_const> Const
%type<pn_tok_double> tok_double
%type<sn_string> tok_string
%type<sn_typedef> Typedef

%type<sn_st> tok_t_bool tok_t_char tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64 tok_t_string
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
		parser_on_generator_definition(GET_PARSER, &yylloc, &GET_DEFINITION);
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
		check_strlen_too_long(&yylloc, $2, "", MAX_PACKAGE_NAME_LENGTH);
		

		reduce_Import(&$$, $2);

		parser_on_import(GET_PARSER, &$$);
	};

Typedef :
	tok_typedef SimpleType tok_identifier ';'
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $3);
		check_string_length(&yylloc, &$2, TRUE);

		reduce_Typedef(&$$, &$2, $3);

		symbols_add_Typedef(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_typedef(GET_PARSER, &$$);
	};

Const :
	tok_const SimpleType tok_identifier '=' Value ';'
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $3);

		check_string_length(&yylloc, &$2, FALSE);

		check_value_type(&GET_SYMBOLS, &yylloc, &$2, &$5);


		reduce_Const(&$$, &$2, $3, &$5);		
		symbols_add_Const(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_const(GET_PARSER, &$$);
	};

Enum :
	tok_enum tok_identifier	
	{
		GET_SYMBOLS.enum_name = $2;
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 0;

		parser_on_enum_begin(GET_PARSER, $2);
	}
	'{' EnumDefList '}'	';'
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $2);

		reduce_Enum(&GET_DEFINITION.definition.de_enum, $2);

		symbols_add_Enum(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_enum);

		parser_on_enum_end(GET_PARSER, $2);
	};

EnumDefList :
	EnumDefList EnumDef
	{
		if(GET_DEFINITION.definition.de_enum.enum_def_list_num >= MAX_ENUM_DEF_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_ENUM_DEF_LIST_NUM);
		}
		
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num] = $2;
		check_enumdef_is_unique(&yylloc, &GET_DEFINITION.definition.de_enum, GET_DEFINITION.definition.de_enum.enum_def_list_num);
		++GET_DEFINITION.definition.de_enum.enum_def_list_num;
	}
|	
	EnumDef
	{
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num++] = $1;
	};
	
EnumDef : 
	tok_identifier '=' Value ',' UnixCommentOrNot
	{
		syn_simple_type_t enum_type;
		enum_type.st = E_ST_INT32;

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $1);
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.enum_name, $1);
				
		check_value_type(&GET_SYMBOLS, &yylloc, &enum_type, &$3);

		reduce_EnumDef_Value(&$$, $1, &$3, &$5);

		symbols_add_EnumDef(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_enum_field(GET_PARSER, &$$);
	}
|	tok_identifier ',' UnixCommentOrNot
	{
		syn_simple_type_t enum_type;
		enum_type.st = E_ST_INT32;

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $1);
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.enum_name, $1);
		
		if(GET_DEFINITION.definition.de_enum.enum_def_list_num == 0)
		{
			reduce_EnumDef(&$$, $1, NULL, &$3);
		}
		else
		{
			reduce_EnumDef(&$$, $1, 
			&GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num - 1].val, &$3);
		}

		symbols_add_EnumDef(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_enum_field(GET_PARSER, &$$);
	};

Union :
	tok_union tok_identifier Parameters 
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $2);
		GET_SYMBOLS.union_name = $2;
		GET_UNION_FIELD_LIST.union_field_list_num = 0;

		parser_on_union_begin(GET_PARSER, $2, $3.par_list[0].type.st_refer);
	}
	'{' UnionFieldList '}' ';'
	{
		reduce_Union(&GET_DEFINITION.definition.de_union, $2, &$3);

		symbols_add_Union(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_union);

		parser_on_union_end(GET_PARSER, $2);
	};
	
UnionFieldList: 
	UnionFieldList UnionField
	{
		if(GET_UNION_FIELD_LIST.union_field_list_num >= MAX_UNION_FIELD_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_UNION_FIELD_LIST_NUM);
		}

		GET_UNION_FIELD_LIST.union_field_list[GET_UNION_FIELD_LIST.union_field_list_num++] = $2;
	}
|	
	UnionField
	{
		GET_UNION_FIELD_LIST.union_field_list[GET_UNION_FIELD_LIST.union_field_list_num++] = $1;
	};
	
UnionField : 
	tok_identifier ':' SimpleType tok_identifier ';' UnixCommentOrNot
	{
		check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", $4);

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.union_name, $1);

		check_string_length_defined(&yylloc, &$3);

		reduce_UnionField(&$$, $1, &$3, $4, &$6);

		symbols_add_UnionField(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_union_field(GET_PARSER, &$$);
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
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_PARAMETERS, MAX_PARAMETER_NUM);
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
		check_str_equal(&yylloc, $2, "selector");

		check_simpletype_is_enum(&GET_SYMBOLS, &yylloc, &$1);

		$$.type = $1;
		strncpy($$.identifier, $2, TDR_MAX_LENGTH_OF_IDENTIFIER - 1);
		$$.identifier[TDR_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;
	};






	
Struct : 
	tok_struct tok_identifier
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", $2);

		GET_SYMBOLS.struct_name = $2;
		GET_FIELD_LIST.field_list_num = 0;

		parser_on_struct_begin(GET_PARSER, $2);
	}
	'{' FieldList '}' ';'
	{
		strncpy(GET_DEFINITION.definition.de_struct.name, $2, TDR_MAX_LENGTH_OF_IDENTIFIER);
		GET_DEFINITION.definition.de_struct.name[TDR_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		symbols_add_Struct(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_struct);

		parser_on_struct_end(GET_PARSER, $2);
	};

FieldList: 
	FieldList Field
	{
		if(GET_FIELD_LIST.field_list_num >= MAX_FIELD_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_FIELD_LIST_NUM);
		}

		GET_FIELD_LIST.field_list[GET_FIELD_LIST.field_list_num++] = $2;
	}
|	
	Field
	{
		GET_FIELD_LIST.field_list[GET_FIELD_LIST.field_list_num++] = $1;
	};

Field : 
	Condition Type tok_identifier Arguments	';' UnixCommentOrNot
	{
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", $3);
		

		if(($2.type == E_SNT_CONTAINER) && ($2.ct.ct == E_CT_VECTOR))
		{
			char name[TDR_MAX_LENGTH_OF_IDENTIFIER];
			check_strlen_too_long(&yylloc, $3, "_num", TDR_MAX_LENGTH_OF_IDENTIFIER);
			snprintf(name, TDR_MAX_LENGTH_OF_IDENTIFIER, "%s_num", $3);

			check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, name);

			check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", name);
		}
		

		if($2.type == E_SNT_SIMPLE)
		{
			check_string_length_defined(&yylloc, &$2.st);
		}

		check_arguments(&GET_SYMBOLS, &yylloc, &$2, &$4);

		reduce_Field(&$$, &$1, &$2, $3, &$4, &$6);		
		
		symbols_add_Field(&GET_SYMBOLS, &yylloc, &$$);

		parser_on_struct_field(GET_PARSER, &$$);
	};
	
Condition : 
	tok_if 	'(' tok_identifier '&' Value	')'	
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_integer_value(&GET_SYMBOLS, &yylloc, &$5);

		strncpy($$.op0, $3, TDR_MAX_LENGTH_OF_IDENTIFIER);
		$$.op0[TDR_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		$$.oper = E_EO_AND;
		$$.op1 = $5;
	}
|	tok_if 	'(' tok_identifier tok_equal Value	')'	
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_integer_value(&GET_SYMBOLS, &yylloc, &$5);


		strncpy($$.op0, $3, TDR_MAX_LENGTH_OF_IDENTIFIER);
		$$.op0[TDR_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		$$.oper = E_EO_EQUAL;
		$$.op1 = $5;
	}
|	tok_if '(' tok_identifier tok_unequal Value ')'
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $3);

		check_integer_value(&GET_SYMBOLS, &yylloc, &$5);


		strncpy($$.op0, $3, TDR_MAX_LENGTH_OF_IDENTIFIER);
		$$.op0[TDR_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

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
		reduce_Type_SimpleType(&$$, &$1);
	}
|	ContainerType
	{
		$$ = $1;
	}	

ContainerType:
	tok_t_vector '<' SimpleType ',' tok_identifier '>'
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", $5);

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", $5);

		check_identifier_is_non_zero_positive_integer(&GET_SYMBOLS, &yylloc, "", $5);

		check_string_length_defined(&yylloc, &$3);

		reduce_ContainerType_tok_t_vector(&$$, &$3, $5);
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
|	tok_t_bool
	{
		$$.st = $1;
	}
|	tok_identifier
   	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", $1);

		check_identifier_is_type(&GET_SYMBOLS, &yylloc, "", $1);

		reduce_SimpleType_tok_identifier(&$$, $1);
	}
|	tok_t_string '<' tok_identifier '>'
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", $3);

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", $3);

		check_identifier_is_non_zero_positive_integer(&GET_SYMBOLS, &yylloc, "", $3);

		reduce_SimpleType_tok_t_string(&$$, $3);
	}
|	tok_t_string
	{
		reduce_SimpleType_tok_t_string(&$$, NULL);
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
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_ARGUMENTS, MAX_ARGUMENT_NUM);
		}

		reduce_ArgumentList_ArgumentList_tok_identifier(&$$, &$1, $3);
	}
|	tok_identifier
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, $1);

		reduce_ArgumentList_tok_identifier(&$$, $1);
	};

Value :
	tok_uint64
	{
		reduce_Value_tok_uint64(&$$, $1);
	}
|	tok_hex_uint64
	{
		reduce_Value_tok_hex_uint64(&$$, $1);
	}
|	tok_int64
	{
		reduce_Value_tok_int64(&$$, $1);
	}
|	tok_hex_int64
	{
		reduce_Value_tok_hex_int64(&$$, $1);
	}
|	tok_double
	{
		reduce_Value_tok_double(&$$, $1);
	}
|	tok_string
	{
		reduce_Value_tok_string(&$$, $1);
	}
|	tok_char
	{
		reduce_Value_tok_char(&$$, $1);
	}
|	tok_bool
	{
		reduce_Value_tok_bool(&$$, $1);
	}
|	tok_identifier
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", $1);

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", $1);

		reduce_Value_tok_identifier(&$$, $1);
	}
|	tok_count '(' tok_identifier ')'
	{
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", $3);

		check_identifier_is_type(&GET_SYMBOLS, &yylloc, "", $3);

		reduce_Value_tok_count(&GET_SYMBOLS, &$$, $3);
	};

UnixComment:
	tok_unixcomment
	{
		strncpy($$.text, $1, MAX_COMMENT_LENGTH);
		$$.text[MAX_COMMENT_LENGTH - 1] = 0;

		parser_on_unit_comment(GET_PARSER, &$$);
	};

UnixCommentOrNot:
	tok_unixcomment
	{
		strncpy($$.text, $1, MAX_COMMENT_LENGTH);
		$$.text[MAX_COMMENT_LENGTH - 1] = 0;
	}
|
	{
		$$.text[0] = 0;
	};
    
%%
