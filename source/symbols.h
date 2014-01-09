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

//back_searching为true时， 首先在当前域下搜索符号， 如果找不到那么到上一层进行搜索
const SYMBOL* symbols_search_string(SYMBOLS *self, const char* name, tbool back_searching);
const SYMBOL* symbols_search_identifier(SYMBOLS *self, const tbytes* tok_identifier, tbool back_searching);

//保存符号到当前域
tint32 symbols_save(SYMBOLS *self, const char *name, const SYMBOL *symbol);

//域开始
void symbols_domain_begin(SYMBOLS *self, const tbytes *tok_identifier);

//域结束
void symbols_domain_end(SYMBOLS *self);

const ST_TYPE* symbols_get_real_type(SYMBOLS *self, const ST_TYPE* sn_type);

const ST_VALUE* symbols_get_real_value(SYMBOLS *self, const ST_VALUE* sn_value);

void symbols_add_Const(SYMBOLS *self, const ST_Const *pn_const);

void symbols_add_Typedef(SYMBOLS *self, const ST_TYPEDEF *pn_typedef);

void symbols_add_Enum(SYMBOLS *self, const ST_ENUM *pn_enum);

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def);

#endif //_H_SYMBOLS
