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

set(INCLUDE  ${DIR_HOME}/include)
set(SOURCE  ${DIR_HOME}/source)

add_subdirectory(source)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})
