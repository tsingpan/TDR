#include "hotdata_check.h"

void dp_check_Const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	char id[1024];
	hpuint32 i;
	hpuint32 data;

	for(i = 0; i < tok_identifier->len; ++i)
	{
		id[i] = tok_identifier->ptr[i];
	}
	id[i] = 0;

	if(trie_retrieve(self->symbols, id, &data))
	{
		dp_error(self, yylloc, E_SID_SYMBOL_REDEFINITION, id);
	}
}

/*
void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const hpbytes* sn_identifier, const ST_VALUE* sn_value)
{
	char id[1024];
	hpuint32 i;
	const SN_VALUE* val = NULL;
	size_t size;

	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		if(self->symbol_list[sn_value->index].type == E_SST_Const)
		{
			val = &self->symbol_list[sn_value->index].sn.sn_value;
		}
	}
	else
	{
		val = sn_value;
	}

	if(val == NULL)
	{
		goto done;
	}

	for(i = 0; i < sn_identifier->len; ++i)
	{
		id[i] = sn_identifier->ptr[i];
	}
	id[i] = 0;

	switch(sn_type->type)
	{
	case E_SNT_INT8:
		size = sizeof(hpint8);
		break;
	case E_SNT_INT16:
		size = sizeof(hpint16);		
		break;
	case E_SNT_INT32:
		size = sizeof(hpint32);
		break;
	case E_SNT_INT64:
		size = sizeof(hpint64);
		break;
	case E_SNT_UINT8:
		size = sizeof(hpuint8);
		break;
	case E_SNT_UINT16:
		size = sizeof(hpuint16);
		break;
	case E_SNT_UINT32:
		size = sizeof(hpuint32);
		break;
	case E_SNT_UINT64:
		size = sizeof(hpuint64);
		break;
	default:
		dp_error(self, yylloc, (hpint32)E_HP_INVALID_CONSTANCE_TYPE);
		goto done;
	}
	size *= 8;
	if((val->type == E_SNVT_UINT64) || (val->type == E_SNVT_HEX_UINT64))
	{
		if(val->ui64 >> size)
		{
			dp_error(self, yylloc, (hpint32)E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
			goto done;
		}
	}
	else if((val->type == E_SNVT_INT64) || (val->type == E_SNVT_HEX_INT64))
	{
		if(val->i64 >> size)
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

	self->symbol_list[self->symbol_list_num].type = E_SST_Const;
	self->symbol_list[self->symbol_list_num].sn.sn_value = *sn_value;


	if(!trie_store_if_absent(self->symbols, id, self->symbol_list_num))
	{
		dp_error(self, yylloc, (hpint32)E_HP_ERROR, id);
		goto done;
	}

	++(self->symbol_list_num);

done:
	return;
}
*/