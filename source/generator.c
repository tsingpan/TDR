#include "generator.h"
#include "error.h"
#include "globals.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void generator_init(generator_t *self, const symbols_t *symbols)
{
	self->symbols = symbols;
	self->fout = NULL;
	self->on_definition = NULL;
	self->on_document_begin = NULL;
	self->on_document_end = NULL;
}

error_code_t generator_replace_extension(char *filename, uint32_t filename_length, const char *suffix)
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
		else if(filename[i] == TDR_FILE_SEPARATOR)
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

void strncpy_notdir(char *dest, const char*src, size_t dest_len)
{
	size_t i;
	size_t src_len = strlen(src);
	const char* ptr = src;

	for(i = 0; i < src_len; ++i)
	{
		if((src[i] == '/') || (src[i] == '\\'))
		{
			if(i + 1 < src_len)
			{
				ptr = src + i + 1;
			}
		}
	}

	strncpy(dest, ptr, dest_len);
}

void strncpy_dir(char *dest, const char*src, size_t dest_len)
{
	size_t i;
	size_t src_len = strlen(src);
	const char* ptr = NULL;
	size_t len;

	for(i = 0; i < src_len; ++i)
	{
		if((src[i] == '/') || (src[i] == '\\'))
		{
			ptr = src + i;
		}
	}

	if((ptr) && (ptr > src))
	{
        len = ptr - src;
		memcpy(dest, src , ptr - src);
		dest[len] = 0;
	}
	else
	{
		if(dest_len > 0)
		{
			dest[0] = 0;
		}
	}	
}

error_code_t generator_open(generator_t *self, const char *original_file, const char *suffix)
{
	uint32_t i, document_name_length;
	char file_name[TDR_MAX_PATH_LENGTH];
	char errormsg_filename[TDR_MAX_PATH_LENGTH];

	strncpy_notdir(file_name, original_file, TDR_MAX_PATH_LENGTH);
	
	generator_replace_extension(file_name, TDR_MAX_PATH_LENGTH, suffix);

	//计算输出目标文件的路径
	if(g_output_dir)
	{
		snprintf(self->target_filename, TDR_MAX_PATH_LENGTH, "%s%c%s", g_output_dir, TDR_FILE_SEPARATOR, file_name);
	}
	else
	{
		char opath[TDR_MAX_PATH_LENGTH];
		strncpy_dir(opath, original_file, TDR_MAX_PATH_LENGTH);
		if(opath[0])
		{
			snprintf(self->target_filename, TDR_MAX_PATH_LENGTH, "%s%c%s", opath, TDR_FILE_SEPARATOR, file_name);
		}
		else
		{
			snprintf(self->target_filename, TDR_MAX_PATH_LENGTH, "%s", file_name);
		}		
	}
	
	

	//计算文档名字
	strncpy(self->document_name, file_name, TDR_MAX_PATH_LENGTH);
	self->document_name[TDR_MAX_PATH_LENGTH - 1] = 0;
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



	self->fout = fopen(self->target_filename, "w");
	if(self->fout == NULL)
	{
		memcpy(errormsg_filename, self->target_filename, TDR_MAX_PATH_LENGTH);
		goto ERROR_RET;
	}

	return E_TD_NOERROR;
ERROR_RET:
	scanner_error_halt(NULL, E_LS_CANNOT_OPEN_FILE, errormsg_filename);
	return E_TD_ERROR;
}

void generator_print(generator_t *self, size_t tabs, const char* fmt, ...)
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

void generator_printline(generator_t *self, size_t tabs, const char* fmt, ...)
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

void generator_close(generator_t *self)
{
	fclose(self->fout);
}

static void write_char(generator_t *self, char c)
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

error_code_t generator_print_value(generator_t *self, const syn_value_t *val)
{
	switch (val->type)
	{
	case E_SNVT_REFER:
		fprintf(self->fout, "%s", val->val.refer);
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

error_code_t generator_print_type_name(generator_t *self, const syn_simple_type_t *simple_type)
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
	case E_ST_BOOL:
		fprintf(self->fout, "bool");
		return E_TD_NOERROR;
	case E_ST_DOUBLE:
		fprintf(self->fout, "double");
		return E_TD_NOERROR;
	case E_ST_REFER:
		fprintf(self->fout, "%s", simple_type->st_refer);
		return E_TD_NOERROR;
	default:
		return E_TD_ERROR;
	}
}

error_code_t generator_print_ctype(generator_t *self, const syn_simple_type_t *simple_type)
{
	switch(simple_type->st)
	{
	case E_ST_BOOL:
		fprintf(self->fout, "bool");
		return E_TD_NOERROR;
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
		{			
			const symbol_t *symbol = symbols_search(self->symbols, "", simple_type->st_refer);

			switch(symbol->type)
			{
			case EN_HST_TYPEDEF:
				fprintf(self->fout, "%s", simple_type->st_refer);
				break;
			case EN_HST_ENUM:
				fprintf(self->fout, "enum %s", simple_type->st_refer);
				break;
			case EN_HST_STRUCT:
				fprintf(self->fout, "struct %s", simple_type->st_refer);
				break;
			case EN_HST_UNION:
				fprintf(self->fout, "union %s", simple_type->st_refer);
				break;
			default:
				break;
			}

			return E_TD_NOERROR;
		}
	default:
		return E_TD_ERROR;
	}
}

error_code_t generator_on_definition(generator_t *self, const YYLTYPE *yylloc, const syn_definition_t *definition)
{
	if(self->on_definition != NULL)
	{
		return self->on_definition(self, yylloc, definition);
	}
	return E_TD_NOERROR;
}

error_code_t generator_on_document_begin(generator_t *self, const YYLTYPE *yylloc, const char *file_name)
{
	if(self->on_document_begin != NULL)
	{
		return self->on_document_begin(self, yylloc, file_name);
	}
	return E_TD_NOERROR;
}

error_code_t generator_on_document_end(generator_t *self, const YYLTYPE *yylloc, const char *file_name)
{
	if(self->on_document_end != NULL)
	{
		return self->on_document_end(self, yylloc, file_name);
	}

	return E_TD_NOERROR;
}
