#include "reduce.h"
#include <string.h>

void dp_reduce_Definition_Import(PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_Import* pn_import)
{
	pn_current->type = E_DT_IMPORT;
	pn_current->definition.de_import = *pn_import;
}

void dp_reduce_Definition_Const(PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_Const* pn_const)
{
	pn_current->type = E_DT_CONST;
	pn_current->definition.de_const = *pn_const;

}

void dp_reduce_Definition_Typedef(PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_TYPEDEF* pn_typedef)
{
	pn_current->type = E_DT_TYPEDEF;
	pn_current->definition.de_typedef = *pn_typedef;
}

void dp_reduce_Import_tok_string(PARSER *self, const YYLTYPE *yylloc, ST_Import* current, const tstring str)
{
	snprintf(current->package_name, sizeof(current->package_name), str);
}

void dp_reduce_ObjectType_tok_identifier(PARSER *self, const YYLTYPE *yylloc, ST_TYPE* current, const tbytes *tok_identifier)
{
	tuint32 i;
	char id[1024];
	ST_TYPE *type;

	current->type = E_SNT_REFER;

	for(i = 0; i < tok_identifier->len; ++i)
	{
		id[i] = tok_identifier->ptr[i];
	}
	id[i] = 0;
	strncpy(current->ot, id, TLIBC_MAX_IDENTIFIER_LENGTH);
done:
	return;
}

void dp_reduce_ContainerType_tok_t_vector(PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current)
{
	current->type = E_SNT_CONTAINER;
	current->ct = E_CT_VECTOR;
}

void dp_reduce_ContainerType_tok_t_string(PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current)
{
	current->type = E_SNT_CONTAINER;
	current->ct = E_CT_STRING;
}

void dp_reduce_SimpleType(PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current, const SN_SIMPLE_TYPE type)
{
	current->type = E_SNT_SIMPLE;
	current->st = type;
}

void dp_reduce_Value_tok_identifier(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tbytes sn_identifier)
{
	tuint32 data;
	tuint32 i;

	current->type = E_SNVT_IDENTIFIER;
	memcpy(current->val.identifier, sn_identifier.ptr, sn_identifier.len);
	current->val.identifier[sn_identifier.len] = 0;

	
done:
	return;
error_ret:
	current->type = E_SNVT_IDENTIFIER;
	return;
}

void dp_reduce_Value_tok_char(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tchar pn_char)
{
	current->type = E_SNVT_CHAR;
	current->val.c = pn_char;
}

void dp_reduce_Value_tok_int64(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tint64 i64)
{
	current->type = E_SNVT_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_hex_int64(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tint64 i64)
{
	current->type = E_SNVT_HEX_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_uint64(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tuint64 ui64)
{
	current->type = E_SNVT_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_hex_uint64(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tuint64 ui64)
{
	current->type = E_SNVT_HEX_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_double(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const double d)
{
	current->type = E_SNVT_DOUBLE;
	current->val.d = d;
}

void dp_reduce_Value_tok_bool(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tbool b)
{
	current->type = E_SNVT_BOOL;
	current->val.b = b;
}

void dp_reduce_Value_tok_string(PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tstring str)
{
	current->type = E_SNVT_STRING;
	strncpy(current->val.str, str, TLIBC_MAX_IDENTIFIER_LENGTH);
}

void dp_reduce_Const(PARSER *self, const YYLTYPE *yylloc, ST_Const* current, const ST_TYPE *type, const tbytes *identifier, const ST_VALUE *val)
{
	current->type = *type;
	//warning
	memcpy(current->identifier, identifier->ptr, identifier->len);
	current->identifier[identifier->len] = 0;
	
	current->val = *val;
}

void dp_reduce_ArgumentList_ArgumentList_Type(PARSER *self, const YYLTYPE *yylloc, ST_ARGUMENTS* current, const ST_ARGUMENTS* argument_list, const ST_TYPE *pn_type)
{
	*current = *argument_list;
	current->arg_list[current->arg_list_num] = *pn_type;
	++(current->arg_list_num);
}

void dp_reduce_ArgumentList_Type(PARSER *self, const YYLTYPE *yylloc, ST_ARGUMENTS* current, const ST_TYPE *pn_type)
{
	current->arg_list_num = 0;
	current->arg_list[current->arg_list_num] = *pn_type;
	++(current->arg_list_num);
}

void dp_reduce_Typedef_Type_Arguments_tok_identifier(PARSER *self, const YYLTYPE *yylloc, ST_TYPEDEF *current, ST_TYPE* type, const tbytes *tok_identifier)
{
	current->type = *type;
	memcpy(current->name, tok_identifier->ptr, tok_identifier->len);
	current->name[tok_identifier->len] = 0;
}

void dp_reduce_Condition_tok_case(PARSER *self, const YYLTYPE *yylloc, ST_CONDITION *current, const ST_VALUE *val)
{
	tuint32 i;
	const ST_TypeAnnotations *ta = NULL;

	if(self->in_struct)
	{
		ta = &self->pn_definition.definition.de_struct.ta;
	}
	else
	{
		ta = &self->pn_definition.definition.de_union.ta;
	}

	current->empty = hpfalse;
	current->exp.neg = hpfalse;
	current->exp.op0.type = E_SNVT_IDENTIFIER;		
	snprintf(current->exp.op0.val.identifier, TLIBC_MAX_IDENTIFIER_LENGTH, "s");
	for(i = 0; i < ta->ta_list_num; ++i)
	{
		if(ta->ta_list[i].type == E_TA_SWITCH)
		{
			snprintf(current->exp.op0.val.identifier, TLIBC_MAX_IDENTIFIER_LENGTH, ta->ta_list[i].val.val.identifier);			
		}
	}
	current->exp.oper = E_EO_EQUAL;
	current->exp.op1 = *val;
}
