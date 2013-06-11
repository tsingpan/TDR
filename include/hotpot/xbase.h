#ifndef _H_XBASE
#define _H_XBASE

#include "typedef.h"

#ifndef NULL
#define NULL				0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif


#define MAX_FILE_PATH_LENGTH 1024

#define OFFSET_OF(type, member) ((hpuint64) &((type *)0)->member)

#define CONTAINER_OF(ptr, type, member) ((type *)((hpuint64)ptr - OFFSET_OF(type, member)))


#ifndef XIN
#define XIN
#endif

#ifndef XOUT
#define XOUT
#endif

#ifndef XINOUT
#define XINOUT
#endif

#if (defined (XGAME_SHARED_EXPORTS))
	#define XAPI __declspec(dllexport)
#else
	#define XAPI extern
#endif

#ifdef _MSC_VER
	#define SNPRINTF _snprintf
#else
	#define SNPRINTF snprintf
#endif

#endif//_H_XBASE
