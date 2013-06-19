#include <stdio.h>
#include "xml_parser.h"
#include "script_parser.h"


XML_PARSER xp;
SCRIPT_PARSER sp;

int main()
{
	FILE *fin = fopen("d:/1.xml", "rb");
	//if(xml_parser(&xp, fin) == 0)
	if(script_parser(&sp, fin) == 0)
	{
		printf("succeed\n");
	}
	else
	{
		printf("failed\n");
	}
	fclose(fin);

	
	return 0;
}

