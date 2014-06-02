#include <stdio.h>
#include "platform/tlibc_platform.h"
#include "parse/parser.h"
#include "language/language.h"
#include "version.h"
#include "getopt.h"


#include "generate/generator_types_h.h"
#include "generate/generator_reader_c.h"
#include "generate/generator_reader_h.h"
#include "generate/generator_writer_c.h"
#include "generate/generator_writer_h.h"
#include "generate/generator_sql.h"
#include "generate/generator_cs.h"


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
	fprintf(stderr, "Usage: %s [options] file.\n", PROGRAM_NAME);
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
	fprintf(stderr, "  -g STR, --gen=STR            Generate code with a generator.\n");
	fprintf(stderr, "Available generators:\n");
	fprintf(stderr, "types_h        Generate %%_types.h for TLibC.\n");
	fprintf(stderr, "reader_h       Generate %%_reader.h for TLibC.\n");
	fprintf(stderr, "reader_c       Generate %%_reader.c for TLibC.\n");
	fprintf(stderr, "writer_h       Generate %%_writer.h for TLibC.\n");
	fprintf(stderr, "writer_c       Generate %%_writer.c for TLibC.\n");	
	fprintf(stderr, "cs             Generate %%.cs for TLibCS.\n");
	fprintf(stderr, "sql            Generate %%.sql for MySQL.\n");	
}

const char* const short_options = "vho:I:Mg:";

const struct option long_options[] = {
	{ "version",	0, NULL, 'v' },
	{ "help"	,	0, NULL, 'h' },
	{ "output"	,	1, NULL, 'o' },
	{ "include"	,	1, NULL, 'I' },
	{ "MMD",		0, NULL, 'M' },
	{ "gen"	,		1, NULL, 'g' },
	{ NULL,			0, NULL,  0  },
};

#define TD_MAX_GENERATOR 16

static PARSER parser;
static generator_types_h_t tlibc_types_generator;
static generator_reader_c_t tlibc_reader_generator;
static generator_reader_h_t tlibc_reader_header_generator;
static generator_writer_c_t tlibc_writer_generator;
static td_generator_writer_h_t tlibc_writer_header_generator;
static generator_sql_t tlibc_sql_generator;
static generator_cs_t tlibc_cs_generator;

static generator_t *generator = NULL;
int main(int32_t argc, char *argv[])
{
	int opt;
	int32_t i;
	int make_rule = FALSE;

	parser_init(&parser);
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
		case 'g':
			{
				const char *arg = optarg;
				if(strcmp(arg, "types_h") == 0)
				{
					generator_types_h_init(&tlibc_types_generator, &parser.symbols, make_rule);
					generator = &tlibc_types_generator.super;
				}
				else if(strcmp(arg, "reader_h") == 0)
				{
					generator_reader_h_init(&tlibc_reader_header_generator, &parser.symbols, make_rule);
					generator = &tlibc_reader_header_generator.super;
				}
				else if(strcmp(arg, "reader_c") == 0)
				{
					generator_reader_c_init(&tlibc_reader_generator, &parser.symbols, make_rule);
					generator = &tlibc_reader_generator.super;
				}			
				else if(strcmp(arg, "writer_h") == 0)
				{
					generator_writer_h_init(&tlibc_writer_header_generator, &parser.symbols, make_rule);
					generator = &tlibc_writer_header_generator.super;
				}
				else if(strcmp(arg, "writer_c") == 0)
				{
					generator_writer_c_init(&tlibc_writer_generator, &parser.symbols, make_rule);
					generator = &tlibc_writer_generator.super;
				}
				else if(strcmp(arg, "cs") == 0)
				{
					generator_cs_init(&tlibc_cs_generator, &parser.symbols, make_rule);
					generator = &tlibc_cs_generator.super;
				}
				else if(strcmp(arg, "sql") == 0)
				{
					generator_sql_init(&tlibc_sql_generator, &parser.symbols, make_rule);
					generator = &tlibc_sql_generator.super;
				}
			}
			break;
		default:
			goto ERROR_RET;
		}
	}
	
	if(generator == NULL)
	{
		fprintf(stderr, "Missing --gen=types\n");
		usage();
		goto ERROR_RET;
	}

	for(i = optind; i < argc; ++i)
	{
		if(parser_parse(&parser, argv[i], generator) != E_TD_NOERROR)
		{
			goto ERROR_RET;
		}
	}

done:
	return 0;
ERROR_RET:
	return 1;
}
