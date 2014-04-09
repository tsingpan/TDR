#ifndef _H_CHECK
#define _H_CHECK


#include "platform/tlibc_platform.h"

#include "parse/parser.h"
#include <stdint.h>


//检查符号定义
void check_identifier_defined(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查符号未定义
void check_identifier_not_defined(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查符号是否指向一个类型enum, struct, union
void check_identifier_is_type(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查符号指向一个值value, enumdef
void check_identifier_is_value(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查符号不指向一个值value, enumdef
void check_identifier_not_defined_as_value(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查符号是否指向一个正整数常量
void check_identifier_is_positive_integer(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查string长度定义
void check_string_length_defined(const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *symbol_type);

//检查string长度未定义
void check_string_length(const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *symbol_type, int defined);

//检查类型是整数
void check_integer_type(const symbols_t *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *simple_type);

//检查值是整数
void check_integer_value(const symbols_t *symbols, const YYLTYPE *yylloc, const ST_VALUE *value);

//检查符号指向一个整数类型的struct成员
void check_identifier_refer_to_a_field_with_integer_type(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier);

//检查实际参数和形式参数是否匹配
void check_arguments(const symbols_t *symbols, const YYLTYPE *yylloc, const ST_TYPE *type, const ST_ARGUMENTS *arguments);

//检查字符串长度是否超过限制
void check_strlen_too_long(const YYLTYPE *yylloc, const char *str, const char *suffix, uint32_t limit);

//检查类型和值是否匹配
void check_value_type(const symbols_t *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *type, const ST_VALUE *val);

//检查不包含重复值的枚举类型
void check_simpletype_is_enum(const symbols_t *symbols, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *simple_type);

//检查枚举不包含重复元素
void check_enumdef_is_unique(const YYLTYPE *yylloc, const ST_ENUM *pn_enum, uint32_t index);

//检查字符串是否相等
void check_str_equal(const YYLTYPE *yylloc, const char *src, const char* dst);

#endif //_H_CHECK
