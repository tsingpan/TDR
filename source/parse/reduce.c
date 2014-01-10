#include "reduce.h"
#include <string.h>

void dp_reduce_Definition_Import(PARSER *self, ST_DEFINITION *pn_current, const ST_Import* pn_import)
{
	pn_current->type = E_DT_IMPORT;
	pn_current->definition.de_import = *pn_import;
}

void dp_reduce_Definition_Const(PARSER *self, ST_DEFINITION *pn_current, const ST_Const* pn_const)
{
	pn_current->type = E_DT_CONST;
	pn_current->definition.de_const = *pn_const;

}

void dp_reduce_Definition_Typedef(PARSER *self, ST_DEFINITION *pn_current, const ST_TYPEDEF* pn_typedef)
{
	pn_current->type = E_DT_TYPEDEF;
	pn_current->definition.de_typedef = *pn_typedef;
}

void dp_reduce_Import(PARSER *self, ST_Import* current, const tstring str)
{
	snprintf(current->package_name, sizeof(current->package_name), str);
}

void dp_reduce_ContainerType_tok_t_vector(PARSER *self, ST_TYPE *current, const ST_SIMPLE_TYPE *simple_type, const tbytes *tok_identifier)
{
	current->type = E_SNT_CONTAINER;
	current->ct = E_CT_VECTOR;

	current->vector_type = *simple_type;
	memcpy(current->vector_length, tok_identifier->ptr, tok_identifier->len);
	current->vector_length[tok_identifier->len] = 0;
}

void dp_reduce_SimpleType_tok_t_string(PARSER *self, ST_SIMPLE_TYPE *current, const tbytes *tok_identifier)
{
	current->st = E_ST_STRING;

	if(tok_identifier == NULL)
	{
		current->string_length[0] = 0;
	}
	else
	{
		memcpy(current->string_length, tok_identifier->ptr, tok_identifier->len);
		current->string_length[tok_identifier->len] = 0;
	}
}

void dp_reduce_SimpleType_tok_identifier(PARSER *self, ST_SIMPLE_TYPE* current, const tbytes *tok_identifier)
{
	tuint32 i;
	char id[1024];
	ST_TYPE *type;

	current->st = E_ST_REFER;

	for(i = 0; i < tok_identifier->len; ++i)
	{
		id[i] = tok_identifier->ptr[i];
	}
	id[i] = 0;
	strncpy(current->st_refer, id, TLIBC_MAX_IDENTIFIER_LENGTH);
done:
	return;
}

void dp_reduce_Type_SimpleType(PARSER *self, ST_TYPE *current, const ST_SIMPLE_TYPE *simple_type)
{
	current->type = E_SNT_SIMPLE;
	current->st = *simple_type;
}

void dp_reduce_Value_tok_identifier(PARSER *self, ST_VALUE* current, const tbytes sn_identifier)
{
	tuint32 data;
	tuint32 i;

	current->type = E_SNVT_IDENTIFIER;
	memcpy(current->val.identifier, sn_identifier.ptr, sn_identifier.len);
	current->val.identifier[sn_identifier.len] = 0;

	
done:
	return;
}

void dp_reduce_Value_tok_char(PARSER *self, ST_VALUE* current, const tchar pn_char)
{
	current->type = E_SNVT_CHAR;
	current->val.c = pn_char;
}

void dp_reduce_Value_tok_int64(PARSER *self, ST_VALUE* current, const tint64 i64)
{
	current->type = E_SNVT_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_hex_int64(PARSER *self, ST_VALUE* current, const tint64 i64)
{
	current->type = E_SNVT_HEX_INT64;
	current->val.i64 = i64;
}

void dp_reduce_Value_tok_uint64(PARSER *self, ST_VALUE* current, const tuint64 ui64)
{
	current->type = E_SNVT_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_hex_uint64(PARSER *self, ST_VALUE* current, const tuint64 ui64)
{
	current->type = E_SNVT_HEX_UINT64;
	current->val.ui64 = ui64;
}

void dp_reduce_Value_tok_double(PARSER *self, ST_VALUE* current, const double d)
{
	current->type = E_SNVT_DOUBLE;
	current->val.d = d;
}

void dp_reduce_Value_tok_bool(PARSER *self, ST_VALUE* current, const tbool b)
{
	current->type = E_SNVT_BOOL;
	current->val.b = b;
}

void dp_reduce_Value_tok_string(PARSER *self, ST_VALUE* current, const tstring str)
{
	current->type = E_SNVT_STRING;
	strncpy(current->val.str, str, TLIBC_MAX_IDENTIFIER_LENGTH);
}

void dp_reduce_Const(PARSER *self, ST_Const* current, const ST_SIMPLE_TYPE *type, const tbytes *identifier, const ST_VALUE *val)
{
	current->type = *type;
	//warning
	memcpy(current->identifier, identifier->ptr, identifier->len);
	current->identifier[identifier->len] = 0;
	
	current->val = *val;
}

void dp_reduce_ArgumentList_ArgumentList_tok_identifier(PARSER *self, ST_ARGUMENTS* current, const ST_ARGUMENTS* argument_list, const tbytes *identifier)
{
	*current = *argument_list;

	memcpy(current->arg_list[current->arg_list_num], identifier->ptr, identifier->len);
	current->arg_list[current->arg_list_num][identifier->len] = 0;
	++(current->arg_list_num);
}

void dp_reduce_ArgumentList_tok_identifier(PARSER *self, ST_ARGUMENTS* current, const tbytes *identifier)
{
	current->arg_list_num = 0;
	memcpy(current->arg_list[current->arg_list_num], identifier->ptr, identifier->len);
	current->arg_list[current->arg_list_num][identifier->len] = 0;
	++(current->arg_list_num);
}

void dp_reduce_Typedef(PARSER *self, ST_TYPEDEF *current, ST_SIMPLE_TYPE* type, const tbytes *tok_identifier)
{
	current->type = *type;
	memcpy(current->name, tok_identifier->ptr, tok_identifier->len);
	current->name[tok_identifier->len] = 0;
}

void dp_reduce_Value_tok_count(PARSER *self, ST_VALUE *current, const tbytes *identifier)
{
	const SYMBOL *symbols = symbols_search_identifier(&self->symbols, identifier, hpfalse);

	current->type = E_SNVT_UINT64;
	switch(symbols->type)
	{
	case EN_HST_ENUM:		
		current->val.ui64 = symbols->body.enum_def_list_num;
		break;
	case EN_HST_STRUCT:
		current->val.ui64 = symbols->body.enum_def_list_num;
		break;
	case EN_HST_UNION:
		current->val.ui64 = symbols->body.enum_def_list_num;
		break;
	}
}

void dp_reduce_UnionField(PARSER *self, ST_UNION_FIELD *current, const tbytes *key, const ST_SIMPLE_TYPE *simple_type, const tbytes *identifier, const ST_UNIX_COMMENT *comment)
{
	strncpy(current->key, key->ptr, key->len);
	current->key[key->len] = 0;

	current->simple_type = *simple_type;

	strncpy(current->name, identifier->ptr, identifier->len);
	current->name[identifier->len] = 0;

	current->comment = *comment;
}
