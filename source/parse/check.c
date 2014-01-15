#include "parse/check.h"
#include "symbols.h"
#include <string.h>
#include "tdata/tdata_types.h"

#include <assert.h>

void check_identifier_defined(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	if(symbols_search(symbols, prefix, identifier) == NULL)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_DEFINED, prefix, identifier);
	}
}

void check_identifier_not_defined(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	if(symbols_search(symbols, prefix, identifier) != NULL)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_DEFINED, prefix, identifier);
	}
}

void check_identifier_is_type(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	const SYMBOL *symbol = symbols_search(symbols, prefix, identifier);
	assert(symbols != NULL);

	if((symbol->type != EN_HST_TYPEDEF) && (symbol->type != EN_HST_ENUM) 
		&& (symbol->type != EN_HST_STRUCT) && (symbol->type != EN_HST_UNION))
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_TYPE, prefix, identifier);
	}
}

void check_identifier_is_value(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	const SYMBOL * symbol = symbols_search(symbols, prefix, identifier);

	if(symbol->type != EN_HST_VALUE)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_CONSTANCE, identifier);
	}
}

void check_identifier_is_positive_integer(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	const ST_VALUE *val = NULL;
	const SYMBOL *symbol = symbols_search(symbols, prefix, identifier);
	assert(symbols != NULL);

	if(symbol->type != EN_HST_VALUE)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_POSITIVE_INTEGER, prefix, identifier);
	}

	val = symbols_get_real_value(symbols, &symbol->body.val);
	assert(val != NULL);

	switch(val->type)
	{
	case E_SNVT_INT64:
	case E_SNVT_HEX_INT64:
		if(val->val.i64 < 0)
		{
			scanner_error(yylloc, E_LS_IDENTIFIER_NOT_POSITIVE_INTEGER, prefix, identifier);
		}
	case E_SNVT_UINT64:
	case E_SNVT_HEX_UINT64:
		break;
	default:
		scanner_error(yylloc, E_LS_IDENTIFIER_NOT_POSITIVE_INTEGER, prefix, identifier);
	}
}

void check_string_length_defined(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *symbol_type)
{
	if(symbol_type->st != E_ST_STRING)
	{
		goto done;
	}

	if(symbol_type->string_length[0] == 0)
	{
		scanner_error(yylloc, E_LS_STRING_LENGTH_NOT_DEFINED);
	}
done:
	return;
}

//检查类型是否为整数
void check_integer_type(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *simple_type)
{
	const ST_SIMPLE_TYPE *real_type = symbols_get_real_type(symbols, simple_type);
	assert(real_type != NULL);
	
	switch(real_type->st)
	{
	case E_ST_INT8:
	case E_ST_INT16:
	case E_ST_INT32:
	case E_ST_INT64:
	case E_ST_UINT8:
	case E_ST_UINT16:
	case E_ST_UINT32:
	case E_ST_UINT64:
		break;
	case E_ST_REFER:
		{
			const SYMBOL* enum_type = symbols_search(symbols, "", real_type->st_refer);
			assert(enum_type != NULL);
			if(enum_type->type != EN_HST_ENUM)
			{
				scanner_error(yylloc, E_LS_NOT_INTEGER_TYPE);
			}
			break;
		}
	default:
		scanner_error(yylloc, E_LS_NOT_INTEGER_TYPE);
	}
}

void check_integer_value(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_VALUE *val)
{
	const ST_VALUE *real_val;
	real_val = symbols_get_real_value(symbols, val);
	assert(real_val != NULL);

	switch(real_val->type)
	{
	case E_SNVT_INT64:
	case E_SNVT_UINT64:
	case E_SNVT_HEX_INT64:
	case E_SNVT_HEX_UINT64:
		break;
	default:
		scanner_error(yylloc, E_LS_NOT_INTEGER_VALUE);
	}
}

void check_identifier_refer_to_a_field_with_integer_type(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *prefix, const tchar *identifier)
{
	const SYMBOL* symbol = symbols_search(symbols, prefix, identifier);
	assert(symbol != NULL);
	assert(symbol->type == EN_HST_FIELD);
	if(symbol->body.field.type.type != E_SNT_SIMPLE)
	{
		scanner_error(yylloc, E_LS_NOT_INTEGER_TYPE);
	}

	check_integer_type(symbols, yylloc, &symbol->body.field.type.st);
}

