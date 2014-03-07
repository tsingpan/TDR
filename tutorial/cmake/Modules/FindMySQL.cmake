# - Try to find MySQL library
# Find the MySQL includes and client library
# This module defines
#  MYSQL_INCLUDE_DIR, where to find mysql.h
#  MYSQL_LIBRARIES, the libraries needed to use MySQL.
#  MYSQL_LIB_DIR, path to the MYSQL_LIBRARIES
#  MYSQL_FOUND, If false, do not try to use MySQL.

if(WIN32)
   find_path(MYSQL_INCLUDE_DIR mysql.h
      PATHS
      $ENV{PROGRAMFILES}/MySQL/**/include
   )
else(WIN32)
   find_path(MYSQL_INCLUDE_DIR mysql.h
      PATHS
      $ENV{MYSQL_INCLUDE_DIR}
      $ENV{MYSQL_DIR}/include
      /usr/local/mysql/include
      PATH_SUFFIXES
      mysql
   )
endif(WIN32)

if(WIN32)
#   find_library(MYSQL_LIBRARIES NAMES mysqlclient
   find_library(MYSQL_LIBRARIES NAMES libmysql
      PATHS
      $ENV{MYSQL_DIR}/**/lib
      $ENV{PROGRAMFILES}/MySQL/**/lib/
   )
else(WIN32)
#   find_library(MYSQL_LIBRARIES NAMES mysqlclient
   find_library(MYSQL_LIBRARIES NAMES libmysql
      PATHS
      $ENV{MYSQL_DIR}/lib
      /usr/local/mysql/lib
      PATH_SUFFIXES
      mysql
   )
endif(WIN32)

if(WIN32)
   set(MYSQL_LIB_PATHS
      $ENV{PROGRAMFILES}/MySQL/**/lib/opt
      $ENV{MYSQL_DIR}/lib
   )
   find_library(MYSQL_LIBRARIES NAMES mysqlclient
      PATHS
      ${MYSQL_LIB_PATHS}
   )
else(WIN32)
   set(MYSQL_LIB_PATHS
      /usr/local/mysql/lib
      $ENV{MYSQL_DIR}/lib
      PATH_SUFFIXES
      mysql
   )
   find_library(MYSQL_LIBRARIES NAMES mysqlclient
      PATHS
      ${MYSQL_LIB_PATHS}
   )
endif(WIN32)

if(MYSQL_LIBRARIES)
   get_filename_component(MYSQL_LIB_DIR ${MYSQL_LIBRARIES} PATH)
endif(MYSQL_LIBRARIES)

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
   set(MYSQL_FOUND TRUE)
   message(STATUS "Found MySQL: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
   set(MYSQL_FOUND FALSE)
   message(STATUS "MySQL not found.")
endif(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARIES)

