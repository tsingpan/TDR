#include "symbols.h"
#include <string.h>
#include "error.h"
#include "parse/scanner.h"
#include <stdio.h>
#include "platform/tlibc_platform.h"
#include "core/tlibc_hash.h"

#include <assert.h>

void symbols_init(symbols_t *self)
{
	tlibc_hash_init(&self->symbols, self->symbol_buckets, MAX_SYMBOL_BUCKETS);
	self->symbol_list_num = 0;
}

void symbols_clear(symbols_t *self)
{
	tlibc_hash_clear(&self->symbols);
	self->symbol_list_num = 0;
}

static symbol_t *symbols_alloc(symbols_t *self)
{
	symbol_t *ret = NULL;
	if(self->symbol_list_num >= MAX_SYMBOL_LIST_NUM)
	{
		scanner_error_halt(NULL, E_LS_UNKNOW);
	}
	ret = &self->symbol_list[self->symbol_list_num];
	++(self->symbol_list_num);
	return ret;
}

void symbols_save(symbols_t *self, const char *preffix, const char *name, symbol_t *symbol)
{
	snprintf(symbol->key, MAX_SYMBOL_KEY_LENGTH, "%s::%s", preffix, name);
	symbol->key[MAX_SYMBOL_KEY_LENGTH - 1] = 0;
	symbol->key_len = strlen(symbol->key);

	tlibc_hash_insert(&self->symbols, symbol->key, symbol->key_len, &symbol->hash_head);
}

const symbol_t* symbols_search(const symbols_t *self, const char* preffix, const char* name)
{
	const symbol_t *symbol;
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
	symbol = TLIBC_CONTAINER_OF(ele, const symbol_t, hash_head);

	return symbol;
ERROR_RET:
	return NULL;
}

const syn_simple_type_t* symbols_get_real_type(const symbols_t *self, const syn_simple_type_t* sn_type)
{
	if(sn_type->st == E_ST_REFER)
	{
		const symbol_t *ptr = symbols_search(self, "", sn_type->st_refer);
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

const syn_value_t* symbols_get_real_value(const symbols_t *self, const syn_value_t* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const symbol_t *ptr = symbols_search(self, "", sn_value->val.identifier);
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



void symbols_add_Typedef(symbols_t *self, const YYLTYPE *yylloc, const syn_typedef_t *pn_typedef)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_TYPEDEF;
	symbol->body.type = *pn_typedef;
	symbols_save(self, "", pn_typedef->name, symbol);
}

void symbols_add_Const(symbols_t *self, const YYLTYPE *yylloc, const syn_const_t *pn_const)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_const->val;

	symbols_save(self, "", pn_const->identifier, symbol);
}

void symbols_add_Enum(symbols_t *self, const YYLTYPE *yylloc, const syn_enum_t *pn_enum)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;

	symbol->type = EN_HST_ENUM;
	strncpy(symbol->body.symbol_enum.name, pn_enum->name, TLIBC_MAX_LENGTH_OF_IDENTIFIER);
	symbol->body.symbol_enum.name[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

	symbol->body.symbol_enum.enum_def_list_num = pn_enum->enum_def_list_num;

	symbols_save(self, "", pn_enum->name, symbol);
}

void symbols_add_EnumDef(symbols_t *self, const YYLTYPE *yylloc, const enum_def_t *pn_enum_def)
{
	symbol_t *symbol = symbols_alloc(self);
	symbol_t *enum_symbol = symbols_alloc(self);
	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_enum_def->val;

	symbol->yylloc = *yylloc;

	*enum_symbol = *symbol;	
	symbols_save(self, "", pn_enum_def->identifier, symbol);
	symbols_save(self, self->enum_name, pn_enum_def->identifier, enum_symbol);
}

void symbols_add_UnionField(symbols_t *self, const YYLTYPE *yylloc, const syn_union_field_t *pn_union_field)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->type = EN_HST_UNION_FIELD;

	symbol->yylloc = *yylloc;
	symbols_save(self, self->union_name, pn_union_field->name, symbol);

}

void symbols_add_Field(symbols_t *self, const YYLTYPE *yylloc, const syn_field_t *pn_field)
{
	symbol_t *symbol = NULL;

	//如果是数据， 那么构造出一个计数器变量
	if(pn_field->type.type == E_SNT_CONTAINER)
	{
		syn_field_t list_num;
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

void symbols_add_Struct(symbols_t *self, const YYLTYPE *yylloc, const syn_struct_t *de_struct)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_STRUCT;
	symbol->body.struct_field_list_num = de_struct->field_list.field_list_num;

	symbols_save(self, "", de_struct->name, symbol);
}

void symbols_add_Union(symbols_t *self, const YYLTYPE *yylloc, const syn_union_t *de_union)
{
	symbol_t *symbol = symbols_alloc(self);

	symbol->yylloc = *yylloc;
	symbol->type = EN_HST_UNION;
	symbol->body.union_symbol.para = de_union->parameters;
	symbol->body.union_symbol.union_field_list_num = de_union->union_field_list.union_field_list_num;

	symbols_save(self, "", de_union->name, symbol);
}
