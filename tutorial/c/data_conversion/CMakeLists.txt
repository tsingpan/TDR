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
	COMMAND ${HOTDATA_EXECUTABLE} -hs c_types -o ${GEN_PATH}/proto_types.h ${data_conversion_SOURCE_DIR}/proto.dd
	COMMAND ${HOTDATA_EXECUTABLE} -hs c_reader -o ${GEN_PATH}/proto_reader.c -jin ${data_conversion_SOURCE_DIR}/build.json ${data_conversion_SOURCE_DIR}/proto.dd
	COMMAND ${HOTDATA_EXECUTABLE} -hs c_reader_header -o ${GEN_PATH}/proto_reader.h -jin ${data_conversion_SOURCE_DIR}/build.json ${data_conversion_SOURCE_DIR}/proto.dd
	COMMAND ${HOTDATA_EXECUTABLE} -hs c_writer -o ${GEN_PATH}/proto_writer.c -jin ${data_conversion_SOURCE_DIR}/build.json ${data_conversion_SOURCE_DIR}/proto.dd
	COMMAND ${HOTDATA_EXECUTABLE} -hs c_writer_header -o ${GEN_PATH}/proto_writer.h -jin ${data_conversion_SOURCE_DIR}/build.json ${data_conversion_SOURCE_DIR}/proto.dd
)

ADD_EXECUTABLE(data_conversion ${FILE_LIST} ${DDC_OUTPUTS})
TARGET_LINK_LIBRARIES(data_conversion ${HOTPOT_LIBRARIES} ${HOTPROTOCOL_LIBRARIES})
