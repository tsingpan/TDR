#ifndef _H_JSON_PARSER
#define _H_JSON_PARSER

#include <stdarg.h>
#include <stdio.h>
#include "hotscript/script_parser.h"

#include "hotpot/hp_platform.h"




#include "hotscript/hotobject.h"
#include "hotscript/hotlex.h"
#include "hotprotocol/hp_abstract_reader.h"
#include "hotprotocol/hp_abstract_writer.h"


#define MAX_STACK_DEEP 1024





typedef struct tagJSON_STACK_NODE
{
	hpint32 first_element_index;
	hpint32 last_element_index;
}JSON_STACK_NODE;



#define MAX_INCLUDE_FILE_LEVEL 1024
typedef struct tagJSON_PARSER_STACK_NODE
{
	FILE *f;
}JSON_PARSER_STACK_NODE;

#define MAX_BUFF_SIZE 1024

typedef struct tagJSON_PARSER JSON_PARSER;
struct tagJSON_PARSER
{
	SCANNER_STACK scanner_stack;
	SCRIPT_PARSER *sp;

	hpint32 result;
	hpuint32 stack_num;
	JSON_PARSER_STACK_NODE stack[MAX_INCLUDE_FILE_LEVEL];

	HPAbstractWriter *writer;
	HPAbstractReader *reader;
};

hpint32 json_parser(JSON_PARSER *self, const char* file_name, HotObject *obj, SCRIPT_PARSER *sp);

hpint32 json_parser_str(JSON_PARSER *self, const char* str, hpuint32 str_len, HotObject *obj, SCRIPT_PARSER *sp);

#endif//_H_JSON_PARSER

