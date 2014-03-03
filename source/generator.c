#include "generator.h"
#include "error.h"
#include "globals.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void generator_init(GENERATOR *self, const SYMBOLS *symbols)
{
	self->symbols = symbols;
	self->fout = NULL;
	self->on_definition = NULL;
	self->on_document_begin = NULL;
	self->on_document_end = NULL;
	self->on_struct_begin = NULL;
	self->on_field = NULL;
	self->on_struct_end = NULL;
}

TD_ERROR_CODE generator_replace_extension(char *filename, uint32_t filename_length, const char *suffix)
{
	uint32_t i;
	uint32_t length = strlen(filename);

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
	char primary[TLIBC_MAX_PATH_LENGTH];
	char target_path[TLIBC_MAX_PATH_LENGTH];
	uint32_t path_length = 0;
	uint32_t i, document_name_length;

	//把扩展名替换为指定后缀
	if(strlen(primary_file) + 1 >= TLIBC_MAX_PATH_LENGTH)
	{
		goto ERROR_RET;
	}
	strncpy(primary, primary_file, TLIBC_MAX_PATH_LENGTH);
	generator_replace_extension(primary, TLIBC_MAX_PATH_LENGTH, suffix);

	//创建目录
	if(strlen(g_target_dir) + strlen(primary) + 1 + 1 >= TLIBC_MAX_PATH_LENGTH)
	{
		goto ERROR_RET;
	}
	snprintf(target_path, TLIBC_MAX_PATH_LENGTH, "%s%c%s", g_target_dir, TLIBC_FILE_SEPARATOR, primary);
	target_path[TLIBC_MAX_PATH_LENGTH - 1] = 0;
	path_length = strlen(target_path);
	for(i = 0; i < path_length; ++i)
	{
		if(target_path[i] == TLIBC_FILE_SEPARATOR)
		{
			target_path[i] = 0;
			tlibc_mkdir(target_path, 0755);
			target_path[i] = TLIBC_FILE_SEPARATOR;
		}
	}

	//计算文档名字
	strncpy(self->document_name, primary, TLIBC_MAX_PATH_LENGTH);
	self->document_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
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

void generator_print(GENERATOR *self, size_t tabs, const char* fmt, ...)
{
	va_list ap;
	size_t i;
	va_start(ap, fmt);
	for(i = 0;i < tabs; ++i)
	{
		fputc('\t', self->fout);
	}	
	vfprintf(self->fout, fmt, ap);
	va_end(ap);
}

void generator_printline(GENERATOR *self, size_t tabs, const char* fmt, ...)
{
	va_list ap;
	size_t i;
	va_start(ap, fmt);
	for(i = 0;i < tabs; ++i)
	{
		fputc('\t', self->fout);
	}	
	vfprintf(self->fout, fmt, ap);
	fputc('\n', self->fout);
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
		fputc('\\', self->fout);
		fputc('b', self->fout);
		break;
	case '\f':
		fputc('\\', self->fout);
		fputc('f', self->fout);
		break;
	case '\n':
		fputc('\\', self->fout);
		fputc('n', self->fout);
		break;
	case '\r':
		fputc('\\', self->fout);
		fputc('r', self->fout);
		break;
	case '\t':
		fputc('\\', self->fout);
		fputc('t', self->fout);
		break;
	case '\'':
		fputc('\\', self->fout);
		fputc('\'', self->fout);
		break;
	case '\"':
		fputc('\\', self->fout);
		fputc('"', self->fout);
		break;
	case '\\':
		fputc('\\', self->fout);
		fputc('\\', self->fout);
		break;
	default:
		fputc(c, self->fout);
	}
}

TD_ERROR_CODE generator_print_value(GENERATOR *self, const ST_VALUE *val)
{
	switch (val->type)
	{
	case E_SNVT_IDENTIFIER:
		fprintf(self->fout, "%s", val->val.identifier);
		return E_TD_NOERROR;
	case E_SNVT_CHAR:
		fputc('\'', self->fout);
		write_char(self, val->val.c);
		fputc('\'', self->fout);		
		return E_TD_NOERROR;
	case E_SNVT_STRING:
		{
			uint32_t i;
			uint32_t len = strlen(val->val.str);
			fputc('\"', self->fout);
			for(i = 0;i < len; ++i)
			{
				write_char(self, val->val.str[i]);
			}
			fputc('\"', self->fout);
			return E_TD_NOERROR;
		}
	case E_SNVT_DOUBLE:
		fprintf(self->fout, "%lf", val->val.d);
		return E_TD_NOERROR;		
	case E_SNVT_INT64:
		fprintf(self->fout, "%"PRIi64, val->val.i64);
		return E_TD_NOERROR;
	case E_SNVT_UINT64:
		fprintf(self->fout, "%"PRIu64, val->val.ui64);
		return E_TD_NOERROR;
	case E_SNVT_HEX_INT64:
		fprintf(self->fout, "0x%"PRIx64, val->val.i64);
		return E_TD_NOERROR;
	case E_SNVT_HEX_UINT64:
		fprintf(self->fout, "0x%"PRIx64, val->val.ui64);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;		
	}
}

