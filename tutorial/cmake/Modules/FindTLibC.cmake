if(WIN32)
	find_path(TLIBC_INCLUDE_DIR NAMES platform/tlibc_platform.h
		PATHS
		$ENV{PROGRAMFILES}/TLibC/include
	   )
else()
	find_path(TLIBC_INCLUDE_DIR NAMES platform/tlibc_platform.h
		PATHS
		/usr/local/TLibC/include
	   )
endif(WIN32)

if(WIN32)
	find_library(TLIBC_LIBRARIES NAMES tlibc
		PATHS 
		$ENV{PROGRAMFILES}/TLibC/lib
	   )
else()
	find_library(TLIBC_LIBRARIES NAMES tlibc
		PATHS 
		/usr/local/TLibC/lib
	   )
endif(WIN32)

mark_as_advanced(LIBC_INCLUDE_DIR TLIBC_LIBRARIES)
