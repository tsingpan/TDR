project(hd)

cmake_minimum_required(VERSION 2.8)

set(GEN_INCLUDE ${BUILD_HOME}/hd_gen/include)
set(GEN_SOURCE ${BUILD_HOME}/hd_gen/source)
set(GEN_PYTHON ${BUILD_HOME}/hd_gen/python)

add_custom_target(hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_writer_header hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_writer hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_PYTHON} -python python_types hotdata/syntactic_node.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotscript/hp_error_msg.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_reader_header hotscript/hp_error_msg.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_reader hotscript/hp_error_msg.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_reader_header hotpot/hp_error_code.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_reader hotpot/hp_error_code.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotpot/hp_error_code.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_PYTHON} -python python_types hotpot/hp_config.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotpot/hp_config.hd
)
