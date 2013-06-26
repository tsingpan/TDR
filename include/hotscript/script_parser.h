#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotscript/hotobject.h"
#include "hot_vm.h"
#include "script_y.h"


#define MAX_INCLUDE_FILE_LEVEL 1024
typedef struct tagSCRIPT_PARSER_STACK_NODE
{
	FILE *f;
	const char *buff;
	hpuint32 buff_size;
}SCRIPT_PARSER_STACK_NODE;

#define MAX_BUFF_SIZE 102400
typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	int yy_state;
	unsigned char *yy_last;
	unsigned char *yy_cursor;
	unsigned char *yy_limit;
	unsigned char *yy_text;
	unsigned char *yy_marker;
	unsigned char buff[MAX_BUFF_SIZE];
	hpuint32 yy_leng;
	hpuint32 buff_size;

	hpuint32 yylineno;
	hpuint32 yycolumn;


	hpint32 result;
	HotOpArr hotoparr;

	hpuint32 stack_num;
	SCRIPT_PARSER_STACK_NODE stack[MAX_INCLUDE_FILE_LEVEL];

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

