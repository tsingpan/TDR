#include "hotprotocol/hp_json_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

hpint32 ddekit_json_encoding_writer_init(HP_JSON_WRITER *self, FILE *f)
{
	self->f = f;
	self->count = 0;
	self->count = 0;
	self->need_tab = hpfalse;

	self->super.write_struct_begin = ddekit_json_encoding_write_struct_begin;
	self->super.write_struct_end = ddekit_json_encoding_write_struct_end;
	self->super.write_vector_begin = ddekit_json_encoding_write_vector_begin;
	self->super.write_vector_end = ddekit_json_encoding_write_vector_end;
	self->super.write_field_begin = ddekit_json_encoding_write_field_begin;
	self->super.write_field_end = ddekit_json_encoding_write_field_end;
	
	self->super.write_hpint8 = ddekit_json_encoding_write_hpint8;
	self->super.write_hpint16 = ddekit_json_encoding_write_hpint16;
	self->super.write_hpint32 = ddekit_json_encoding_write_hpint32;
	self->super.write_hpint64 = ddekit_json_encoding_write_hpint64;
	self->super.write_hpuint8 = ddekit_json_encoding_write_hpuint8;
	self->super.write_hpuint16 = ddekit_json_encoding_write_hpuint16;
	self->super.write_hpuint32 = ddekit_json_encoding_write_hpuint32;
	self->super.write_hpuint64 = ddekit_json_encoding_write_hpuint64;

	self->super.write_enum = ddekit_json_encoding_write_enum;
	self->super.write_hpchar = ddekit_json_encoding_write_hpchar;
	self->super.write_hpdouble = ddekit_json_encoding_write_hpdouble;
	self->super.write_bytes = ddekit_json_encoding_write_bytes;
	self->super.write_hpstring = ddekit_json_encoding_write_string;
	self->super.write_hpbool = ddekit_json_encoding_write_hpbool;
	self->super.write_null = ddekit_json_encoding_write_null;
	self->super.write_semicolon = ddekit_json_encoding_write_semicolon;
	self->super.write_vector_item_begin = ddekit_json_encoding_write_vector_item_begin;

	

	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_writer_fini(HP_JSON_WRITER *self)
{
	self->super.write_struct_begin = NULL;
	self->super.write_struct_end = NULL;
	self->super.write_field_begin = NULL;
	self->super.write_field_end = NULL;
	self->super.write_vector_begin = NULL;
	self->super.write_vector_end = NULL;
	self->super.write_enum = NULL;
	self->super.write_hpchar = NULL;
	self->super.write_hpdouble = NULL;
	self->super.write_hpint8 = NULL;
	self->super.write_hpint16 = NULL;
	self->super.write_hpint32 = NULL;
	self->super.write_hpint64 = NULL;
	self->super.write_hpuint8 = NULL;
	self->super.write_hpuint16 = NULL;
	self->super.write_hpuint32 = NULL;
	self->super.write_hpuint64 = NULL;


	return E_HP_NOERROR;
}

static void printf_tab(HP_JSON_WRITER *self)
{
	hpuint32 i;
	for(i = 0;i < self->count + self->count; ++i)
	{
		fputc(' ', self->f);
		fputc(' ', self->f);
		fputc(' ', self->f);
		fputc(' ', self->f);
	}
}

hpint32 ddekit_json_encoding_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);

	
	if(self->count != 0)
	{
		fputc('\n', self->f);
	}
	printf_tab(self);
	fputc('{', self->f);
	fputc('\n', self->f);

	++(self->count);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	--(self->count);
	fputc('\n', self->f);
	printf_tab(self);
	fputc('}', self->f);
	if(self->count == 0)
	{
		fputc('\n', self->f);
	}
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_vector_begin(HPAbstractWriter *super)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);	
	
	
	self->need_tab = hptrue;
	fputc('\n', self->f);	
	printf_tab(self);
	fputc('[', self->f);
	fputc('\n', self->f);
	++(self->count);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_vector_end(HPAbstractWriter *super)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	
	--(self->count);
	fputc('\n', self->f);
	printf_tab(self);
	fputc(']', self->f);
	
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_field_begin(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	hpuint32 i;
	self->need_tab = hpfalse;
	printf_tab(self);
	fputc('\"', self->f);
	for(i = 0;i < len; ++i)
	{
		fputc(var_name[i], self->f);
	}
	fputc('\"', self->f);
	fputc(' ', self->f);
	fputc(':', self->f);
	fputc(' ', self->f);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_field_end(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);	
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_enum(HPAbstractWriter *super, const int val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);	
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	if(self->need_tab)
	{
		printf_tab(self);
	}
	fprintf(self->f, "%lf", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);	
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	if(self->need_tab)
	{
		printf_tab(self);
	}
	fprintf(self->f, "%lld", val);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	if(self->need_tab)
	{
		printf_tab(self);
	}
	fprintf(self->f, "%llu", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_bytes(HPAbstractWriter *super, const hpbytes bytes)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	hpuint32 i;
	if(self->need_tab)
	{
		printf_tab(self);
	}
	fputc('"', self->f);
	for(i = 0;i < bytes.len; ++i)
		switch (bytes.ptr[i])
		{
			case '"':
				fputc('\\', self->f);
				fputc('"', self->f);
				continue;
			case '\\':
				fputc('\\', self->f);
				fputc('\\', self->f);
				continue;
			case '/':
				fputc('\\', self->f);
				fputc('/', self->f);
				continue;
			case '\b':
				fputc('\\', self->f);
				fputc('b', self->f);
				continue;
			case '\f':
				fputc('\\', self->f);
				fputc('f', self->f);
				continue;
			case '\n':
				fputc('\\', self->f);
				fputc('n', self->f);
				continue;
			case '\r':
				fputc('\\', self->f);
				fputc('r', self->f);
				continue;
			case '\t':
				fputc('\\', self->f);
				fputc('t', self->f);
				continue;					
			default:
				{
					fputc(bytes.ptr[i], self->f);
				}
		}
	fputc('"', self->f);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_string(HPAbstractWriter *super, const hpchar* str)
{
	hpbytes bytes;
	bytes.ptr = str;
	bytes.len = strlen(str);
	ddekit_json_encoding_write_bytes(super, bytes);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	if(self->need_tab)
	{
		printf_tab(self);
	}
	if(val == hptrue)
	{
		fprintf(self->f, "true");
	}
	else
	{
		fprintf(self->f, "false");
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_null(HPAbstractWriter *super)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	if(self->need_tab)
	{
		printf_tab(self);
	}
	fprintf(self->f, "null");

	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_semicolon(HPAbstractWriter *super)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	fprintf(self->f, ",");
	fputc('\n', self->f);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index)
{
	HP_JSON_WRITER *self = HP_CONTAINER_OF(super, HP_JSON_WRITER, super);
	return E_HP_NOERROR;
}
