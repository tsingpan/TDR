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
set(DDC_OUTPUTS ${GEN_PATH}/proto_types.h ${GEN_PATH}/proto_reader.c ${GEN_PATH}/proto_reader.h ${GEN_PATH}/proto_writer.c ${GEN_PATH}/proto_writer.h)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_PATH})

add_custom_command(OUTPUT ${DDC_OUTPUTS}
	COMMAND ${HOTDATA_EXECUTABLE} -lua ${data_conversion_SOURCE_DIR}/build.lua -lua c_types ${data_conversion_SOURCE_DIR}/proto.hd > ${GEN_PATH}/proto_types.h

	COMMAND ${HOTDATA_EXECUTABLE} -lua ${data_conversion_SOURCE_DIR}/build.lua -lua c_reader ${data_conversion_SOURCE_DIR}/proto.hd > ${GEN_PATH}/proto_reader.c
	COMMAND ${HOTDATA_EXECUTABLE} -lua ${data_conversion_SOURCE_DIR}/build.lua -lua c_reader_header ${data_conversion_SOURCE_DIR}/proto.hd > ${GEN_PATH}/proto_reader.h
	COMMAND ${HOTDATA_EXECUTABLE} -lua ${data_conversion_SOURCE_DIR}/build.lua -lua c_writer ${data_conversion_SOURCE_DIR}/proto.hd > ${GEN_PATH}/proto_writer.c
	COMMAND ${HOTDATA_EXECUTABLE} -lua ${data_conversion_SOURCE_DIR}/build.lua -lua c_writer_header ${data_conversion_SOURCE_DIR}/proto.hd > ${GEN_PATH}/proto_writer.h
)

ADD_EXECUTABLE(data_conversion ${FILE_LIST} ${DDC_OUTPUTS})
TARGET_LINK_LIBRARIES(data_conversion ${HOTPOT_LIBRARIES} ${HOTPROTOCOL_LIBRARIES})
