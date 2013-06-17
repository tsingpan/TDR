%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hotpot/hp_platform.h"
#include "hs_l.h"

#define YYERROR_VERBOSE

//这里的代码生成在自身的文件中

#define GET_PARSER PARSER *parser = HP_CONTAINER_OF(ddekit_parameter, PARSER, scanner);
%}
%locations

%code requires
{
#include "ddc_globals.h"
#include "ast_base.h"
//这里的代码会生成到头文件中
#include "parse/semantic_analysis/gnArgumentList.h"
#include "parse/semantic_analysis/gnParameterList.h"
#include "parse/semantic_analysis/gnUnixComment.h"
#include "parse/semantic_analysis/gnTypeAnnotationList.h"
#include "parse/semantic_analysis/gnFieldList.h"
#include "parse/semantic_analysis/gnType.h"
#include "parse/semantic_analysis/gnEnum.h"



#define YYMALLOC
#define YYFREE
#define YYLEX_PARAM *ddekit_parameter

}//code requires end

%define api.pure
%parse-param { yyscan_t *ddekit_parameter }
%pure_parser

%token tok_import 
%token tok_struct
%token tok_union 
%token tok_if 
%token tok_type 
%token tok_integer
%token tok_package_path 
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
%token tok_bool
%token tok_typedef
%token tok_switch

%left '='


%union
{
	int i32;
	char str[MAX_TOKEN_LENGTH];
	GN_TYPE *gn_type;
	GN_PARAMETER *gn_parameter;
	GN_PARAMETER_LIST *gn_parameter_list;
	GN_ARGUMENT *gn_argument;
	GN_ARGUMENT_LIST *gn_argument_list;
	GN_TYPE_ANNOTATION *gn_annotation;
	GN_TYPE_ANNOTATION_LIST *gn_annotation_list;
	GN_UNIX_COMMENT *gn_unix_comment;
	GN_FIELD_LIST *gn_field_list;
	GN_FIELD *gn_field;	
	GN_FIELD_CONDITION *gn_field_condition;
	GN_FIELD_EXPRESSION *gn_field_expression;
	GN_ENUM_DEF *gn_enum_def;
	GN_ENUM_DEF_LIST *gn_enum_def_list;
	GN_ENUM *gn_enum;
}

%type <i32> tok_type
%type <i32> tok_bool

%type <str>								tok_integer
%type <str>								tok_import
%type <str>								tok_package_path
%type <str>								tok_identifier
%type <str>								tok_unixcomment
%type <gn_type>							Type
%type <gn_argument>						Argument
%type <gn_argument_list>				ArgumentList
%type <gn_argument_list>				Arguments
%type <gn_parameter>					Parameter
%type <gn_parameter_list>				ParameterList
%type <gn_parameter_list>				Parameters
%type <gn_annotation>					TypeAnnotation
%type <gn_annotation_list>				TypeAnnotationList
%type <gn_annotation_list>				TypeAnnotations
%type <gn_field>						Field
%type <gn_field_condition>				FieldCondition
%type <gn_field_expression>				FieldExpression
%type <gn_field_list>					FieldList
%type <gn_unix_comment>					UnixComment
%type <gn_enum_def>						EnumDef
%type <gn_enum_def_list>				EnumDefList
%type <gn_enum>							Enum


%start Document

%%

Document :
	DefinitionList
	{
	};

DefinitionList :
	DefinitionList Definition 
	{
	}
|	{
	}

Definition :
	Import
	{
	} 
|	Const
	{
	}
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
	}

Import : 
	tok_import tok_package_path
	{
	}

Const : 
	tok_const tok_type tok_identifier '=' tok_integer CommaOrSemicolonOptional
	{
	}
	
Typedef :
	tok_typedef Type Arguments tok_identifier CommaOrSemicolonOptional
	{
	}
	
Enum :
	tok_enum tok_identifier '{' EnumDefList '}' TypeAnnotations CommaOrSemicolonOptional
	{	
    }
    
EnumDefList : 
	EnumDefList EnumDef
	{
	}
|
	{
		GET_PARSER
		$$ = &parser->gn_enum_def
	}
	
