#include "hotdata_check.h"
#include "hotdata_symbols.h"
#include <string.h>

static const HOTDATA_SYMBOLS* dp_find_symbol_by_string(DATA_PARSER *self, const char* name)
{
	const HOTDATA_SYMBOLS *symbol;
	char global_name[MAX_IDENTIFIER_LENGTH * 2];

	if(self->domain[0])
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}

	if(!trie_retrieve(self->hotdata_symbols, global_name, (void**)&symbol))
	{
		if(!trie_retrieve(self->hotdata_symbols, name, (void**)&symbol))
		{
			return NULL;
		}
	}
	return symbol;
}

static const HOTDATA_SYMBOLS* dp_find_symbol(DATA_PARSER *self, const PN_IDENTIFIER* tok_identifier)
{
	const HOTDATA_SYMBOLS *symbol;
	char name[MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return dp_find_symbol_by_string(self, name);
}

static hpint32 dp_save_symbol(DATA_PARSER *self, const PN_IDENTIFIER *tok_identifier, const HOTDATA_SYMBOLS *symbol)
{
	char name[MAX_IDENTIFIER_LENGTH];
	char global_name[MAX_IDENTIFIER_LENGTH * 2];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	if(self->domain[0])
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}
	
	if(!trie_store_if_absent(self->hotdata_symbols, global_name, (void**)&symbol))
	{
		E_HP_ERROR;
	}
	return E_HP_NOERROR;
}


void dp_check_Const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	char id[MAX_IDENTIFIER_LENGTH];
	void *data;

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	if(dp_find_symbol(self, tok_identifier) != NULL)
	{
		dp_error(self, yylloc, E_SID_SYMBOL_REDEFINITION, id);
	}
}

void dp_check_Const_add_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier, const PN_VALUE *pn_value)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	char id[1024];
	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	ptr->type = EN_HST_VALUE;
	ptr->body.val = *pn_value;

	if(dp_save_symbol(self, tok_identifier, ptr) != E_HP_NOERROR)
	{
		dp_error(self, yylloc, E_SID_ERROR, id);
	}
}

static const ST_TYPE* get_type(DATA_PARSER *self, const ST_TYPE* sn_type)
{
	if(sn_type->type == E_SNT_SIMPLE)
	{
		return sn_type;
	}
	else if(sn_type->type == E_SNT_REFER)
	{
		const HOTDATA_SYMBOLS *ptr = dp_find_symbol_by_string(self, sn_type->ot);
		if(ptr == NULL)
		{
			return NULL;
		}
		if(ptr->type == EN_HST_TYPE)
		{
			return &ptr->body.type;
		}
		else
		{
			return sn_type;
		}
	}
	else
	{
		return sn_type;
	}
}

static const ST_VALUE* get_value(DATA_PARSER *self, const ST_VALUE* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const HOTDATA_SYMBOLS *ptr = dp_find_symbol_by_string(self, sn_value->val.identifier);
		if(ptr == NULL)
		{
			return NULL;
		}

		if(ptr->type == EN_HST_VALUE)
		{
			return &ptr->body.val;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return sn_value;
	}
}

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const PN_IDENTIFIER* tok_identifier, const PN_VALUE* sn_value)
{
	char id[MAX_IDENTIFIER_LENGTH];
	const ST_VALUE* val = get_value(self, sn_value);
	const ST_TYPE* type = get_type(self, sn_type);
	size_t size;

	if((type == NULL) || (val == NULL))
	{
		dp_error(self, yylloc, E_SID_ERROR, id);
		goto done;
	}

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;	
	
	if(type->type == E_SNT_SIMPLE)
	{
		switch(type->st)
		{
		case E_ST_INT8:
			size = sizeof(hpint8);
			break;
		case E_ST_INT16:
			size = sizeof(hpint16);		
			break;
		case E_ST_INT32:
			size = sizeof(hpint32);
			break;
		case E_ST_INT64:
			size = sizeof(hpint64);
			break;
		case E_ST_UINT8:
			size = sizeof(hpuint8);
			break;
		case E_ST_UINT16:
			size = sizeof(hpuint16);
			break;
		case E_ST_UINT32:
			size = sizeof(hpuint32);
			break;
		case E_ST_UINT64:
			size = sizeof(hpuint64);
			break;
		case E_ST_CHAR:
		case E_ST_DOUBLE:
			goto done;
		default:
			dp_error(self, yylloc, (hpint32)E_HP_INVALID_CONSTANCE_TYPE);
			goto done;
		}
		size *= 8;
		if((val->type == E_SNVT_UINT64) || (val->type == E_SNVT_HEX_UINT64))
		{
			if(val->val.ui64 >> size)
			{
				dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
				goto done;
			}
		}
		else if((val->type == E_SNVT_INT64) || (val->type == E_SNVT_HEX_INT64))
		{
			if(val->val.i64 >> size)
			{
				dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
				goto done;
			}
		}
		else if(val->type == E_SNVT_IDENTIFIER)
		{

		}
		else
		{
			dp_error(self, yylloc, (hpint32)E_HP_UNKNOW_CONSTANT_VALUE);
			goto done;
		}
	}
	else if(type->type == E_SNT_REFER)
	{
		dp_error(self, yylloc, E_SID_ERROR, id);
	}
	else if(type->type == E_SNT_CONTAINER)
	{
		if(type->ct == E_CT_STRING)
		{
			if(sn_value->type != E_SNVT_STRING)
			{
				dp_error(self, yylloc, E_SID_ERROR, id);
			}
		}
		else
		{
			dp_error(self, yylloc, E_SID_ERROR, id);
		}
	}

done:
	return;
}

void dp_check_Typedef(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef)
{
	HOTDATA_SYMBOLS *symbol = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));
	const ST_TYPE*type = get_type(self, &sn_typedef->type);
	if(type == NULL)
	{
		dp_error(self, yylloc, E_HP_ERROR);
		goto done;
	}

	symbol->type = EN_HST_TYPE;
	symbol->body.type = *type;

	if(!trie_store_if_absent(self->hotdata_symbols, sn_typedef->name, symbol))
	{
		dp_error(self, yylloc, E_HP_SYMBOL_REDEFINITION, sn_typedef->name);
		goto done;
	}

done:
	return;
}

void dp_check_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	return;
}
