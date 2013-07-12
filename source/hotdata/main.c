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
#include "language/language.h"

#define HOTDATA_VERSION "0.0.1"

void version()
{
  printf("HotData version %s\n", HOTDATA_VERSION);
}

void usage()
{
  fprintf(stderr, "Usage: hd [options] file\n\n");
  fprintf(stderr, "Use hd -help for a list of options\n");
}

/**
 * Diplays the help message and then exits with an error code.
 */
void help()
{
  fprintf(stderr, "Usage: thrift [options] file\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -version    Print the compiler version\n");
  fprintf(stderr, "  -t dir      Set the template file\n");
  fprintf(stderr, "  -i dir      Add a directory to the list of directories\n");
}


LanguageLib language_lib;

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

	data_parser_init(&dp);
	for (i = 1; i < argc; ++i)
	{
		char* arg;

		arg = strtok(argv[i], " ");
		// Treat double dashes as single dashes
		if (arg[0] == '-' && arg[1] == '-')
		{
			++arg;
		}

		if (strcmp(arg, "-help") == 0)
		{
			help();
			goto ERROR_RET;
		}
		else if (strcmp(arg, "-version") == 0)
		{
			version();
			goto ERROR_RET;
		}
		else if (strcmp(arg, "-t") == 0)
		{
			arg = argv[++i];
			if (arg == NULL)
			{
				fprintf(stderr, "Missing template file specification\n");
				usage();
				goto ERROR_RET;
			}
			strncpy(file_name, arg, HP_MAX_FILE_PATH_LENGTH);
		}
		else if (strcmp(arg, "-i") == 0)
		{
			arg = argv[++i];
			scanner_stack_add_path(&dp.scanner_stack, arg);
		}
		else
		{
			//默认后面都是文件名
			break;
		}
	}
	
	load_language(&language_lib, "D:\\HotPot\\resource\\language\\simplified_chinese.xml");

	for(; i < argc; ++i)
	{
		HotObject *obj = hotobject_new();
		
		hpuint32 j;
		fout = fopen("d:/ast_base.out", "w");
		ddekit_json_encoding_writer_init(&jw, fout);

		fout_xml = fopen("d:/ast_base.xml", "w");
		xml_writer_init(&xml_writer, fout_xml);

		hotobject_writer_init(&writer, obj);
		hotobject_reader_init(&reader, obj);
		if(data_parser(&dp, argv[i], &jw.super, &language_lib) != E_HP_NOERROR)
		{
			continue;
		}
		
		fclose(fout);

		if(data_parser(&dp, argv[i], &xml_writer.super, &language_lib) != E_HP_NOERROR)
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

		if(data_parser(&dp, argv[i], &writer.super, &language_lib) == E_HP_NOERROR)
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
ERROR_RET:
	return 1;
}