EnumDef : 
	tok_identifier '=' tok_integer CommaOrSemicolonOptional UnixComment
	{
		GET_PARSER
		gn_enum_set_name(&parser->gn_enum_def, $1);
		parser->gn_enum_def.val = $3;
		parser->gn_enum_def.unix_comment = *$5;
		$$ = &parser->gn_enum_def;
	}
    

Union :
	tok_union tok_identifier  Parameters '{' FieldList '}' TypeAnnotations CommaOrSemicolonOptional
	{
	}
	
	
Struct : 
	tok_struct tok_identifier Parameters '{' FieldList '}' TypeAnnotations CommaOrSemicolonOptional
	{
	}
	

	
FieldList: 
	FieldList Field
	{
		GET_PARSER
		gn_field_list_append(&parser->gn_field_list, $2);
		$$ = &parser->gn_field_list;
	}
|
	{
		GET_PARSER
		$$ = &parser->gn_field_list;
	}
	

Field : 
	FieldCondition Type Arguments tok_identifier ';' UnixComment
	{
		GET_PARSER

		parser->gn_field.field_condition = *$1;
		parser->gn_field.type = *$2;
		parser->gn_field.arguments = *$3;
		gn_field_set_name(&parser->gn_field, $4);
		parser->gn_field.unix_comment = *$6;

		gn_field_condition_init($1);
		gn_type_init($2);
		gn_argument_list_init($3);
		gn_unix_comment_init($6);
		

		$$ = &parser->gn_field;
	}
	

FieldCondition : 
	tok_if '(' FieldExpression ')'
	{
		GET_PARSER

		parser->gn_field_condition.neg = E_HP_TRUE;
		gn_field_condition_set_field_expression(&parser->gn_field_condition, $3);

		gn_field_expression_init($3);

		$$ = &parser->gn_field_condition;
	}
|	tok_if '!' '(' FieldExpression ')'
	{
		GET_PARSER

		parser->gn_field_condition.neg = E_HP_FALSE;
		gn_field_condition_set_field_expression(&parser->gn_field_condition, $4);

		gn_field_expression_init($4);
		$$ = &parser->gn_field_condition;
	}
//加点糖
//if (selector == tok_identifier)
|	tok_case tok_identifier ':'
	{
		GET_PARSER

		gn_field_expression_set_op(&parser->gn_field_expression, E_OP_EQUAL);
		gn_field_expression_set_arg0(&parser->gn_field_expression, "s");//默认为一个名叫s的参数
		gn_field_expression_set_arg1(&parser->gn_field_expression, $2);


		parser->gn_field_condition.neg = E_HP_FALSE;
		gn_field_condition_set_field_expression(&parser->gn_field_condition, &parser->gn_field_expression);
		gn_field_expression_init(&parser->gn_field_expression);
		$$ = &parser->gn_field_condition;
	}
//if !(tok_identifier == tok_identifier)
|	tok_if '(' tok_identifier tok_unequal tok_identifier ')'
	{
		GET_PARSER

		gn_field_expression_set_op(&parser->gn_field_expression, E_OP_EQUAL);
		gn_field_expression_set_arg0(&parser->gn_field_expression, $3);//默认为一个名叫s的参数
		gn_field_expression_set_arg1(&parser->gn_field_expression, $5);


		parser->gn_field_condition.neg = E_HP_TRUE;
		gn_field_condition_set_field_expression(&parser->gn_field_condition, &parser->gn_field_expression);
		gn_field_expression_init(&parser->gn_field_expression);
		$$ = &parser->gn_field_condition;
	}
|
	{
		GET_PARSER
		$$ = &parser->gn_field_condition;
	}

FieldExpression :
	tok_identifier tok_equal tok_identifier
	{
		GET_PARSER
		
		gn_field_expression_set_op(&parser->gn_field_expression, E_OP_EQUAL);
		gn_field_expression_set_arg0(&parser->gn_field_expression, $1);
		gn_field_expression_set_arg1(&parser->gn_field_expression, $3);

		$$ = &parser->gn_field_expression;
	}

|	tok_identifier '&' tok_identifier
	{
		GET_PARSER

		gn_field_expression_set_op(&parser->gn_field_expression, E_OP_AND);
		gn_field_expression_set_arg0(&parser->gn_field_expression, $1);
		gn_field_expression_set_arg1(&parser->gn_field_expression, $3);

		$$ = &parser->gn_field_expression;
	}



