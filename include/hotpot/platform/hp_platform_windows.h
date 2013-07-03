#ifndef _H_HP_PLATFORM_WINDOWS
#define _H_HP_PLATFORM_WINDOWS

#ifndef _WIN32
#error "This is a Windows header only"
#endif

#include <stddef.h>
//如果不包含stdlib.h strtoll会出错
#include <stdlib.h>

#define strtoll(begin_ptr, end_ptr, radix) _strtoi64(begin_ptr, end_ptr, radix)
#define strtoull(begin_ptr, end_ptr, radix) _strtoui64(begin_ptr, end_ptr, radix)
#define snprintf _snprintf

#define PRIu64 "I64u"
#define PRIi64 "I64d"
#define HP_FILE_SEPARATOR '\\'

#pragma warning(disable:4996)

#endif//_H_HP_PLATFORM_WINDOWS
