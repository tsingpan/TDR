#include "generator.h"
#include "error/error_code.h"

#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include <stdarg.h>

void generator_init(GENERATOR *self, const char *target_dir)
{
	self->target_dir = target_dir;
	self->fout = NULL;
	self->on_definition = NULL;
	self->on_document_begin = NULL;
	self->on_document_end = NULL;
}


TD_ERROR_CODE generator_open(GENERATOR *self, const char *primary_file, const char *suffix)
{
	char path[TLIBC_MAX_FILE_PATH_LENGTH];
	tuint32 path_length = 0;
	char file[TLIBC_MAX_FILE_PATH_LENGTH];
	tuint32 file_length = 0;
	tuint32 i;

	//step 1复制字符串
	if(strlen(self->target_dir) + strlen(primary_file) + 1 >= TLIBC_MAX_FILE_PATH_LENGTH - 1)
	{
		goto ERROR_RET;
	}
	snprintf(file, TLIBC_MAX_FILE_PATH_LENGTH, "%s%c%s", self->target_dir, TLIBC_FILE_SEPARATOR, primary_file);
	file[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
	file_length = strlen(file);


	//step 2删除扩展名
	for(i = file_length; i > 0; --i)
	{
		if(file[i] == '.')
		{
			file[i] = 0;
			file_length = i;
			break;
		}
		else if(file[i] == TLIBC_FILE_SEPARATOR)
		{
			break;
		}
	}

	//step 4获取路径
	strncpy(path, file, TLIBC_MAX_FILE_PATH_LENGTH);
	path[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
	path_length = strlen(path);
	for(i = 0; i < path_length; ++i)
	{
		if(path[i] == TLIBC_FILE_SEPARATOR)
		{
			path[i] = 0;
			mkdir(path);
			path[i] = TLIBC_FILE_SEPARATOR;
		}
	}

	//step 4添加后缀
	if(file_length + strlen(suffix) < TLIBC_MAX_FILE_PATH_LENGTH - 1)
	{
		strncpy(file + file_length, suffix, TLIBC_MAX_FILE_PATH_LENGTH - file_length);
		file[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;
	}

	self->fout = fopen(file, "w");
	if(self->fout == NULL)
	{
		goto ERROR_RET;
	}

	

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

TD_ERROR_CODE generator_print(GENERATOR *self, const char* fmt, ...)
{
	va_list ap;

	if(self->fout == NULL)
	{
		goto ERROR_RET;
	}

	va_start(ap, fmt);
	vfprintf(self->fout, fmt, ap);
	va_end(ap);

	

	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
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

TD_ERROR_CODE generator_print_value(GENERATOR *self, const ST_VALUE *val)
{
	switch (val->type)
	{
	case E_SNVT_IDENTIFIER:
		return generator_print(self, "%s", val->val.identifier);
	case E_SNVT_CHAR:
		//转义？
		return generator_print(self, "\'%c\'", val->val.c);
	case E_SNVT_DOUBLE:
		return generator_print(self, "%d", val->val.d);
	case E_SNVT_BOOL:
		if(val->val.b)
		{
			return generator_print(self, "hptrue");
		}
		else
		{
			return generator_print(self, "hpfalse");
		}
	case E_SNVT_INT64:
		return generator_print(self, "%lld", val->val.i64);
	case E_SNVT_UINT64:
		return generator_print(self, "%llu", val->val.ui64);
	case E_SNVT_HEX_INT64:
		return generator_print(self, "%llx", val->val.i64);
	case E_SNVT_HEX_UINT64:
		return generator_print(self, "%llx", val->val.ui64);
	default:
		return E_TD_ERROR;
	}
}

TD_ERROR_CODE generator_print_type(GENERATOR *self, const ST_TYPE *type, const ST_ARGUMENTS *arg)
{
	if(type->type == E_SNT_SIMPLE)
	{
		switch(type->st)
		{
		case E_ST_INT8:
			return generator_print(self, "tint8");
		case E_ST_INT16:
			return generator_print(self, "tint16");
		case E_ST_INT32:
			return generator_print(self, "tint32");
		case E_ST_INT64:
			return generator_print(self, "tint64");

		case E_ST_UINT8:
			return generator_print(self, "tuint8");
		case E_ST_UINT16:
			return generator_print(self, "tuint16");
		case E_ST_UINT32:
			return generator_print(self, "tuint32");
		case E_ST_UINT64:
			return generator_print(self, "tuint64");

		case E_ST_CHAR:
			return generator_print(self, "tchar");
		case E_ST_BOOL:
			return generator_print(self, "tbool");
		case E_ST_DOUBLE:
			return generator_print(self, "tdouble");
		default:
			return E_TD_ERROR;
		}
	}
	else if(type->type == E_SNT_CONTAINER)
	{
		if(type->ct == E_CT_VECTOR)
		{
			return generator_print(self, arg->arg_list[0].ot);
		}
		else if(type->ct == E_CT_STRING)
		{
			return generator_print(self, "tchar");
		}
	}
	else if(type->type == E_SNT_REFER)
	{
		return generator_print(self, type->ot);
	}

	return E_TD_ERROR;
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
