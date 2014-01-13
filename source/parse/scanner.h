#ifndef _H_SCANNER
#define _H_SCANNER

#include "platform/tlibc_platform.h"
#include "language/language_types.h"
#include "definition.h"
#include "error.h"
#include <stdarg.h>

typedef struct _YYLTYPE
{
	const char *file_name;
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;

typedef union _SCANNER_TOKEN_VALUE
{
	ST_Import sn_import;
	ST_TYPE sn_type;
	ST_SIMPLE_TYPE sn_simple_type;
	ST_VALUE sn_value;	
	tbytes sn_tok_identifier;
	ST_Const sn_const;
	tdouble pn_tok_double;
	const char* sn_tok_unixcomment;
	tbool sn_bool;
	tuint64 sn_uint64;
	tint64 sn_int64;
	tdouble sn_d;
	tuint64 sn_hex_uint64;
	tint64 sn_hex_int64;
	tstring sn_string;
	tchar sn_char;
	SN_SIMPLE_TYPE sn_st;
	ST_ARGUMENTS sn_arguments;
	SN_CONTAINER_TYPE sn_ct;
	ST_TYPEDEF sn_typedef;
	ST_UNIX_COMMENT sn_unix_comment;
	ST_ENUM_DEF sn_enum_def;
	ST_Parameter sn_parameter;
	ST_Parameters sn_parameters;
	ST_CONDITION sn_condition;
	ST_UNION_FIELD sn_union_field;
	ST_FIELD sn_field;
}SCANNER_TOKEN_VALUE;

#define YYSTYPE SCANNER_TOKEN_VALUE
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  scanner_top(self)->yy_cursor
#define YYLIMIT   scanner_top(self)->yy_limit
#define YYMARKER scanner_top(self)->yy_marker
#define YYGETCONDITION()  scanner_top(self)->yy_state
#define YYSETCONDITION(s) scanner_top(self)->yy_state = s
#define yytext scanner_top(self)->yy_text
#define yyleng scanner_top(self)->yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()


typedef struct _SCANNER_CONTEXT SCANNER_CONTEXT;
struct _SCANNER_CONTEXT
{
	char file_name[TLIBC_MAX_FILE_PATH_LENGTH];
	int yy_state;
	YYCTYPE *yy_last;
	YYCTYPE *yy_cursor;
	YYCTYPE *yy_limit;
	YYCTYPE *yy_text;
	YYCTYPE *yy_marker;
	YYCTYPE *yy_start;
	tuint32 yy_leng;

	tuint32 yylineno;
	tuint32 yycolumn;
};

#define MAX_LEX_BUFF_SIZE 10000000
#define MAX_SCANNER_STACK_SIZE 1024
typedef struct _SCANNER SCANNER;
typedef struct _SCANNER
{
	tuint32 stack_num;
	SCANNER_CONTEXT stack[MAX_SCANNER_STACK_SIZE];

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_LEX_BUFF_SIZE];
};


void scanner_init(SCANNER *self);
void scanner_locate(SCANNER *self);
tint32 scanner_scan(SCANNER *self, YYLTYPE *yylloc, SCANNER_TOKEN_VALUE * yylval);
SCANNER_CONTEXT *scanner_top(SCANNER *self);
tint32 scanner_push(SCANNER *self, const char *file_name, int state);
void scanner_pop(SCANNER *self);
tuint32 scanner_size(SCANNER *self);

int tdatalex(SCANNER_TOKEN_VALUE * yylval_param, YYLTYPE * yylloc_param , SCANNER *self);



//此函数会调用exit
void scanner_error(SCANNER *self, const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, ...);
//此函数会调用exit
void tdataerror(const YYLTYPE *yylloc, SCANNER *self, const char *s, ...);

#endif//_H_SCANNER
