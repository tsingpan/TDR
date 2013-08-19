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

	//printf("find: %s : %x\n", name, symbol);
	return symbol;
}

static const HOTDATA_SYMBOLS* dp_find_symbol(DATA_PARSER *self, const PN_IDENTIFIER* tok_identifier)
{
	char name[MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return dp_find_symbol_by_string(self, name);
}


static const HOTDATA_SYMBOLS* dp_find_symbol_by_string_local(DATA_PARSER *self, const char* name)
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
		return NULL;
	}
	return symbol;
}

static const HOTDATA_SYMBOLS* dp_find_symbol_local(DATA_PARSER *self, const PN_IDENTIFIER* tok_identifier)
{
	char name[MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return dp_find_symbol_by_string_local(self, name);
}

static hpint32 dp_save_symbol_string(DATA_PARSER *self, const char *name, const HOTDATA_SYMBOLS *symbol)
{
	char global_name[MAX_IDENTIFIER_LENGTH * 2];

	

	if(self->domain[0])
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s:%s", self->domain, name);
	}
	else
	{
		snprintf(global_name, MAX_IDENTIFIER_LENGTH * 2, "%s", name);
	}

	//printf("save %s : %x\n", global_name, symbol);
	if(!trie_store_if_absent(self->hotdata_symbols, global_name, symbol))
	{
		return E_HP_ERROR;
	}
	return E_HP_NOERROR;
}

static hpint32 dp_save_symbol(DATA_PARSER *self, const PN_IDENTIFIER *tok_identifier, const HOTDATA_SYMBOLS *symbol)
{
	char name[MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	return dp_save_symbol_string(self, name, symbol);
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

void dp_check_Const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	char id[MAX_IDENTIFIER_LENGTH];

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	if(dp_find_symbol(self, tok_identifier) != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_SYMBOL_REDEFINITION, id);
	}
}

void dp_check_Const_add_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier, const PN_VALUE *pn_value)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));
	const PN_VALUE *val = get_value(self, pn_value);
	char id[1024];
	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	if(val == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	ptr->type = EN_HST_VALUE;
	ptr->body.val = *val;
	
	if(dp_save_symbol(self, tok_identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR, id);
	}
done:
	return;
}

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const PN_IDENTIFIER* tok_identifier, const PN_VALUE* sn_value)
{
	char id[MAX_IDENTIFIER_LENGTH];
	const ST_VALUE* val = get_value(self, sn_value);
	const ST_TYPE* type = get_type(self, sn_type);

	if((type == NULL) || (val == NULL))
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR, id);
		goto done;
	}

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;	
	
	if(type->type == E_SNT_SIMPLE)
	{
		switch(type->st)
		{
		case E_ST_INT8:
		case E_ST_INT16:
		case E_ST_INT32:
		case E_ST_INT64:
		case E_ST_UINT8:
		case E_ST_UINT16:
		case E_ST_UINT32:
		case E_ST_UINT64:
		case E_ST_CHAR:
		case E_ST_DOUBLE:
			break;
		default:
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_INVALID_CONSTANCE_TYPE);
			goto done;
		}
		if((val->type == E_SNVT_UINT64) || (val->type == E_SNVT_HEX_UINT64))
		{
			hpuint64 tmp = val->val.ui64 >> 32;
			tmp >>= 32;
			if(tmp)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
				goto done;
			}
		}
		else if((val->type == E_SNVT_INT64) || (val->type == E_SNVT_HEX_INT64))
		{
			hpint64 tmp = val->val.ui64 >> 32;
			tmp >>= 32;
			if(tmp)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_CONSTANCE_TYPE_TOO_SMALL, id);
				goto done;
			}
		}
		else if(val->type == E_SNVT_IDENTIFIER)
		{

		}
		else if(val->type == E_SNVT_DOUBLE)
		{

		}
		else if(val->type == E_SNVT_CHAR)
		{

		}
		else if(val->type == E_SNVT_BOOL)
		{

		}
		else
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_UNKNOW_CONSTANT_VALUE);
			goto done;
		}
	}
	else if(type->type == E_SNT_REFER)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR, id);
	}
	else if(type->type == E_SNT_CONTAINER)
	{
		if(type->ct == E_CT_STRING)
		{
			if(sn_value->type != E_SNVT_STRING)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR, id);
			}
		}
		else
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR, id);
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
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	if(type->type == E_SNT_CONTAINER)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	symbol->type = EN_HST_TYPE;
	symbol->body.type = *type;
	if(dp_save_symbol_string(self, sn_typedef->name, symbol) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_SYMBOL_REDEFINITION, sn_typedef->name);
		goto done;
	}

