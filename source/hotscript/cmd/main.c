#include <stdio.h>
#include "hotjson/json_parser.h"
#include "hotscript/script_parser.h"
#include <string.h>
#include "hotscript/hotobject.h"
#include "hotscript/hot_vm.h"
#include "hotprotocol/hp_json_writer.h"
#include "hotprotocol/hp_json_reader.h"
#include "hotpot/hp_platform.h"
#include "hotscript/hotobject_reader.h"
#include "hotscript/hotobject_writer.h"

#include <stdio.h>

JSON_PARSER xp;
SCRIPT_PARSER sp;
HotObject *obj;
HotObjectReader reader;
HotObjectWriter writer;
HP_JSON_WRITER json_writer;
HP_JSON_READER json_reader;


HotObject *ho;
HotObjectWriter iter;
HotObjectReader citer;

const char *str;

char buff[102400];
size_t buff_size;
const char *sstr = "<% \'2222\' %>";

#define HOTSCRIPTCMD_VERSION "0.0.1"
void version()
{
	printf("HotScript version %s\n", HOTSCRIPTCMD_VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: hs [options] file\n\n");
	fprintf(stderr, "Use hs -help for a list of options\n");
}

void help()
{
	fprintf(stderr, "Usage: thrift [options] file\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -version					Print the compiler version\n");
	fprintf(stderr, "  -j filename				Set the json file\n");
	fprintf(stderr, "  -o filename				set output file\n");
}

#define HP_MAX_FILE_PATH_LENGTH 1024

FILE *fout = NULL;
char json_file_name[HP_MAX_FILE_PATH_LENGTH];

void script_putc(HotVM *self, char c)
{
	fputc(c, (FILE*)self->user_data);
}


int main(int argc, char **argv)
{
	hpint32 i;
	fout = stdout;

	for (i = 1; i < argc - 1; ++i)
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
		else if (strcmp(arg, "-f") == 0)
		{
			arg = argv[++i];
			if (arg == NULL)
			{
				fprintf(stderr, "Missing template file specification\n");
				usage();
				goto ERROR_RET;
			}
			strncpy(json_file_name, arg, HP_MAX_FILE_PATH_LENGTH);
		}
		else if (strcmp(arg, "-o") == 0)
		{
			arg = argv[++i];
			if (arg == NULL)
			{
				fprintf(stderr, "Missing template file specification\n");
				usage();
				goto ERROR_RET;
			}
			fout = fopen(arg, "wb");
		}
		else
		{
			fprintf(stderr, "Unrecognized option: %s\n", arg);
			usage();
			goto ERROR_RET;
		}
	}

	
	obj = hotobject_new();
	

	if(json_parser(&xp, json_file_name, obj, &sp) != 0)
	{
		goto ERROR_RET;
	}
	printf("json_parser succeed!\n");

	hotobject_reader_init(&reader, obj);
	if(script_parser(&sp, argv[i], &reader.super, fout, script_putc) != 0)
	{
		goto ERROR_RET;
	}
	
	printf("script_parser succeed!\n");
	return 0;
ERROR_RET:
	return 1;
}

