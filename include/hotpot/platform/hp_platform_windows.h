#ifndef _H_HP_PLATFORM_WINDOWS
#define _H_HP_PLATFORM_WINDOWS

#if defined(_MSC_VER) && (_MSC_VER > 1200)
#pragma once
#endif // _MSC_VER

#ifndef _WIN32
#error "This is a Windows header only"
#endif

#include <io.h>
#include <stdlib.h>
#include <direct.h>

#define strtoll(begin_ptr, end_ptr, length) _strtoi64(begin_ptr, end_ptr, length)
#define strtoull(begin_ptr, end_ptr, length) _strtoui64(begin_ptr, end_ptr, length)
#define snprintf _snprintf

#define PRIu64 "I64d"
#define PRIi64 "I64d"
#define HP_FILE_SEPARATOR '\\'

#pragma warning(disable:4996)

#endif//_H_HP_PLATFORM_WINDOWS