#include "hotlib/hp_error_code.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotdata_parser.h"
#include "protocol/tlibc_xml_writer.h"
#include "protocol/tlibc_xml_reader.h"

#include "hotlib/hp_error_msg_reader.h"
#include "hotlib/hp_error_msg.h"

#include "hotdata/syntactic_node.h"
#include "hotscript/hp_script_vm.h"


#include "Python.h"

void version()
{
	printf("HotData version %s\n", HOTPOT_VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: hotdata [options] file\n\n");
	fprintf(stderr, "Use hotdata -help for a list of options\n");
}

void help()
{
	fprintf(stderr, "Usage: hotdata [options] file\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -version					Print the compiler version\n");
	fprintf(stderr, "  -python filename			Run the python script\n");
	fprintf(stderr, "  -s dir					Set the source directory\n");
	fprintf(stderr, "  -t dir					Set the target directory\n");
}


DATA_PARSER dp;

#include "hp_python_writer.h"

TLIBC_PYTHON_WRITER python_writer;

char root_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char lua_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char python_dir[TLIBC_MAX_FILE_PATH_LENGTH];
char real_script_path[TLIBC_MAX_FILE_PATH_LENGTH];
char path_prefix[TLIBC_MAX_FILE_PATH_LENGTH];

void script_putc(HotVM *self, char c)
{
	fputc(c, (FILE*)self->user_data);
}

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
	snprintf(root_dir, TLIBC_MAX_FILE_PATH_LENGTH, getenv("HOTPOT_DIR"));
	if(root_dir[strlen(root_dir) - 1] != TLIBC_FILE_SEPARATOR)
	{
		root_dir[strlen(root_dir) + 1] = 0;
		root_dir[strlen(root_dir)] = TLIBC_FILE_SEPARATOR;
	}
	

	data_parser_init(&dp, root_dir);
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
	


	Py_Initialize();
	snprintf(pyscript, sizeof(pyscript), "import sys;sys.path.append(\'%s\');", python_dir);
	PyRun_SimpleString(pyscript);

	for(i = option_end; i < argc; ++i)
	{
		const char *output_dir = "./";
		python_writer_init(&python_writer);
		if(data_parser(&dp, argv[i], &python_writer.super) != E_TLIBC_NOERROR)
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
			else if (strcmp(arg, "-python") == 0)
			{
				
				PyObject *pModule, *pDict, *pFunc, *pRetVal, *pParam;
				arg = argv[++j];


				
				pModule = PyImport_ImportModule(arg);
				if(pModule == NULL)
				{
					goto PYTHON_ERROR;
				}				

				pFunc = PyObject_GetAttrString(pModule, "hpmain");
				if(pFunc == NULL)
				{
					goto PYTHON_ERROR;
				}

				pDict = python_writer.stack[0];
				if(pDict == NULL)
				{
					goto PYTHON_ERROR;
				}

				pParam = PyTuple_New(2);
				PyTuple_SetItem(pParam, 0, pDict);
				PyTuple_SetItem(pParam, 1, PyUnicode_FromString(output_dir));
				pRetVal = PyObject_CallObject(pFunc, pParam);

				if(!pRetVal)
				{
					goto PYTHON_ERROR;
				}

				if(pRetVal != Py_True)
				{
					goto ERROR_RET;
				}
			}
		}

		//Py_DECREF(python_writer.stack[0]);
	}

	Py_Finalize();

	return 0;
PYTHON_ERROR:
	PyErr_Print();
	Py_Finalize();
ERROR_RET:
	return 1;
}
