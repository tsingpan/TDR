%{
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"

#define YY_USER_ACTION																	\
	yylloc->first_line = yylloc->last_line = yylineno;									\
	yylloc->first_column = yycolumn;													\
	yylloc->last_column = yycolumn + yyleng - 1;										\
	yycolumn += yyleng;																	
		
		
static void hotscript_reserved_keyword(char* keyword)
{
	//yyerror(&yylloc, hotscript_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}

%}


%option noyywrap yylineno reentrant nounistd bison-bridge bison-locations
%x IMPORT
%option extra-type = "yyscan_t*"


identifier		([a-zA-Z_][a-zA-Z_0-9]*)
package_path	(({identifier}\.)*{identifier}+)
intconstant		([+-]?[0-9]+)
hexconstant		("0x"[0-9A-Fa-f]+)
dubconstant		([+-]?[0-9]*(\.[0-9]+)?([eE][+-]?[0-9]+)?)
comment			("//"[^\n]*)
unixcomment		("#"[^\n]*)
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			([<>{}\(\);,=:&!])
newline			("\r"|"\n"|"\r\n")
%%


"import" { BEGIN IMPORT; return tok_import;}

<IMPORT>{package_path} {
	//去掉末尾的;
	char c;
	for(c = input(*yyextra); c != ';'; c = input(*yyextra))
	{
		if(c == EOF)
		{
			goto ERROR_RET;
		}
	}
	strncpy(yylval->str, yytext, MAX_TOKEN_LENGTH);
	if(parser_push_package(yyextra, yytext) != E_HP_NOERROR)
	{
		goto ERROR_RET;		
	}
	BEGIN INITIAL;
	return tok_package_path;
ERROR_RET:
	yyterminate();
}

<<EOF>> {
	if(parser_pop_package(yyextra) != E_HP_NOERROR)
	{
		yyterminate();
	}
	st_on_popfile();
}

#首先跳过注释
<*>{comment}			{ /* do nothing */																}
<*>{sillycomm}			{ /* do nothing */																}
<*>{multicomm}			{ /* do nothing */																}
<*>{newline}			{yycolumn = 1;																	}

#然后读取关键字
{symbol}				{return yytext[0];																}
"union"					{return tok_union;																}
"struct"				{return tok_struct;																}
"if"					{return tok_if;																	}
"const"					{return tok_const;																}
"enum"					{return tok_enum;																}
"case"                  {return tok_case;																}
"typename"				{return tok_typename;															}
"unique"				{return tok_unique;																}
"typedef"				{return tok_typedef;															}
"true"					{yylval->i32 = TRUE; return tok_bool;											}
"false"					{yylval->i32 = FALSE; return tok_bool;											}
"lower_bound"			{return tok_lower_bound;														}
"upper_bound"			{return tok_upper_bound;														}
"switch"				{return tok_switch;																}
"vector"				{yylval->i32 = X_VECTOR; return tok_type;										}
"string"				{yylval->i32 = X_STRING; return tok_type;										}
"int8"					{yylval->i32 = X_INT8; return tok_type;											}
"int16"					{yylval->i32 = X_INT16; return tok_type;										}
"int32"					{yylval->i32 = X_INT32; return tok_type;										}
"int64"					{yylval->i32 = X_INT64; return tok_type;										}
"uint8"					{yylval->i32 = X_UINT8; return tok_type;										}
"uint16"				{yylval->i32 = X_UINT16; return tok_type;										}
"uint32"				{yylval->i32 = X_UINT32; return tok_type;										}
"uint64"				{yylval->i32 = X_UINT64; return tok_type;										}
"char"					{yylval->i32 = X_CHAR; return tok_type;											}
"double"				{yylval->i32 = X_DOUBLE; return tok_type;										}
"=="					{return tok_equal;																}
"!="					{return tok_unequal;															}


#检测保留字
"BEGIN"              { hotscript_reserved_keyword(yytext); }
"END"                { hotscript_reserved_keyword(yytext); }
"__CLASS__"          { hotscript_reserved_keyword(yytext); }
"__DIR__"            { hotscript_reserved_keyword(yytext); }
"__FILE__"           { hotscript_reserved_keyword(yytext); }
"__FUNCTION__"       { hotscript_reserved_keyword(yytext); }
"__LINE__"           { hotscript_reserved_keyword(yytext); }
"__METHOD__"         { hotscript_reserved_keyword(yytext); }
"__NAMESPACE__"      { hotscript_reserved_keyword(yytext); }
"abstract"           { hotscript_reserved_keyword(yytext); }
"alias"              { hotscript_reserved_keyword(yytext); }
"and"                { hotscript_reserved_keyword(yytext); }
"args"               { hotscript_reserved_keyword(yytext); }
"as"                 { hotscript_reserved_keyword(yytext); }
"assert"             { hotscript_reserved_keyword(yytext); }
"begin"              { hotscript_reserved_keyword(yytext); }
"break"              { hotscript_reserved_keyword(yytext); }
"catch"              { hotscript_reserved_keyword(yytext); }
"class"              { hotscript_reserved_keyword(yytext); }
"clone"              { hotscript_reserved_keyword(yytext); }
"continue"           { hotscript_reserved_keyword(yytext); }
"declare"            { hotscript_reserved_keyword(yytext); }
"def"                { hotscript_reserved_keyword(yytext); }
"default"            { hotscript_reserved_keyword(yytext); }
"del"                { hotscript_reserved_keyword(yytext); }
"delete"             { hotscript_reserved_keyword(yytext); }
"do"                 { hotscript_reserved_keyword(yytext); }
"dynamic"            { hotscript_reserved_keyword(yytext); }
"elif"               { hotscript_reserved_keyword(yytext); }
"else"               { hotscript_reserved_keyword(yytext); }
"elseif"             { hotscript_reserved_keyword(yytext); }
"elsif"              { hotscript_reserved_keyword(yytext); }
"end"                { hotscript_reserved_keyword(yytext); }
"enddeclare"         { hotscript_reserved_keyword(yytext); }
"endfor"             { hotscript_reserved_keyword(yytext); }
"endforeach"         { hotscript_reserved_keyword(yytext); }
"endif"              { hotscript_reserved_keyword(yytext); }
"endswitch"          { hotscript_reserved_keyword(yytext); }
"endwhile"           { hotscript_reserved_keyword(yytext); }
"ensure"             { hotscript_reserved_keyword(yytext); }
"except"             { hotscript_reserved_keyword(yytext); }
"exec"               { hotscript_reserved_keyword(yytext); }
"finally"            { hotscript_reserved_keyword(yytext); }
"float"              { hotscript_reserved_keyword(yytext); }
"for"                { hotscript_reserved_keyword(yytext); }
"foreach"            { hotscript_reserved_keyword(yytext); }
"function"           { hotscript_reserved_keyword(yytext); }
"global"             { hotscript_reserved_keyword(yytext); }
"goto"               { hotscript_reserved_keyword(yytext); }
"if"                 { hotscript_reserved_keyword(yytext); }
"implements"         { hotscript_reserved_keyword(yytext); }
"import"             { hotscript_reserved_keyword(yytext); }
"in"                 { hotscript_reserved_keyword(yytext); }
"inline"             { hotscript_reserved_keyword(yytext); }
"instanceof"         { hotscript_reserved_keyword(yytext); }
"interface"          { hotscript_reserved_keyword(yytext); }
"is"                 { hotscript_reserved_keyword(yytext); }
"lambda"             { hotscript_reserved_keyword(yytext); }
"module"             { hotscript_reserved_keyword(yytext); }
"native"             { hotscript_reserved_keyword(yytext); }
"new"                { hotscript_reserved_keyword(yytext); }
"next"               { hotscript_reserved_keyword(yytext); }
"nil"                { hotscript_reserved_keyword(yytext); }
"not"                { hotscript_reserved_keyword(yytext); }
"or"                 { hotscript_reserved_keyword(yytext); }
"pass"               { hotscript_reserved_keyword(yytext); }
"public"             { hotscript_reserved_keyword(yytext); }
"print"              { hotscript_reserved_keyword(yytext); }
"private"            { hotscript_reserved_keyword(yytext); }
"protected"          { hotscript_reserved_keyword(yytext); }
"public"             { hotscript_reserved_keyword(yytext); }
"raise"              { hotscript_reserved_keyword(yytext); }
"redo"               { hotscript_reserved_keyword(yytext); }
"rescue"             { hotscript_reserved_keyword(yytext); }
"retry"              { hotscript_reserved_keyword(yytext); }
"register"           { hotscript_reserved_keyword(yytext); }
"return"             { hotscript_reserved_keyword(yytext); }
"self"               { hotscript_reserved_keyword(yytext); }
"sizeof"             { hotscript_reserved_keyword(yytext); }
"static"             { hotscript_reserved_keyword(yytext); }
"super"              { hotscript_reserved_keyword(yytext); }
"synchronized"       { hotscript_reserved_keyword(yytext); }
"then"               { hotscript_reserved_keyword(yytext); }
"this"               { hotscript_reserved_keyword(yytext); }
"throw"              { hotscript_reserved_keyword(yytext); }
"transient"          { hotscript_reserved_keyword(yytext); }
"try"                { hotscript_reserved_keyword(yytext); }
"undef"              { hotscript_reserved_keyword(yytext); }
"union"              { hotscript_reserved_keyword(yytext); }
"unless"             { hotscript_reserved_keyword(yytext); }
"unsigned"           { hotscript_reserved_keyword(yytext); }
"until"              { hotscript_reserved_keyword(yytext); }
"use"                { hotscript_reserved_keyword(yytext); }
"var"                { hotscript_reserved_keyword(yytext); }
"virtual"            { hotscript_reserved_keyword(yytext); }
"volatile"           { hotscript_reserved_keyword(yytext); }
"when"               { hotscript_reserved_keyword(yytext); }
"while"              { hotscript_reserved_keyword(yytext); }
"with"               { hotscript_reserved_keyword(yytext); }
"xor"                { hotscript_reserved_keyword(yytext); }
"yield"              { hotscript_reserved_keyword(yytext); }
"list"               { hotscript_reserved_keyword(yytext); }
"required"           { hotscript_reserved_keyword(yytext); }
"optional"           { hotscript_reserved_keyword(yytext); }
"async"              { hotscript_reserved_keyword(yytext); }
"service"            { hotscript_reserved_keyword(yytext); }
"throws"             { hotscript_reserved_keyword(yytext); }
"extends"            { hotscript_reserved_keyword(yytext); }
"exception"          { hotscript_reserved_keyword(yytext); }
"binary"             { hotscript_reserved_keyword(yytext); }
"include"            { hotscript_reserved_keyword(yytext); }
"bool"               { hotscript_reserved_keyword(yytext); }
"void"               { hotscript_reserved_keyword(yytext); }
"byte"               { hotscript_reserved_keyword(yytext); }
"namespace"          { hotscript_reserved_keyword(yytext); }


#返回标识符和数字常量
{hexconstant}			{strncpy(yylval->str, yytext, MAX_TOKEN_LENGTH); return tok_integer;			}
{intconstant}			{strncpy(yylval->str, yytext, MAX_TOKEN_LENGTH); return tok_integer;			}
{identifier}			{strncpy(yylval->str, yytext, MAX_TOKEN_LENGTH); return tok_identifier;			}
{unixcomment}			{strncpy(yylval->str, yytext, MAX_TOKEN_LENGTH); return tok_unixcomment;		}

#跳过没用的字符
<*>.					{/* do nothing */																}

%%
