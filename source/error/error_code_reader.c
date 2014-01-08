#include "platform/tlibc_platform.h"
#include "lib/tlibc_error_code.h"

#include "error/error_code_types.h"
#include "error/error_code_reader.h"
#include <string.h>
TLIBC_ERROR_CODE read_TD_ERROR_CODE_name(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data)
{
    char name[TLIBC_MAX_IDENTIFIER_LENGTH];
    if(read_enum_name(self, name, TLIBC_MAX_IDENTIFIER_LENGTH) != E_TD_NOERROR) goto ERROR_RET;
    if(strcmp(name, "E_TD_NOERROR") == 0)
    {
        *data = E_TD_NOERROR;
        goto done;
    }
    if(strcmp(name, "E_TD_ERROR") == 0)
    {
        *data = E_TD_ERROR;
        goto done;
    }
    if(strcmp(name, "E_TD_BAD_PARAMETER") == 0)
    {
        *data = E_TD_BAD_PARAMETER;
        goto done;
    }
    if(strcmp(name, "E_TD_SYNTAX_ERROR") == 0)
    {
        *data = E_TD_SYNTAX_ERROR;
        goto done;
    }
    if(strcmp(name, "E_TD_NOT_ENOUGH_BYTEBUFF_SIZE") == 0)
    {
        *data = E_TD_NOT_ENOUGH_BYTEBUFF_SIZE;
        goto done;
    }
    if(strcmp(name, "E_TD_NOT_ENOUGH_STRING_SIZE") == 0)
    {
        *data = E_TD_NOT_ENOUGH_STRING_SIZE;
        goto done;
    }
    if(strcmp(name, "E_TD_CONSTANT_REDEFINITION") == 0)
    {
        *data = E_TD_CONSTANT_REDEFINITION;
        goto done;
    }
    if(strcmp(name, "E_TD_INTEGER_OVERFLOW") == 0)
    {
        *data = E_TD_INTEGER_OVERFLOW;
        goto done;
    }
    if(strcmp(name, "E_TD_CAN_NOT_OPEN_FILE") == 0)
    {
        *data = E_TD_CAN_NOT_OPEN_FILE;
        goto done;
    }
    if(strcmp(name, "E_TD_CAN_NOT_FIND_CONSTANCE") == 0)
    {
        *data = E_TD_CAN_NOT_FIND_CONSTANCE;
        goto done;
    }
    if(strcmp(name, "E_TD_CONSTANCE_TYPE_TOO_SMALL") == 0)
    {
        *data = E_TD_CONSTANCE_TYPE_TOO_SMALL;
        goto done;
    }
    if(strcmp(name, "E_TD_INVALID_CONSTANCE_TYPE") == 0)
    {
        *data = E_TD_INVALID_CONSTANCE_TYPE;
        goto done;
    }
    if(strcmp(name, "E_TD_UNKNOW_CONSTANT_VALUE") == 0)
    {
        *data = E_TD_UNKNOW_CONSTANT_VALUE;
        goto done;
    }
    if(strcmp(name, "E_TD_SYMBOL_REDEFINITION") == 0)
    {
        *data = E_TD_SYMBOL_REDEFINITION;
        goto done;
    }
    if(strcmp(name, "E_TD_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE") == 0)
    {
        *data = E_TD_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE;
        goto done;
    }
ERROR_RET:
    return E_TLIBC_ERROR;
done:
    return E_TLIBC_NOERROR;
}
TLIBC_ERROR_CODE read_TD_ERROR_CODE_number(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data)
{
    return read_enum_number(self, (tint32*)data);
}
TLIBC_ERROR_CODE read_TD_ERROR_CODE(TLIBC_ABSTRACT_READER *self, TD_ERROR_CODE *data)
{
    if(read_TD_ERROR_CODE_name(self, data) != E_TD_NOERROR) goto ERROR_RET;
    if(read_TD_ERROR_CODE_number(self, data) != E_TD_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
