find_path(TLIBC_INCLUDE_DIR NAMES tlibc/platform/tlibc_platform.h
	PATHS $ENV{TLIBC_HOME}/include
   )

find_library(TLIBC_LIBRARIES NAMES tlibc
	PATHS $ENV{TLIBC_HOME}/lib
   )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TLibC  DEFAULT_MSG
	TLIBC_INCLUDE_DIR TLIBC_LIBRARIES)

mark_as_advanced(TLIBC_LIBRARIES TLIBC_LIBRARIES)
