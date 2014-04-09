#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"

#include "platform/tlibc_platform.h"
#include "parse/scanner.h"

#include "core/tlibc_hash.h"

typedef enum td_symbol_type_e
{
	EN_HST_VALUE = 0,		//const and enumdef
	EN_HST_TYPEDEF = 1,
	EN_HST_FIELD = 2,
	EN_HST_UNION_FIELD = 3,
	EN_HST_ENUM = 4,
	EN_HST_STRUCT = 5,
	EN_HST_UNION = 6,
}td_symbol_type_t;

typedef struct td_symbol_enum_s
{
	uint32_t enum_def_list_num;
	char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
}td_symbol_enum_t;

typedef struct ts_symbol_union_s ts_symbol_union_t;
struct ts_symbol_union_s
{
	ST_Parameters para;
	uint32_t union_field_list_num;
};

typedef union td_symbol_body_u
{
	ST_TYPEDEF type;
	ST_VALUE val;
	ts_symbol_union_t union_symbol;
	ST_FIELD field;
	td_symbol_enum_t symbol_enum;

	uint32_t struct_field_list_num;
}td_symbol_body_t;

#define MAX_SYMBOL_KEY_LENGTH TLIBC_MAX_LENGTH_OF_IDENTIFIER * 2
typedef struct td_symbol_s
{
	tlibc_hash_head_t hash_head;
	char key[MAX_SYMBOL_KEY_LENGTH];
	uint32_t key_len;

	YYLTYPE yylloc;

	td_symbol_type_t type;
	td_symbol_body_t body;
}td_symbol_t;

#define MAX_SYMBOL_LIST_NUM 65536
#define MAX_SYMBOL_BUCKETS 65536
typedef struct td_symbols_s
{
	tlibc_hash_bucket_t symbol_buckets[MAX_SYMBOL_BUCKETS];
	tlibc_hash_t symbols;
	

	const char *enum_name;
	const char *union_name;
	const char *struct_name;
	const ST_Parameter *para;

	uint32_t symbol_list_num;
	td_symbol_t symbol_list[MAX_SYMBOL_LIST_NUM];
}td_symbols_t;



void symbols_init(td_symbols_t *self);

void symbols_clear(td_symbols_t *self);

const td_symbol_t* symbols_search(const td_symbols_t *self, const char* preffix, const char* name);

void symbols_save(td_symbols_t *self, const char* preffix, const char *name, td_symbol_t *symbol);

//过滤typedef
const ST_SIMPLE_TYPE* symbols_get_real_type(const td_symbols_t *self, const ST_SIMPLE_TYPE* sn_type);

//过滤常量赋值
const ST_VALUE* symbols_get_real_value(const td_symbols_t *self, const ST_VALUE* sn_value);


void symbols_add_Typedef(td_symbols_t *self, const YYLTYPE *yylloc, const ST_TYPEDEF *pn_typedef);

void symbols_add_Enum(td_symbols_t *self, const YYLTYPE *yylloc, const ST_ENUM *pn_enum);

void symbols_add_UnionField(td_symbols_t *self, const YYLTYPE *yylloc, const ST_UNION_FIELD *pn_union_field);

void symbols_add_Field(td_symbols_t *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void symbols_add_Struct(td_symbols_t *self, const YYLTYPE *yylloc, const ST_STRUCT *de_struct);

void symbols_add_Union(td_symbols_t *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

void symbols_add_EnumDef(td_symbols_t *self, const YYLTYPE *yylloc, const ST_ENUM_DEF *pn_enum_def);

void symbols_add_Const(td_symbols_t *self, const YYLTYPE *yylloc, const ST_Const *pn_const);


#endif //_H_SYMBOLS
