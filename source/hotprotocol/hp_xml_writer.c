#include "hotprotocol/hp_xml_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

hpint32 xml_writer_init(HP_XML_WRITER *self, FILE *f)
{
	self->f = f;
	self->count = 0;
	self->need_tab = hpfalse;

	self->super.write_struct_begin = xml_write_struct_begin;
	self->super.write_struct_end = xml_write_struct_end;
	self->super.write_vector_begin = xml_write_vector_begin;
	self->super.write_vector_end = xml_write_vector_end;
	self->super.write_field_begin = xml_write_field_begin;
	self->super.write_field_end = xml_write_field_end;
	
	self->super.write_hpint64 = xml_write_hpint64;

	self->super.write_hpdouble = xml_write_hpdouble;
	self->super.write_bytes = xml_write_bytes;
	self->super.write_hpstring = xml_write_string;
	self->super.write_hpbool = xml_write_hpbool;
	self->super.write_vector_item_begin = xml_write_vector_item_begin;
	self->super.write_vector_item_end= xml_write_vector_item_end;

	

	return E_HP_NOERROR;
}

hpint32 xml_writer_fini(HP_XML_WRITER *self)
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

static void printf_tab(HP_XML_WRITER *self)
{
	hpuint32 i;
	for(i = 1;i < self->count; ++i)
	{
		fputc('\t', self->f);
	}
}

hpint32 xml_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	

	return E_HP_NOERROR;
}

hpint32 xml_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	

	
	return E_HP_NOERROR;
}

hpint32 xml_write_vector_begin(HPAbstractWriter *super)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	
	
	return E_HP_NOERROR;
}

hpint32 xml_write_vector_end(HPAbstractWriter *super)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	

	
	return E_HP_NOERROR;
}

hpint32 xml_write_field_begin(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	hpuint32 i;
	if(self->count != 0)
	{
		fputc('\n', self->f);
	}
	
	printf_tab(self);
	++(self->count);	
	fputc('<', self->f);
	for(i = 0;i < len; ++i)
	{
		fputc(var_name[i], self->f);
	}
	fputc('>', self->f);	
	return E_HP_NOERROR;
}

hpint32 xml_write_field_end(HPAbstractWriter *super, const char *var_name, hpuint32 len)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	hpuint32 i;
	--(self->count);
	if(self->need_tab)
	{
		fputc('\n', self->f);
		printf_tab(self);
	}
	self->need_tab = hptrue;
	fputc('<', self->f);
	fputc('/', self->f);
	for(i = 0;i < len; ++i)
	{
		fputc(var_name[i], self->f);
	}
	fputc('>', self->f);
	//fputc('\n', self->f);
	return E_HP_NOERROR;
}

HP_API hpint32 xml_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	return E_HP_NOERROR;
}


hpint32 xml_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	fprintf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}


hpint32 xml_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	fprintf(self->f, "%lld", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}


hpint32 xml_write_bytes(HPAbstractWriter *super, const hpbytes bytes)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	hpuint32 i;
	self->need_tab = hpfalse;
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
	return E_HP_NOERROR;
}

hpint32 xml_write_string(HPAbstractWriter *super, const hpchar* str)
{
	hpbytes bytes;
	bytes.ptr = str;
	bytes.len = strlen(str);
	xml_write_bytes(super, bytes);
	return E_HP_NOERROR;
}

hpint32 xml_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	if(val == hptrue)
	{
		fprintf(self->f, "true");
	}
	else
	{
		fprintf(self->f, "false");
	}
	self->need_tab = hpfalse;


	return E_HP_NOERROR;
}


HP_API hpint32 xml_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	//printf_tab(self);
	++(self->count);
	
	return E_HP_NOERROR;
}

HP_API hpint32 xml_write_vector_item_end(HPAbstractWriter *super, hpuint32 index)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	
	--(self->count);
	//printf_tab(self);

	return E_HP_NOERROR;
}
