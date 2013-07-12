PROJECT(data_conversion)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE FILE_LIST *.h *.c)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{HotPot_Dir}/cmake/Modules/)
find_package(HotPot)

set(GEN_PATH ${data_conversion_BINARY_DIR}/gen)
INCLUDE_DIRECTORIES(
	${HOTPOT_INCLUDE_DIR}/
	${GEN_PATH}/
	)
set(DDC_OUTPUTS ${GEN_PATH}/proto.h ${GEN_PATH}/proto_read.h ${GEN_PATH}/proto_read.c ${GEN_PATH}/proto_write.h ${GEN_PATH}/proto_write.c)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_PATH})

add_custom_command(OUTPUT ${DDC_OUTPUTS}
	COMMAND ${HOTDATA_EXECUTABLE} -t D:/HotPot/resource/template/c_hotpot_types -o ${GEN_PATH}/proto.h ${data_conversion_SOURCE_DIR}/proto.dd
)

ADD_EXECUTABLE(data_conversion ${FILE_LIST} ${DDC_OUTPUTS})
TARGET_LINK_LIBRARIES(data_conversion ${HOTPOT_LIBRARIES})
