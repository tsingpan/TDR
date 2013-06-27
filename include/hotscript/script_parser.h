#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hot_vm.h"
#include "hotscript/hotlex.h"

typedef struct _SP_NODE
{
	HPVar var;
	HotOp *op;
}SP_NODE;


typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	SCANNER_STACK scanner_stack;

	hpint32 result;
	HotOpArr hotoparr;


	HPAbstractReader *reader;
	HotVM hotvm;
};

hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, HPAbstractReader *reader, 
					  void *user_data, vm_user_putc uputc);

hpint32 script_parser_str(SCRIPT_PARSER *self, const char* script, const char *script_limit, 
						  HPAbstractReader *reader, void *user_data, vm_user_putc uputc);



hpint32 hotscript_do_text(SCANNER_STACK *super, const SP_NODE *text);

hpint32 hotscript_do_literal(SCANNER_STACK *super, const SP_NODE *text);

hpint32 hotscript_do_push(SCANNER_STACK *super, const SP_NODE *prefix, SP_NODE *name);

hpint32 hotscript_do_push_index(SCANNER_STACK *super, SP_NODE *index);

hpint32 hotscript_do_pop_index(SCANNER_STACK *super, SP_NODE *index);

hpint32 hotscript_do_pop(SCANNER_STACK *super, SP_NODE *id);

hpint32 hotscript_do_echo_trie(SCANNER_STACK *super);


#endif//_H_SCRIPT_PARSER

