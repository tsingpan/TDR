#ifndef _H_GLOBALS_H
#define _H_GLOBALS_H

#include "parse/parser.h"

extern const char *g_target_dir;

extern const char *g_source_dir;

extern ST_TD_LANGUAGE_STRING_LIBRARY g_language_string_library;


//这里储存了所有解析所需要的数据， 而且除此之外的全局变量都是只读的。
extern PARSER g_parser;

#endif _H_GLOBALS_H
