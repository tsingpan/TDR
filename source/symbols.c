#include "symbols.h"
#include <string.h>
#include "error.h"
#include "parse/scanner.h"
#include <stdio.h>
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_hash.h"

#include <assert.h>

void symbols_init(SYMBOLS *self)
{
	tlibc_hash_init(&self->symbols, self->symbol_buckets, MAX_SYMBOL_BUCKETS);
	self->symbol_list_num = 0;
}

void symbols_clear(SYMBOLS *self)
{
	tlibc_hash_clear(&self->symbols);
	self->symbol_list_num = 0;
}

static SYMBOL *symbols_alloc(SYMBOLS *self)
{
	SYMBOL *ret = NULL;
	if(self->symbol_list_num >= MAX_SYMBOL_LIST_NUM)
	{
		scanner_error_halt(NULL, E_LS_UNKNOW);
	}
	ret = &self->symbol_list[self->symbol_list_num];
	++(self->symbol_list_num);
	return ret;
}

void symbols_save(SYMBOLS *self, const char *preffix, const char *name, SYMBOL *symbol)
{
	snprintf(symbol->key, MAX_SYMBOL_KEY_LENGTH, "%s::%s", preffix, name);
	symbol->key[MAX_SYMBOL_KEY_LENGTH - 1] = 0;
	symbol->key_len = strlen(symbol->key);

	tlibc_hash_insert(&self->symbols, symbol->key, symbol->key_len, &symbol->hash_head);
}

const SYMBOL* symbols_search(const SYMBOLS *self, const char* preffix, const char* name)
{
	const SYMBOL *symbol;
	char key[MAX_SYMBOL_KEY_LENGTH];
	const tlibc_hash_head_t *ele;
	uint32_t key_len;

	snprintf(key, MAX_SYMBOL_KEY_LENGTH, "%s::%s", preffix, name);

	key[MAX_SYMBOL_KEY_LENGTH - 1] = 0;
	key_len = strlen(key);
	ele = tlibc_hash_find_const(&self->symbols, key, key_len);
	if(ele == NULL)
	{
		goto ERROR_RET;
	}
	symbol = TLIBC_CONTAINER_OF(ele, const SYMBOL, hash_head);

	return symbol;
ERROR_RET:
	return NULL;
}

const ST_SIMPLE_TYPE* symbols_get_real_type(const SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type)
{
	if(sn_type->st == E_ST_REFER)
	{
		const SYMBOL *ptr = symbols_search(self, "", sn_type->st_refer);
		if(ptr == NULL)
		{
			return NULL;
		}
		if(ptr->type == EN_HST_TYPEDEF)
		{
			return symbols_get_real_type(self, &ptr->body.type.type);
		}
		else
		{
			return sn_type;
		}
	}
	return sn_type;
}

const ST_VALUE* symbols_get_real_value(const SYMBOLS *self, const ST_VALUE* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const SYMBOL *ptr = symbols_search(self, "", sn_value->val.identifier);
		if(ptr == NULL)
		{
			return NULL;
		}
		if(ptr->type == EN_HST_VALUE)
		{
			return symbols_get_real_value(self, &ptr->body.val);
		}
		else
		{
			return sn_value;
		}
	}
	return sn_value;
}



void symbols_add_Typedef(SYMBOLS *self, const YYLTYPE *yylloc, const ST_TYPEDEF *pn_typedef)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_TYPEDEF;
	symbol->body.type = *pn_typedef;
	symbols_save(self, "", pn_typedef->name, symbol);
}

void symbols_add_Const(SYMBOLS *self, const YYLTYPE *yylloc, const ST_Const *pn_const)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_const->val;

	symbols_save(self, "", pn_const->identifier, symbol);
}

void symbols_add_Enum(SYMBOLS *self, const YYLTYPE *yylloc, const ST_ENUM *pn_enum)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;

	symbol->type = EN_HST_ENUM;
	strncpy(symbol->body.symbol_enum.name, pn_enum->name, TLIBC_MAX_LENGTH_OF_IDENTIFIER);
	symbol->body.symbol_enum.name[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

	symbol->body.symbol_enum.enum_def_list_num = pn_enum->enum_def_list_num;

	symbols_save(self, "", pn_enum->name, symbol);
}

void symbols_add_EnumDef(SYMBOLS *self, const YYLTYPE *yylloc, const ST_ENUM_DEF *pn_enum_def)
{
	SYMBOL *symbol = symbols_alloc(self);
	SYMBOL *enum_symbol = symbols_alloc(self);
	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_enum_def->val;

	symbol->yylloc = *yylloc;

	*enum_symbol = *symbol;	
	symbols_save(self, "", pn_enum_def->identifier, symbol);
	symbols_save(self, self->enum_name, pn_enum_def->identifier, enum_symbol);
}

void symbols_add_UnionField(SYMBOLS *self, const YYLTYPE *yylloc, const ST_UNION_FIELD *pn_union_field)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_UNION_FIELD;

	symbol->yylloc = *yylloc;
	symbols_save(self, self->union_name, pn_union_field->name, symbol);

}

void symbols_add_Field(SYMBOLS *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field)
{
	SYMBOL *symbol = NULL;

	//如果是数据， 那么构造出一个计数器变量
	if(pn_field->type.type == E_SNT_CONTAINER)
	{
		ST_FIELD list_num;
		assert(pn_field->type.ct.ct == E_CT_VECTOR);
		list_num.condition.oper = E_EO_NON;

		list_num.type.type = E_SNT_SIMPLE;
		list_num.type.st.st = E_ST_UINT16;

		//这里的标识符长度已经检查过了， 不会出界
		snprintf(list_num.identifier, TLIBC_MAX_LENGTH_OF_IDENTIFIER, "%s_num", pn_field->identifier);
		list_num.identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;
		

		list_num.args.arg_list_num = 0;
		list_num.comment.text[0] = 0;


		symbol = symbols_alloc(self);
		symbol->yylloc = *yylloc;
		symbol->type = EN_HST_FIELD;
		symbol->body.field = list_num;
		symbols_save(self, self->struct_name, list_num.identifier, symbol);
	}

	symbol = symbols_alloc(self);
	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_FIELD;
	symbol->body.field = *pn_field;
	symbols_save(self, self->struct_name, pn_field->identifier, symbol);
}

void symbols_add_Struct(SYMBOLS *self, const YYLTYPE *yylloc, const ST_STRUCT *de_struct)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_STRUCT;
	symbol->body.struct_field_list_num = de_struct->field_list.field_list_num;

	symbols_save(self, "", de_struct->name, symbol);
}

void symbols_add_Union(SYMBOLS *self, const YYLTYPE *yylloc, const ST_UNION *de_union)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_UNION;
	symbol->body.union_symbol.para = de_union->parameters;
	symbol->body.union_symbol.union_field_list_num = de_union->union_field_list.union_field_list_num;

	symbols_save(self, "", de_union->name, symbol);
}
