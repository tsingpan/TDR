%{
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "script_y.h"

#define YY_USER_ACTION																	\
	yylloc->first_line = yylloc->last_line = yylineno;									\
	yylloc->first_column = yycolumn;													\
	yylloc->last_column = yycolumn + yyleng - 1;										\
	yycolumn += yyleng;																	
		
		
static void hotscript_reserved_keyword(char* keyword)
{
	//yyerror(&yylloc, hotscript_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}
void yyerror(const YYLTYPE *yylloc, yyscan_t *yyscan, char *s, ...);
%}


%option noyywrap yylineno reentrant nounistd bison-bridge bison-locations
%x INTAG
%option extra-type = "yyscan_t*"

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
comment			("//"[^\n]*)
unixcomment		("#"[^\n]*)
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			([$])
newline			("\r"|"\n"|"\r\n")
%%

#首先跳过注释
<*>{comment}			{ /* do nothing */																}
<*>{sillycomm}			{ /* do nothing */																}
<*>{multicomm}			{ /* do nothing */																}
<*>{newline}			{yycolumn = 1;																	}

#然后读取关键字
#然后读取关键字
{symbol}				{return yytext[0];																}
"include"				{return tok_include;															}

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
"union"				 { hotscript_reserved_keyword(yytext); }
"struct"			 { hotscript_reserved_keyword(yytext); }
"if"				 { hotscript_reserved_keyword(yytext); }
"const"				 { hotscript_reserved_keyword(yytext); }
"enum"				 { hotscript_reserved_keyword(yytext); }
"case"               { hotscript_reserved_keyword(yytext); }
"typename"			 { hotscript_reserved_keyword(yytext); }
"unique"			 { hotscript_reserved_keyword(yytext); }
"typedef"			 { hotscript_reserved_keyword(yytext); }
"true"				 { hotscript_reserved_keyword(yytext); }
"false"				 { hotscript_reserved_keyword(yytext); }
"lower_bound"		 { hotscript_reserved_keyword(yytext); }
"upper_bound"		 { hotscript_reserved_keyword(yytext); }
"switch"			 { hotscript_reserved_keyword(yytext); }
"vector"			 { hotscript_reserved_keyword(yytext); }
"string"			 { hotscript_reserved_keyword(yytext); }
"int8"				 { hotscript_reserved_keyword(yytext); }
"int16"				 { hotscript_reserved_keyword(yytext); }
"int32"				 { hotscript_reserved_keyword(yytext); }
"int64"				 { hotscript_reserved_keyword(yytext); }
"uint8"				 { hotscript_reserved_keyword(yytext); }
"uint16"			 { hotscript_reserved_keyword(yytext); }
"uint32"			 { hotscript_reserved_keyword(yytext); }
"uint64"			 { hotscript_reserved_keyword(yytext); }
"char"				 { hotscript_reserved_keyword(yytext); }
"double"			 { hotscript_reserved_keyword(yytext); }
"=="				 { hotscript_reserved_keyword(yytext); }
"!="				 { hotscript_reserved_keyword(yytext); }

#跳过没用的字符
<*>.					{/* do nothing */																}



%%
