#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#include <stdarg.h>
#include <stdio.h>


#include "hotpot/hp_platform.h"


#include "json_y.h"

#include "hotobject.h"
#include "hotscript/hotlex.h"


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

	hpint32 result;
	hpuint32 stack_num;
	JSON_PARSER_STACK_NODE stack[MAX_INCLUDE_FILE_LEVEL];
	HPAbstractWriter *writer;
};

hpint32 json_parser(JSON_PARSER *self, const char* file_name, HPAbstractWriter *writer);

#endif//_H_XML_PARSER

