#include "symbols.h"
#include <string.h>
#include "error.h"
#include "parse/scanner.h"
#include <stdio.h>
#include "platform/tlibc_platform.h"
#include "lib/tlibc_hash.h"

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
		scanner_error(NULL, NULL, E_LS_UNKNOW);
	}
	ret = &self->symbol_list[self->symbol_list_num];
	++(self->symbol_list_num);
	return ret;
}

void symbols_save(SYMBOLS *self, const char *name, SYMBOL *symbol, const char *preffix)
{
	if(preffix)
	{
		snprintf(symbol->key, MAX_SYMBOL_KEY_LENGTH, "%s:%s", preffix, name);
	}
	else
	{
		snprintf(symbol->key, MAX_SYMBOL_KEY_LENGTH, "%s", name);
	}
	symbol->key[MAX_SYMBOL_KEY_LENGTH - 1] = 0;
	symbol->key_len = strlen(symbol->key);

	tlibc_hash_insert(&self->symbols, symbol->key, symbol->key_len, &symbol->hash_head);
}

const SYMBOL* symbols_search(const SYMBOLS *self, const char* name, const char* preffix)
{
	SYMBOL *symbol;
	char key[MAX_SYMBOL_KEY_LENGTH];
	const tlibc_hash_head_t *ele;
	tuint32 key_len;

	if(preffix)
	{
		snprintf(key, MAX_SYMBOL_KEY_LENGTH, "%s:%s", preffix, name);
	}
	else
	{
		snprintf(key, MAX_SYMBOL_KEY_LENGTH, "%s", name);
	}

	key[MAX_SYMBOL_KEY_LENGTH - 1] = 0;
	key_len = strlen(key);
	ele = tlibc_hash_find(&self->symbols, key, key_len);
	if(ele == NULL)
	{
		goto ERROR_RET;
	}
	symbol = TLIBC_CONTAINER_OF(ele, SYMBOL, hash_head);

	return symbol;
ERROR_RET:
	return NULL;
}

const ST_SIMPLE_TYPE* symbols_get_real_type(const SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type)
{
	if(sn_type->st == E_ST_REFER)
	{
		const SYMBOL *ptr = symbols_search(self, sn_type->st_refer, NULL);
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

const ST_VALUE* symbols_get_real_value(SYMBOLS *self, const ST_VALUE* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const SYMBOL *ptr = symbols_search(self, sn_value->val.identifier, NULL);
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



void symbols_add_Typedef(SYMBOLS *self, const ST_TYPEDEF *pn_typedef)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_TYPEDEF;
	symbol->body.type = *pn_typedef;
	symbols_save(self, pn_typedef->name, symbol, NULL);
}

void symbols_add_Const(SYMBOLS *self, const ST_Const *pn_const)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_const->val;

	symbols_save(self, pn_const->identifier, symbol, NULL);
}

void symbols_add_Enum(SYMBOLS *self, const ST_ENUM *pn_enum)
{
	tuint32 i, j;

	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_ENUM;
	strncpy(symbol->body.symbol_enum.name, pn_enum->name, TLIBC_MAX_IDENTIFIER_LENGTH);
	symbol->body.symbol_enum.name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;

	symbol->body.symbol_enum.enum_def_list_num = pn_enum->enum_def_list_num;
	symbol->body.symbol_enum.unique = hptrue;
	for(i = 0;i < pn_enum->enum_def_list_num; ++i)
	{
		for(j = i + 1; j < pn_enum->enum_def_list_num; ++j)
		{
			if(pn_enum->enum_def_list[i].val.val.i64 == pn_enum->enum_def_list[j].val.val.i64)
			{
				symbol->body.symbol_enum.unique = hpfalse;
				goto done;
			}
		}
	}
done:
	symbols_save(self, pn_enum->name, symbol, NULL);
}

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def)
{
	SYMBOL *symbol = symbols_alloc(self);
	SYMBOL *enum_symbol = symbols_alloc(self);

	symbol->type = EN_HST_VALUE;
	symbol->body.val = pn_enum_def->val;

	*enum_symbol = *symbol;
	
	symbols_save(self, pn_enum_def->identifier, symbol, NULL);

	symbols_save(self, pn_enum_def->identifier, enum_symbol, self->enum_name);
}

void symbols_add_Parameters(SYMBOLS *self, const ST_Parameters *parameters, const char *identifier)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_PARAMETER;
	symbol->body.para = parameters->par_list[0];

	symbols_save(self, parameters->par_list[0].identifier, symbol, identifier);

	self->para = &parameters->par_list[0];
}

void symbols_add_UnionField(SYMBOLS *self, const ST_UNION_FIELD *pn_union_field)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_UNION_FIELD;

	symbols_save(self, pn_union_field->name, symbol, self->union_name);

}

void symbols_add_Field(SYMBOLS *self, const ST_FIELD *pn_field)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_FIELD;
	symbol->body.field = *pn_field;

	symbols_save(self, pn_field->identifier, symbol, self->struct_name);
}

void symbols_add_Struct(SYMBOLS *self, const ST_STRUCT *de_struct)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_STRUCT;
	symbol->body.field_list_num = de_struct->field_list.field_list_num;

	symbols_save(self, de_struct->name, symbol, NULL);
}

void symbols_add_Union(SYMBOLS *self, const ST_UNION *de_union)
{
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_UNION;
	symbol->body.field_list_num = de_union->union_field_list.union_field_list_num;

	symbols_save(self, de_union->name, symbol, NULL);
}
