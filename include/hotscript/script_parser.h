#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hot_vm.h"
#include "script_y.h"


#define MAX_INCLUDE_FILE_LEVEL 1024
typedef struct tagSCRIPT_PARSER_STACK_NODE
{
	FILE *f;
	const char *buff;
	hpuint32 buff_size;
}SCRIPT_PARSER_STACK_NODE;

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
typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	hpuint32 stack_num;
	SCANNER stack[MAX_SCANNER_STACK_DEEP];

	char buff_[MAX_BUFF_SIZE];
	hpuint32 buff_size;

	hpint32 result;
	HotOpArr hotoparr;


	HPAbstractReader *reader;
	HotVM hotvm;
};

hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, void *user_data, vm_user_putc uputc);
hpint32 script_parser_str(SCRIPT_PARSER *self, const char* script, size_t script_size, HPAbstractReader *reader, void *user_data, vm_user_putc uputc);



hpint32 hotscript_do_text(SCRIPT_PARSER *self, const SNODE *text);

hpint32 hotscript_do_literal(SCRIPT_PARSER *self, const SNODE *text);

hpint32 hotscript_do_push(SCRIPT_PARSER *self, const SNODE *prefix, SNODE *name);

hpint32 hotscript_do_push_index(SCRIPT_PARSER *self, SNODE *index);

hpint32 hotscript_do_pop(SCRIPT_PARSER *self, SNODE *id);

hpint32 hotscript_do_pop_index(SCRIPT_PARSER *self, SNODE *index);

hpint32 hotscript_do_echo_trie(SCRIPT_PARSER *self);


#endif//_H_SCRIPT_PARSER

