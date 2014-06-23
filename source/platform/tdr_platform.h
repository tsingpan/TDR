#ifndef _H_TDR_PLATFORM
#define _H_TDR_PLATFORM

#ifdef  __cplusplus
extern "C" {
#endif


#include "platform/tdr_platform_common.h"

#ifndef _WINDOWS
	#include "platform/tdr_platform_linux.h"
#else
	#include "platform/tdr_platform_windows.h"
#endif

#ifdef  __cplusplus
}
#endif

#endif
