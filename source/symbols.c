#include "symbols.h"
#include <string.h>
#include "error.h"
#include "parse/scanner.h"


void symbols_init(SYMBOLS *self)
{
	AlphaMap *alpha_map = NULL;

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->symbols = trie_new(alpha_map);
	alpha_map_free(alpha_map);

	self->symbol_list_num = 0;
}

void symbols_fini(SYMBOLS *self)
{
	trie_free(self->symbols);
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

void symbols_save(SYMBOLS *self, const char *name, const SYMBOL *symbol, const char *preffix)
{
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(preffix)
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", preffix, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}

	if(!trie_store_if_absent(self->symbols, global_name, symbol))
	{
		scanner_error(NULL, NULL, E_LS_UNKNOW);
	}
}

const SYMBOL* symbols_search(SYMBOLS *self, const char* name, const char* preffix)
{
	const SYMBOL *symbol;
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(preffix)
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", preffix, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	
	if(!trie_retrieve(self->symbols, global_name, (void**)&symbol))
	{
		goto ERROR_RET;
	}

	return symbol;
ERROR_RET:
	return NULL;
}

const ST_SIMPLE_TYPE* symbols_get_real_type(SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type)
{
	if(sn_type->st == E_ST_REFER)
	{
		const SYMBOL *ptr = symbols_search(self, sn_type->st_refer, hpfalse);
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
		const SYMBOL *ptr = symbols_search(self, sn_value->val.identifier, hpfalse);
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
	SYMBOL *symbol = symbols_alloc(self);

	symbol->type = EN_HST_ENUM;
	symbol->body.enum_def_list_num = pn_enum->enum_def_list_num;

	symbols_save(self, pn_enum->name, symbol, NULL);
}

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def)
{
	ST_Const c;
	memcpy(c.identifier , pn_enum_def->identifier, TLIBC_MAX_IDENTIFIER_LENGTH);
	c.type.st = E_ST_INT32;
	c.val = pn_enum_def->val;
	symbols_add_Const(self, &c);
}



void symbols_add_Parameter(SYMBOLS *self, const ST_Parameter *pn_parameter)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_PARAMETER;
	ptr->body.para = *pn_parameter;

	//symbols_save(self, pn_parameter->identifier, ptr, NULL);
}

void symbols_add_Field(SYMBOLS *self, const ST_FIELD *pn_field)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_FIELD;
	ptr->body.field = *pn_field;

	//symbols_save(self, pn_field->identifier, ptr, NULL);
}

void symbols_add_Struct(SYMBOLS *self, const ST_STRUCT *de_struct)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_STRUCT;
	ptr->body.field_list_num = de_struct->field_list.field_list_num;

	symbols_save(self, de_struct->name, ptr, NULL);
}

void symbols_add_Union(SYMBOLS *self, const ST_UNION *de_union)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_UNION;
	ptr->body.field_list_num = de_union->union_field_list.union_field_list_num;

	symbols_save(self, de_union->name, ptr, NULL);
}
