#ifndef _H_XML_TREE
#define _H_XML_TREE

#include "hotpot/hp_platform.h"

#include "globals.h"

typedef struct tagXML_ELEMENT
{
	char name[MAX_TOKEN_LENGTH];
	char text[MAX_TOKEN_LENGTH];
	hpuint32	first_child_index;
	hpuint32	next_sibling_index;
}XML_ELEMENT;

#define MAX_ELEMENT_LIST_NUM 1024
typedef struct tagXML_TREE
{
		hpuint32 element_list_num;
		XML_ELEMENT element_list[MAX_ELEMENT_LIST_NUM];
}XML_TREE;

#endif//_H_XML_TREE

