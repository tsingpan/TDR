PROJECT(Example)
find_package(ZLIB REQUIRED)
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
	${ZLIB_INCLUDE_DIRS}
	)

set(TDATA_OUTPUTS 
	${GEN_PATH}/common/common_types.h ${GEN_PATH}/common/common_reader.c ${GEN_PATH}/common/common_reader.h 
	${GEN_PATH}/common/common_writer.c ${GEN_PATH}/common/common_writer.h
	${GEN_PATH}/protocol_types.h
	${GEN_PATH}/protocol_reader.c ${GEN_PATH}/protocol_reader.h 
	${GEN_PATH}/protocol_writer.c ${GEN_PATH}/protocol_writer.h
	${GEN_PATH}/tconnd_inc.xml ${GEN_PATH}/tconnd_instance1.xml.inc
	)

add_custom_command(OUTPUT ${TDATA_OUTPUTS}
	COMMAND ${TDATA_EXECUTABLE} -s ${Example_SOURCE_DIR}/protocol -t ${GEN_PATH} -gen tlibc common/common.td protocol.td
	COMMAND ${CMAKE_COMMAND} -E copy ${Example_SOURCE_DIR}/tconnd_inc.xml ${GEN_PATH}/
	COMMAND ${CMAKE_COMMAND} -E copy ${Example_SOURCE_DIR}/tconnd_instance1.xml.inc ${GEN_PATH}/
)

ADD_EXECUTABLE(Example ${FILE_LIST} ${TDATA_OUTPUTS})
TARGET_LINK_LIBRARIES(Example ${TLIBC_LIBRARIES} ${ZLIB_LIBRARIES})
