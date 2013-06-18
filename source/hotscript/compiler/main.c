#include <stdio.h>
#include "xml_parser.h"



XML_PARSER xp;

int main()
{
	FILE *fin = fopen("d:/1.xml", "rb");

	if(xml_parser(&xp, fin) == 0)
	{
		printf("succeed\n");
	}
	fclose(fin);
	return 0;
}

