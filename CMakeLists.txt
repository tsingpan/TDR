SET(PNAME "TDR")
SET(PONAME "tdr")
project(${PNAME})

cmake_minimum_required(VERSION 2.6)
OPTION(WITH_RE2C "选择此项让re2c重新生成读取正则表达式的代码, 请确认re2c已被正确安装， 并且在命令行输入re2c可以访问。" OFF)
OPTION(WITH_BISON "选择此项让bison重新生成语法分析的代码, 请确认bison已被正确安装， 并且在命令行输入re2c可以访问。" OFF)

IF(WIN32)
	SET(CMAKE_C_FLAGS_DEBUG "/Od /Z7 /W4 /MDd")
	SET(CMAKE_C_FLAGS_RELEASE "/O2 /W4 /MD")
ELSE()
	SET(CMAKE_C_FLAGS_DEBUG "-g -Wall")
	SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
	add_definitions(-DLUA_COMPAT_ALL -DLUA_USE_POSIX)
ENDIF()

SET(BUILDTYPE_DOCSTRING "Choose the type of build, options are: Debug(${CMAKE_C_FLAGS_DEBUG}) Release(${CMAKE_C_FLAGS_RELEASE})")
IF(WIN32)
	SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
ELSE()
	SET(CMAKE_BUILD_TYPE "Release" CACHE STRING ${BUILDTYPE_DOCSTRING})
	SET(CMAKE_INSTALL_PREFIX "/usr/local/${PNAME}" CACHE STRING "")
ENDIF()

SET(CMAKE_MODULE_PATH "${${PNAME}_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}" )
set(DIR_HOME ${${PNAME}_SOURCE_DIR})
set(BUILD_HOME ${${PNAME}_BINARY_DIR})

set(INCLUDE  ${DIR_HOME}/include)
set(SOURCE  ${DIR_HOME}/source)

add_subdirectory(source)
