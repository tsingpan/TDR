#include <stdio.h>

#include "data_description_l.h"
#include "data_description_y.h"

#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"
#include "hotscript/hotlex.h"

#include <errno.h>
		
static void ddekit_reserved_keyword(const char* keyword)
{
	//yyerror(&yylloc, ddekit_parameter, "Cannot use reserved language keyword: \"%s\"\n", keyword);
}


hpint32 ddc_lex_scan(SCANNER *self, YYLTYPE *yylloc, YYSTYPE * yylval)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return 0;
	}
	yylloc->first_line = self->yylineno;
	yylloc->first_column = self->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
intconstant		([+-]?[0-9]+)
hexconstant		("0x"[0-9A-Fa-f]+)
newline			("\r\n"|"\r"|"\n")
comment			("//".*{newline})
unixcomment		("#".*{newline})
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			([<>{}\(\);,=:&!])
anychar			([^])


<!*> := yyleng = YYCURSOR - yytext; scanner_process(self);

<*>{comment}			{	goto restart;																}
<*>{sillycomm}			{	goto restart;																}
<*>{multicomm}			{	goto restart;																}

<INITIAL>"import"				{return tok_import;																}
<INITIAL>{symbol}				{return yytext[0];																}
<INITIAL>"union"				{return tok_union;																}
<INITIAL>"struct"				{return tok_struct;																}
<INITIAL>"if"					{return tok_if;																	}
<INITIAL>"const"				{return tok_const;																}
<INITIAL>"enum"					{return tok_enum;																}
<INITIAL>"case"                 {return tok_case;																}
<INITIAL>"typename"				{return tok_typename;															}
<INITIAL>"unique"				{return tok_unique;																}
<INITIAL>"typedef"				{return tok_typedef;															}
<INITIAL>"lower_bound"			{return tok_lower_bound;														}
<INITIAL>"upper_bound"			{return tok_upper_bound;														}
<INITIAL>"switch"				{return tok_switch;																}

<INITIAL>"int8"					{return tok_t_int8;																}
<INITIAL>"int16"				{return tok_t_int16;															}
<INITIAL>"int32"				{return tok_t_int32;															}
<INITIAL>"int64"				{return tok_t_int64;															}
<INITIAL>"uint8"				{return tok_t_uint8;															}
<INITIAL>"uint16"				{return tok_t_uint16;															}
<INITIAL>"uint32"				{return tok_t_uint32;															}
<INITIAL>"uint64"				{return tok_t_uint64;															}
<INITIAL>"bool"					{return tok_t_bool;																}
<INITIAL>"double"				{return tok_t_double;															}
<INITIAL>"char"					{return tok_t_char;																}
<INITIAL>"vector"				{return tok_t_vector;															}
<INITIAL>"string"				{return tok_t_string;															}
<INITIAL>"=="					{return tok_equal;																}
<INITIAL>"!="					{return tok_unequal;															}




<INITIAL>"true"					{return tok_true;																}
<INITIAL>"false"				{return tok_false;																}
<INITIAL>{hexconstant}			{return tok_hex;																}
<INITIAL>{intconstant}			{return tok_int;																}
<INITIAL>{identifier}			{return tok_identifier;															}
<INITIAL>{unixcomment}			{return tok_unixcomment;														}



<INITIAL>"BEGIN"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"END"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"__CLASS__"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"__DIR__"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"__FILE__"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"__FUNCTION__"       { ddekit_reserved_keyword(yytext); }
<INITIAL>"__LINE__"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"__METHOD__"         { ddekit_reserved_keyword(yytext); }
<INITIAL>"__NAMESPACE__"      { ddekit_reserved_keyword(yytext); }
<INITIAL>"abstract"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"alias"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"and"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"args"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"as"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"assert"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"begin"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"break"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"catch"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"class"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"clone"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"continue"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"declare"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"def"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"default"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"del"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"delete"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"do"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"dynamic"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"elif"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"else"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"elseif"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"elsif"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"end"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"enddeclare"         { ddekit_reserved_keyword(yytext); }
<INITIAL>"endfor"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"endforeach"         { ddekit_reserved_keyword(yytext); }
<INITIAL>"endif"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"endswitch"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"endwhile"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"ensure"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"except"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"exec"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"finally"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"float"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"for"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"foreach"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"function"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"global"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"goto"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"if"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"implements"         { ddekit_reserved_keyword(yytext); }
<INITIAL>"import"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"in"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"inline"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"instanceof"         { ddekit_reserved_keyword(yytext); }
<INITIAL>"interface"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"is"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"lambda"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"module"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"native"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"new"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"next"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"nil"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"not"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"or"                 { ddekit_reserved_keyword(yytext); }
<INITIAL>"pass"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"public"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"print"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"private"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"protected"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"public"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"raise"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"redo"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"rescue"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"retry"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"register"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"return"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"self"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"sizeof"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"static"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"super"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"synchronized"       { ddekit_reserved_keyword(yytext); }
<INITIAL>"then"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"this"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"throw"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"transient"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"try"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"undef"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"union"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"unless"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"unsigned"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"until"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"use"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"var"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"virtual"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"volatile"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"when"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"while"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"with"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"xor"                { ddekit_reserved_keyword(yytext); }
<INITIAL>"yield"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"list"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"required"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"optional"           { ddekit_reserved_keyword(yytext); }
<INITIAL>"async"              { ddekit_reserved_keyword(yytext); }
<INITIAL>"service"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"throws"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"extends"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"exception"          { ddekit_reserved_keyword(yytext); }
<INITIAL>"binary"             { ddekit_reserved_keyword(yytext); }
<INITIAL>"include"            { ddekit_reserved_keyword(yytext); }
<INITIAL>"bool"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"void"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"byte"               { ddekit_reserved_keyword(yytext); }
<INITIAL>"namespace"          { ddekit_reserved_keyword(yytext); }

<*>{anychar}					{ goto restart;					 }

*/
}
