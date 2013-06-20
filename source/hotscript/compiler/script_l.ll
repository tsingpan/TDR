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
%x ST_IN_SCRIPTING
%x ST_INCLUDE

%option extra-type = "yyscan_t*"

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
intconstant		([0-9]+)
file_name		("<"[^>]*">")
comment			("//"[^\n]*)
unixcomment		("#"[^\n]*)
sillycomm		("/*""*"*"*/")
multicomm		("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
symbol			(["$""#"\[\]"*"\{\}])
newline			("\r"|"\n"|"\r\n")
whitespace		([ \n\r\t]+)
literal_begin	(['\"])
any_char		(.)
%%

#首先跳过注释
<ST_IN_SCRIPTING>{comment}				{ /* do nothing */																}
<ST_IN_SCRIPTING>{sillycomm}			{ /* do nothing */																}
<ST_IN_SCRIPTING>{multicomm}			{ /* do nothing */																}
<ST_IN_SCRIPTING>{whitespace}			{ /* do nothing */																}


#然后读取关键字
<INITIAL>"<%"								{ BEGIN ST_IN_SCRIPTING;													}
<ST_IN_SCRIPTING>"%>"						{ BEGIN INITIAL;															}
<ST_IN_SCRIPTING>{symbol}					{ return yytext[0];															}



<ST_IN_SCRIPTING>"#include"					{ BEGIN ST_INCLUDE; return tok_include;											}
<ST_INCLUDE>{file_name} {
	char c;
	hpuint32 i;
	hpuint32 len;
	for(c = input(*yyextra); c != ';'; c = input(*yyextra))
	{
		if(c == EOF)
		{
			yyterminate();
		}
	}

	len = strlen(yytext);
	if((len <= 2) || (len >= MAX_TOKEN_LENGTH))
	{
		yyterminate();
	}
	strncpy(yylval->file_name, yytext + 1, MAX_TOKEN_LENGTH);
	yylval->file_name[len - 2] = 0;
	//这里切缓存
	script_open_file(yyextra, yylval->file_name);
	BEGIN INITIAL; 
	return tok_file_name;
}


<ST_IN_SCRIPTING>{intconstant}			{ yylval->ui64 = strtoull(yytext, NULL, 10); return tok_integer;}
<ST_IN_SCRIPTING>{identifier}			{ strncpy(yylval->identifier, yytext, MAX_TOKEN_LENGTH); return tok_identifier;}
<<EOF>>	{ 
	if(script_close_file(yyextra) == E_HP_NOERROR)
	{
		BEGIN ST_IN_SCRIPTING; 
	}	
	else
	{
		yyterminate();
	}
}


#检测保留字
<ST_IN_SCRIPTING>"BEGIN"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"END"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__CLASS__"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__DIR__"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__FILE__"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__FUNCTION__"       { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__LINE__"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__METHOD__"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"__NAMESPACE__"      { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"abstract"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"alias"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"and"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"args"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"as"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"assert"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"begin"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"break"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"catch"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"class"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"clone"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"continue"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"declare"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"def"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"default"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"del"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"delete"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"do"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"dynamic"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elif"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"else"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elseif"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"elsif"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"end"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"enddeclare"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endfor"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endforeach"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endif"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endswitch"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"endwhile"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"ensure"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"except"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"exec"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"finally"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"float"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"for"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"foreach"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"function"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"global"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"goto"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"if"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"implements"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"import"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"in"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"inline"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"instanceof"         { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"interface"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"is"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"lambda"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"module"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"native"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"new"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"next"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"nil"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"not"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"or"                 { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"pass"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"public"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"print"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"private"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"protected"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"public"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"raise"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"redo"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"rescue"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"retry"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"register"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"return"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"self"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"sizeof"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"static"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"super"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"synchronized"       { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"then"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"this"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"throw"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"transient"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"try"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"undef"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"union"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unless"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unsigned"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"until"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"use"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"var"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"virtual"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"volatile"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"when"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"while"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"with"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"xor"                { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"yield"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"list"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"required"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"optional"           { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"async"              { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"service"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"throws"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"extends"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"exception"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"binary"             { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"include"            { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"bool"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"void"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"byte"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"namespace"          { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"union"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"struct"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"if"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"const"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"enum"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"case"               { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"typename"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"unique"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"typedef"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"true"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"false"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"lower_bound"		  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"upper_bound"		  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"switch"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"vector"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"string"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int8"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int16"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int32"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"int64"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint8"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint16"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint32"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"uint64"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"char"				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"double"			  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"=="				  { hotscript_reserved_keyword(yytext); }
<ST_IN_SCRIPTING>"!="				  { hotscript_reserved_keyword(yytext); }

<ST_IN_SCRIPTING>{literal_begin} {
  char mark = yytext[0];
  hpuint32 len = 0;
  for(;;)
  {
    int ch = input(*yyextra);
    switch (ch) {
      case EOF:
        //yyerror("end of file while read string at %d\n", yylineno);
        exit(1);
      case '\n':
        //yyerror("end of line while read string at %d\n", yylineno - 1);
        exit(1);
      case '\\':
        ch = input(*yyextra);
        switch (ch) {
          case 'r':
			yylval->literal[len++] = '\r';
            continue;
          case 'n':
			yylval->literal[len++] = '\n';
            continue;
          case 't':
			yylval->literal[len++] = '\t';
            continue;
          case '"':
			yylval->literal[len++] = '"';
            continue;
          case '\'':
			yylval->literal[len++] = '\'';
            continue;
          case '\\':
			yylval->literal[len++] = '\\';
            continue;
          default:
            //yyerror("bad escape character\n");
            return -1;
        }
        break;
      default:
        if (ch == mark)
        {
			yylval->literal[len] = 0;
			return tok_literal;
        } else {
          yylval->literal[len++] = ch;
        }
    }
  }
}

#跳过没用的字符
<ST_IN_SCRIPTING>{newline}			     {/* do nothing */																}
<ST_IN_SCRIPTING>{any_char}			     {/* reutrn error?*/																}


#非脚本中的字符需要进行记录
<INITIAL>{newline}				 { printf("%s", yytext); yycolumn = 1;													}
<INITIAL>{any_char}			     { putchar(yytext[0]);

}



%%
