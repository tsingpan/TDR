#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"
#include "datrie/trie.h"

typedef enum _SYMBOL_TYPE
{
	EN_HST_VALUE = 0,
	EN_HST_TYPE = 1,
	EN_HST_PARAMETER = 2,
	EN_HST_FIELD = 3,
	EN_HST_ENUM = 4,
	EN_HST_STRUCT = 5,
	EN_HST_UNION = 6,
}SYMBOL_TYPE;

typedef union _SYMBOL_BODY
{
	ST_VALUE val;
	ST_TYPE type;
	ST_Parameter para;
	ST_FIELD field;
	tuint32 enum_def_list_num;
	tuint32 field_list_num;
}SYMBOL_BODY;

typedef struct _SYMBOL
{
	SYMBOL_TYPE type;
	SYMBOL_BODY body;
}SYMBOL;

#define MAX_DOMAIN_LENGTH 1024

typedef struct _PARSER_SYMBOLS
{
	Trie *symbols;
	char domain[MAX_DOMAIN_LENGTH];
}SYMBOLS;



void symbols_init(SYMBOLS *self);
void symbols_fini(SYMBOLS *self);
const SYMBOL* symbols_find_by_string(SYMBOLS *self, const char* name);
const SYMBOL* symbols_find(SYMBOLS *self, const tbytes* tok_identifier);
const SYMBOL* symbols_find_by_string_local(SYMBOLS *self, const char* name);
const SYMBOL* symbols_find_local(SYMBOLS *self, const tbytes* tok_identifier);
tint32 symbols_save_string(SYMBOLS *self, const char *name, const SYMBOL *symbol);
tint32 symbols_save(SYMBOLS *self, const tbytes *tok_identifier, const SYMBOL *symbol);
void symbols_domain_begin(SYMBOLS *self, const tbytes *tok_identifier);
void symbols_domain_end(SYMBOLS *self);

#endif //_H_SYMBOLS
