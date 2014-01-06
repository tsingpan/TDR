#ifndef _H_HOTDATA_PARSER
#define _H_HOTDATA_PARSER


#include "platform/tlibc_platform.h"

#include <stdio.h>
#include "hp_script_lex.h"
#include "protocol/tlibc_abstract_writer.h"

#include "datrie/trie.h"
#include "hp_error_msg.h"


#include "parser_node.h"

typedef struct _DATA_PARSER DATA_PARSER;
struct _DATA_PARSER 
{
	SCANNER_STACK scanner_stack;
		
	char file_name[MAX_FILE_NAME_LENGTH];

	//·ûºÅ±í
	Trie *hotdata_symbols;
	char domain[MAX_FILE_NAME_LENGTH];

	PN_DEFINITION pn_definition;
	PN_FIELD_LIST pn_field_list;
	PN_FIELD pn_field;
	tuint32 definition_list_num;
	tbool in_union;
	tbool in_struct;
};

void data_parser_init(DATA_PARSER *self, const char* root_dir);

tint32 data_parser(DATA_PARSER *self, const char* file_name);

//do
void dp_do_Definition(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_DEFINITION *pn_definition);

#endif//_H_HOTDATA_PARSER

