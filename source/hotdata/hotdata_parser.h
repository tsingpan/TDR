#ifndef _H_HOTDATA_PARSER
#define _H_HOTDATA_PARSER


#include "hotpot/hp_platform.h"
#include <stdarg.h>
#include <stdio.h>
#include "hotscript/hotlex.h"
#include "hotpot/hp_value.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "datrie/trie.h"
#include "language/language.h"


#include "parser_node.h"

#define MAX_SYMBOL_LIST_NUM 1024
#define MAX_ERROR_NUM 1024
typedef struct _DATA_PARSER DATA_PARSER;
struct _DATA_PARSER 
{
	SCANNER_STACK scanner_stack;
	HPAbstractWriter *writer;
	
	hpint32 result[MAX_ERROR_NUM];
	char result_str[MAX_ERROR_NUM][MAX_RESULT_STRING_LENGTH];
	hpuint32 result_num;

	const LanguageLib *language_lib;

/*
	hpuint32 symbol_list_num;
	SyntacticSymbol symbol_list[MAX_SYMBOL_LIST_NUM];
*/
	char file_name[MAX_FILE_NAME_LENGTH];

	//·ûºÅ±í
	Trie *symbols;


	PN_DEFINITION pn_definition;
	hpuint32 definition_list_num;
};

hpint32 data_parser_init(DATA_PARSER *self);

hpint32 data_parser(DATA_PARSER *self, const char* file_name, HPAbstractWriter *writer, const LanguageLib *language_lib);

//do
void dp_do_Definition(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_DEFINITION *pn_definition);
/*
//check
void dp_check_constant_identifier(DATA_PARSER *self, const YYLTYPE *yylloc,const SyntacticNode* sn_type, const hpbytes sn_tok_identifier);

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const SN_TYPE* sn_type, const hpbytes* sn_identifier, const SN_VALUE* sn_value);
*/

#endif//_H_HOTDATA_PARSER

