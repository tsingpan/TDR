#ifndef _H_HOTLEX
#define _H_HOTLEX

#include "hotpot/hp_platform.h"

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

typedef struct _SCANNER SCANNER;
struct _SCANNER
{
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

#define MAX_BUFF_SIZE 10240
#define MAX_SCANNER_STACK_DEEP 1024
#define MAX_FILE_NAME_LENGTH 128

typedef struct _SCANNER_STACK SCANNER_STACK;
typedef struct _SCANNER_STACK
{
	hpuint32 stack_num;
	SCANNER stack[MAX_SCANNER_STACK_DEEP];
	hpchar file_name_list[MAX_SCANNER_STACK_DEEP][MAX_FILE_NAME_LENGTH];
	hpuint32 file_name_list_num;

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_BUFF_SIZE];	
};

hpint32 scanner_init(SCANNER *self, const char *yy_start, const char *yy_limit, int state);
hpint32 scanner_fini(SCANNER *self);

hpint32 scanner_process(SCANNER *sp);
SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self);
hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state);
hpint32 scanner_stack_push(SCANNER_STACK *self, const char *yy_start, const char *yy_limit, int state);
hpint32 scanner_stack_pop(SCANNER_STACK *self);
hpint32 scanner_stack_init(SCANNER_STACK *self);
hpuint32 scanner_stack_get_num(SCANNER_STACK *self);

#endif//_H_HOTLEX

