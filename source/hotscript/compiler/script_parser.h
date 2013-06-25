#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hotscript/hotobject.h"
#include "hot_vm.h"

#include "script_y.h"
#include "script_l.h"

#define MAX_INCLUDE_FILE_LEVEL 1024
typedef struct tagSCRIPT_PARSER_STACK_NODE
{
	FILE *f;
	YY_BUFFER_STATE bs;
}SCRIPT_PARSER_STACK_NODE;
typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	yyscan_t scanner;
	hpint32 result;
	HotOpArr hotoparr;

	hpuint32 stack_num;
	SCRIPT_PARSER_STACK_NODE stack[MAX_INCLUDE_FILE_LEVEL];

	HPAbstractReader *reader;
};

hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader);

hpint32 script_open_file(yyscan_t *super, const char *file_name);

hpint32 script_close_file(yyscan_t *super);


hpint32 hotscript_do_text(SCRIPT_PARSER *self, const SNODE *text);

hpint32 hotscript_do_literal(SCRIPT_PARSER *self, const SNODE *text);

hpint32 hotscript_do_push(SCRIPT_PARSER *self, const SNODE *prefix, SNODE *name);

hpint32 hotscript_do_push_index(SCRIPT_PARSER *self, SNODE *index);

hpint32 hotscript_do_pop(SCRIPT_PARSER *self, SNODE *id);

hpint32 hotscript_do_pop_index(SCRIPT_PARSER *self, SNODE *index);

hpint32 hotscript_do_echo_trie(SCRIPT_PARSER *self);


#endif//_H_SCRIPT_PARSER

