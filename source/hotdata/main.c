#include "hotpot/hp_getopt.h"
#include "hotpot/hp_error.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotdata_parser.h"
#include "encoding/ddekit_xml_encoding_writer.h"


const char* const short_options = "?h:i:c:";

const struct option long_options[] =
{
	{ "help",					0, NULL, '?' },		//查看帮助



	{ "include",				1, NULL, 'i' },		//设置包含文件目录
	
	{ "c_hotpot",				1, NULL, 'c' },		//输出c语言头文件

	{ NULL,						0, NULL, 0     }
};//最后一个元素标识为NULL

typedef void (*FUNC) (const char*, const char*);
#define MAX_FUNC_NUM 1024

int g_gen_c_hotpot = FALSE;
char g_gen_c_hotpot_output_path[HP_MAX_FILE_PATH_LENGTH];

DATA_PARSER dp;

XML_WRITER xml_writer;
int main(int argc, char **argv)
{
	int i;
	int oc;	

	
	while((oc = hp_getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
	{
		switch (oc)
		{
		case '?':
			//need useage
			break;
		case 'i':
			break;		
		case 'c':
			g_gen_c_hotpot = TRUE;
			strncpy(g_gen_c_hotpot_output_path, hp_optarg, HP_MAX_FILE_PATH_LENGTH);
			break;
		default:
			break;
		}
	}


	hp_xml_writer_init(&xml_writer, stdout);
	for(i = hp_optind; i < argc; ++i)
	{
		if(data_parser(&dp, argv[i], &xml_writer.super) == E_HP_NOERROR)
		{
			printf("succeed\n");
		}
		else
		{
			printf("failed\n");
		}
	}
	return 0;
}
