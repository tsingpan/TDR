#include "parse/check.h"
#include "symbols.h"
#include <string.h>
#include "tdata/tdata_types.h"
void dp_check_Const(PARSER *self, const YYLTYPE *yylloc, ST_Const* current, const ST_TYPE *type, const tbytes *identifier, const ST_VALUE *val)
{
	const ST_TYPE *real_type = NULL;
	char id[TLIBC_MAX_IDENTIFIER_LENGTH];

	memcpy(id, identifier->ptr, identifier->len);
	id[identifier->len] = 0;


	//1, 判断符号是否重复
	if(symbols_search_string(&self->symbols, id, hpfalse) != NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_SYMBOL_REDEFINITION, id);
		goto done;
	}

	//2, 判断类型
	real_type = symbols_get_real_type(&self->symbols, type);
	if(real_type == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}
	if((real_type->type == E_SNT_CONTAINER) && (real_type->ct == E_CT_VECTOR))
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	//3, 判断值是否会溢出
	//todo
done:
	return;
}

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef)
{	
	const ST_TYPE*type = symbols_get_real_type(&self->symbols, &sn_typedef->type);
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

done:
	return;
}

void dp_check_Parameter_add(PARSER *self, const YYLTYPE *yylloc, const ST_Parameter *pn_parameter)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_PARAMETER;
	ptr->body.para = *pn_parameter;

	if(symbols_save(&self->symbols, pn_parameter->identifier, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}
done:
	return;
}

void dp_check_EnumDef(PARSER *self, const YYLTYPE *yylloc, const ST_ENUM_DEF *enum_def)
{
	const SYMBOL *symbol = symbols_search_string(&self->symbols, enum_def->identifier, hpfalse);
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
	const ST_TYPE *pn_type = symbols_get_real_type(&self->symbols, type);
	if(pn_type == NULL)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
		goto done;
	}

	if(pn_type->type == E_SNT_SIMPLE)
	{
		if(pn_type->st.st == E_ST_REFER)
		{
			const SYMBOL *symbols = symbols_search_string(&self->symbols, pn_type->st.st_refer, hptrue);
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
			const SYMBOL *symbol = symbols_search_string(&self->symbols, pn_field->type.st.st_refer, hptrue);
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

	if(symbols_save(&self->symbols, pn_field->identifier, ptr) != E_TD_NOERROR)
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

	if(symbols_save(&self->symbols, de_struct->name, ptr) != E_TD_NOERROR)
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

	if(symbols_save(&self->symbols, de_union->name, ptr) != E_TD_NOERROR)
	{
		scanner_stack_error(&self->scanner_stack, yylloc, E_TD_ERROR);
	}
done:
	return;
}
