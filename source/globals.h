#ifndef _H_GLOBALS_H
#define _H_GLOBALS_H

#include "parse/parser.h"


//全局变量都是只读的哦

extern const char *g_output_dir;
#define INCLUDE_DIR_NUM 1024
extern const char *g_include_dir[INCLUDE_DIR_NUM];
extern size_t g_include_dir_num;

extern YYLTYPE g_yyloc;
extern PARSER g_parser;

#endif //_H_GLOBALS_H
