%{
//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>

#define YYERROR_VERBOSE

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
%token tok_type 
%token tok_integer
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
	};

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
	};

Import : 
	tok_import
	{
	};

Const : 
	tok_const tok_type tok_identifier '=' tok_integer CommaOrSemicolonOptional
	{
	}
|	tok_const tok_type tok_identifier '=' tok_identifier CommaOrSemicolonOptional
	{
	};
	
Typedef :
	tok_typedef Type Arguments tok_identifier CommaOrSemicolonOptional
	{
	};
	
Enum :
	tok_enum tok_identifier '{' EnumDefList '}' TypeAnnotations CommaOrSemicolonOptional
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
	tok_identifier '=' tok_integer CommaOrSemicolonOptional UnixComment
	{
	};
    

Union :
	tok_union tok_identifier  Parameters '{' FieldList '}' TypeAnnotations CommaOrSemicolonOptional
	{
	};
	
	
Struct : 
	tok_struct tok_identifier Parameters '{' FieldList '}' TypeAnnotations CommaOrSemicolonOptional
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
	tok_type
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

TypeAnnotation:
	tok_unique '= ' tok_bool CommaOrSemicolonOptional
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
