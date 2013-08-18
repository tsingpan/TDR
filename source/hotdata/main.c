#include "hotpot/hp_error_code.h"
#include "globals.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "hotscript/hp_script_vm.h"
#include "hotscript/hp_script_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotdata_parser.h"
#include "hotprotocol/hp_xml_writer.h"
#include "hotprotocol/hp_xml_reader.h"
#include "hp_lua_writer.h"

#include "hotscript/hp_error_msg_reader.h"
#include "hotscript/hp_error_msg.h"

#include "hotdata/syntactic_node.h"
#include "hotscript/hp_script_vm.h"


#include "Python.h"

void version()
{
	printf("HotData version %s\n", HOTPOT_VERSION);
}

void usage()
{
	fprintf(stderr, "Usage: hd [options] file\n\n");
	fprintf(stderr, "Use hd -help for a list of options\n");
}

void help()
{
	fprintf(stderr, "Usage: thrift [options] file\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -version					Print the compiler version\n");
	fprintf(stderr, "  -lua filename			Run the lua script\n");
	fprintf(stderr, "  -python filename			Run the python script\n");
	fprintf(stderr, "  -pythonstr script		Run the python script\n");
	fprintf(stderr, "  -i dir					Add a directory to the list of directories\n");
}


DATA_PARSER dp;

#include "hp_python_writer.h"

HP_LUA_WRITER writer;
HP_PYTHON_WRITER python_writer;

char root_dir[HP_MAX_FILE_PATH_LENGTH];
char lua_dir[HP_MAX_FILE_PATH_LENGTH];
char python_dir[HP_MAX_FILE_PATH_LENGTH];
char real_script_path[HP_MAX_FILE_PATH_LENGTH];
char path_prefix[HP_MAX_FILE_PATH_LENGTH];

void script_putc(HotVM *self, char c)
{
	fputc(c, (FILE*)self->user_data);
}

void get_real_file_path(const char *script_dir, const char *file_name)
{
	if(access(file_name, 00) == 0)
	{
		snprintf(real_script_path, HP_MAX_FILE_PATH_LENGTH, "%s", file_name);		
	}
	else
	{
		snprintf(real_script_path, HP_MAX_FILE_PATH_LENGTH, "%s%s", script_dir, file_name);
	}
}


SCRIPT_PARSER sp;

#define MAX_PY_SCRIPT_LENGTH 32768

int main(hpint32 argc, char **argv)
{
	hpuint32 j, option_end;
	lua_State *L;
	hpint32 i;
	strncpy(root_dir, argv[0], HP_MAX_FILE_PATH_LENGTH);
	
	option_end = 0xffffffff;
	//首先获得根目录
	snprintf(root_dir, HP_MAX_FILE_PATH_LENGTH, getenv("HOTPOT_DIR"));
	if(root_dir[strlen(root_dir) - 1] != HP_FILE_SEPARATOR)
	{
		root_dir[strlen(root_dir) + 1] = 0;
		root_dir[strlen(root_dir)] = HP_FILE_SEPARATOR;
	}

	data_parser_init(&dp, root_dir);
	snprintf(lua_dir, HP_MAX_FILE_PATH_LENGTH, "%slua%c", root_dir, HP_FILE_SEPARATOR);
	snprintf(python_dir, HP_MAX_FILE_PATH_LENGTH, "%spython%c", root_dir, HP_FILE_SEPARATOR);
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
		else if (strcmp(arg, "-i") == 0)
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
		else if (strcmp(arg, "-lua") == 0)
		{
			++i;
		}
		else if (strcmp(arg, "-python") == 0)
		{
			++i;
		}
		else if (strcmp(arg, "-pythonstr") == 0)
		{
			++i;
		}
		else if (strcmp(arg, "-luastr") == 0)
		{
			++i;
		}
		else
		{
			option_end = i;
			break;
		}
	}
	

	L = luaL_newstate();
	luaL_openlibs(L);



	lua_pushstring(L, root_dir);
	lua_setglobal( L, "root_dir" );

	lua_pushstring(L, lua_dir);
	lua_setglobal( L, "lua_dir" );


	Py_Initialize();
	for(i = option_end; i < argc; ++i)
	{
		lua_writer_init(&writer, L);
		python_writer_init(&python_writer);
		if(data_parser(&dp, argv[i], &python_writer.super) != E_HP_NOERROR)
		{
			goto ERROR_RET;
		}
		lua_setglobal( L, "document" );

		for(j = 1; j < option_end; ++j)
		{
			char* arg;

			arg = strtok(argv[j], " ");
			// Treat double dashes as single dashes
			if (arg[0] == '-' && arg[1] == '-')
			{
				++arg;
			}

			if (strcmp(arg, "-lua") == 0)
			{
				arg = argv[++j];
				get_real_file_path(lua_dir, arg);
				if(luaL_dofile(L, real_script_path) != 0)
				{
					const char* error = lua_tostring(L, -1);
					fprintf(stderr, error);
					goto ERROR_RET;
				}
			}
			else if (strcmp(arg, "-luastr") == 0)
			{
				arg = argv[++j];
				if(luaL_dostring(L, arg) != 0)
				{
					const char* error = lua_tostring(L, -1);
					fprintf(stderr, error);
					goto ERROR_RET;
				}
			}
			else if (strcmp(arg, "-pythonstr") == 0)
			{
				arg = argv[++j];
				if(PyRun_SimpleString(arg) != 0)
				{
					goto PYTHON_ERROR;
				}
			}
			else if (strcmp(arg, "-python") == 0)
			{
				PyObject *pModule, *pDict, *pFunc, *pRetVal, *pParam;
				char py_script[MAX_PY_SCRIPT_LENGTH];				
				arg = argv[++j];
				get_real_file_path(python_dir, arg);

				PyRun_SimpleString("import sys;");
				snprintf(py_script, sizeof(py_script), "sys.path.append(\"%s\")", python_dir);
				PyRun_SimpleString(py_script);

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

				pParam = PyTuple_New(1);
				PyTuple_SetItem(pParam, 0, pDict);
				pRetVal = PyObject_CallObject(pFunc, pParam);

				if(!pRetVal)
				{
					goto PYTHON_ERROR;
				}

				if(pRetVal != Py_True)
				{
					goto ERROR_RET;
				}
				Py_DECREF(pDict);
			}
		}
	}

	Py_Finalize();

	return 0;
PYTHON_ERROR:
	PyErr_Print();
	Py_Finalize();
ERROR_RET:
	return 1;
}
