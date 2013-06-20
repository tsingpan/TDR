#include <stdio.h>
#include "xml_parser.h"
#include "script_parser.h"


XML_PARSER xp;
SCRIPT_PARSER sp;

int main()
{
	if(xml_parser(&xp, "d:/_1.xml") == 0)
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

