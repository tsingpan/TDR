project(hd)

cmake_minimum_required(VERSION 2.8)

set(_GEN_INCLUDE ${BUILD_HOME}/hd_gen/include)
set(_GEN_SOURCE ${BUILD_HOME}/hd_gen/source)
set(_GEN_LUA ${BUILD_HOME}/hd_gen/lua/lib)

add_custom_target(hd
	COMMAND cmake -E make_directory ${_GEN_INCLUDE}
	COMMAND cmake -E make_directory ${_GEN_SOURCE}
	COMMAND cmake -E make_directory ${_GEN_LUA}
	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hotpot/
	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hotdata/
	COMMAND cmake -E make_directory ${_GEN_SOURCE}/hotdata/
	COMMAND cmake -E make_directory ${_GEN_LUA}/hotpot/
	COMMAND cmake -E make_directory ${_GEN_LUA}/hotdata/

	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_types hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_writer_header hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node_writer.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_writer hotdata/syntactic_node.hd > ${_GEN_SOURCE}/hotdata/syntactic_node_writer.c
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua lua_types hotdata/syntactic_node.hd > ${_GEN_LUA}/hotdata/syntactic_node.lua

	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_types hotscript/hp_error_msg.hd > ${_GEN_INCLUDE}/hotscript/hp_error_msg.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_reader_header hotscript/hp_error_msg.hd > ${_GEN_INCLUDE}/hotscript/hp_error_msg_reader.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_reader hotscript/hp_error_msg.hd > ${_GEN_SOURCE}/hotscript/hp_error_msg_reader.c

	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_reader_header hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hotscript/hp_error_code_reader.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_reader hotpot/hp_error_code.hd > ${_GEN_SOURCE}/hotscript/hp_error_code_reader.c
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_types hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hotpot/hp_error_code.h

	COMMAND HotData -i ${hd_SOURCE_DIR} -lua c_types hotpot/hp_config.hd > ${_GEN_INCLUDE}/hotpot/hp_config.h
	COMMAND HotData -i ${hd_SOURCE_DIR} -lua lua_types hotpot/hp_config.hd > ${_GEN_LUA}/hotpot/hp_config.lua
)
