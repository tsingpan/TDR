#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"
#include "datrie/trie.h"

typedef enum _SYMBOL_TYPE
{
	EN_HST_VALUE = 0,		//const and enumdef
	EN_HST_TYPEDEF = 1,
	EN_HST_PARAMETER = 2,
	EN_HST_FIELD = 3,
	EN_HST_ENUM = 4,
	EN_HST_STRUCT = 5,
	EN_HST_UNION = 6,
}SYMBOL_TYPE;

typedef union _SYMBOL_BODY
{
	ST_TYPEDEF type;
	ST_VALUE val;

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

#define MAX_SYMBOL_LIST_NUM 65536
typedef struct _PARSER_SYMBOLS
{
	Trie *symbols;

	tuint32 symbol_list_num;
	SYMBOL symbol_list[MAX_SYMBOL_LIST_NUM];
}SYMBOLS;



void symbols_init(SYMBOLS *self);

void symbols_fini(SYMBOLS *self);

const SYMBOL* symbols_search(SYMBOLS *self, const char* name, const char* preffix);

void symbols_save(SYMBOLS *self, const char *name, const SYMBOL *symbol, const char* preffix);


const ST_SIMPLE_TYPE* symbols_get_real_type(SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type);

const ST_VALUE* symbols_get_real_value(SYMBOLS *self, const ST_VALUE* sn_value);

void symbols_add_Typedef(SYMBOLS *self, const ST_TYPEDEF *pn_typedef);

void symbols_add_Const(SYMBOLS *self, const ST_Const *pn_const);

void symbols_add_Enum(SYMBOLS *self, const ST_ENUM *pn_enum);

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def);



void symbols_add_Parameter(SYMBOLS *self, const ST_Parameter *pn_parameter);

void symbols_add_Field(SYMBOLS *self, const ST_FIELD *pn_field);

void symbols_add_Struct(SYMBOLS *self, const ST_STRUCT *de_struct);

void symbols_add_Union(SYMBOLS *self, const ST_UNION *de_union);

#endif //_H_SYMBOLS
