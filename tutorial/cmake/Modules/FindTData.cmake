find_program(TDATA_EXECUTABLE tdata
	PATHS $ENV{TDATA_HOME}/bin
   )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TData  DEFAULT_MSG
	TDATA_EXECUTABLE)

mark_as_advanced(TDATA_EXECUTABLE)
