project(TData)

cmake_minimum_required(VERSION 2.8)

IF (CMAKE_BUILD_TYPE STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")
ENDIF()

SET(CMAKE_MODULE_PATH "$ENV{TLIBC_HOME}/cmake/Modules/;${CMAKE_MODULE_PATH}" )
set(DIR_HOME ${TData_SOURCE_DIR})
set(BUILD_HOME ${TData_BINARY_DIR})

set(_BUILD_INCLUDE ${BUILD_HOME}/_include)
set(_BUILD_SOURCE ${BUILD_HOME}/_source)

set(INCLUDE  ${DIR_HOME}/include)
set(SOURCE  ${DIR_HOME}/source)

add_subdirectory(source)
install(DIRECTORY cmake DESTINATION ".")
install(DIRECTORY language DESTINATION ".")
install(DIRECTORY common DESTINATION ".")
install(FILES README.md DESTINATION ".")
