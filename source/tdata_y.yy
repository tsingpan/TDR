%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>


#define YYERROR_VERBOSE
#define GET_SELF TLIBC_CONTAINER_OF(ss, PARSER, scanner_stack)
#define GET_DEFINITION GET_SELF->pn_definition
#define YYLEX_PARAM ss
%}
%locations

%code requires
{

#include "parse/scanner.h"
#include "parse/check.h"
#include "parse/parser.h"
#include "parse/reduce.h"
#define YYSTYPE PARSER_VALUE

#include <string.h>

int tdatalex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss);
void tdataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...);
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
%token tok_count
%token tok_case
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
%token tok_t_bool
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

%type<sn_bool> tok_bool
%type<sn_simple_type> SimpleType
%type<sn_type> Type ContainerType
%type<sn_value> Value Function
%type<sn_const> Const
%type<pn_tok_double> tok_double
%type<sn_string> tok_string
%type<sn_typedef> Typedef

%type<sn_st> tok_t_char tok_t_bool tok_t_double tok_t_int8 tok_t_int16 tok_t_int32 tok_t_int64 tok_t_uint8 tok_t_uint16 tok_t_uint32 tok_t_uint64
%type<sn_ct> tok_t_vector tok_t_string

%type<sn_arguments> Arguments ArgumentList


%type<sn_enum_def> EnumDef

%type<sn_parameter> Parameter
%type<sn_parameters> Parameters ParameterList

%type<sn_condition> Condition FieldCondition



%left '='


%start Document

%%

Document :
	{
		tuint32 i;
		for(i = 0; i < GET_SELF->generator_num; ++i)
		{
			generator_on_document_begin(GET_SELF->generator_list[i], GET_SELF->file_name);
		}
	}
	DefinitionList
	{
		tuint32 i;
		for(i = 0; i < GET_SELF->generator_num; ++i)
		{
			generator_on_document_end(GET_SELF->generator_list[i], GET_SELF->file_name);
		}
	};

DefinitionList :
	DefinitionList Definition
	{
		if(scanner_stack_get_num(&GET_SELF->scanner_stack) == 1)
		{
			tuint32 i;
			for(i = 0; i < GET_SELF->generator_num; ++i)
			{
				generator_on_definition(GET_SELF->generator_list[i], &GET_DEFINITION);
			}
		}		
		
		parser_on_definition(GET_SELF, &yylloc, &GET_DEFINITION);
	}
|	{
	};

Definition :
	Import
	{
		dp_reduce_Definition_Import(GET_SELF, &GET_DEFINITION, &$1);
	}
|	Const
	{
		dp_reduce_Definition_Const(GET_SELF, &GET_DEFINITION, &$1);
	}
