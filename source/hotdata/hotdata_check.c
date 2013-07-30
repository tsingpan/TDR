#include "hotdata_check.h"
#include "hotdata_symbols.h"
#include <string.h>

void dp_check_Const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	char id[1024];
	hpuint32 i;
	void *data;

	for(i = 0; i < tok_identifier->len; ++i)
	{
		id[i] = tok_identifier->ptr[i];
	}
	id[i] = 0;

	if(trie_retrieve(self->constant_symbols, id, &data))
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

	if(!trie_store(self->constant_symbols, id, ptr))
	{
		dp_error(self, yylloc, E_SID_ERROR, id);
	}
}


void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const PN_IDENTIFIER* tok_identifier, const PN_VALUE* sn_value)
{
	char id[1024];
	const ST_VALUE* val = NULL;
	size_t size;

	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const HOTDATA_SYMBOLS *ptr;
		if(!trie_retrieve(self->constant_symbols, sn_value->val.identifier, &ptr))
		{
			dp_error(self, yylloc, E_SID_ERROR, id);
			goto done;
		}

		if(ptr->type != EN_HST_VALUE)
		{
			dp_error(self, yylloc, E_SID_ERROR, id);
			goto done;
		}

		val = &ptr->body.val;
	}
	else
	{
		val = sn_value;
	}

	if(val == NULL)
	{
		goto done;
	}

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	tok_identifier->ptr[tok_identifier->len] = 0;

	if(sn_type->type == E_SNT_SIMPLE)
	{
		switch(sn_type->st)
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
	else if(sn_type->type == E_SNT_OBJECT)
	{
		dp_error(self, yylloc, E_SID_ERROR, id);
	}
	else if(sn_type->type == E_SNT_CONTAINER)
	{
		if(sn_type->ct == E_CT_STRING)
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
	symbol->type = EN_HST_TYPE;
	symbol->body.type = sn_typedef->type;

	if((sn_typedef->type.type != E_SNT_SIMPLE) && (sn_typedef->type.type != E_SNT_OBJECT))
	{
		dp_error(self, yylloc, E_SID_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE);
	}


	if(!trie_store_if_absent(self->constant_symbols, sn_typedef->name, symbol))
	{
		dp_error(self, yylloc, E_HP_SYMBOL_REDEFINITION, sn_typedef->name);
		goto done;
	}

done:
	return;
}
