#include <stdio.h>
#include "json_parser.h"
#include "hotscript/script_parser.h"
#include <string.h>
#include "hotobject.h"
#include "hotscript/hot_vm.h"

JSON_PARSER xp;
SCRIPT_PARSER sp;
HotObject *obj;
HotObjectReader reader;
HotObjectWriter writer;


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
	hotobject_get_writer(&writer, obj);
	hotobject_get_reader(&reader, obj);
	if(json_parser(&xp, "d:/1.json", &writer.super, &reader.super, &sp) == 0)
	{
		printf("input succeed\n");
	}
	else
	{
		printf("input failed\n");
		return 1;
	}
	
	hotobject_get_reader(&reader, obj);
	if(script_parser(&sp, "d:/2.xml", &reader.super, NULL, NULL) == 0)
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

