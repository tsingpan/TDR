#include <stdio.h>
#include "json_parser.h"
#include "script_parser.h"

#include "hotobject.h"

JSON_PARSER xp;
SCRIPT_PARSER sp;


HotObject *ho;
HotObjectIterator iter;
HotObjectConstIterator citer;

const char *str;

int main()
{
	if(json_parser(&xp, "d:/1.xml") == 0)
	//if(script_parser(&sp, "d:/1.xml") == 0)
	{
		printf("succeed\n");
	}
	else
	{
		printf("failed\n");
	}

	ho = hotobject_new();
	hotobject_get_iterator(&iter, ho);
	hotobject_write_object_begin(&iter, "haha");
	hotobject_write_string(&iter, 0, "123");
	hotobject_write_string(&iter, 0, "321");
	hotobject_write_object_end(&iter, "haha");

	hotobject_get_const_iterator(&citer, ho);
	hotobject_read_object_begin(&citer, "haha");
	hotobject_read_string(&citer, 0, &str);
	hotobject_read_string(&citer, 0, &str);
	hotobject_read_object_end(&citer, "haha");

	hotobject_free(ho);

	
	return 0;
}