| Typedef
	{
		dp_reduce_Definition_Typedef(GET_SELF, &GET_DEFINITION, &$1);
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
	tok_import tok_string
	{
		dp_reduce_Import_tok_string(GET_SELF, &$$, $2);
	};

Typedef :
	tok_typedef Type tok_identifier ';'
	{
		dp_reduce_Typedef_Type_Arguments_tok_identifier(GET_SELF, &$$, &$2, &$3);

		dp_check_tok_identifier(GET_SELF, &yylloc, &$3);
		dp_check_Typedef(GET_SELF, &yylloc, &$$);
	};

Const :
	tok_const Type tok_identifier '=' Value ';'
	{
		dp_check_Const(GET_SELF, &yylloc, &$$, &$2, &$3, &$5);

		dp_reduce_Const(GET_SELF, &$$, &$2, &$3, &$5);
		
		symbols_add_Const(&GET_SELF->symbols, &$$);
	}



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
|	tok_bool
	{
		dp_reduce_Value_tok_bool(GET_SELF, &$$, $1);
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
		dp_reduce_Value_tok_identifier(GET_SELF, &$$, $1);
	}
|	Function
	{
		$$ = $1;
	}
;

Function:
	tok_count '(' tok_identifier ')'
	{
		dp_reduce_Function_tok_count(GET_SELF, &$$, &$3);
	};

Enum :
	tok_enum tok_identifier	
	{
		memcpy(GET_DEFINITION.definition.de_enum.name, $2.ptr, $2.len);
		GET_DEFINITION.definition.de_enum.name[$2.len] = 0;
		
		dp_check_tok_identifier(GET_SELF, &yylloc, &$2);
		
		
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 0;
	}
	'{' EnumDefList '}'
	{ }
	';'
	{
		dp_check_Enum_Add(GET_SELF, &yylloc, &$2, &GET_DEFINITION.definition.de_enum);
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
//		dp_check_Const_add_tok_identifier(GET_SELF, &yylloc, &$1, &$4);		
		
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
	tok_identifier
	{
		memcpy(GET_DEFINITION.definition.de_union.name, $3.ptr, $3.len);
		GET_DEFINITION.definition.de_union.name[$3.len] = 0;
		
		dp_check_tok_identifier(GET_SELF, &yylloc, &$3);
		
		symbols_domain_begin(&GET_SELF->symbols, &$3);
	}
	Parameters
	{
		GET_DEFINITION.definition.de_union.parameters = $5;
		
		dp_check_Union_Parameters(GET_SELF, &yylloc, &GET_DEFINITION.definition.de_union);
		
		GET_SELF->pn_field_list.field_list_num = 0;
	}
	'{' FieldList
	{
		GET_DEFINITION.definition.de_union.field_list = GET_SELF->pn_field_list;
	}
	'}'
	{
		symbols_domain_end(&GET_SELF->symbols);
	}
	';'
	{
		dp_check_Union_end(GET_SELF, &yylloc);

		dp_check_Union_Add(GET_SELF, &yylloc, &GET_DEFINITION.definition.de_union);
	};
	
	
Struct : 
	tok_struct
	{
		dp_check_Struct_begin(GET_SELF, &yylloc);
	}
	tok_identifier
	{
		symbols_domain_begin(&GET_SELF->symbols, &$3);
	}
	Parameters
	{
		GET_SELF->pn_field_list.field_list_num = 0;
	}
	'{' FieldList '}' ';'
	{
		symbols_domain_end(&GET_SELF->symbols);

		memcpy(GET_DEFINITION.definition.de_struct.name, $3.ptr, $3.len);
		GET_DEFINITION.definition.de_struct.name[$3.len] = 0;
		GET_DEFINITION.definition.de_struct.parameters = $5;
		GET_DEFINITION.definition.de_struct.field_list = GET_SELF->pn_field_list;

		dp_check_Struct_end(GET_SELF, &yylloc);

		dp_check_Struct_Add(GET_SELF, &yylloc, &GET_DEFINITION.definition.de_struct);
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
	FieldCondition Type tok_identifier Arguments
	{
		dp_check_tok_identifier_local(GET_SELF, &yylloc, &$3);
	}
	';' UnixCommentOrNot
	{
		GET_SELF->pn_field.condition = $1;
		GET_SELF->pn_field.type = $2;
		
		memcpy(GET_SELF->pn_field.identifier, $3.ptr, $3.len);
		GET_SELF->pn_field.identifier[$3.len] = 0;
		GET_SELF->pn_field.args = $4;
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
		$$.oper = E_EO_NON;
	};

//这里要检查Value， 保证只能是整数, 还得有符号无符号匹配
Condition : 
	tok_if 	'(' Value '&' Value	')'	
	{
		$$.op0 = $3;
		$$.oper = E_EO_AND;
		$$.op1 = $5;
	}
|	tok_if 	'(' Value tok_equal Value	')'	
	{
		$$.op0 = $3;
		$$.oper = E_EO_EQUAL;
		$$.op1 = $5;
	}
|	tok_if '(' Value tok_unequal Value ')'
	{
		$$.op0 = $3;
		$$.oper = E_EO_UNEQUAL;
		$$.op1 = $5;
	}
|	tok_case Value ':'
	{
		dp_reduce_Condition_tok_case(GET_SELF, &$$, &$2);
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
		dp_reduce_ContainerType_tok_t_vector(GET_SELF, &$$, &$3, &$5);
	}
|	tok_t_string '<' tok_identifier '>'
	{
		dp_reduce_ContainerType_tok_t_string(GET_SELF, &$$, &$3);
	};

	
SimpleType:
	tok_t_bool
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_char
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_double
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_int8
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_int16
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_int32
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_int64
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_uint8 
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_uint16 
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_uint32 
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_t_uint64
	{
		dp_reduce_SimpleType(GET_SELF, &$$, $1);
	}
|	tok_identifier
   	{
		dp_reduce_SimpleType_tok_identifier(GET_SELF, &$$, &$1);
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
		//参数类型只能为简单类型
		dp_check_Parameter_add(GET_SELF, &yylloc, &$$);
	};

Arguments:
	'(' ArgumentList ')'
	{
		//参数只能传递本对象中的简单类型
		$$ = $2;
	}
|
	{
		$$.arg_list_num = 0;
	};
	
ArgumentList:
	ArgumentList ',' tok_identifier
	{
		dp_reduce_ArgumentList_ArgumentList_tok_identifier(GET_SELF, &$$, &$1, &$3);
	}
|	tok_identifier
	{
		dp_reduce_ArgumentList_tok_identifier(GET_SELF, &$$, &$1);
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
