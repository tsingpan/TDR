#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hot_vm.h"
#include "script_y.h"
#include "hotscript/hotlex.h"


typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	SCANNER_STACK scanner_stack;

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

