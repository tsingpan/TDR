#include <stdio.h>
#include "json_parser.h"
#include "hotscript/script_parser.h"
#include <string.h>
#include "hotscript/hotobject.h"
#include "hotscript/hot_vm.h"
#include "hotscript/hotobject_reader.h"
#include "hotscript/hotobject_writer.h"
#include "encoding/ddekit_json_encoding_writer.h"
#include "encoding/ddekit_json_encoding_reader.h"
#include <stdio.h>

JSON_PARSER xp;
SCRIPT_PARSER sp;
HotObject *obj;
HotObjectReader reader;
HotObjectWriter writer;
JSON_WRITER json_writer;
JSON_READER json_reader;


HotObject *ho;
HotObjectWriter iter;
HotObjectReader citer;

const char *str;

char buff[102400];
size_t buff_size;
const char *sstr = "<% \'2222\' %>";

int main()
{
	FILE *fin;
	char c;

	
	obj = hotobject_new();
	hotobject_writer_init(&writer, obj);
	hotobject_reader_init(&reader, obj);

	ddekit_json_encoding_writer_init(&json_writer, stdout);
	if(json_parser(&xp, "d:/ttt.json", &writer.super, &reader.super, &sp) == 0)
	{
		printf("input succeed\n");
	}
	else
	{
		printf("input failed\n");
		return 1;
	}
	hotobject_reader_init(&reader, obj);
	if(script_parser(&sp, "d:/c_hotpot_types", &reader.super, NULL, NULL) == 0)
	{
		printf("output succeed\n");
	}
	else
	{
		printf("output failed\n");
	}
	/*
	buff_size = 0;
	fin = fopen("d:/2.xml", "r");
	while((c = fgetc(fin)) != EOF)
	{
		buff[buff_size++] = c;
	}
	buff[buff_size++] = 0;

	script_parser_str(&sp, buff, buff_size, &reader.super, NULL, NULL);
	fclose(fin);
	*/
	return 0;
}

