PROJECT(Example)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE FILE_LIST *.h *.c)

SET(CMAKE_MODULE_PATH "${Example_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}" )

find_package(TDR REQUIRED)
find_package(TLibC REQUIRED)
find_package(MySQL REQUIRED)
find_package(ZLIB REQUIRED)

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
ENDIF()

SET(BUILDTYPE_DOCSTRING "Choose the type of build, options are: Debug(${CMAKE_C_FLAGS_DEBUG}) Release(${CMAKE_C_FLAGS_RELEASE})")
IF(WIN32)
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
ELSE()
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
    SET(CMAKE_INSTALL_PREFIX "/usr/local/${PNAME}" CACHE STRING "" FORCE)
ENDIF()

set(GEN_PATH ${Example_BINARY_DIR}/gen)
INCLUDE_DIRECTORIES(
	${TLIBC_INCLUDE_DIR}
	${GEN_PATH}/
	${ZLIB_INCLUDE_DIRS}
	${MYSQL_INCLUDE_DIR}
	)

set(TDR_OUTPUTS 
	${GEN_PATH}/common_types.h ${GEN_PATH}/common_reader.c ${GEN_PATH}/common_reader.h 
	${GEN_PATH}/common_writer.c ${GEN_PATH}/common_writer.h
	${GEN_PATH}/protocol_types.h
	${GEN_PATH}/protocol_reader.c ${GEN_PATH}/protocol_reader.h 
	${GEN_PATH}/protocol_writer.c ${GEN_PATH}/protocol_writer.h
	${GEN_PATH}/sql_types.h
	${GEN_PATH}/sql_reader.c ${GEN_PATH}/sql_reader.h 
	${GEN_PATH}/sql_writer.c ${GEN_PATH}/sql_writer.h
	${GEN_PATH}/tconnd_inc.xml ${GEN_PATH}/tconnd_instance1.xml.inc
	${GEN_PATH}/item.xlsx.xml ${GEN_PATH}/item.xlsx
	${GEN_PATH}/sql_tables.sql
	)

add_custom_command(OUTPUT ${TDR_OUTPUTS}
	COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_PATH}
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o${GEN_PATH} -gtypes_h ${Example_SOURCE_DIR}/protocol/protocol.tdr ${Example_SOURCE_DIR}/protocol/common/common.tdr ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o${GEN_PATH} -greader_h ${Example_SOURCE_DIR}/protocol/protocol.tdr ${Example_SOURCE_DIR}/protocol/common/common.tdr ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o${GEN_PATH} -greader_c ${Example_SOURCE_DIR}/protocol/protocol.tdr ${Example_SOURCE_DIR}/protocol/common/common.tdr ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o${GEN_PATH} -gwriter_h ${Example_SOURCE_DIR}/protocol/protocol.tdr ${Example_SOURCE_DIR}/protocol/common/common.tdr ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o${GEN_PATH} -gwriter_c ${Example_SOURCE_DIR}/protocol/protocol.tdr ${Example_SOURCE_DIR}/protocol/common/common.tdr ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${TDR_EXECUTABLE} -I${Example_SOURCE_DIR}/protocol -o ${GEN_PATH} -g sql ${Example_SOURCE_DIR}/protocol/sql.tdr
	COMMAND ${CMAKE_COMMAND} -E copy ${Example_SOURCE_DIR}/tconnd_inc.xml ${GEN_PATH}
	COMMAND ${CMAKE_COMMAND} -E copy ${Example_SOURCE_DIR}/tconnd_instance1.xml.inc ${GEN_PATH}
	COMMAND ${CMAKE_COMMAND} -E copy ${Example_SOURCE_DIR}/item.xlsx ${GEN_PATH}
)

ADD_EXECUTABLE(Example ${FILE_LIST} ${TDR_OUTPUTS})
TARGET_LINK_LIBRARIES(Example ${ZLIB_LIBRARIES} ${MYSQL_LIBRARIES} ${TLIBC_LIBRARIES})