TD_ERROR_CODE generator_print_type_name(GENERATOR *self, const ST_SIMPLE_TYPE *simple_type)
{
	switch(simple_type->st)
	{
	case E_ST_INT8:
		fprintf(self->fout, "int8");
		return E_TD_NOERROR;
	case E_ST_INT16:
		fprintf(self->fout, "int16");
		return E_TD_NOERROR;
	case E_ST_INT32:
		fprintf(self->fout, "int32");
		return E_TD_NOERROR;
	case E_ST_INT64:
		fprintf(self->fout, "int64");
		return E_TD_NOERROR;
	case E_ST_UINT8:
		fprintf(self->fout, "uint8");
		return E_TD_NOERROR;
	case E_ST_UINT16:
		fprintf(self->fout, "uint16");
		return E_TD_NOERROR;
	case E_ST_UINT32:
		fprintf(self->fout, "uint32");
		return E_TD_NOERROR;
	case E_ST_UINT64:
		fprintf(self->fout, "uint64");
		return E_TD_NOERROR;
	case E_ST_STRING:
		fprintf(self->fout, "char");
		return E_TD_NOERROR;
	case E_ST_CHAR:
		fprintf(self->fout, "char");
		return E_TD_NOERROR;
	case E_ST_DOUBLE:
		fprintf(self->fout, "double");
		return E_TD_NOERROR;
	case E_ST_REFER:
		fprintf(self->fout, simple_type->st_refer);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;
	}
}

TD_ERROR_CODE generator_print_ctype(GENERATOR *self, const ST_SIMPLE_TYPE *simple_type)
{
	switch(simple_type->st)
	{
	case E_ST_INT8:
		fprintf(self->fout, "int8_t");
		return E_TD_NOERROR;
	case E_ST_INT16:
		fprintf(self->fout, "int16_t");
		return E_TD_NOERROR;
	case E_ST_INT32:
		fprintf(self->fout, "int32_t");
		return E_TD_NOERROR;
	case E_ST_INT64:
		fprintf(self->fout, "int64_t");
		return E_TD_NOERROR;
	case E_ST_UINT8:
		fprintf(self->fout, "uint8_t");
		return E_TD_NOERROR;
	case E_ST_UINT16:
		fprintf(self->fout, "uint16_t");
		return E_TD_NOERROR;
	case E_ST_UINT32:
		fprintf(self->fout, "uint32_t");
		return E_TD_NOERROR;
	case E_ST_UINT64:
		fprintf(self->fout, "uint64_t");
		return E_TD_NOERROR;
	case E_ST_STRING:
		fprintf(self->fout, "char");
		return E_TD_NOERROR;
	case E_ST_CHAR:
		fprintf(self->fout, "char");
		return E_TD_NOERROR;
	case E_ST_DOUBLE:
		fprintf(self->fout, "double");
		return E_TD_NOERROR;
	case E_ST_REFER:
		fprintf(self->fout, simple_type->st_refer);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;
	}
}

TD_ERROR_CODE generator_print_cstype(GENERATOR *self, const ST_SIMPLE_TYPE *simple_type)
{
	const ST_SIMPLE_TYPE *st = symbols_get_real_type(self->symbols, simple_type);

	switch(st->st)
	{
	case E_ST_INT8:
		fprintf(self->fout, "sbyte");
		return E_TD_NOERROR;
	case E_ST_INT16:
		fprintf(self->fout, "short");
		return E_TD_NOERROR;
	case E_ST_INT32:
		fprintf(self->fout, "int");
		return E_TD_NOERROR;
	case E_ST_INT64:
		fprintf(self->fout, "long");
		return E_TD_NOERROR;
	case E_ST_UINT8:
		fprintf(self->fout, "byte");
		return E_TD_NOERROR;
	case E_ST_UINT16:
		fprintf(self->fout, "ushort");
		return E_TD_NOERROR;
	case E_ST_UINT32:
		fprintf(self->fout, "uint");
		return E_TD_NOERROR;
	case E_ST_UINT64:
		fprintf(self->fout, "ulong");
		return E_TD_NOERROR;
	case E_ST_STRING:
		fprintf(self->fout, "string");
		return E_TD_NOERROR;
	case E_ST_CHAR:
		fprintf(self->fout, "char");
		return E_TD_NOERROR;
	case E_ST_DOUBLE:
		fprintf(self->fout, "double");
		return E_TD_NOERROR;
	case E_ST_REFER:
		{
			fprintf(self->fout, st->st_refer);
			return E_TD_NOERROR;
		}
	default:
		return E_TD_ERROR;
	}
}

TD_ERROR_CODE generator_on_definition(GENERATOR *self, const YYLTYPE *yylloc, const ST_DEFINITION *definition)
{
	if(self->on_definition != NULL)
	{
		return self->on_definition(self, yylloc, definition);
	}
	return E_TD_NOERROR;
}


TD_ERROR_CODE generator_on_document_begin(GENERATOR *self, const YYLTYPE *yylloc, const char *file_name)
{
	if(self->on_document_begin != NULL)
	{
		return self->on_document_begin(self, yylloc, file_name);
	}
	return E_TD_NOERROR;
}

TD_ERROR_CODE generator_on_document_end(GENERATOR *self, const YYLTYPE *yylloc, const char *file_name)
{
	if(self->on_document_end != NULL)
	{
		return self->on_document_end(self, yylloc, file_name);
	}
	return E_TD_NOERROR;
}

TD_ERROR_CODE generator_on_struct_begin(GENERATOR *self, const YYLTYPE *yylloc, const char * struct_name)
{
	if(self->on_struct_begin != NULL)
	{
		return self->on_struct_begin(self, yylloc, struct_name);
	}
	return E_TD_NOERROR;
}

TD_ERROR_CODE generator_on_field(GENERATOR *self, const YYLTYPE *yylloc, const ST_FIELD *field)
{
	if(self->on_field != NULL)
	{
		return self->on_field(self, yylloc, field);
	}
	return E_TD_NOERROR;
}

TD_ERROR_CODE generator_on_struct_end(GENERATOR *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct)
{
	if(self->on_struct_end != NULL)
	{
		return self->on_struct_end(self, yylloc, pn_struct);
	}
	return E_TD_NOERROR;
}