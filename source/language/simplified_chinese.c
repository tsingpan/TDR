#include "language/language.h"

const char* tdata_strerror(EN_TD_LANGUAGE_STRING sid)
{
	switch(sid)
	{
	case E_LS_UNKNOW:
		return "未知错误.";
	case E_LS_IDENTIFIER_NOT_DEFINED:
		return "符号未定义[%s::%s]";
	case E_LS_IDENTIFIER_REDEFINITION:
		return "[%s::%s]符号重定义";
	case E_LS_IDENTIFIER_NOT_INTEGER:
		return "符号[%s::%s]不是合法整数";
	case E_LS_IDENTIFIER_NOT_STRING:
		return "标识符[%s]不是[%s]";
	case E_LS_STRING_LENGTH_NOT_DEFINED:
		return "string长度未定义";
	case E_LS_IDENTIFIER_LENGTH_ERROR:
		return "符号长度超过%u";
	case E_LS_NOT_INTEGER_TYPE:
		return "不是整数类型";
	case E_LS_NOT_INTEGER_VALUE:
		return "不是合法整数";
	case E_LS_ARGUMENT_TYPE_MISMATCH:
		return "参数类型不匹配";
	case E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD:
		return "不能使用保留字[%s]";
	case E_LS_SYNTAX_ERROR:
		return "语法分析时发生错误[%s]";
	case E_LS_CANNOT_OPEN_FILE:
		return "无法打开文件[%s]";
	case E_LS_OUT_OF_MEMORY:
		return "试图打开文件[%s]发生内存分配错误";
	case E_LS_CHARACTER_CONSTANT_FORMAT_ERROR:
		return "字符型常量格式错误";
	case E_LS_STRING_CONSTANT_FORMAT_ERROR:
		return "字符串型常量格式错误";
	case E_LS_NUMBER_ERROR_RANGE:
		return "数字常量超出范围";
	case E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE:
		return "定义常量时不可指定string类型的长度";
	case E_LS_CONSTANT_TYPES_DO_NOT_MATCH:
		return "常量类型不匹配";
	case E_LS_CONST_TYPE_ERROR:
		return "常量类型错误";
	case E_LS_ENUM_HAVE_EQUAL_ELEMENTS:
		return "枚举类型含有重复元素[%s]和[%s]";
	case E_LS_TOO_MANY_MEMBERS:
		return "类型成员数量不可以超过[%u]";
	case E_LS_TOO_MANY_PARAMETERS:
		return "形式参数不可以超过[%u]个";
	case E_LS_TOO_MANY_ARGUMENTS:
		return "实际参数不可以超过[%u]个";
	case E_LS_NOT_ENUM_TYPE:
		return "不是枚举类型";
	default:
		return "未知错误";
	}
}
