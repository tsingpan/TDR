#ifndef _H_HP_PLATFORM
#define _H_HP_PLATFORM

#include "hotpot/platform/hp_platform_common.h"

#ifndef _WIN32
	#include "hotpot/platform/hp_platform_linux.h"
#else
	#include "hotpot/platform/hp_platform_windows.h"
#endif

#endif//_H_HP_PLATFORM