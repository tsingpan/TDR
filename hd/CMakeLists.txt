project(hd)

cmake_minimum_required(VERSION 2.8)

set(GEN_INCLUDE ${BUILD_HOME}/hd_gen/include)
set(GEN_SOURCE ${BUILD_HOME}/hd_gen/source)
set(GEN_PYTHON ${BUILD_HOME}/hd_gen/python)

add_custom_target(hd
	COMMAND cmake -E make_directory ${BUILD_HOME}/hd_gen/
	COMMAND cmake -E make_directory ${GEN_INCLUDE}
	COMMAND cmake -E make_directory ${GEN_SOURCE}
	COMMAND cmake -E make_directory ${GEN_PYTHON}

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_writer_header hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_writer hotdata/syntactic_node.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_PYTHON} -python python_types hotdata/syntactic_node.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotlib/hp_error_msg.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_reader_header hotlib/hp_error_msg.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_reader hotlib/hp_error_msg.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_reader_header hotlib/hp_error_code.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_reader hotlib/hp_error_code.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotlib/hp_error_code.hd

	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_PYTHON} -python python_types hotlib/hp_config.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_writer_header hotlib/hp_config.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_SOURCE} -python c_writer hotlib/hp_config.hd
	COMMAND HotData -s ${hd_SOURCE_DIR} -t ${GEN_INCLUDE} -python c_types hotlib/hp_config.hd
)