done:
	return;
}

void dp_check_domain_begin(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	memcpy(self->domain, tok_identifier->ptr, tok_identifier->len);
	self->domain[tok_identifier->len] = 0;
}

void dp_check_domain_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	self->domain[0] = 0;
}

void dp_check_Parameter_add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_Parameter *pn_parameter)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	ptr->type = EN_HST_PARAMETER;
	ptr->body.para = *pn_parameter;

	if(dp_save_symbol_string(self, pn_parameter->identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
	}
done:
	return;
}

void dp_check_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	const HOTDATA_SYMBOLS *symbol = dp_find_symbol(self, tok_identifier);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

done:
	return;
}


void dp_check_tok_identifier_local(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	const HOTDATA_SYMBOLS *symbol = dp_find_symbol_local(self, tok_identifier);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

done:
	return;
}

void dp_check_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	const HOTDATA_SYMBOLS *symbol = dp_find_symbol(self, tok_identifier);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

done:
	return;
}

void dp_check_EnumDef_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val)
{
	const PN_TypeAnnotations *ta = &self->pn_definition.definition.de_enum.type_annotations;
	hpuint32 i, j;
	hpint64 i64;
	val = get_value(self, val);
	if(val == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	if(val->type == E_SNVT_INT64)
	{
		i64 = val->val.i64;
	}
	else if(val->type == E_SNVT_HEX_INT64)
	{
		i64 = val->val.hex_i64;		
	}
	else
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	for(i = 0;i < ta->ta_list_num; ++i)
	{
		switch(ta->ta_list[i].type)
		{
		case E_TA_UNIQUE:
			//检查值的类型
			if(!ta->ta_list[i].val.val.b)
			{
				break;
			}
			for(j = 0; j < self->pn_definition.definition.de_enum.enum_def_list_num; ++j)
			{
				ST_VALUE *_val = &self->pn_definition.definition.de_enum.enum_def_list[j].val;
				hpint64 _i64;
				if(_val->type == E_SNVT_HEX_INT64)
				{
					_i64 = _val->val.hex_i64;
				}
				else
				{
					_i64 = _val->val.i64;
				}
				if(i64 == _i64)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}
			break;
		case E_TA_LOWER_BOUND:
			{
				const ST_VALUE *_val = val;
				hpint64 _i64;
				const PN_VALUE *lower_bound = get_value(self, &ta->ta_list[i].val);
				hpint64 li64;
				if(lower_bound == NULL)
				{
					break;
				}

				if(lower_bound->type == E_SNVT_INT64)
				{
					li64 = lower_bound->val.i64;
				}
				else if(lower_bound->type == E_SNVT_HEX_INT64)
				{
					li64 = lower_bound->val.hex_i64;
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}

				
				if(_val->type == E_SNVT_HEX_INT64)
				{
					_i64 = _val->val.hex_i64;
				}
				else
				{
					_i64 = _val->val.i64;
				}
				if(_i64 < li64)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}

				break;
			}
		case E_TA_UPPER_BOUND:
			{
				const ST_VALUE *_val = val;
				hpint64 _i64;
				const PN_VALUE *upper_bound = get_value(self, &ta->ta_list[i].val);
				hpint64 upi64;
				if(upper_bound == NULL)
				{
					break;
				}

				if(upper_bound->type == E_SNVT_INT64)
				{
					upi64 = upper_bound->val.i64;
				}
				else if(upper_bound->type == E_SNVT_HEX_INT64)
				{
					upi64 = upper_bound->val.hex_i64;
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}


				if(_val->type == E_SNVT_HEX_INT64)
				{
					_i64 = _val->val.hex_i64;
				}
				else
				{
					_i64 = _val->val.i64;
				}
				if(_i64 >= upi64)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}

				break;
			}
		}
	}

done:
	return;
}


void dp_check_TypeAnnotation_tok_unique_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val)
{
	if(val->type != E_SNVT_BOOL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}
	
done:
	return;
}

void dp_check_TypeAnnotation_bound_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val)
{
	val = get_value(self, val);

	if((val == NULL) || (val->type != E_SNVT_INT64))
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

done:
	return;
}


void dp_check_TypeAnnotation_tok_switch_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val)
{
	if(val->type != E_SNVT_IDENTIFIER)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

done:
	return;
}

