#ifndef _H_SCANNER
#define _H_SCANNER

#include "platform/tlibc_platform.h"
#include "error/error_msg_types.h"

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
#define YYCURSOR  self->yy_cursor
#define YYLIMIT   self->yy_limit
#define YYMARKER self->yy_marker
#define YYGETCONDITION()  self->yy_state
#define YYSETCONDITION(s) self->yy_state = s

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()

#define yytext self->yy_text
#define yyleng self->yy_leng

#define MAX_FILE_NAME_LENGTH 128
typedef struct _SCANNER SCANNER;
struct _SCANNER
{
	char file_name[MAX_FILE_NAME_LENGTH];
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

void scanner_init(SCANNER *self, char *yy_start, char *yy_limit, int state, const char *file_name);
void scanner_fini(SCANNER *self);
void scanner_process(SCANNER *sp);

void scanner_stack_init(SCANNER_STACK *self);
SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self);
tint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state);
tint32 scanner_stack_push(SCANNER_STACK *self, char *yy_start, char *yy_limit, int state);
tint32 scanner_stack_pop(SCANNER_STACK *self);
tuint32 scanner_stack_get_num(SCANNER_STACK *self);
void scanner_stack_set_root_dir(SCANNER_STACK *self, const char* path);
void scanner_stack_errorap(SCANNER_STACK *self, const YYLTYPE *yylloc, TD_ERROR_CODE result, const char *s, va_list ap);
void scanner_stack_error_halt(SCANNER_STACK *self, const YYLTYPE *yylloc, TD_ERROR_CODE result, ...);

#endif//_H_SCANNER

