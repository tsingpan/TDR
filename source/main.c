#include <stdio.h>
#include "parse/parser.h"
#include "generate/tlibc_types.h"
#include "error/error.h"

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


PARSER parser;

TLIBC_TYPES_GENERATOR tlibc_types_generator;

int main(tint32 argc, char **argv)
{
	tint32 i;

	error_init();

	parser_init(&parser);

	for (i = 1; i < argc; ++i)
	{
		char* arg;

		arg = strtok(argv[i], " ");
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
			const char *source_dir = "./";
			source_dir = argv[++i];
			if (source_dir == NULL)
			{
				fprintf(stderr, "Missing source directory specification\n");
				usage();
				goto ERROR_RET;
			}
			scanner_stack_add_path(&parser.scanner_stack, source_dir);
		}
		else if (strcmp(arg, "-t") == 0)
		{
			const char *target_dir = "./";
			target_dir = argv[++i];
			if (target_dir == NULL)
			{
				fprintf(stderr, "Missing target directory specification\n");
				usage();
				goto ERROR_RET;
			}
		}
		else if (strcmp(arg, "-gen") == 0)
		{
			arg = argv[++i];
			if(strcmp(arg, "tlibc") == 0)
			{
				tlibc_types_generator_init(&tlibc_types_generator);
				parser_add_generator(&parser, &tlibc_types_generator.super);
			}
		}		
		else
		{
			break;
		}
	}
	
	for(; i < argc; ++i)
	{
		if(parser_parse(&parser, argv[i]) != E_TD_NOERROR)
		{
			goto ERROR_RET;
		}
	}

	return 0;
ERROR_RET:
	return 1;
}
