/**
 * Autogenerated by HotData (0.0.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#include "platform/tlibc_platform.h"
#include "hotlib/hp_error_code.h"
#include "hotlib/hp_error_code_reader.h"
#include "hotlib/hp_config.h"
#include <string.h>
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE_name(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data)
{
    char name[MAX_IDENTIFIER_LENGTH];
    if(read_enum_name(self, name, MAX_IDENTIFIER_LENGTH) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(strcmp(name, "E_TLIBC_NOERROR") == 0)
    {
        *data = E_TLIBC_NOERROR;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_ERROR") == 0)
    {
        *data = E_TLIBC_ERROR;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_BAD_PARAMETER") == 0)
    {
        *data = E_TLIBC_BAD_PARAMETER;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_SYNTAX_ERROR") == 0)
    {
        *data = E_TLIBC_SYNTAX_ERROR;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_NOT_ENOUGH_BYTEBUFF_SIZE") == 0)
    {
        *data = E_TLIBC_NOT_ENOUGH_BYTEBUFF_SIZE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_NOT_ENOUGH_STRING_SIZE") == 0)
    {
        *data = E_TLIBC_NOT_ENOUGH_STRING_SIZE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_CONSTANT_REDEFINITION") == 0)
    {
        *data = E_TLIBC_CONSTANT_REDEFINITION;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_INTEGER_OVERFLOW") == 0)
    {
        *data = E_TLIBC_INTEGER_OVERFLOW;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_CAN_NOT_OPEN_FILE") == 0)
    {
        *data = E_TLIBC_CAN_NOT_OPEN_FILE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_CAN_NOT_FIND_CONSTANCE") == 0)
    {
        *data = E_TLIBC_CAN_NOT_FIND_CONSTANCE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_CONSTANCE_TYPE_TOO_SMALL") == 0)
    {
        *data = E_TLIBC_CONSTANCE_TYPE_TOO_SMALL;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_INVALID_CONSTANCE_TYPE") == 0)
    {
        *data = E_TLIBC_INVALID_CONSTANCE_TYPE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_UNKNOW_CONSTANT_VALUE") == 0)
    {
        *data = E_TLIBC_UNKNOW_CONSTANT_VALUE;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_SYMBOL_REDEFINITION") == 0)
    {
        *data = E_TLIBC_SYMBOL_REDEFINITION;
        goto done;
    }
    if(strcmp(name, "E_TLIBC_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE") == 0)
    {
        *data = E_TLIBC_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE;
        goto done;
    }
ERROR_RET:
    return E_TLIBC_ERROR;
done:
    return E_TLIBC_NOERROR;
}
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE_number(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data)
{
    return read_enum_number(self, (tint32*)data);
}
TLIBC_ERROR_CODE read_TLIBC_ERROR_CODE(TLIBC_ABSTRACT_READER *self, TLIBC_ERROR_CODE *data)
{
    if(read_TLIBC_ERROR_CODE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(read_TLIBC_ERROR_CODE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
