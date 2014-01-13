#include "platform/tlibc_platform.h"
#include "parse/scanner.h"


#include "tdata_l.h"
#include "tdata_y.h"


		
tint32 scanner_scan(SCANNER *self, YYLTYPE *yylloc, SCANNER_TOKEN_VALUE * yylval)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return 0;
	}
	yylloc->first_line = scanner_top(self)->yylineno;
	yylloc->first_column = scanner_top(self)->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

LNUM			[0-9]+
DNUM			([0-9]*"."[0-9]+)|([0-9]+"."[0-9]*)
doubleconstant	(({LNUM}|{DNUM})[eE][+-]?{LNUM})
literal_begin	([\"])
char_begin		(['])
intconstant		([+-]?{LNUM})
hexconstant		("0x"[0-9A-Fa-f]+)
identifier		([a-zA-Z_][a-zA-Z_0-9]*)
newline			("\r\n"|"\r"|"\n")
comment			("//".*{newline})
unixcomment		("#".*{newline})
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			([<>{}\(\);,=:&!])
anychar			([^])


<!*> := yyleng = YYCURSOR - yytext; scanner_locate(self);

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
<INITIAL>"typedef"				{return tok_typedef;															}
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
<INITIAL>"count"				{return tok_count;																}
<INITIAL>{unixcomment}			{return tok_unixcomment;														}
<INITIAL>{literal_begin}		{return tok_string;																}
<INITIAL>{char_begin}			{return tok_char;																}
<INITIAL>{doubleconstant}|{DNUM}{return tok_double;																}
<INITIAL>{hexconstant}			{return tok_hex;																}
<INITIAL>{intconstant}			{return tok_int;																}

<INITIAL>"BEGIN"              { return tok_reserved_keyword; }
<INITIAL>"END"                { return tok_reserved_keyword; }
<INITIAL>"__CLASS__"          { return tok_reserved_keyword; }
<INITIAL>"__DIR__"            { return tok_reserved_keyword; }
<INITIAL>"__FILE__"           { return tok_reserved_keyword; }
<INITIAL>"__FUNCTION__"       { return tok_reserved_keyword; }
<INITIAL>"__LINE__"           { return tok_reserved_keyword; }
<INITIAL>"__METHOD__"         { return tok_reserved_keyword; }
<INITIAL>"__NAMESPACE__"      { return tok_reserved_keyword; }
<INITIAL>"abstract"           { return tok_reserved_keyword; }
<INITIAL>"alias"              { return tok_reserved_keyword; }
<INITIAL>"and"                { return tok_reserved_keyword; }
<INITIAL>"args"               { return tok_reserved_keyword; }
<INITIAL>"as"                 { return tok_reserved_keyword; }
<INITIAL>"assert"             { return tok_reserved_keyword; }
<INITIAL>"begin"              { return tok_reserved_keyword; }
<INITIAL>"break"              { return tok_reserved_keyword; }
<INITIAL>"catch"              { return tok_reserved_keyword; }
<INITIAL>"class"              { return tok_reserved_keyword; }
<INITIAL>"clone"              { return tok_reserved_keyword; }
<INITIAL>"continue"           { return tok_reserved_keyword; }
<INITIAL>"declare"            { return tok_reserved_keyword; }
<INITIAL>"def"                { return tok_reserved_keyword; }
<INITIAL>"default"            { return tok_reserved_keyword; }
<INITIAL>"del"                { return tok_reserved_keyword; }
<INITIAL>"delete"             { return tok_reserved_keyword; }
<INITIAL>"do"                 { return tok_reserved_keyword; }
<INITIAL>"dynamic"            { return tok_reserved_keyword; }
<INITIAL>"elif"               { return tok_reserved_keyword; }
<INITIAL>"else"               { return tok_reserved_keyword; }
<INITIAL>"elseif"             { return tok_reserved_keyword; }
<INITIAL>"elsif"              { return tok_reserved_keyword; }
<INITIAL>"end"                { return tok_reserved_keyword; }
<INITIAL>"enddeclare"         { return tok_reserved_keyword; }
<INITIAL>"endfor"             { return tok_reserved_keyword; }
<INITIAL>"endforeach"         { return tok_reserved_keyword; }
<INITIAL>"endif"              { return tok_reserved_keyword; }
<INITIAL>"endswitch"          { return tok_reserved_keyword; }
<INITIAL>"endwhile"           { return tok_reserved_keyword; }
<INITIAL>"ensure"             { return tok_reserved_keyword; }
<INITIAL>"except"             { return tok_reserved_keyword; }
<INITIAL>"exec"               { return tok_reserved_keyword; }
<INITIAL>"finally"            { return tok_reserved_keyword; }
<INITIAL>"float"              { return tok_reserved_keyword; }
<INITIAL>"for"                { return tok_reserved_keyword; }
<INITIAL>"foreach"            { return tok_reserved_keyword; }
<INITIAL>"function"           { return tok_reserved_keyword; }
<INITIAL>"global"             { return tok_reserved_keyword; }
<INITIAL>"goto"               { return tok_reserved_keyword; }
<INITIAL>"if"                 { return tok_reserved_keyword; }
<INITIAL>"implements"         { return tok_reserved_keyword; }
<INITIAL>"import"             { return tok_reserved_keyword; }
<INITIAL>"in"                 { return tok_reserved_keyword; }
<INITIAL>"inline"             { return tok_reserved_keyword; }
<INITIAL>"instanceof"         { return tok_reserved_keyword; }
<INITIAL>"interface"          { return tok_reserved_keyword; }
<INITIAL>"is"                 { return tok_reserved_keyword; }
<INITIAL>"lambda"             { return tok_reserved_keyword; }
<INITIAL>"module"             { return tok_reserved_keyword; }
<INITIAL>"native"             { return tok_reserved_keyword; }
<INITIAL>"new"                { return tok_reserved_keyword; }
<INITIAL>"next"               { return tok_reserved_keyword; }
<INITIAL>"nil"                { return tok_reserved_keyword; }
<INITIAL>"not"                { return tok_reserved_keyword; }
<INITIAL>"or"                 { return tok_reserved_keyword; }
<INITIAL>"pass"               { return tok_reserved_keyword; }
<INITIAL>"public"             { return tok_reserved_keyword; }
<INITIAL>"print"              { return tok_reserved_keyword; }
<INITIAL>"private"            { return tok_reserved_keyword; }
<INITIAL>"protected"          { return tok_reserved_keyword; }
<INITIAL>"public"             { return tok_reserved_keyword; }
<INITIAL>"raise"              { return tok_reserved_keyword; }
<INITIAL>"redo"               { return tok_reserved_keyword; }
<INITIAL>"rescue"             { return tok_reserved_keyword; }
<INITIAL>"retry"              { return tok_reserved_keyword; }
<INITIAL>"register"           { return tok_reserved_keyword; }
<INITIAL>"return"             { return tok_reserved_keyword; }
<INITIAL>"self"               { return tok_reserved_keyword; }
<INITIAL>"sizeof"             { return tok_reserved_keyword; }
<INITIAL>"static"             { return tok_reserved_keyword; }
<INITIAL>"super"              { return tok_reserved_keyword; }
<INITIAL>"synchronized"       { return tok_reserved_keyword; }
<INITIAL>"then"               { return tok_reserved_keyword; }
<INITIAL>"this"               { return tok_reserved_keyword; }
<INITIAL>"throw"              { return tok_reserved_keyword; }
<INITIAL>"transient"          { return tok_reserved_keyword; }
<INITIAL>"try"                { return tok_reserved_keyword; }
<INITIAL>"undef"              { return tok_reserved_keyword; }
<INITIAL>"union"              { return tok_reserved_keyword; }
<INITIAL>"unless"             { return tok_reserved_keyword; }
<INITIAL>"unsigned"           { return tok_reserved_keyword; }
<INITIAL>"until"              { return tok_reserved_keyword; }
<INITIAL>"use"                { return tok_reserved_keyword; }
<INITIAL>"var"                { return tok_reserved_keyword; }
<INITIAL>"virtual"            { return tok_reserved_keyword; }
<INITIAL>"volatile"           { return tok_reserved_keyword; }
<INITIAL>"when"               { return tok_reserved_keyword; }
<INITIAL>"while"              { return tok_reserved_keyword; }
<INITIAL>"with"               { return tok_reserved_keyword; }
<INITIAL>"xor"                { return tok_reserved_keyword; }
<INITIAL>"yield"              { return tok_reserved_keyword; }
<INITIAL>"list"               { return tok_reserved_keyword; }
<INITIAL>"required"           { return tok_reserved_keyword; }
<INITIAL>"optional"           { return tok_reserved_keyword; }
<INITIAL>"async"              { return tok_reserved_keyword; }
<INITIAL>"service"            { return tok_reserved_keyword; }
<INITIAL>"throws"             { return tok_reserved_keyword; }
<INITIAL>"extends"            { return tok_reserved_keyword; }
<INITIAL>"exception"          { return tok_reserved_keyword; }
<INITIAL>"binary"             { return tok_reserved_keyword; }
<INITIAL>"bool"               { return tok_reserved_keyword; }
<INITIAL>"void"               { return tok_reserved_keyword; }
<INITIAL>"byte"               { return tok_reserved_keyword; }
<INITIAL>"namespace"          { return tok_reserved_keyword; }
<INITIAL>"typename"			  { return tok_reserved_keyword; }
<INITIAL>"switch"			  { return tok_reserved_keyword; }
<INITIAL>"case"               { return tok_reserved_keyword; }
<INITIAL>"true"               { return tok_reserved_keyword; }
<INITIAL>"false"              { return tok_reserved_keyword; }

<INITIAL>{identifier}    	  {return tok_identifier;															}

<*>{anychar}				  { goto restart;					 }	
*/	
}
