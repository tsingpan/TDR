project(HotPot)

cmake_minimum_required(VERSION 2.8)

IF (CMAKE_BUILD_TYPE STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")
ENDIF()

set(DIR_HOME ${HotPot_SOURCE_DIR})
set(BUILD_HOME ${HotPot_BINARY_DIR})
set(LIBRARY_PATH "lib")
set(BINARY_PATH "bin")
set(_GEN_INCLUDE ${BUILD_HOME}/gen/include)
set(_GEN_SOURCE ${BUILD_HOME}/gen/source)

set(_BUILD_INCLUDE ${BUILD_HOME}/_include)
set(_BUILD_SOURCE ${BUILD_HOME}/_source)

set(INCLUDE  ${DIR_HOME}/include)
set(DEPS  ${DIR_HOME}/deps)
set(SOURCE  ${DIR_HOME}/source)

add_subdirectory(source)
add_subdirectory(deps)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY resource DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_target(genall
	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hotdata/
	COMMAND cmake -E make_directory ${_GEN_SOURCE}/hotdata/
	COMMAND HOTDATA -i ${INCLUDE} -lua c_types hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node.h
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotdata/syntactic_node.h'}\" -lua c_writer_header hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node_writer.h
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotdata/syntactic_node.h','hotdata/syntactic_node_writer.h'}\" -lua c_writer hotdata/syntactic_node.hd > ${_GEN_SOURCE}/hotdata/syntactic_node_writer.c

	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hoterror/
	COMMAND cmake -E make_directory ${_GEN_SOURCE}/hoterror/
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_types hoterror/hp_error_msg.hd > ${_GEN_INCLUDE}/hoterror/hp_error_msg.h
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotpot/hp_error_code.h','hoterror/hp_error_msg.h'}\" -lua c_reader_header hoterror/hp_error_msg.hd > ${_GEN_INCLUDE}/hoterror/hp_error_msg_reader.h
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotpot/hp_error_code.h','hoterror/hp_error_code_reader.h','hoterror/hp_error_msg.h'}\" -lua c_reader hoterror/hp_error_msg.hd > ${_GEN_SOURCE}/hoterror/hp_error_msg_reader.c
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_reader_header hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hoterror/hp_error_code_reader.h
	COMMAND HOTDATA -i ${INCLUDE} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_reader hotpot/hp_error_code.hd > ${_GEN_SOURCE}/hoterror/hp_error_code_reader.c

	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hotpot/
	COMMAND HOTDATA -i ${INCLUDE} -lua c_types hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hotpot/hp_error_code.h
	COMMAND HOTDATA -i ${INCLUDE} -lua c_types hotpot/hp_config.hd > ${_GEN_INCLUDE}/hotpot/hp_config.h
)
