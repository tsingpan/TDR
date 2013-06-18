#ifndef _H_XML_PARSER
#define _H_XML_PARSER

#include <stdarg.h>
#include <stdio.h>

#include "hotpot/hp_platform.h"


#include "xml_y.h"
#include "xml_l.h"



typedef struct tagXML_PARSER XML_PARSER;
struct tagXML_PARSER
{
	yyscan_t scanner;
	YY_BUFFER_STATE bs;	
	hpint32 result;
};

hpint32 xml_parser(XML_PARSER *self, FILE *fin);

#endif//_H_XML_PARSER

