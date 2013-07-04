#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"
#include "hot_vm.h"
#include "hotscript/hotlex.h"

typedef struct _SP_NODE
{
	INDEX_TYPE it;
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

hpint32 hotscript_do_field_begin(SCANNER_STACK *super, const SP_NODE *prefix, SP_NODE *name);

hpint32 hotscript_do_field_end(SCANNER_STACK *super, SP_NODE *id);

hpint32 hotscript_do_vector_begin(SCANNER_STACK *super, const SP_NODE *index);

hpint32 hotscript_do_vector_seek(SCANNER_STACK *super, SP_NODE *index);

hpint32 hotscript_do_vector_end(SCANNER_STACK *super, SP_NODE *index);

hpint32 hotscript_do_field(SCANNER_STACK *super);

hpint32 hotscript_do_jmp(SCANNER_STACK *super);


#endif//_H_SCRIPT_PARSER

