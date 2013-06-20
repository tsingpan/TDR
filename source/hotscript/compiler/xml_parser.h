#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"

#include "xml_tree.h"

#include "xml_y.h"
#include "xml_l.h"


#define MAX_STACK_DEEP 1024

typedef struct tagXML_STACK_NODE
{
	hpint32 first_element_index;
	hpint32 last_element_index;
}XML_STACK_NODE;



#define MAX_INCLUDE_FILE_LEVEL 1024
typedef struct tagXML_PARSER_STACK_NODE
{
	FILE *f;
	YY_BUFFER_STATE bs;
}XML_PARSER_STACK_NODE;


typedef struct tagXML_PARSER XML_PARSER;
struct tagXML_PARSER
{
	yyscan_t scanner;
	hpint32 result;

	XML_TREE tree;											//这是一个xml文件

	XML_ELEMENT element;
	hpuint32 xml_stack_num;
	XML_STACK_NODE xml_stack[MAX_STACK_DEEP];

	hpuint32 stack_num;
	XML_PARSER_STACK_NODE stack[MAX_INCLUDE_FILE_LEVEL];
};

hpint32 xml_parser(XML_PARSER *self, const char* file_name);

#endif//_H_XML_PARSER