void dp_check_TypeAnnotation_tok_counter_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	if(val->type != E_SNVT_STRING)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	ptr->type = EN_HST_VALUE;
	ptr->body.val.type = E_ST_UINT32;
	if(dp_find_symbol_by_string(self, val->val.str) != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}
done:
	return;
}

void dp_check_Union_Parameters(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union)
{
	hpuint32 i, j;
	for(i = 0; i < de_union->ta.ta_list_num; ++i)
	{
		if(de_union->ta.ta_list[i].type == E_TA_SWITCH)
		{
			hpbool found = hpfalse;
			for(j = 0; j < de_union->parameters.par_list_num; ++j)
			{
				if(strcmp(de_union->parameters.par_list[j].identifier, de_union->ta.ta_list[i].val.val.identifier) == 0)
				{
					found = hptrue;
					break;
				}
			}
			if(!found)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
		}
	}

done:
	return;
}

void dp_check_Union_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	self->in_union = hptrue;
}

void dp_check_Union_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	self->in_union = hpfalse;
}

void dp_check_Struct_begin(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	self->in_struct = hptrue;
}

void dp_check_Struct_end(DATA_PARSER *self, const YYLTYPE *yylloc)
{
	self->in_struct = hpfalse;
}

static void dp_check_expression_value_type(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE *type)
{
	const PN_TYPE *pn_type = get_type(self, type);
	if(pn_type == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}

	if(pn_type->type == E_SNT_SIMPLE)
	{
		if((pn_type->st < E_ST_INT8) || (pn_type->st > E_ST_BOOL))
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
	}
	else if(pn_type->type == E_SNT_REFER)
	{
		const HOTDATA_SYMBOLS *symbols = dp_find_symbol_by_string(self, pn_type->ot);
		if((symbols == NULL) || (symbols->type != EN_HST_ENUM))
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
	}
	else
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}
done:
	return;
}

void dp_check_FieldExpression_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val)
{
	switch(val->type)
	{
	case E_SNVT_IDENTIFIER:
		{
			const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, val->val.identifier);
			if(symbol == NULL)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
			
			switch(symbol->type)
			{
			case EN_HST_VALUE:
				{
					const PN_VALUE *val = get_value(self, &symbol->body.val);
					if(val == NULL)
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}

					if((val->type != E_SNVT_INT64) && (val->type != E_SNVT_UINT64)
						&& (val->type != E_SNVT_HEX_INT64) && (val->type != E_SNVT_HEX_UINT64))
					{					
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
					break;
				}
			case EN_HST_FIELD:
				{
					dp_check_expression_value_type(self, yylloc, &symbol->body.field.type);
					break;
				}
			case EN_HST_PARAMETER:
				{
					dp_check_expression_value_type(self, yylloc, &symbol->body.para.type);
					break;
				}
			default:
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}

			break;
		}
	case E_SNVT_INT64:
	case E_SNVT_UINT64:
	case E_SNVT_HEX_INT64:
	case E_SNVT_HEX_UINT64:
		break;
	default:
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
		goto done;
	}
done:
	return;
}

static void dp_check_field_vector_args(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_ARGUMENTS *args, hpuint32 start_index)
{
	hpuint32 i;
	for(i = start_index; i < args->arg_list_num; ++i)
	{
		if(args->arg_list[i].type == E_SNT_REFER)
		{
			const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, args->arg_list[i].ot);
			if(symbol == NULL)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}

			if(symbol->type == EN_HST_VALUE)
			{
				if(!((symbol->body.val.type >= E_SNVT_BOOL) && (symbol->body.val.type <= E_SNVT_HEX_UINT64)))
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}
			else if(symbol->type == EN_HST_PARAMETER)
			{
			}
			else if(symbol->type == EN_HST_FIELD)
			{
				if(symbol->body.field.type.type == E_SNT_SIMPLE)
				{
					if(!((symbol->body.field.type.st >= E_ST_INT8) && (symbol->body.field.type.st <= E_ST_BOOL)))
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
				}
				else if(symbol->body.field.type.type == E_SNT_REFER)
				{

				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}
			else
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
		}
		else
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
	}

done:
	return;
}

