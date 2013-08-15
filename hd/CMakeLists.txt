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
	COMMAND cmake -E make_directory ${_GEN_INCLUDE}/hoterror/
	COMMAND cmake -E make_directory ${_GEN_SOURCE}/hotdata/
	COMMAND cmake -E make_directory ${_GEN_SOURCE}/hoterror/
	COMMAND cmake -E make_directory ${_GEN_LUA}/hotpot/
	COMMAND cmake -E make_directory ${_GEN_LUA}/hotdata/

	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -lua c_types hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotdata/syntactic_node.h'}\" -lua c_writer_header hotdata/syntactic_node.hd > ${_GEN_INCLUDE}/hotdata/syntactic_node_writer.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotdata/syntactic_node.h','hotdata/syntactic_node_writer.h'}\" -lua c_writer hotdata/syntactic_node.hd > ${_GEN_SOURCE}/hotdata/syntactic_node_writer.c
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -lua lua_types hotdata/syntactic_node.hd > ${_GEN_LUA}/hotdata/syntactic_node.lua

	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_types hoterror/hp_error_msg.hd > ${_GEN_INCLUDE}/hoterror/hp_error_msg.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotpot/hp_error_code.h','hoterror/hp_error_msg.h'}\" -lua c_reader_header hoterror/hp_error_msg.hd > ${_GEN_INCLUDE}/hoterror/hp_error_msg_reader.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotpot/hp_error_code.h','hoterror/hp_error_code_reader.h','hoterror/hp_error_msg.h'}\" -lua c_reader hoterror/hp_error_msg.hd > ${_GEN_SOURCE}/hoterror/hp_error_msg_reader.c

	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_reader_header hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hoterror/hp_error_code_reader.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -luastr \"ifiles={'hotpot/hp_error_code.h'}\" -lua c_reader hotpot/hp_error_code.hd > ${_GEN_SOURCE}/hoterror/hp_error_code_reader.c
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -lua c_types hotpot/hp_error_code.hd > ${_GEN_INCLUDE}/hotpot/hp_error_code.h

	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -lua c_types hotpot/hp_config.hd > ${_GEN_INCLUDE}/hotpot/hp_config.h
	COMMAND HOTDATA -i ${hd_SOURCE_DIR} -lua lua_types hotpot/hp_config.hd > ${_GEN_LUA}/hotpot/hp_config.lua
)
