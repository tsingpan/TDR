#include "hotpot/hp_getopt.h"
#include "hotpot/hp_error.h"
#include "globals.h"

#include "encoding/ddekit_json_encoding_writer.h"
#include "hotscript/hotobject.h"
#include "hotscript/hot_vm.h"
#include "hotscript/hotobject_reader.h"
#include "hotscript/hotobject_writer.h"
#include "hotscript/script_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotdata_parser.h"


const char* const short_options = "?h:i:t:";

const struct option long_options[] =
{
	{ "help",					0, NULL, '?' },		//查看帮助



	{ "include",				1, NULL, 'i' },		//设置包含文件目录
	
	{ "template",				1, NULL, 't' },		//模版文件名

	{ NULL,						0, NULL, 0     }
};//最后一个元素标识为NULL

typedef void (*FUNC) (const char*, const char*);
#define MAX_FUNC_NUM 1024

DATA_PARSER dp;
JSON_WRITER jw;

char file_name[HP_MAX_FILE_PATH_LENGTH];

int main(int argc, char **argv)
{
	int i;
	int oc;		
	FILE *fout;
	
	
	while((oc = hp_getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
	{
		switch (oc)
		{
		case '?':
			//need useage
			break;
		case 'i':
			break;		
		case 't':
			strncpy(file_name, hp_optarg, HP_MAX_FILE_PATH_LENGTH);
			break;
		default:
			break;
		}
	}

	
	for(i = hp_optind; i < argc; ++i)
	{
		HotObject *obj = hotobject_new();
		HotObjectReader reader;
		HotObjectWriter writer;
		SCRIPT_PARSER sp;
		fout = fopen("d:/ast_base.out", "w");
		ddekit_json_encoding_writer_init(&jw, fout);

		hotobject_writer_init(&writer, obj);
		hotobject_reader_init(&reader, obj);
		data_parser(&dp, argv[i], &jw.super);
		fclose(fout);

		if(data_parser(&dp, argv[i], &writer.super) == E_HP_NOERROR)
		{
			printf("compile succeed\n");

			hotobject_reader_init(&reader, obj);
			if(script_parser(&sp, file_name, &reader.super, NULL, NULL) == 0)
			{
				printf("output succeed\n");
			}
		}
		else
		{
			printf("failed\n");
		}
		
		hotobject_free(obj);
	}
	//fclose(fout);
	return 0;
}
