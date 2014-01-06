/**
 * Autogenerated by HotData (0.0.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#ifndef _H_hotlib_hp_error_code_hd_TYPES
#define _H_hotlib_hp_error_code_hd_TYPES
#include "platform/tlibc_platform.h"
typedef enum _TLIBC_ERROR_CODE
{
    E_TLIBC_NOERROR = 0,
    E_TLIBC_ERROR = -1,
    E_TLIBC_BAD_PARAMETER = -10000,
    E_TLIBC_SYNTAX_ERROR = -10001,
    E_TLIBC_NOT_ENOUGH_BYTEBUFF_SIZE = -10002,
    E_TLIBC_NOT_ENOUGH_STRING_SIZE = -10003,
    E_TLIBC_CONSTANT_REDEFINITION = -10004,
    E_TLIBC_INTEGER_OVERFLOW = -10005,
    E_TLIBC_CAN_NOT_OPEN_FILE = -10006,
    E_TLIBC_CAN_NOT_FIND_CONSTANCE = -10007,
    E_TLIBC_CONSTANCE_TYPE_TOO_SMALL = -10008,
    E_TLIBC_INVALID_CONSTANCE_TYPE = -10009,
    E_TLIBC_UNKNOW_CONSTANT_VALUE = -10010,
    E_TLIBC_SYMBOL_REDEFINITION = -10011,
    E_TLIBC_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE = -10012,
}TLIBC_ERROR_CODE;
#define TLIBC_ERROR_CODE_NUM 15
#endif//_H_hotlib_hp_error_code_hd_TYPES
