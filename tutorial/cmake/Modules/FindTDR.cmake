if(WIN32)
	find_program(TDR_EXECUTABLE tdr
		PATHS $ENV{PROGRAMFILES}/tdr/bin
   )
else()
	find_program(TDR_EXECUTABLE tdr
		PATHS /usr/local/tdr/bin
   )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TDR  DEFAULT_MSG
		TDR_EXECUTABLE)

mark_as_advanced(TDR_EXECUTABLE)
