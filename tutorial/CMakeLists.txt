PROJECT(Example)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE FILE_LIST *.h *.c)

SET(CMAKE_MODULE_PATH "$ENV{TLIBC_HOME}/cmake/Modules/;${CMAKE_MODULE_PATH}" )
SET(CMAKE_MODULE_PATH "$ENV{TDATA_HOME}/cmake/Modules/;${CMAKE_MODULE_PATH}" )

find_package(TData REQUIRED)
find_package(TLibC REQUIRED)

set(GEN_PATH ${Example_BINARY_DIR}/gen)
INCLUDE_DIRECTORIES(
	${TLIBC_INCLUDE_DIR}
	${GEN_PATH}/
	)

set(TDATA_OUTPUTS 
	${GEN_PATH}/common/common_types.h ${GEN_PATH}/common/common_reader.c ${GEN_PATH}/common/common_reader.h 
	${GEN_PATH}/common/common_writer.c ${GEN_PATH}/common/common_writer.h
	${GEN_PATH}/definition/definition_types.h ${GEN_PATH}/definition/definition_reader.c ${GEN_PATH}/definition/definition_reader.h 
	${GEN_PATH}/definition/definition_writer.c ${GEN_PATH}/definition/definition_writer.h
	)

add_custom_command(OUTPUT ${TDATA_OUTPUTS}
	COMMAND ${TDATA_EXECUTABLE} -s ${Example_SOURCE_DIR}/protocol -t ${GEN_PATH} -gen tlibc common/common.td definition/definition.td
)

ADD_EXECUTABLE(Example ${FILE_LIST} ${TDATA_OUTPUTS})
TARGET_LINK_LIBRARIES(Example ${TLIBC_LIBRARIES})
