#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"

#include "xml_tree.h"

#include "xml_y.h"
#include "xml_l.h"


#define MAX_STACK_DEEP 1024

typedef struct tagSTACK_NODE
{
	hpint32 first_element_index;
	hpint32 last_element_index;
}STACK_NODE;

typedef struct tagXML_PARSER XML_PARSER;
struct tagXML_PARSER
{
	yyscan_t scanner;
	YY_BUFFER_STATE bs;	
	hpint32 result;

	XML_TREE tree;											//这是一个xml文件

	XML_ELEMENT element;
	hpuint32 stack_num;
	STACK_NODE stack[MAX_STACK_DEEP];
};

hpint32 xml_parser(XML_PARSER *self, FILE *fin);

#endif//_H_XML_PARSER

