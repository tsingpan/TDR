find_path(HOTPOT_INCLUDE_DIR NAMES hotpot/hp_platform.h
	PATHS $ENV{HotPot_Dir}/include
   )

find_library(HOTPOT_LIBRARIES NAMES hotpot libhotpot
	PATHS $ENV{HotPot_Dir}/lib
   )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HotPot  DEFAULT_MSG
                                  HOTPOT_LIBRARIES HOTPOT_INCLUDE_DIR)

mark_as_advanced(HOTPOT_LIBRARIES HOTPOT_INCLUDE_DIR)
