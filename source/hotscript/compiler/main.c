#include <stdio.h>
#include "json_parser.h"
#include "script_parser.h"

#include "hotscript/hotobject.h"
#include "hot_vm.h"

JSON_PARSER xp;
SCRIPT_PARSER sp;
HotObject *obj;
HotObjectReader reader;
HotObjectWriter writer;


HotObject *ho;
HotObjectWriter iter;
HotObjectReader citer;

const char *str;

int main()
{
	obj = hotobject_new();
	hotobject_get_writer(&writer, obj);
	if(json_parser(&xp, "d:/1.json", &writer.super) == 0)
	{
		printf("input succeed\n");
	}
	else
	{
		printf("input failed\n");
	}
	
	hotobject_get_reader(&reader, obj);
	if(script_parser(&sp, "d:/2.xml", &reader.super) == 0)
	{
		printf("output succeed\n");
	}
	else
	{
		printf("output failed\n");
	}
	
	return 0;
}

