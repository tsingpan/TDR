#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"

#include "platform/tlibc_platform.h"

#include "lib/tlibc_hash.h"

typedef enum _SYMBOL_TYPE
{
	EN_HST_VALUE = 0,		//const and enumdef
	EN_HST_TYPEDEF = 1,
	EN_HST_PARAMETER = 2,
	EN_HST_FIELD = 3,
	EN_HST_UNION_FIELD = 4,
	EN_HST_ENUM = 5,
	EN_HST_STRUCT = 6,
	EN_HST_UNION = 7,
}SYMBOL_TYPE;

typedef struct _ST_SYMBOL_ENUM
{
	tuint32 enum_def_list_num;
	int unique;
	tchar name[TLIBC_MAX_IDENTIFIER_LENGTH];
}ST_SYMBOL_ENUM;

typedef struct _ST_UNION_SYMBOL ST_UNION_SYMBOL;
struct _ST_UNION_SYMBOL
{
	ST_Parameters para;
	tuint32 union_field_list_num;
};

typedef union _SYMBOL_BODY
{
	ST_TYPEDEF type;
	ST_VALUE val;
	ST_UNION_SYMBOL union_symbol;
	ST_FIELD field;
	ST_SYMBOL_ENUM symbol_enum;

	ST_Parameter para;		
	tuint32 struct_field_list_num;
}SYMBOL_BODY;

#define MAX_SYMBOL_KEY_LENGTH TLIBC_MAX_IDENTIFIER_LENGTH * 2
typedef struct _SYMBOL
{
	tlibc_hash_head_t hash_head;
	char key[MAX_SYMBOL_KEY_LENGTH];
	tuint32 key_len;


	SYMBOL_TYPE type;
	SYMBOL_BODY body;
}SYMBOL;

#define MAX_SYMBOL_LIST_NUM 65536
#define MAX_SYMBOL_BUCKETS 65536
typedef struct _PARSER_SYMBOLS
{
	tlibc_hash_bucket_t symbol_buckets[MAX_SYMBOL_BUCKETS];
	tlibc_hash_t symbols;
	

	const char *enum_name;
	const char *union_name;
	const char *struct_name;
	const ST_Parameter *para;

	tuint32 symbol_list_num;
	SYMBOL symbol_list[MAX_SYMBOL_LIST_NUM];
}SYMBOLS;



void symbols_init(SYMBOLS *self);

void symbols_clear(SYMBOLS *self);

const SYMBOL* symbols_search(const SYMBOLS *self, const char* preffix, const char* name);

void symbols_save(SYMBOLS *self, const char* preffix, const char *name, SYMBOL *symbol);


const ST_SIMPLE_TYPE* symbols_get_real_type(const SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type);

const ST_VALUE* symbols_get_real_value(const SYMBOLS *self, const ST_VALUE* sn_value);

void symbols_add_Typedef(SYMBOLS *self, const ST_TYPEDEF *pn_typedef);

void symbols_add_Const(SYMBOLS *self, const ST_Const *pn_const);

void symbols_add_Enum(SYMBOLS *self, const ST_ENUM *pn_enum);

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def);

void symbols_add_Parameters(SYMBOLS *self, const ST_Parameters *parameters, const char *identifier);


void symbols_add_UnionField(SYMBOLS *self, const ST_UNION_FIELD *pn_union_field);

void symbols_add_Field(SYMBOLS *self, const ST_FIELD *pn_field);

void symbols_add_Struct(SYMBOLS *self, const ST_STRUCT *de_struct);

void symbols_add_Union(SYMBOLS *self, const ST_UNION *de_union);

#endif //_H_SYMBOLS