void check_arguments(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_TYPE *type, const ST_ARGUMENTS *arguments)
{
	const ST_SIMPLE_TYPE *field_type = NULL;

	const ST_SIMPLE_TYPE *par_type = NULL;
	const ST_SIMPLE_TYPE *arg_type = NULL;

	//获取变量类型
	if(type->type == E_SNT_CONTAINER)
	{
		assert(type->ct.ct == E_CT_VECTOR);
		field_type = &type->ct.vector_type;
	}
	else
	{
		assert(type->type == E_SNT_SIMPLE);
		field_type = &type->st;
	}
	field_type = symbols_get_real_type(symbols, field_type);


	//获取实际参数类型
	if(arguments->arg_list_num == 1)
	{
		const SYMBOL* arg_field_symbol = NULL;
		arg_field_symbol = symbols_search(symbols, symbols->struct_name, arguments->arg_list[0]);
		assert(arg_field_symbol != NULL);
		assert(arg_field_symbol->type == EN_HST_FIELD);
		if(arg_field_symbol->body.field.type.type != E_SNT_SIMPLE)
		{
			scanner_error(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}
		arg_type = symbols_get_real_type(symbols, &arg_field_symbol->body.field.type.st);
		assert(arg_type != NULL);
	}
	
	//获取形式参数类
	if(field_type->st == E_ST_REFER)
	{
		const SYMBOL* union_symbol = symbols_search(symbols, "", field_type->st_refer);
		assert(union_symbol != NULL);
		if(union_symbol->type == EN_HST_UNION)
		{
			assert(union_symbol->body.union_symbol.para.par_list_num == 1);
			par_type = symbols_get_real_type(symbols, &union_symbol->body.union_symbol.para.par_list[0].type);
			assert(par_type != NULL);
		}
	}

	if((par_type == NULL) && (arg_type == NULL))
	{
		//okey~
	}
	else if((par_type != NULL) && (arg_type != NULL))
	{
		if((par_type->st != E_ST_REFER) || (arg_type->st != E_ST_REFER))
		{
			scanner_error(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}

		if(strcmp(par_type->st_refer, arg_type->st_refer) != 0)
		{
			scanner_error(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}
	}
	else
	{
		scanner_error(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
	}
}

//检查字符串长度是否超过限制
void check_strlen_too_long(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *str, const tchar *suffix, tuint32 limit)
{
	if(strlen(str) + strlen(suffix) + 1 > limit)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_LENGTH_ERROR, str, suffix, limit);
	}
}

void check_value_type(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *type, const ST_VALUE *val)
{
	const ST_SIMPLE_TYPE *real_type = NULL;
	const ST_VALUE *real_val = NULL;

	real_type = symbols_get_real_type(symbols, type);
	real_val = symbols_get_real_value(symbols, val);
	switch(real_type->st)
	{
	case E_ST_INT8:
	case E_ST_INT16:
	case E_ST_INT32:
	case E_ST_INT64:
		{
			if((real_val->type != E_SNVT_INT64) && (real_val->type != E_SNVT_HEX_INT64))
			{
				scanner_error(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}
			switch(real_type->st)
			{
			case E_ST_INT8:
				if((tint8)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT16:
				if((tint16)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT32:
				if((tint32)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_INT64:
				if((tint64)real_val->val.i64 != real_val->val.i64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
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
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				ui64 = real_val->val.i64;
			}
			else if((real_val->type == E_SNVT_UINT64) || (real_val->type == E_SNVT_HEX_UINT64))
			{
				ui64 = real_val->val.ui64;
			}
			else
			{
				scanner_error(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}

			switch(real_type->st)
			{
			case E_ST_UINT8:
				if((tuint8)ui64 != ui64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT16:
				if((tuint16)ui64 != ui64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT32:
				if((tuint32)ui64 != ui64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			case E_ST_UINT64:
				if((tuint64)ui64 != ui64)
				{
					scanner_error(yylloc, E_LS_CONSTANT_OVER_THE_RANGE);
				}
				break;
			}
			break;
		}
	case E_ST_CHAR:
		if(real_val->type != E_SNVT_CHAR)
		{
			scanner_error(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_DOUBLE:
		if(real_val->type != E_SNVT_DOUBLE)
		{
			scanner_error(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_STRING:
		{
			if(real_val->type != E_SNVT_STRING)
			{
				scanner_error(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
			}

			if(real_type->string_length[0] != 0)
			{
				scanner_error(yylloc, E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE);
			}
			break;
		}
	default:
		scanner_error(yylloc, E_LS_CONST_TYPE_ERROR);
	}
}

void check_simpletype_is_enum_with_unique(const SYMBOLS *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *simple_type)
{
	const SYMBOL* enum_symbol;
	simple_type = symbols_get_real_type(symbols, simple_type);
	assert(simple_type != NULL);
	if(simple_type->st != E_ST_REFER)
	{
		scanner_error(yylloc, E_LS_UNION_PARAMETERS_ERROR);
	}

	enum_symbol = symbols_search(symbols, "", simple_type->st_refer);

	if(enum_symbol->type != EN_HST_ENUM)
	{
		scanner_error(yylloc, E_LS_UNION_PARAMETERS_ERROR);
	}

	if(!enum_symbol->body.symbol_enum.unique)
	{
		scanner_error(yylloc, E_LS_ENUM_HAVE_EQUAL_ELEMENTS);
	}
}

void check_str_equal(const SYMBOLS *symbols, const YYLTYPE *yylloc, const tchar *src, const tchar* dst)
{
	if(strcmp(src, dst) != 0)
	{
		scanner_error(yylloc, E_LS_UNION_PARAMETERS_ERROR);
	}
}