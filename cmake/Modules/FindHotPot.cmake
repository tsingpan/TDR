find_program(HOTDATA_EXECUTABLE hd
	PATHS $ENV{HotPot_Dir}/bin
   )

find_program(HOTSCRIPT_EXECUTABLE hs
	PATHS $ENV{HotPot_Dir}/bin
   )

find_path(HOTPOT_INCLUDE_DIR NAMES hotpot/hp_platform.h
	PATHS $ENV{HotPot_Dir}/include
   )

find_library(HOTPOT_LIBRARIES NAMES hotpot hotprotocol
	PATHS $ENV{HotPot_Dir}/lib
   )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HotPot  DEFAULT_MSG
                                  HOTPOT_LIBRARIES HOTPOT_INCLUDE_DIR HOTDATA_EXECUTABLE HOTSCRIPT_EXECUTABLE)

mark_as_advanced(HOTPOT_LIBRARIES HOTPOT_INCLUDE_DIR HOTDATA_EXECUTABLE HOTSCRIPT_EXECUTABLE)
