#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"

#include "platform/tlibc_platform.h"
#include "parse/scanner.h"

#include "core/tlibc_hash.h"

typedef enum symbol_type_e
{
	EN_HST_VALUE = 0,		//const and enumdef
	EN_HST_TYPEDEF = 1,
	EN_HST_FIELD = 2,
	EN_HST_UNION_FIELD = 3,
	EN_HST_ENUM = 4,
	EN_HST_STRUCT = 5,
	EN_HST_UNION = 6,
}symbol_type_t;

typedef struct symbol_enum_s
{
	uint32_t enum_def_list_num;
	char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}symbol_enum_t;

typedef struct symbol_union_s symbol_union_t;
struct symbol_union_s
{
	ST_Parameters para;
	uint32_t union_field_list_num;
};

typedef union symbol_body_u
{
	ST_TYPEDEF type;
	syn_value_t val;
	symbol_union_t union_symbol;
	ST_FIELD field;
	symbol_enum_t symbol_enum;

	uint32_t struct_field_list_num;
}symbol_body_t;

#define MAX_SYMBOL_KEY_LENGTH TLIBC_MAX_LENGTH_OF_IDENTIFIER * 2
typedef struct symbol_s
{
	tlibc_hash_head_t hash_head;
	char key[MAX_SYMBOL_KEY_LENGTH];
	uint32_t key_len;

	YYLTYPE yylloc;

	symbol_type_t type;
	symbol_body_t body;
}symbol_t;

#define MAX_SYMBOL_LIST_NUM 65536
#define MAX_SYMBOL_BUCKETS 65536
typedef struct symbols_s
{
	tlibc_hash_bucket_t symbol_buckets[MAX_SYMBOL_BUCKETS];
	tlibc_hash_t symbols;
	

	const char *enum_name;
	const char *union_name;
	const char *struct_name;
	const ST_Parameter *para;

	uint32_t symbol_list_num;
	symbol_t symbol_list[MAX_SYMBOL_LIST_NUM];
}symbols_t;



void symbols_init(symbols_t *self);

void symbols_clear(symbols_t *self);

const symbol_t* symbols_search(const symbols_t *self, const char* preffix, const char* name);

void symbols_save(symbols_t *self, const char* preffix, const char *name, symbol_t *symbol);

//过滤typedef
const syn_simple_type_t* symbols_get_real_type(const symbols_t *self, const syn_simple_type_t* sn_type);

//过滤常量赋值
const syn_value_t* symbols_get_real_value(const symbols_t *self, const syn_value_t* sn_value);


void symbols_add_Typedef(symbols_t *self, const YYLTYPE *yylloc, const ST_TYPEDEF *pn_typedef);

void symbols_add_Enum(symbols_t *self, const YYLTYPE *yylloc, const ST_ENUM *pn_enum);

void symbols_add_UnionField(symbols_t *self, const YYLTYPE *yylloc, const ST_UNION_FIELD *pn_union_field);

void symbols_add_Field(symbols_t *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void symbols_add_Struct(symbols_t *self, const YYLTYPE *yylloc, const ST_STRUCT *de_struct);

void symbols_add_Union(symbols_t *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

void symbols_add_EnumDef(symbols_t *self, const YYLTYPE *yylloc, const ST_ENUM_DEF *pn_enum_def);

void symbols_add_Const(symbols_t *self, const YYLTYPE *yylloc, const ST_Const *pn_const);


#endif //_H_SYMBOLS