void dp_check_Field(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_FIELD *pn_field)
{
	if(self->in_union)
	{
		if(pn_field->condition.empty)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}

		if(pn_field->condition.exp.neg)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}

		if(pn_field->condition.exp.oper != E_EO_EQUAL)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
	}



	if(pn_field->type.type == E_SNT_SIMPLE)
	{
		if(pn_field->args.arg_list_num != 0)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
	}
	else if(pn_field->type.type == E_SNT_REFER)
	{
		const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->type.ot);
		if(symbol == NULL)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
		if((symbol->type != EN_HST_TYPE) && (symbol->type != EN_HST_ENUM) && (symbol->type != EN_HST_UNION) && (symbol->type != EN_HST_STRUCT))
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			goto done;
		}
		dp_check_field_vector_args(self, yylloc, &pn_field->args, 0);
	}
	else if(pn_field->type.type == E_SNT_CONTAINER)
	{
		switch(pn_field->type.ct)
		{
		case E_CT_VECTOR:
			if(pn_field->args.arg_list_num < 3)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}

			if(pn_field->args.arg_list[0].type == E_SNT_REFER)
			{
				const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[0].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_TYPE)
				{
					const ST_TYPE *type = get_type(self, &symbol->body.type);
					if(type == NULL)
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
					if(type->type == E_SNT_SIMPLE)
					{
						if(pn_field->args.arg_list_num != 3)
						{
							scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
							goto done;
						}
					}
					else if(type->type == E_SNT_REFER)
					{
						dp_check_field_vector_args(self, yylloc, &pn_field->args, 3);
					}
				}
				else if(symbol->type == EN_HST_ENUM)
				{
					if(pn_field->args.arg_list_num != 3)
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
				}
				else if(symbol->type == EN_HST_STRUCT)
				{
					dp_check_field_vector_args(self, yylloc, &pn_field->args, 3);
				}
				else if(symbol->type == EN_HST_UNION)
				{
					dp_check_field_vector_args(self, yylloc, &pn_field->args, 3);
				}
			}
			else if(pn_field->args.arg_list[0].type == E_SNT_SIMPLE)
			{
				if(pn_field->args.arg_list_num != 3)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}


			if(pn_field->args.arg_list[1].type == E_SNT_REFER)
			{
				const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[1].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_VALUE)
				{
					if((symbol->body.val.type >= E_SNVT_INT64) && (symbol->body.val.type <= E_SNVT_HEX_UINT64))
					{

					}
					else
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}
			else
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}

			if(pn_field->args.arg_list[2].type != E_SNT_REFER)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
			break;
		case E_CT_STRING:
			if(pn_field->args.arg_list_num != 1)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
			if(pn_field->args.arg_list[0].type == E_SNT_REFER)
			{
				const HOTDATA_SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[0].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_VALUE)
				{
					if((symbol->body.val.type >= E_SNVT_INT64) && (symbol->body.val.type <= E_SNVT_HEX_UINT64))
					{

					}
					else
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
						goto done;
					}
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
					goto done;
				}
			}
			else
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
				goto done;
			}
			break;
		}
	}

done:
	return;
}

void dp_check_Field_add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_FIELD *pn_field)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	ptr->type = EN_HST_FIELD;
	ptr->body.field = *pn_field;

	if(dp_save_symbol_string(self, pn_field->identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
	}
done:
	return;
}


void dp_check_Enum_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier, const ST_ENUM *pn_enum)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));
	hpuint32 i;

	ptr->type = EN_HST_ENUM;

	if(dp_save_symbol(self, tok_identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
	}


	for(i = 0;i < pn_enum->type_annotations.ta_list_num; ++i)
		if(pn_enum->type_annotations.ta_list[i].type == E_TA_COUNTER)
		{
			ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));
			ptr->type = EN_HST_VALUE;
			ptr->body.val.type = E_SNVT_UINT64;
			ptr->body.val.val.ui64 = pn_enum->enum_def_list_num;

			if(dp_save_symbol_string(self, pn_enum->type_annotations.ta_list[i].val.val.str, ptr) != E_HP_NOERROR)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
			}
		}

done:
	return;
}

void dp_check_Struct_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	ptr->type = EN_HST_STRUCT;

	if(dp_save_symbol(self, tok_identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
	}
done:
	return;
}

void dp_check_Union_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier)
{
	HOTDATA_SYMBOLS *ptr = (HOTDATA_SYMBOLS*)malloc(sizeof(HOTDATA_SYMBOLS));

	ptr->type = EN_HST_UNION;

	if(dp_save_symbol(self, tok_identifier, ptr) != E_HP_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_HP_ERROR);
	}
done:
	return;
}
