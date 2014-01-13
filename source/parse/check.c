#include "parse/check.h"
#include "symbols.h"
#include <string.h>
#include "tdata/tdata_types.h"

void dp_check_Import(PARSER *self, const YYLTYPE *yylloc, const char* str)
{
	if(strlen(str) >= MAX_PACKAGE_NAME_LENGTH)
	{
		scanner_error(&self->scanner, yylloc, E_LS_PACKAGE_NAME_TOO_LONG, MAX_PACKAGE_NAME_LENGTH);
	}
}

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, ST_SIMPLE_TYPE* type, const tchar *tok_identifier)
{
	if(type->st == E_ST_REFER)
	{
		const SYMBOL *symbol = symbols_search(&self->symbols, type->st_refer, hpfalse);
		if(symbol == NULL)
		{
			scanner_error(&self->scanner, yylloc, E_LS_CAN_NOT_FIND_IDENTIFIER, type->st_refer);
		}

		if((symbol->type != EN_HST_TYPEDEF) && (symbol->type != EN_HST_ENUM) 
			&& (symbol->type != EN_HST_STRUCT) && (symbol->type != EN_HST_UNION))
		{
			scanner_error(&self->scanner, yylloc, E_LS_NOT_TYPE, type->st_refer);
		}
	}
	
}

void dp_check_Const(PARSER *self, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *type, const tchar *id, const ST_VALUE *val)
{
	const ST_SIMPLE_TYPE *real_type = NULL;
	const ST_VALUE *real_val = NULL;

	if(symbols_search(&self->symbols, id, hpfalse) != NULL)
	{
		scanner_error(&self->scanner, yylloc, E_LS_IDENTIFIER_REDEFINITION, id);
	}

	
	real_type = symbols_get_real_type(&self->symbols, type);
	real_val = symbols_get_real_value(&self->symbols, val);
	switch(real_type->st)
	{
	case E_ST_INT8:
	case E_ST_INT16:
	case E_ST_INT32:
	case E_ST_INT64:
		{
			if((real_val->type != E_SNVT_INT64) && (real_val->type != E_SNVT_HEX_INT64))
			{
				scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}
			switch(real_type->st)
			{
			case E_ST_INT8:
				if((tint8)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT16:
				if((tint16)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT32:
				if((tint32)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT64:
				if((tint64)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			}
			break;
		}
	case E_ST_UINT8:
	case E_ST_UINT16:
	case E_ST_UINT32:
	case E_ST_UINT64:
		{
			tuint64 ui64 = 0;
			if((real_val->type == E_SNVT_INT64) || (real_val->type == E_SNVT_HEX_INT64))
			{
				if(real_val->val.i64 < 0)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				ui64 = real_val->val.i64;
			}
			else if((real_val->type == E_SNVT_UINT64) || (real_val->type == E_SNVT_HEX_UINT64))
			{
				ui64 = real_val->val.ui64;
			}
			else
			{
				scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}

			switch(real_type->st)
			{
			case E_ST_UINT8:
				if((tuint8)ui64 != ui64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT16:
				if((tuint16)ui64 != ui64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT32:
				if((tuint32)ui64 != ui64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT64:
				if((tuint64)ui64 != ui64)
				{
					scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			}
			break;
		}
	case E_ST_CHAR:
		if(real_val->type != E_SNVT_CHAR)
		{
			scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_DOUBLE:
		if(real_val->type != E_SNVT_DOUBLE)
		{
			scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_STRING:
		{
			if(real_val->type != E_SNVT_STRING)
			{
				scanner_error(&self->scanner, yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}

			if(real_type->string_length[0] != 0)
			{
				scanner_error(&self->scanner, yylloc, E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE);
			}
			break;
		}
	default:
		scanner_error(&self->scanner, yylloc, E_LS_CONST_TYPE_ERROR);
	}
}

void dp_check_Enum(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier)
{
	if(symbols_search(&self->symbols, identifier, hpfalse) != NULL)
	{
		scanner_error(&self->scanner, yylloc, E_LS_IDENTIFIER_REDEFINITION, identifier);
	}
}

void dp_check_EnumDefList(PARSER *self, const YYLTYPE *yylloc, tuint32 enum_def_list_num)
{
	if(enum_def_list_num >= MAX_ENUM_DEF_LIST_NUM)
	{
		scanner_error(&self->scanner, yylloc, E_LS_TOO_MANY_MEMBERS, MAX_ENUM_DEF_LIST_NUM);
	}
}

void dp_check_EnumDef(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier, const ST_VALUE *st_value)
{
	ST_SIMPLE_TYPE type;
	type.st = E_ST_INT32;

	dp_check_Const(self, yylloc, &type, identifier, st_value);
}

void dp_check_Union_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier)
{
	if(symbols_search(&self->symbols, identifier, hpfalse) != NULL)
	{
		scanner_error(&self->scanner, yylloc, E_LS_IDENTIFIER_REDEFINITION, identifier);
	}
}

void dp_check_Union_Parameters(PARSER *self, const YYLTYPE *yylloc, const ST_Parameters *parameters)
{
	const ST_SIMPLE_TYPE *type = NULL;

	if(parameters->par_list_num != 1)
	{
		scanner_error(&self->scanner, yylloc, E_LS_UNION_PARAMETERS_ERROR);
	}

	if(strcmp(parameters->par_list[0].identifier, "selector") != 0)
	{
		scanner_error(&self->scanner, yylloc, E_LS_UNION_PARAMETERS_ERROR);
	}

	//type = symbols_get_real_type(&self->symbols, &parameters->par_list[0].type);

}

void dp_check_UnionFieldList(PARSER *self, const YYLTYPE *yylloc, tuint32 union_field_list_num)
{
	if(union_field_list_num >= MAX_UNION_FIELD_LIST_NUM)
	{
		scanner_error(&self->scanner, yylloc, E_LS_TOO_MANY_MEMBERS, MAX_UNION_FIELD_LIST_NUM);
	}
}

void dp_check_UnionField(PARSER *self, const YYLTYPE *yylloc, const tchar *key, const ST_SIMPLE_TYPE *simple_type, const tchar *identifier)
{

}

void dp_check_FieldList(PARSER *self, const YYLTYPE *yylloc, tuint32 field_list_num)
{
	if(field_list_num >= MAX_FIELD_LIST_NUM)
	{
		scanner_error(&self->scanner, yylloc, E_LS_TOO_MANY_MEMBERS, MAX_FIELD_LIST_NUM);
	}
}

static void dp_check_expression_value_type(PARSER *self, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *type)
{
	const ST_SIMPLE_TYPE *pn_type = symbols_get_real_type(&self->symbols, type);
	if(pn_type == NULL)
	{
		scanner_error(&self->scanner, yylloc, E_LS_UNKNOW);
		goto done;
	}

	if(pn_type->st == E_ST_REFER)
	{
		const SYMBOL *symbols = symbols_search(&self->symbols, pn_type->st_refer, hptrue);
		if((symbols == NULL) || (symbols->type != EN_HST_ENUM))
		{
			scanner_error(&self->scanner, yylloc, E_LS_UNKNOW);
			goto done;
		}
	}
	else if((pn_type->st < E_ST_INT8) || (pn_type->st > E_ST_UINT64))
	{
		scanner_error(&self->scanner, yylloc, E_LS_UNKNOW);
		goto done;
	}
done:
	return;
}


void dp_check_Field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field)
{

}
