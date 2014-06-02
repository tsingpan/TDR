#include "globals.h"
#include "parse/parser.h"
#include "platform/tlibc_platform.h"



const char *g_output_dir = NULL;

const char *g_source_dir = "./";

const char *g_include_dir[INCLUDE_DIR_NUM];
size_t g_include_dir_num = 0;
