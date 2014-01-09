#include "parse/check.h"
#include "symbols.h"
#include <string.h>
#include "tdata/tdata_types.h"

static const ST_TYPE* get_type(PARSER *self, const ST_TYPE* sn_type)
{
	if(sn_type->type == E_SNT_SIMPLE)
	{
		if(sn_type->st.st == E_ST_REFER)
		{
			const SYMBOL *ptr = symbols_search_string(&self->parser_symbols, sn_type->st.st_refer, hptrue);
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
	else
	{
		return sn_type;
	}
}

static const ST_VALUE* get_value(PARSER *self, const ST_VALUE* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const SYMBOL *ptr = symbols_search_string(&self->parser_symbols, sn_value->val.identifier, hptrue);
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

void dp_check_Const_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier)
{
	char id[TLIBC_MAX_IDENTIFIER_LENGTH];

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	if(symbols_search_identifier(&self->parser_symbols, tok_identifier, hpfalse) != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_SYMBOL_REDEFINITION, id);
	}
}

void dp_check_Const_add_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier, const ST_VALUE *pn_value)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));
	const ST_VALUE *val = get_value(self, pn_value);
	char id[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;

	if(val == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	ptr->type = EN_HST_VALUE;
	ptr->body.val = *val;
	
	if(symbols_save(&self->parser_symbols, id, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR, id);
	}
done:
	return;
}

void dp_check_constant_value(PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const tbytes* tok_identifier, const ST_VALUE* sn_value)
{
	char id[TLIBC_MAX_IDENTIFIER_LENGTH];
	const ST_VALUE* val = get_value(self, sn_value);
	const ST_TYPE* type = get_type(self, sn_type);

	if((type == NULL) || (val == NULL))
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR, id);
		goto done;
	}

	memcpy(id, tok_identifier->ptr, tok_identifier->len);
	id[tok_identifier->len] = 0;	
	
	if(type->type == E_SNT_SIMPLE)
	{
		
		switch(type->st.st)
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
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_INVALID_CONSTANCE_TYPE);
			goto done;
		}
		if((val->type == E_SNVT_UINT64) || (val->type == E_SNVT_HEX_UINT64))
		{
			tuint64 tmp = val->val.ui64 >> 32;
			tmp >>= 32;
			if(tmp)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_CONSTANCE_TYPE_TOO_SMALL, id);
				goto done;
			}
		}
		else if((val->type == E_SNVT_INT64) || (val->type == E_SNVT_HEX_INT64))
		{
			tint64 tmp = val->val.ui64 >> 32;
			tmp >>= 32;
			if(tmp)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_CONSTANCE_TYPE_TOO_SMALL, id);
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
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_UNKNOW_CONSTANT_VALUE);
			goto done;
		}
	}	
	else if(type->type == E_SNT_CONTAINER)
	{
		if(type->ct == E_CT_STRING)
		{
			if(sn_value->type != E_SNVT_STRING)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR, id);
			}
		}
		else
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR, id);
		}
	}

done:
	return;
}

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef)
{
	SYMBOL *symbol = (SYMBOL*)malloc(sizeof(SYMBOL));
	const ST_TYPE*type = get_type(self, &sn_typedef->type);
	if(type == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	if(type->type == E_SNT_CONTAINER)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	symbol->type = EN_HST_TYPE;
	symbol->body.type = *type;
	if(symbols_save(&self->parser_symbols, sn_typedef->name, symbol) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_SYMBOL_REDEFINITION, sn_typedef->name);
		goto done;
	}

done:
	return;
}

void dp_check_Parameter_add(PARSER *self, const YYLTYPE *yylloc, const ST_Parameter *pn_parameter)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_PARAMETER;
	ptr->body.para = *pn_parameter;

	if(symbols_save(&self->parser_symbols, pn_parameter->identifier, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}
done:
	return;
}

void dp_check_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier)
{
	const SYMBOL *symbol = symbols_search_identifier(&self->parser_symbols, tok_identifier, hpfalse);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

done:
	return;
}


void dp_check_tok_identifier_local(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier)
{
	const SYMBOL *symbol = symbols_search_identifier(&self->parser_symbols, tok_identifier, hpfalse);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

done:
	return;
}

