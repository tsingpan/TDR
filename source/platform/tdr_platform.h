#ifndef _H_TDR_PLATFORM
#define _H_TDR_PLATFORM

#include "platform/tdr_platform_common.h"

#ifndef _WINDOWS
	#include "platform/tdr_platform_linux.h"
#else
	#include "platform/tdr_platform_windows.h"
#endif

#endif