Type :
	tok_type
	{
		GET_PARSER

		parser->gn_type.is_custom_type = E_HP_FALSE;
		parser->gn_type.type = $1;

		$$ = &parser->gn_type;
	}
|	tok_identifier
   	{
		GET_PARSER

		parser->gn_type.is_custom_type = E_HP_TRUE;

		gn_type_set_custom_type(&parser->gn_type, $1);

		$$ = &parser->gn_type;
	}

Parameters :
	'<' ParameterList '>'
	{
		$$ = $2;
	}
|
	{
	}
	
ParameterList:
	ParameterList Parameter 
	{
		GET_PARSER
		
		gn_parameter_list_append(&parser->gn_parameter_list, $2);
		gn_parameter_init($2);
		
		$$ = &parser->gn_parameter_list;
	}
|
	{
		GET_PARSER
	
		$$ = &parser->gn_parameter_list;
	}
	
	
Parameter:
	Type tok_identifier CommaOrSemicolonOptional
	{
		GET_PARSER

		parser->gn_parameter.type = *$1;
		gn_parameter_set_name(&parser->gn_parameter, $2);

		gn_type_init($1);
		$$ = &parser->gn_parameter;
	}




Arguments:
	'<' ArgumentList '>'
	{
		$$ = $2;
	}
|
	{
		GET_PARSER
		$$ = &parser->gn_argument_list;
	}
	
ArgumentList:
	ArgumentList Argument
	{
		GET_PARSER;
		
		gn_argument_list_append(&parser->gn_argument_list, $2);
		
		gn_argument_init($2);		
		$$ = &parser->gn_argument_list;
	}
|
	{
		GET_PARSER
		$$ = &parser->gn_argument_list;
	}
	
Argument:
	tok_identifier CommaOrSemicolonOptional
	{
		GET_PARSER;
		
		gn_argument_set_name(&parser->gn_argument, $1);
		$$ = &parser->gn_argument;
	}

UnixComment:
	tok_unixcomment
	{
		GET_PARSER;
		
		gn_unix_comment_set_content(&parser->gn_unix_comment, $1);
		$$ = &parser->gn_unix_comment;
	}
|
	{
		GET_PARSER;
		$$ = &parser->gn_unix_comment;
	}

//加点盐
TypeAnnotations:
  '(' TypeAnnotationList ')'
    {
		$$ = $2;
    }
|
    {
		GET_PARSER;
		$$ = &parser->gn_type_annotation_list;
    }

TypeAnnotationList:
  TypeAnnotationList TypeAnnotation
    {
		GET_PARSER;

		gn_type_annotation_list_append(&parser->gn_type_annotation_list, $2);

		gn_type_annotation_init($2);

		$$ = &parser->gn_type_annotation_list;
    }
|
    {
		GET_PARSER;
		gn_type_annotation_init(&parser->gn_type_annotation_list);
		$$ = &parser->gn_type_annotation_list;
    }

TypeAnnotation:
	tok_unique '= ' tok_bool CommaOrSemicolonOptional
    {
		GET_PARSER;
		
		parser->gn_type_annotation.unique = $3;
		parser->gn_type_annotation.mask |= MASK_UNIQUE;

		$$ = &parser->gn_type_annotation;
    }
|	tok_lower_bound '=' tok_identifier CommaOrSemicolonOptional
	{
		GET_PARSER;

		gn_type_annotation_set_lower_bound(&parser->gn_type_annotation, $3);
		parser->gn_type_annotation.mask |= MASK_LOWER_BOUND;

		$$ = &parser->gn_type_annotation;
	}
|	tok_upper_bound '=' tok_identifier CommaOrSemicolonOptional
	{
		GET_PARSER;

		gn_type_annotation_set_upper_bound(&parser->gn_type_annotation, $3);
		parser->gn_type_annotation.mask |= MASK_UPPER_BOUND;

		$$ = &parser->gn_type_annotation;
	}
|	tok_switch '=' tok_identifier CommaOrSemicolonOptional
	{
		GET_PARSER;

		gn_type_annotation_set_selector(&parser->gn_type_annotation, $3);
		parser->gn_type_annotation.mask |= MASK_SELECTOR;

		$$ = &parser->gn_type_annotation;
	}




CommaOrSemicolonOptional:
  ','
    {
    
    }
| ';'
    {
    
    }
|
    {
    
    }
    
%%

