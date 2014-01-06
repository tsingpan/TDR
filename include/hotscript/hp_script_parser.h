#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "platform/tlibc_platform.h"
#include "hotlib/hp_error_code.h"
#include "hotlib/hp_error_msg.h"
#include "hotlib/hp_script_lex.h"
#include "hotscript/hp_script_op.h"


typedef struct _SP_NODE
{
	int token;
	tbytes bytes;
	tuint32 ui32;
	char file_name[HS_MAX_NAME_LENGTH];

	//以下几个玩意记录需要回溯处理的指令	
	tuint32 call_field_begin_index;
	tuint32 field_begin_index;
	tuint32 vector_begin_index;
	tuint32 vector_item_begin_index;
	tuint32 echo_field_index;	
}SP_NODE;


typedef enum _E_SP_NODE_TYPE
{
	E_SP_OBJECT,
	E_SP_ARRAY,
}E_SP_NODE_TYPE;

#define MAX_SP_STACK_DEEP 1024
typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	SCANNER_STACK scanner_stack;
	E_SP_NODE_TYPE stack[MAX_SP_STACK_DEEP];
	tuint32 stack_num;

	HotOp *hotop;
	size_t next_op;
	size_t op_size;
};

tint32 script_malloc(SCRIPT_PARSER *self, const char* file_name, const char* root_dir, const char *work_dir,
					  HotOp *hotop, size_t hotop_size
					  );

void hotscript_do_text(SCRIPT_PARSER *self, const YYLTYPE *yylloc, const SP_NODE *text);

void hotscript_do_literal(SCRIPT_PARSER *self, const YYLTYPE *yylloc, const SP_NODE *text);

void hotscript_do_field_begin(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *identifier);

void hotscript_do_field_end(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *identifier);

void hotscript_do_vector_begin(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *identifier);

void hotscript_do_vector_end(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *identifier);

void hotscript_do_field(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *current);

void hotscript_do_echo_field(SCRIPT_PARSER *self, const YYLTYPE *yylloc, SP_NODE *identifier);

void yyscripterror(const YYLTYPE *yylloc, SCANNER_STACK *ss, char *s, ...);

#endif//_H_SCRIPT_PARSER

