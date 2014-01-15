#include <stdio.h>
#include "platform/tlibc_platform.h"

#include "protocol/tlibc_xml_reader.h"
#include "protocol/tlibc_xml_writer.h"

#include "definition/definition_types.h"


#include <string.h>

#define MAX_BUFF_SIZE 1024

static ST_DEFINITION g_definition;

void init()
{
	g_definition.type = E_DT_CONST;
	snprintf(g_definition.definition.de_const.identifier, MAX_IDENTIFIER_LENGTH, "hello");
	g_definition.definition.de_const.identifier[MAX_IDENTIFIER_LENGTH - 1] = 0;

	g_definition.definition.de_const.type.type = E_SNT_SIMPLE;
	g_definition.definition.de_const.type.st = E_ST_INT32;

	g_definition.definition.de_const.val.type = E_SNVT_INT64;
	g_definition.definition.de_const.val.val.i64 = 123321;
}

void test_xml()
{
	TLIBC_XML_WRITER xml_writer;
	TLIBC_XML_READER xml_reader;
	FILE *fout = NULL;
	FILE *fin = NULL;

	init();


	fout = fopen("t.xml", "wb");
	xml_writer_init(&xml_writer, fout);
	fclose(fout);


	fin = fopen("t.xml", "rb");
	xml_reader_init(&xml_reader, fin);
	fclose(fin);
}

void test_compact()
{
	
	
}

int main()
{
	test_xml();

	test_compact();

	return 0;
}
