#ifndef _H_XML_PARSER
#define _H_XML_PARSER


#include "hotpot/hp_platform.h"
#include <stdarg.h>
#include <stdio.h>
#include "hotscript/hotlex.h"
#include "hotpot/hp_value.h"
#include "hotprotocol/hp_abstract_writer.h"

//语法节点的值
typedef struct _SyntacticNode SyntacticNode;
struct _SyntacticNode
{
	HPVar var;
};


typedef struct _DATA_PARSER DATA_PARSER;
struct _DATA_PARSER 
{
	SCANNER_STACK scanner_stack;
	HPAbstractWriter *writer;

	hpint32 result;
};

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer);

hpint32 dp_on_const(SCANNER_STACK *super, const SyntacticNode* sn_type, const SyntacticNode* sn_identifier, const SyntacticNode* sn_value);
#endif//_H_XML_PARSER

