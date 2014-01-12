#ifndef _H_SCANNER
#define _H_SCANNER

#include "platform/tlibc_platform.h"
#include "language/language_types.h"
#include "definition.h"
#include "error.h"
#include <stdarg.h>



typedef struct YYLTYPE
{
	const char *file_name;
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  scanner_get(self)->yy_cursor
#define YYLIMIT   scanner_get(self)->yy_limit
#define YYMARKER scanner_get(self)->yy_marker
#define YYGETCONDITION()  scanner_get(self)->yy_state
#define YYSETCONDITION(s) scanner_get(self)->yy_state = s

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()

#define yytext scanner_get(self)->yy_text
#define yyleng scanner_get(self)->yy_leng



typedef union _PARSER_VALUE
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
}PARSER_VALUE;


typedef struct _SCANNER SCANNER;
struct _SCANNER
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

//脚本文件最大10m
#define MAX_LEX_BUFF_SIZE 10000000
#define MAX_SCANNER_STACK_DEEP 1024

typedef struct _SCANNER_STACK SCANNER_STACK;
typedef struct _SCANNER_STACK
{
	tuint32 stack_num;
	SCANNER stack[MAX_SCANNER_STACK_DEEP];

	char include_path[TLIBC_MAX_FILE_PATH_LENGTH];

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_LEX_BUFF_SIZE];			//对于需要频繁解析的脚本， 固定大小的缓存要比malloc效率更好。	
};


void scanner_init(SCANNER_STACK *self);
void scanner_process(SCANNER_STACK *sp);
tint32 scanner_scan(SCANNER_STACK *self, YYLTYPE *yylloc, PARSER_VALUE * yylval);

SCANNER *scanner_get(SCANNER_STACK *self);
tint32 scanner_push(SCANNER_STACK *self, const char *file_name, int state);
tint32 scanner_pop(SCANNER_STACK *self);
tuint32 scanner_get_stack_deep(SCANNER_STACK *self);

void scanner_error(SCANNER_STACK *self, const YYLTYPE *yylloc, EN_TD_LANGUAGE_STRING result, ...);

void tdataerror(const YYLTYPE *yylloc, SCANNER_STACK *jp, const char *s, ...);
int tdatalex(PARSER_VALUE * yylval_param, YYLTYPE * yylloc_param , SCANNER_STACK *ss);


#endif//_H_SCANNER

