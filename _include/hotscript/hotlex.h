#ifndef _H_HOTLEX
#define _H_HOTLEX

#include "hotpot/hp_platform.h"


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
	const YYCTYPE *yy_last;
	const YYCTYPE *yy_cursor;
	const YYCTYPE *yy_limit;
	const YYCTYPE *yy_text;
	const YYCTYPE *yy_marker;
	const YYCTYPE *yy_start;
	hpuint32 yy_leng;

	hpuint32 yylineno;
	hpuint32 yycolumn;
};

#define MAX_BUFF_SIZE 1000000
#define MAX_SCANNER_STACK_DEEP 1024
#define MAX_RESULT_STRING_LENGTH 1024
#define MAX_INCLUDE_PATH 1024

typedef struct _SCANNER_STACK SCANNER_STACK;
typedef struct _SCANNER_STACK
{
	hpuint32 stack_num;
	SCANNER stack[MAX_SCANNER_STACK_DEEP];

	char include_path[MAX_INCLUDE_PATH][HP_MAX_FILE_PATH_LENGTH];
	hpuint32 include_path_tail;

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_BUFF_SIZE];
};

hpint32 scanner_init(SCANNER *self, const char *yy_start, const char *yy_limit, int state, const char *file_name);
hpint32 scanner_fini(SCANNER *self);

hpint32 scanner_process(SCANNER *sp);
SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self);
hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state);
hpint32 scanner_stack_push(SCANNER_STACK *self, const char *yy_start, const char *yy_limit, int state);
hpint32 scanner_stack_pop(SCANNER_STACK *self);
hpint32 scanner_stack_init(SCANNER_STACK *self);
hpuint32 scanner_stack_get_num(SCANNER_STACK *self);
hpint32 scanner_stack_add_path(SCANNER_STACK *self, const char* path);

#endif//_H_HOTLEX

