#ifndef _H_HOTDATA_PARSER
#define _H_HOTDATA_PARSER


#include "hotpot/hp_platform.h"
#include <stdarg.h>
#include <stdio.h>
#include "hotscript/hotlex.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "datrie/trie.h"
#include "hotpot/hp_error_msg.h"


#include "parser_node.h"

typedef struct _DATA_PARSER DATA_PARSER;
struct _DATA_PARSER 
{
	SCANNER_STACK scanner_stack;
	HPAbstractWriter *writer;
	
	char file_name[MAX_FILE_NAME_LENGTH];

	//·ûºÅ±í
	Trie *hotdata_symbols;
	char domain[MAX_FILE_NAME_LENGTH];


	PN_DEFINITION pn_definition;
	PN_FIELD_LIST pn_field_list;
	PN_FIELD pn_field;
	hpuint32 definition_list_num;
	hpbool in_union;
	hpbool in_struct;
};

hpint32 data_parser_init(DATA_PARSER *self);

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer);

//do
void dp_do_Definition(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_DEFINITION *pn_definition);

#endif//_H_HOTDATA_PARSER

