#include <stdio.h>
#include "platform/tdr_platform.h"
#include "parse/parser.h"
#include "language/language.h"
#include "version.h"
#include "getopt.h"
#include "script/script.h"

#include "generate/generator_types_h.h"
#include "generate/generator_reader_c.h"
#include "generate/generator_reader_h.h"
#include "generate/generator_writer_c.h"
#include "generate/generator_writer_h.h"


#include <string.h>
#include "globals.h"


void version()
{
	printf("%s version %s\n", PROJECT_NAME, VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: %s [options] file\n\n", PROGRAM_NAME);
	fprintf(stderr, "Use %s --help for a list of options\n", PROGRAM_NAME);
}

void help()
{
	fprintf(stderr, "Usage: %s [options] file\n", PROGRAM_NAME);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h, --help                   Display this information.\n");
	fprintf(stderr, "  -v, --version                Print the compiler version.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -o dir, --output=dir         Set the output directory for gen-* packages.\n");
	fprintf(stderr, "                               default: current directory.\n");	
	fprintf(stderr, "\n");
	fprintf(stderr, "  -I dir, --include=dir        Add a directory to the list of directories.\n");
	fprintf(stderr, "                               searched for include directives.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -M, --MMD                    Output a rule suitable for make describing \n");
	fprintf(stderr, "                               the dependencies of input files.\n");
	fprintf(stderr, "                               it takes the name of the input file, \n");
	fprintf(stderr, "                               and applies a .%s suffix.\n", DEP_SUFFIX);
	fprintf(stderr, "\n");
	fprintf(stderr, "  -l file, --lua=file          Execute lua script.\n");
	fprintf(stderr, "  -g STR, --gen=STR            Generate code with a generator.\n");
	fprintf(stderr, "Available generators:\n");
	fprintf(stderr, "types_h        Generate %%_types.h for TSF4G.\n");
	fprintf(stderr, "reader_h       Generate %%_reader.h for TSF4G.\n");
	fprintf(stderr, "reader_c       Generate %%_reader.c for TSF4G.\n");
	fprintf(stderr, "writer_h       Generate %%_writer.h for TSF4G.\n");
	fprintf(stderr, "writer_c       Generate %%_writer.c for TSF4G.\n");	
}

const char* const short_options = "vho:I:Ml:g:";

const struct option long_options[] = {
	{ "version",	0, NULL, 'v' },
	{ "help"	,	0, NULL, 'h' },
	{ "output"	,	1, NULL, 'o' },
	{ "include"	,	1, NULL, 'I' },
	{ "MMD",		0, NULL, 'M' },
	{ "lua"	,		1, NULL, 'l' },
	{ "gen"	,		1, NULL, 'g' },
	{ NULL,			0, NULL,  0  },
};

#define TD_MAX_GENERATOR 16

static generator_types_h_t tdr_types_generator;
static generator_reader_c_t tdr_reader_generator;
static generator_reader_h_t tdr_reader_header_generator;
static generator_writer_c_t tdr_writer_generator;
static td_generator_writer_h_t tdr_writer_header_generator;

static generator_t *generator = NULL;
int main(int32_t argc, char *argv[])
{
	int opt;
	int32_t i;
	int make_rule = FALSE;
	const char* script = NULL;

	parser_init(&g_parser);
	while((opt = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
	{
		switch(opt)
		{
		case 'v':
			version();
			goto done;
		case 'h':
			help();
			goto done;
		case 'I':
			if(g_include_dir_num >= INCLUDE_DIR_NUM)
			{
				fprintf(stderr, "Too many include directories.\n");
				goto ERROR_RET;
			}
			g_include_dir[g_include_dir_num] = optarg;
			++g_include_dir_num;
			break;
		case 'o':
			g_output_dir = optarg;
			break;
		case 'M':
			make_rule = TRUE;
			break;
		case 'l':
			script = optarg;
			break;
		case 'g':
			{
				const char *arg = optarg;
				if(strcmp(arg, "types_h") == 0)
				{
					generator_types_h_init(&tdr_types_generator, &g_parser.symbols);
					generator = &tdr_types_generator.super;
				}
				else if(strcmp(arg, "reader_h") == 0)
				{
					generator_reader_h_init(&tdr_reader_header_generator, &g_parser.symbols);
					generator = &tdr_reader_header_generator.super;
				}
				else if(strcmp(arg, "reader_c") == 0)
				{
					generator_reader_c_init(&tdr_reader_generator, &g_parser.symbols);
					generator = &tdr_reader_generator.super;
				}			
				else if(strcmp(arg, "writer_h") == 0)
				{
					generator_writer_h_init(&tdr_writer_header_generator, &g_parser.symbols);
					generator = &tdr_writer_header_generator.super;
				}
				else if(strcmp(arg, "writer_c") == 0)
				{
					generator_writer_c_init(&tdr_writer_generator, &g_parser.symbols);
					generator = &tdr_writer_generator.super;
				}				
			}
			break;
		default:
			goto ERROR_RET;
		}
	}
	
	if((generator == NULL) && (make_rule))
	{
		fprintf(stderr, "When specifying the -MMD parameter, you must specify a code generator.\n");
		usage();
		goto ERROR_RET;
	}

	if((generator == NULL) && (script == NULL))
	{
		fprintf(stderr, "Missing --gen=types or --lua=file\n");
		usage();
		goto ERROR_RET;
	}
	
	if(optind >= argc)
	{
		fprintf(stderr, "Missing file\n");
		usage();
		goto ERROR_RET;
	}

	if(script)
	{
		if(script_init(script) != TRUE)
		{
			goto ERROR_RET;
		}
	}

	for(i = optind; i < argc; ++i)
	{
		if(parser_parse(&g_parser, argv[i], generator, make_rule) != E_TD_NOERROR)
		{
			goto fini_script;
		}
	}

fini_script:
	if(script)
	{
		script_fini();
	}

done:
	return 0;
ERROR_RET:
	return 1;
}
