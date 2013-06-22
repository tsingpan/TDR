#include <stdio.h>
#include "json_parser.h"
#include "script_parser.h"


JSON_PARSER xp;
SCRIPT_PARSER sp;
int main()
{
	FILE* fout;

	if(json_parser(&xp, "d:/1.xml") == 0)
	//if(script_parser(&sp, "d:/1.xml") == 0)
	{
		printf("succeed\n");
	}
	else
	{
		printf("failed\n");
	}

	
	return 0;
}

