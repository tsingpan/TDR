#ifndef _H_HP_PLATFORM_WINDOWS
#define _H_HP_PLATFORM_WINDOWS

#ifndef _WIN32
#error "This is a Windows header only"
#endif

#include <stddef.h>

#define strtoll(begin_ptr, end_ptr, length) _strtoi64(begin_ptr, end_ptr, length)
#define strtoull(begin_ptr, end_ptr, length) _strtoui64(begin_ptr, end_ptr, length)
#define snprintf _snprintf

#define PRIu64 "I64u"
#define PRIi64 "I64d"
#define HP_FILE_SEPARATOR '\\'

#pragma warning(disable:4996)

#endif//_H_HP_PLATFORM_WINDOWS
