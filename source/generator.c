#include "generator.h"
#include "error.h"
#include "globals.h"

#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include <stdarg.h>

void generator_init(GENERATOR *self, const SYMBOLS *symbols)
{
	self->symbols = symbols;
	self->fout = NULL;
	self->on_definition = NULL;
	self->on_document_begin = NULL;
	self->on_document_end = NULL;
}

TD_ERROR_CODE generator_replace_extension(char *filename, tuint32 filename_length, const char *suffix)
{
	tuint32 i;
	tuint32 length = strlen(filename);

	if((filename == NULL) || (suffix == NULL))
	{
		goto ERROR_RET;
	}

	for(i = length; i > 0; --i)
	{
		if(filename[i] == '.')
		{
			filename[i] = 0;
			length = i;
			break;
		}
		else if(filename[i] == TLIBC_FILE_SEPARATOR)
		{
			break;
		}
	}

	if(length + strlen(suffix) + 1 >= filename_length)
	{
		goto ERROR_RET;
	}
	strncpy(filename + length, suffix, filename_length - length);
	filename[filename_length - 1] = 0;

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}
TD_ERROR_CODE generator_open(GENERATOR *self, const char *primary_file, const char *suffix)
{
	char primary[TLIBC_MAX_FILE_PATH_LENGTH];
	char target_path[TLIBC_MAX_FILE_PATH_LENGTH];
	tuint32 path_length = 0;
	tuint32 i, document_name_length;

	//把扩展名替换为指定后缀
	if(strlen(primary_file) + 1 >= TLIBC_MAX_FILE_PATH_LENGTH)
	{
		goto ERROR_RET;
	}
	strncpy(primary, primary_file, TLIBC_MAX_FILE_PATH_LENGTH);
	generator_replace_extension(primary, TLIBC_MAX_FILE_PATH_LENGTH, suffix);

	//创建目录
	if(strlen(g_target_dir) + strlen(primary) + 1 + 1 >= TLIBC_MAX_FILE_PATH_LENGTH)
	{
		goto ERROR_RET;
	}
	snprintf(target_path, TLIBC_MAX_FILE_PATH_LENGTH, "%s%c%s", g_target_dir, TLIBC_FILE_SEPARATOR, primary);
	target_path[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
	path_length = strlen(target_path);
	for(i = 0; i < path_length; ++i)
	{
		if(target_path[i] == TLIBC_FILE_SEPARATOR)
		{
			target_path[i] = 0;
			mkdir(target_path);
			target_path[i] = TLIBC_FILE_SEPARATOR;
		}
	}

	//计算文档名字
	strncpy(self->document_name, primary, TLIBC_MAX_FILE_PATH_LENGTH);
	self->document_name[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
	document_name_length = strlen(self->document_name);
	for(i = 0;i < document_name_length; ++i)
	{
		if((self->document_name[i] >= 'a') && (self->document_name[i] <= 'z'))
		{
			self->document_name[i] = 'A' + self->document_name[i] - 'a';
		}
		else if ((self->document_name[i] >= 'A') && (self->document_name[i] <= 'Z'))
		{
		}
		else if ((self->document_name[i] >= '0') && (self->document_name[i] <= '9'))
		{
		}
		else
		{
			self->document_name[i] = '_';
		}
	}



	self->fout = fopen(target_path, "w");
	if(self->fout == NULL)
	{
		goto ERROR_RET;
	}

	

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

void generator_print(GENERATOR *self, const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(self->fout, fmt, ap);
	va_end(ap);
}

TD_ERROR_CODE generator_close(GENERATOR *self)
{
	if(self->fout == NULL)
	{
		goto ERROR_RET;		
	}

	fclose(self->fout);
	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

static void write_char(GENERATOR *self, char c)
{
	switch(c)
	{
	case '\b':
		generator_print(self, "\\b");
		break;
	case '\f':
		generator_print(self, "\\f");
		break;
	case '\n':
		generator_print(self, "\\n");
		break;
	case '\r':
		generator_print(self, "\\r");
		break;
	case '\t':
		generator_print(self, "\\t");
		break;
	case '\'':
		generator_print(self, "\\'");
		break;
	case '\"':
		generator_print(self, "\\\"");
		break;
	case '\\':
		generator_print(self, "\\\\");
		break;
	default:
		generator_print(self, "%c", c);
	}
}

TD_ERROR_CODE generator_print_value(GENERATOR *self, const ST_VALUE *val)
{
	switch (val->type)
	{
	case E_SNVT_IDENTIFIER:
		generator_print(self, "%s", val->val.identifier);
		return E_TD_NOERROR;
	case E_SNVT_CHAR:
		generator_print(self, "\'");
		write_char(self, val->val.c);
		generator_print(self, "\'");
		return E_TD_NOERROR;
	case E_SNVT_STRING:
		{
			tuint32 i;
			tuint32 len = strlen(val->val.str);
			generator_print(self, "\"");
			for(i = 0;i < len; ++i)
			{
				write_char(self, val->val.str[i]);
			}
			generator_print(self, "\"");
			return E_TD_NOERROR;
		}
	case E_SNVT_DOUBLE:
		generator_print(self, "%lf", val->val.d);
		return E_TD_NOERROR;
	case E_SNVT_INT64:
		generator_print(self, "%lld", val->val.i64);
		return E_TD_NOERROR;
	case E_SNVT_UINT64:
		generator_print(self, "%llu", val->val.ui64);
		return E_TD_NOERROR;
	case E_SNVT_HEX_INT64:
		generator_print(self, "%llx", val->val.i64);
		return E_TD_NOERROR;
	case E_SNVT_HEX_UINT64:
		generator_print(self, "%llx", val->val.ui64);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;		
	}
}

TD_ERROR_CODE generator_print_simple_type(GENERATOR *self, const ST_SIMPLE_TYPE *simple_type)
{
	switch(simple_type->st)
	{
	case E_ST_INT8:
		generator_print(self, "tint8");
		return E_TD_NOERROR;
	case E_ST_INT16:
		generator_print(self, "tint16");
		return E_TD_NOERROR;
	case E_ST_INT32:
		generator_print(self, "tint32");
		return E_TD_NOERROR;
	case E_ST_INT64:
		generator_print(self, "tint64");
		return E_TD_NOERROR;
	case E_ST_UINT8:
		generator_print(self, "tuint8");
		return E_TD_NOERROR;
	case E_ST_UINT16:
		generator_print(self, "tuint16");
		return E_TD_NOERROR;
	case E_ST_UINT32:
		generator_print(self, "tuint32");
		return E_TD_NOERROR;
	case E_ST_UINT64:
		generator_print(self, "tuint64");
		return E_TD_NOERROR;
	case E_ST_STRING:
		generator_print(self, "tstring");
		return E_TD_NOERROR;
	case E_ST_CHAR:
		generator_print(self, "tchar");
		return E_TD_NOERROR;
	case E_ST_DOUBLE:
		generator_print(self, "tdouble");
		return E_TD_NOERROR;
	case E_ST_REFER:
		generator_print(self, simple_type->st_refer);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;
	}
}

TD_ERROR_CODE generator_on_definition(GENERATOR *self, const ST_DEFINITION *definition)
{
	if(self->on_definition != NULL)
	{
		return self->on_definition(self, definition);
	}
	return E_TD_NOERROR;
}


TD_ERROR_CODE generator_on_document_begin(GENERATOR *self, const char *file_name)
{
	if(self->on_document_begin != NULL)
	{
		return self->on_document_begin(self, file_name);
	}
	return E_TD_NOERROR;
}

TD_ERROR_CODE generator_on_document_end(GENERATOR *self, const char *file_name)
{
	if(self->on_document_end != NULL)
	{
		return self->on_document_end(self, file_name);
	}
	return E_TD_NOERROR;
}
