PROJECT(data_conversion)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE FILE_LIST *.h *.c)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{HOTPOT_DIR}/cmake/Modules/)
find_package(HotPot)

set(GEN_PATH ${data_conversion_BINARY_DIR}/gen)
INCLUDE_DIRECTORIES(
	${HOTPOT_INCLUDE_DIR}/
	${GEN_PATH}/
	)

set(DDC_OUTPUTS ${GEN_PATH}/proto_types.h ${GEN_PATH}/proto_reader.c ${GEN_PATH}/proto_reader.h ${GEN_PATH}/proto_writer.c ${GEN_PATH}/proto_writer.h)
add_custom_command(OUTPUT ${DDC_OUTPUTS}
	COMMAND ${HOTDATA_EXECUTABLE} -s ${data_conversion_SOURCE_DIR} -t ${GEN_PATH} -python c_types -python c_reader_header -python c_reader -python c_writer_header -python c_writer proto.hd
)

ADD_EXECUTABLE(data_conversion ${FILE_LIST} ${DDC_OUTPUTS})
TARGET_LINK_LIBRARIES(data_conversion ${HOTSCRIPT_LIBRARIES} ${HOTPROTOCOL_LIBRARIES})
