#include "hotpot/hp_getopt.h"
#include "hotpot/hp_error.h"
#include "globals.h"

#include "hotprotocol/hp_json_writer.h"
#include "hotscript/hotobject.h"
#include "hotscript/hot_vm.h"
#include "hotscript/hotobject_reader.h"
#include "hotscript/hotobject_writer.h"
#include "hotscript/script_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotdata_parser.h"
#include "hotprotocol/hp_xml_writer.h"
#include "hotprotocol/hp_xml_reader.h"

#include "language/language_types.h"
#include "language/language_reader.h"

LanguageLib language_lib;

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
HP_JSON_WRITER jw;
HP_XML_WRITER xml_writer;
HP_XML_READER xml_reader;

char file_name[HP_MAX_FILE_PATH_LENGTH];
HotObjectReader reader;
HotObjectWriter writer;
SCRIPT_PARSER sp;
char file_tag[1024];
hpuint32 file_tag_len;

int main(int argc, char **argv)
{
	int i;
	int oc;		
	FILE *fout;
	FILE *fout_xml;
	FILE *fin_xml;
	int ret;

	fin_xml = fopen("D:\\HotPot\\resource\\language\\simplified_chinese.xml", "r");

	xml_reader_init(&xml_reader, fin_xml);
	ret = read_LanguageLib(&xml_reader.super, &language_lib);

	fclose(fin_xml);
	
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
		
		hpuint32 j;
		fout = fopen("d:/ast_base.out", "w");
		ddekit_json_encoding_writer_init(&jw, fout);

		fout_xml = fopen("d:/ast_base.xml", "w");
		xml_writer_init(&xml_writer, fout_xml);

		hotobject_writer_init(&writer, obj);
		hotobject_reader_init(&reader, obj);
		if(data_parser(&dp, argv[i], &jw.super) != E_HP_NOERROR)
		{
			continue;
		}
		
		fclose(fout);

		if(data_parser(&dp, argv[i], &xml_writer.super) != E_HP_NOERROR)
		{
			continue;
		}
		fclose(fout_xml);

		file_tag_len = strlen(argv[i]);		
		for(j = 0;j < file_tag_len; ++j)
		{
			char ch = argv[i][j];
			if(
				((ch >= 'a') && (ch <= 'z'))
				||((ch >= 'A') && (ch <= 'Z'))
				||((ch >= '0') && (ch <= '9'))
				||(ch == '_')
				)
			{
				file_tag[j] = ch;
			}
			else
			{
				file_tag[j] = '_';
			}
			
		}
		write_field_begin(&writer.super, "file_tag", strlen("file_tag"));
		write_hpstring(&writer.super, file_tag);
		write_field_end(&writer.super, "file_tag", strlen("file_tag"));
		write_field_begin(&writer.super, "file", strlen("file"));
		write_hpstring(&writer.super, argv[i]);
		write_field_end(&writer.super, "file", strlen("file"));

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
