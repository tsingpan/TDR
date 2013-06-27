#ifndef _H_HOTLEX
#define _H_HOTLEX

#include "hotpot/hp_platform.h"

typedef struct _SCANNER SCANNER;
struct _SCANNER
{
	int yy_state;
	const char *yy_last;
	const char *yy_cursor;
	const char *yy_limit;
	const char *yy_text;
	const char *yy_marker;
	const char *buff;
	hpuint32 buff_size;

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
	char file_name_list[MAX_SCANNER_STACK_DEEP][MAX_FILE_NAME_LENGTH];
	hpuint32 file_name_list_num;

	char buff[MAX_BUFF_SIZE];
	hpuint32 buff_size;
};

hpint32 scanner_init(SCANNER *self, const char *str, const hpint32 str_size, int state);
hpint32 scanner_fini(SCANNER *self);

hpint32 scanner_process(SCANNER *sp);
SCANNER *scanner_stack_get_scanner(SCANNER_STACK *self);
hpint32 scanner_stack_push_file(SCANNER_STACK *self, const char *file_name, int state);
hpint32 scanner_stack_push(SCANNER_STACK *self, const char *str, size_t str_size, int state);
hpint32 scanner_stack_pop(SCANNER_STACK *self);
hpint32 scanner_stack_init(SCANNER_STACK *self);
hpuint32 scanner_stack_get_num(SCANNER_STACK *self);

#endif//_H_HOTLEX

