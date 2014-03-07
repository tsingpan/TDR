if(WIN32)
	find_program(TDATA_EXECUTABLE tdata
		PATHS $ENV{PROGRAMFILES}/tdata/bin
   )
else()
	find_program(TDATA_EXECUTABLE tdata
		PATHS /usr/local/tdata/bin
   )
endif()

include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(TData  DEFAULT_MSG
	TDATA_EXECUTABLE)

mark_as_advanced(TDATA_EXECUTABLE)
