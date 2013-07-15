#include <stdio.h>
#include "hotpot/hp_platform.h"
#include "proto_types.h"
#include "hotprotocol/hp_xml_reader.h"
#include "proto_reader.h"

#include <string.h>

#define MAX_BUFF_SIZE 1024

PROTO proto;


HP_XML_READER xml_reader;
int main()
{
	FILE *fin = fopen("d:/t.xml", "rb");

	xml_reader_init(&xml_reader, fin);

	read_PROTO(&xml_reader.super, &proto);
	fclose(fin);
	return 0;
}
