#include "hotdata_reduce.h"
#include <string.h>

void dp_reduce_Definition_Import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_IMPORT* pn_import)
{
	pn_current->type = E_DT_IMPORT;
	pn_current->definition.de_import = *pn_import;
}

void dp_reduce_Definition_Const(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_CONST* pn_const)
{
	pn_current->type = E_DT_CONST;
	pn_current->definition.de_const = *pn_const;

}

void dp_reduce_Definition_Typedef(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_TYPEDEF* pn_typedef)
{
	pn_current->type = E_DT_TYPEDEF;
	pn_current->definition.de_typedef = *pn_typedef;
}

void dp_reduce_Import_tok_import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_IMPORT* current, const hpstring *sn_tok_import)
{
	snprintf(current->package_name, sizeof(current->package_name), *sn_tok_import);
}

void dp_reduce_ObjectType_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE* current, const PN_IDENTIFIER *tok_identifier)
{
	hpuint32 i;
	char id[1024];
	PN_TYPE *type;

	current->type = E_SNT_REFER;

	for(i = 0; i < tok_identifier->len; ++i)
	{
		id[i] = tok_identifier->ptr[i];
	}
	id[i] = 0;
	strncpy(current->ot, id, MAX_IDENTIFIER_LENGTH);
/*
	if(!trie_retrieve(self->typedef_identifier, id, type))
	{
		dp_error(self, yylloc, (hpint32)E_HP_ERROR, id);
		goto done;
	}
	*/
done:
	return;
}

void dp_reduce_ContainerType_tok_t_vector(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current)
{
	current->type = E_SNT_CONTAINER;
	current->ct = E_CT_VECTOR;
}

void dp_reduce_ContainerType_tok_t_string(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current)
{
	current->type = E_SNT_CONTAINER;
	current->ct = E_CT_STRING;
}

void dp_reduce_SimpleType(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_SIMPLE_TYPE type)
{
	current->type = E_SNT_SIMPLE;
	current->st = type;
}

void dp_reduce_Value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbytes sn_identifier)
{
	hpuint32 data;
	hpuint32 i;

	current->type = E_SNVT_IDENTIFIER;
	memcpy(current->val.identifier, sn_identifier.ptr, sn_identifier.len);
	current->val.identifier[sn_identifier.len] = 0;

/*
	if(!trie_retrieve(self->symbols, id, &data))
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
		goto error_ret;
	}
/*
	if((self->symbol_list[data].type != E_SST_Const) && (self->symbol_list[data].type != E_SST_EnumDef))
	{
		dp_error(self, yylloc, (hpint32)E_HP_CAN_NOT_FIND_CONSTANCE, id);
		goto error_ret;
	}
*/
	
done:
	return;
error_ret:
	current->type = E_SNVT_IDENTIFIER;
	return;
}

void dp_reduce_Value_tok_char(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const PN_CHAR pn_char)
{
	current->type = E_SNVT_CHAR;
	current->val.c = pn_char;
}

void dp_reduce_Value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64)
{
	current->type = E_SNVT_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64)
{
	current->type = E_SNVT_HEX_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64)
{
	current->type = E_SNVT_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64)
{
	current->type = E_SNVT_HEX_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_double(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const double d)
{
	current->type = E_SNVT_DOUBLE;
	current->val.d = d;
}

void dp_reduce_Value_tok_bool(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbool b)
{
	current->type = E_SNVT_BOOL;
	current->val.b = b;
}

void dp_reduce_Value_tok_string(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpstring str)
{
	current->type = E_SNVT_STRING;
	current->val.str = str;
}

void dp_reduce_Const(DATA_PARSER *self, const YYLTYPE *yylloc, PN_CONST* current, const PN_TYPE *type, const PN_IDENTIFIER *identifier, const PN_VALUE *val)
{
	current->type = *type;
	//warning
	memcpy(current->identifier, identifier->ptr, identifier->len);
	current->identifier[identifier->len] = 0;
	
	current->val = *val;
}

void dp_reduce_Argument_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENT* current, const PN_IDENTIFIER *tok_identifier)
{
	current->type = E_AT_IDENTIFIER;

	memcpy(current->id, tok_identifier->ptr, tok_identifier->len);
	current->id[tok_identifier->len] = 0;
}

void dp_reduce_Argument_SimpleType(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENT* current, const PN_TYPE *pn_type)
{
	current->type = E_AT_SIMPLE_TYPE;
	current->st = *pn_type;
}

void dp_reduce_ArgumentList_Argument(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENTS* current, const PN_ARGUMENT *pn_argument)
{
	current->arg_list_num = 0;
	current->arg_list[current->arg_list_num] = *pn_argument;
	++(current->arg_list_num);
}

void dp_reduce_ArgumentList_ArgumentList_Argument(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENTS* current, const PN_ARGUMENTS* argument_list, const PN_ARGUMENT *pn_argument)
{
	*current = *argument_list;
	current->arg_list[current->arg_list_num] = *pn_argument;
	++(current->arg_list_num);
}

void dp_reduce_Typedef_Type_Arguments_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPEDEF *current, PN_TYPE* type, const PN_IDENTIFIER *tok_identifier)
{
	current->type = *type;
	memcpy(current->name, tok_identifier->ptr, tok_identifier->len);
	current->name[tok_identifier->len] = 0;
}

void dp_reduce_Condition_tok_case(DATA_PARSER *self, const YYLTYPE *yylloc, PN_CONDITION *current, const PN_VALUE *val)
{
	hpuint32 i;
	const ST_TypeAnnotations *ta = NULL;

	if(self->in_struct)
	{
		ta = &self->pn_definition.definition.de_struct.ta;
	}
	else
	{
		ta = &self->pn_definition.definition.de_union.ta;
	}

	current->exp.neg = hpfalse;
	current->exp.op0.type = E_SNVT_IDENTIFIER;		
	snprintf(current->exp.op0.val.identifier, MAX_IDENTIFIER_LENGTH, "s");
	for(i = 0; i < ta->ta_list_num; ++i)
	{
		if(ta->ta_list[i].type == E_TA_SWITCH)
		{
			snprintf(current->exp.op0.val.identifier, MAX_IDENTIFIER_LENGTH, ta->ta_list[i].val.val.identifier);			
		}
	}
	current->exp.oper = E_EO_AND;
	current->exp.op1 = *val;
}