void dp_check_EnumDef_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier)
{
	const SYMBOL *symbol = symbols_search_identifier(&self->parser_symbols, tok_identifier, hpfalse);
	if(symbol != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

done:
	return;
}

void dp_check_Union_Parameters(PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union)
{
	tuint32 i, j;
done:
	return;
}

void dp_check_Union_begin(PARSER *self, const YYLTYPE *yylloc)
{
	self->in_union = hptrue;
}

void dp_check_Union_end(PARSER *self, const YYLTYPE *yylloc)
{
	self->in_union = hpfalse;
}

void dp_check_Struct_begin(PARSER *self, const YYLTYPE *yylloc)
{
	self->in_struct = hptrue;
}

void dp_check_Struct_end(PARSER *self, const YYLTYPE *yylloc)
{
	self->in_struct = hpfalse;
}

static void dp_check_expression_value_type(PARSER *self, const YYLTYPE *yylloc, const ST_TYPE *type)
{
	const ST_TYPE *pn_type = get_type(self, type);
	if(pn_type == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	if(pn_type->type == E_SNT_SIMPLE)
	{
		if(pn_type->st.st == E_ST_REFER)
		{
			const SYMBOL *symbols = symbols_search_string(&self->parser_symbols, pn_type->st.st_refer, hptrue);
			if((symbols == NULL) || (symbols->type != EN_HST_ENUM))
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}
		}
		else if((pn_type->st.st < E_ST_INT8) || (pn_type->st.st > E_ST_BOOL))
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
			goto done;
		}
	}
	else
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}
done:
	return;
}


void dp_check_Field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field)
{
	if(self->in_union)
	{
		if(pn_field->condition.oper != E_EO_CASE)
		{
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
			goto done;
		}
	}



	if(pn_field->type.type == E_SNT_SIMPLE)
	{
		if(pn_field->type.st.st == E_ST_REFER)
		{
			const SYMBOL *symbol = symbols_search_string(&self->parser_symbols, pn_field->type.st.st_refer, hptrue);
			if(symbol == NULL)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}
			if((symbol->type != EN_HST_TYPE) && (symbol->type != EN_HST_ENUM) && (symbol->type != EN_HST_UNION) && (symbol->type != EN_HST_STRUCT))
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}
		}
		else if(pn_field->args.arg_list_num != 0)
		{			
			scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
			goto done;
		}
	}	
	else if(pn_field->type.type == E_SNT_CONTAINER)
	{
		switch(pn_field->type.ct)
		{
		case E_CT_VECTOR:
			/*
			if(pn_field->args.arg_list_num < 3)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}

			if(pn_field->args.arg_list[0].type == E_SNT_REFER)
			{
				const SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[0].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_TYPE)
				{
					const ST_TYPE *type = get_type(self, &symbol->body.type);
					if(type == NULL)
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
						goto done;
					}
					if(type->type == E_SNT_SIMPLE)
					{
						if(pn_field->args.arg_list_num != 3)
						{
							scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
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
						scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
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
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
			}


			if(pn_field->args.arg_list[1].type == E_SNT_REFER)
			{
				const SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[1].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_VALUE)
				{
					if((symbol->body.val.type >= E_SNVT_INT64) && (symbol->body.val.type <= E_SNVT_HEX_UINT64))
					{

					}
					else
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
						goto done;
					}
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
			}
			else
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}

			if(pn_field->args.arg_list[2].type != E_SNT_REFER)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}*/
			break;
		case E_CT_STRING:/*
			if(pn_field->args.arg_list_num != 1)
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}
			if(pn_field->args.arg_list[0].type == E_SNT_REFER)
			{
				const SYMBOLS *symbol = dp_find_symbol_by_string(self, pn_field->args.arg_list[0].ot);
				if(symbol == NULL)
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
				if(symbol->type == EN_HST_VALUE)
				{
					if((symbol->body.val.type >= E_SNVT_INT64) && (symbol->body.val.type <= E_SNVT_HEX_UINT64))
					{

					}
					else
					{
						scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
						goto done;
					}
				}
				else
				{
					scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
					goto done;
				}
			}
			else
			{
				scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
				goto done;
			}*/
			break;
		}
	}

done:
	return;
}

void dp_check_Field_add(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_FIELD;
	ptr->body.field = *pn_field;

	if(symbols_save(&self->parser_symbols, pn_field->identifier, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}
done:
	return;
}


void dp_check_Enum_Add(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier, const ST_ENUM *pn_enum)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));
	tuint32 i;

	ptr->type = EN_HST_ENUM;
	ptr->body.enum_def_list_num = pn_enum->enum_def_list_num;

	if(symbols_save(&self->parser_symbols, pn_enum->name, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}

done:
	return;
}

void dp_check_Struct_Add(PARSER *self, const YYLTYPE *yylloc, const ST_STRUCT *de_struct)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_STRUCT;
	ptr->body.field_list_num = de_struct->field_list.field_list_num;

	if(symbols_save(&self->parser_symbols, de_struct->name, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}

done:
	return;
}

void dp_check_Union_Add(PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_UNION;
	ptr->body.field_list_num = de_union->field_list.field_list_num;

	if(symbols_save(&self->parser_symbols, de_union->name, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}
done:
	return;
}
