#include "platform/tlibc_platform.h"
#include "lib/tlibc_error_code.h"
#include "tdata/tdata_types.h"
#include "language/language_reader.h"
#include "language/language_types.h"

#include <string.h>

TLIBC_ERROR_CODE read_EN_TD_LANGUAGE_STRING_name(TLIBC_ABSTRACT_READER *self, EN_TD_LANGUAGE_STRING *data)
{
    char name[TLIBC_MAX_IDENTIFIER_LENGTH];
    if(read_enum_name(self, name, TLIBC_MAX_IDENTIFIER_LENGTH) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(strcmp(name, "E_LS_UNKNOW") == 0)
    {
        *data = E_LS_UNKNOW;
        goto done;
    }
	else if(strcmp(name, "E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD") == 0)
	{
		*data = E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD;
		goto done;
	}
	else if(strcmp(name, "E_LS_SYNTAX_ERROR") == 0)
	{
		*data = E_LS_SYNTAX_ERROR;
		goto done;
	}
	else if(strcmp(name, "E_LS_CANNOT_OPEN_FILE") == 0)
	{
		*data = E_LS_CANNOT_OPEN_FILE;
		goto done;
	}
	else if(strcmp(name, "E_LS_OUT_OF_MEMORY") == 0)
	{
		*data = E_LS_OUT_OF_MEMORY;
		goto done;
	}
	else if(strcmp(name, "E_LS_CHARACTER_CONSTANT_FORMAT_ERROR") == 0)
	{
		*data = E_LS_CHARACTER_CONSTANT_FORMAT_ERROR;
		goto done;
	}
	else if(strcmp(name, "E_LS_STRING_CONSTANT_FORMAT_ERROR") == 0)
	{
		*data = E_LS_STRING_CONSTANT_FORMAT_ERROR;
		goto done;
	}
	else if(strcmp(name, "E_LS_NUMBER_ERROR_RANGE") == 0)
	{
		*data = E_LS_NUMBER_ERROR_RANGE;
		goto done;
	}
	
ERROR_RET:
    return E_TLIBC_ERROR;
done:
    return E_TLIBC_NOERROR;
}
TLIBC_ERROR_CODE read_EN_TD_LANGUAGE_STRING_number(TLIBC_ABSTRACT_READER *self, EN_TD_LANGUAGE_STRING *data)
{
    return read_enum_number(self, (tint32*)data);
}
TLIBC_ERROR_CODE read_EN_TD_LANGUAGE_STRING(TLIBC_ABSTRACT_READER *self, EN_TD_LANGUAGE_STRING *data)
{
    if(read_EN_TD_LANGUAGE_STRING_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(read_EN_TD_LANGUAGE_STRING_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}


TLIBC_ERROR_CODE read_ST_TD_LANGUAGE_STRING(TLIBC_ABSTRACT_READER *self, ST_TD_LANGUAGE_STRING *data)
{
	if(read_struct_begin(self, "ST_TD_LANGUAGE_STRING") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_field_begin(self, "language_string_number") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_EN_TD_LANGUAGE_STRING(self, &data->language_string_number) != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_field_end(self, "language_string_number") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_field_begin(self, "language_string") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_string(self, data->language_string, TD_MAX_LANGUAGE_STRING_LENGTH) != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_field_end(self, "language_string") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_struct_end(self, "ST_TD_LANGUAGE_STRING") != E_TLIBC_NOERROR) goto ERROR_RET;
	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE read_ST_TD_LANGUAGE_STRING_LIBRARY(TLIBC_ABSTRACT_READER *self, ST_TD_LANGUAGE_STRING_LIBRARY *data)
{
	if(read_struct_begin(self, "ST_TD_LANGUAGE_STRING_LIBRARY") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_counter(self, "language_string_list_num", &data->language_string_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;	
	if(read_field_begin(self, "language_string_list") != E_TLIBC_NOERROR) goto ERROR_RET;
	{
		tuint32 i;
		if(read_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
		for(i = 0; i < data->language_string_list_num; ++i)
		{
			if(read_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
			if(read_ST_TD_LANGUAGE_STRING(self, &data->language_string_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
			if(read_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
		}
		if(read_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
	}
	if(read_field_end(self, "language_string_list") != E_TLIBC_NOERROR) goto ERROR_RET;
	if(read_struct_end(self, "ST_TD_LANGUAGE_STRING_LIBRARY") != E_TLIBC_NOERROR) goto ERROR_RET;
	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}
