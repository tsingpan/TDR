#ifndef _H_SCRIPT_PARSER
#define _H_SCRIPT_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"

#include "script_y.h"
#include "script_l.h"

typedef struct tagSCRIPT_PARSER SCRIPT_PARSER;
struct tagSCRIPT_PARSER
{
	yyscan_t scanner;
	YY_BUFFER_STATE bs;	
	hpint32 result;

};

hpint32 script_parser(SCRIPT_PARSER *self, FILE *fin);

#endif//_H_SCRIPT_PARSER

