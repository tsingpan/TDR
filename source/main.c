#include <stdio.h>
#include "parse/parser.h"
#include "language/language.h"
#include "version.h"
#include "getopt_long.h"


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
	printf("TData version %s\n", VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: tdata [options] file\n\n");
	fprintf(stderr, "Use tdata --help for a list of options\n");
}

void help()
{
	fprintf(stderr, "Usage: tdata [options] file.\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h, --help                   Display this information.\n");
	fprintf(stderr, "  -v, --version                Print the compiler version.\n");
	fprintf(stderr, "  -o dir, --output=dir         Set the output directory for gen-* packages.\n");
	fprintf(stderr, "                               default: current directory.\n");
	
	fprintf(stderr, "  -I dir, --include=dir        Add a directory to the list of directories.\n");
	fprintf(stderr, "                               searched for include directives.\n");
	fprintf(stderr, "  -g STR, --gen=STR            Generate code with a generator.\n");
	fprintf(stderr, "                               For each named %%.td input files, a generator will generate an output file only.\n");
	fprintf(stderr, "Available generators:\n");
	fprintf(stderr, "types_h        Generate the %%_types.h file for tlibc.\n");
	fprintf(stderr, "reader_h       Generate the %%_reader.h file for tlibc.\n");
	fprintf(stderr, "reader_c       Generate the %%_reader.c file for tlibc.\n");
	fprintf(stderr, "writer_h       Generate the %%_writer.h file for tlibc.\n");
	fprintf(stderr, "writer_c       Generate the %%_writer.c file for tlibc.\n");	
	fprintf(stderr, "cs             Generate the %%.cs file for tlibcs.\n");
	fprintf(stderr, "sql            Generate the %%.sql file for MySQL.\n");	
}

const char* const short_options = "vho:I:g:";

const struct option long_options[] = {
	{ "version",	0, NULL, 'v' },
	{ "help"	,	0, NULL, 'h' },
	{ "output"	,	1, NULL, 'o' },
	{ "include"	,	1, NULL, 'I' },
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

static generator_t *generator_list[TD_MAX_GENERATOR];
static uint32_t generator_num = 0;
int main(int32_t argc, char *argv[])
{
	int opt;
	int32_t i;

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
		case 'g':
			{
				const char *arg = optarg;
				if(strcmp(arg, "types_h") == 0)
				{
					generator_types_h_init(&tlibc_types_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_types_generator.super;
				}
				else if(strcmp(arg, "reader_h") == 0)
				{
					generator_reader_h_init(&tlibc_reader_header_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_reader_header_generator.super;
				}
				else if(strcmp(arg, "reader_c") == 0)
				{
					generator_reader_c_init(&tlibc_reader_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_reader_generator.super;
				}			
				else if(strcmp(arg, "writer_h") == 0)
				{
					generator_writer_h_init(&tlibc_writer_header_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_writer_header_generator.super;
				}
				else if(strcmp(arg, "writer_c") == 0)
				{
					generator_writer_c_init(&tlibc_writer_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_writer_generator.super;
				}
				else if(strcmp(arg, "cs") == 0)
				{
					generator_cs_init(&tlibc_cs_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_cs_generator.super;
				}
				else if(strcmp(arg, "sql") == 0)
				{
					generator_sql_init(&tlibc_sql_generator, &parser.symbols);
					generator_list[generator_num++] = &tlibc_sql_generator.super;
				}
			}
			break;
		default:
			goto ERROR_RET;
		}
	}
	
	if(generator_num == 0)
	{
		fprintf(stderr, "Missing --gen=types\n");
		usage();
		goto ERROR_RET;
	}

	for(i = optind; i < argc; ++i)
	{
		if(parser_parse(&parser, argv[i], generator_list, generator_num) != E_TD_NOERROR)
		{
			goto ERROR_RET;
		}
	}

done:
	return 0;
ERROR_RET:
	return 1;
}
