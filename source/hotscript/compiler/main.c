#include <stdio.h>
#include "json_parser.h"
#include "script_parser.h"

#include "hotobject.h"
#include "hot_vm.h"

JSON_PARSER xp;
SCRIPT_PARSER sp;


HotObject *ho;
HotObjectIterator iter;
HotObjectConstIterator citer;
HotVM hotvm;

const char *str;

int main()
{
	if(json_parser(&xp, "d:/1.json") == 0)
	{
		printf("input succeed\n");
	}
	else
	{
		printf("input failed\n");
	}
	

	if(script_parser(&sp, "d:/2.xml", xp.ho) == 0)
	{
		printf("output succeed\n");
	}
	else
	{
		printf("output failed\n");
	}
	printf("------------------------------------------------------------------------------\n");
	//hotvm_execute(&hotvm, &sp.hotoparr, sp.ho);


	ho = hotobject_new();
	hotobject_get_iterator(&iter, ho);
	hotobject_write_object_begin(&iter, "haha");
	hotobject_write_string(&iter, 0, "123");
	hotobject_write_string(&iter, 0, "321");
	hotobject_write_object_end(&iter, "haha");

	hotobject_get_const_iterator(&citer, ho);
	hotobject_read_object_begin(&citer, "haha");
	hotobject_read_string(&citer, "[0]", &str);
	hotobject_read_string(&citer, "[1]", &str);
	hotobject_read_object_end(&citer, "haha");

	hotobject_free(ho);

	
	return 0;
}

