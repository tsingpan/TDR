#include "error/error_code.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "parse/parser.h"
#include "protocol/tlibc_xml_writer.h"
#include "protocol/tlibc_xml_reader.h"

#include "error/error_msg_reader.h"
#include "error/error_msg.h"

#define TDATA_VERSION "0.0.1"


void version()
{
	printf("TData version %s\n", TDATA_VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: tdata [options] file\n\n");
	fprintf(stderr, "Use tdata -help for a list of options\n");
}

void help()
{
	fprintf(stderr, "Usage: tdata [options] file\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -version					Print the compiler version\n");
	fprintf(stderr, "  -gen types				Gen source\n");
	fprintf(stderr, "  -s dir					Set the source directory\n");
	fprintf(stderr, "  -t dir					Set the target directory\n");
}


PARSER dp;

char root_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char lua_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char python_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char real_script_path[TLIBC_MAX_FILE_PATH_LENGTH];
char path_prefix[TLIBC_MAX_FILE_PATH_LENGTH];


void get_real_file_path(const char *script_dir, const char *file_name)
{
	if(access(file_name, 00) == 0)
	{
		snprintf(real_script_path, TLIBC_MAX_FILE_PATH_LENGTH, "%s", file_name);		
	}
	else
	{
		snprintf(real_script_path, TLIBC_MAX_FILE_PATH_LENGTH, "%s%s", script_dir, file_name);
	}
}


#define MAX_PY_SCRIPT_LENGTH 32768
char pyscript[MAX_PY_SCRIPT_LENGTH];

int main(tint32 argc, char **argv)
{
	tuint32 j, option_end;
	tint32 i;
	strncpy(root_dir, argv[0], TLIBC_MAX_FILE_PATH_LENGTH);
	
	option_end = 0xffffffff;
	//首先获得根目录
	snprintf(root_dir, TLIBC_MAX_FILE_PATH_LENGTH, getenv("TDATA_HOME"));
	if(root_dir[strlen(root_dir) - 1] != TLIBC_FILE_SEPARATOR)
	{
		root_dir[strlen(root_dir) + 1] = 0;
		root_dir[strlen(root_dir)] = TLIBC_FILE_SEPARATOR;
	}
	

	parser_init(&dp, root_dir);
	snprintf(lua_dir, TLIBC_MAX_FILE_PATH_LENGTH, "%slua%c", root_dir, TLIBC_FILE_SEPARATOR);
	snprintf(python_dir, TLIBC_MAX_FILE_PATH_LENGTH, "%spython%c", root_dir, TLIBC_FILE_SEPARATOR);
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
		else if (strcmp(arg, "-s") == 0)
		{
			arg = argv[++i];
			if (arg == NULL)
			{
				fprintf(stderr, "Missing template file specification\n");
				usage();
				goto ERROR_RET;
			}
			scanner_stack_add_path(&dp.scanner_stack, arg);
		}
		else if (strcmp(arg, "-t") == 0)
		{
			++i;
		}
		else if (strcmp(arg, "-python") == 0)
		{
			++i;
		}		
		else
		{
			option_end = i;
			break;
		}
	}
	


	
	for(i = option_end; i < argc; ++i)
	{
		const char *output_dir = "./";
		if(parser_parse(&dp, argv[i]) != E_TD_NOERROR)
		{
			goto ERROR_RET;
		}

		for(j = 1; j < option_end; ++j)
		{
			char* arg;

			arg = strtok(argv[j], " ");
			// Treat double dashes as single dashes
			if (arg[0] == '-' && arg[1] == '-')
			{
				++arg;
			}

			if (strcmp(arg, "-t") == 0)
			{
				output_dir = argv[++j];
			}
		}
	}

	return 0;
ERROR_RET:
	return 1;
}
