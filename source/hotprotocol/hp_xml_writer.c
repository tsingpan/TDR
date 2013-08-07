#include "hotprotocol/hp_xml_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

hpint32 xml_writer_init(HP_XML_WRITER *self, FILE *f)
{
	memset(&self->super, 0, sizeof(HPAbstractWriter));

	self->f = f;
	self->count = 0;
	self->need_tab = hpfalse;

	self->super.write_struct_begin = xml_write_struct_begin;
	self->super.write_struct_end = xml_write_struct_end;

	self->super.write_vector_begin = xml_write_vector_begin;
	self->super.write_vector_end = xml_write_vector_end;
	self->super.write_field_begin = xml_write_field_begin;
	self->super.write_field_end = xml_write_field_end;
	
	self->super.write_hpint8 = xml_write_hpint8;
	self->super.write_hpint16 = xml_write_hpint16;
	self->super.write_hpint32 = xml_write_hpint32;
	self->super.write_hpint64 = xml_write_hpint64;

	self->super.write_hpuint8 = xml_write_hpuint8;
	self->super.write_hpuint16 = xml_write_hpuint16;
	self->super.write_hpuint32 = xml_write_hpuint32;
	self->super.write_hpuint64 = xml_write_hpuint64;


	self->super.write_hpdouble = xml_write_hpdouble;
	self->super.write_string = xml_write_string;
	self->super.write_hpbool = xml_write_hpbool;
	self->super.write_vector_item_begin = xml_write_vector_item_begin;
	self->super.write_vector_item_end= xml_write_vector_item_end;
	self->super.write_hpchar = xml_write_hpchar;

	self->super.write_enum_name = xml_write_enum_name;
	self->super.write_counter = xml_write_counter;

	

	return E_HP_NOERROR;
}

hpint32 xml_writer_fini(HP_XML_WRITER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractWriter, stack));

	return E_HP_NOERROR;
}

static void printf_tab(HP_XML_WRITER *self)
{
	hpuint32 i;
	for(i = 0;i < self->count; ++i)
	{
		fputc('\t', self->f);
	}
}

hpint32 xml_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	hpuint32 i;
	hpuint32 len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fputc('<', self->f);
		for(i = 0;i < len; ++i)
		{
			fputc(struct_name[i], self->f);
		}
		fputc('>', self->f);
	}
	return E_HP_NOERROR;
}

hpint32 xml_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	hpuint32 i;
	hpuint32 len = 0;
	if(self->count == 0)
	{
		len = strlen(struct_name);

		fputc('\n', self->f);
		fputc('<', self->f);
		fputc('/', self->f);
		for(i = 0;i < len; ++i)
		{
			fputc(struct_name[i], self->f);
		}
		fputc('>', self->f);
	}
	return E_HP_NOERROR;
}

hpint32 xml_write_vector_begin(HPAbstractWriter *super)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	++self->count;
	return E_HP_NOERROR;
}

hpint32 xml_write_vector_end(HPAbstractWriter *super)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	--self->count;
	
	return E_HP_NOERROR;
}

hpint32 xml_write_field_begin(HPAbstractWriter *super, const char *var_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	const char *i;

	fputc('\n', self->f);	
	++(self->count);
	printf_tab(self);
	
	fputc('<', self->f);
	for(i = var_name;*i; ++i)
	{
		fputc(*i, self->f);
	}
	fputc('>', self->f);	
	return E_HP_NOERROR;
}

hpint32 xml_write_field_end(HPAbstractWriter *super, const char *var_name)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);	
	const char *i;

	if(self->need_tab)
	{
		fputc('\n', self->f);
		printf_tab(self);
	}
	--(self->count);
	self->need_tab = hptrue;
	fputc('<', self->f);
	fputc('/', self->f);
	for(i = var_name; *i; ++i)
	{
		fputc(*i, self->f);
	}
	fputc('>', self->f);
	//fputc('\n', self->f);
	return E_HP_NOERROR;
}

HP_API hpint32 xml_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_write_field_begin(super, str);
}

HP_API hpint32 xml_write_vector_item_end(HPAbstractWriter *super, hpuint32 index)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_write_field_end(super, str);
}

hpint32 xml_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	fprintf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}

hpint32 xml_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	return xml_write_hpint64(super, val);
}

hpint32 xml_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	return xml_write_hpint64(super, val);
}

hpint32 xml_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	return xml_write_hpint64(super, val);
}

hpint32 xml_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	fprintf(self->f, "%lld", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}


hpint32 xml_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	return xml_write_hpuint64(super, val);
}

hpint32 xml_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	return xml_write_hpuint64(super, val);
}

hpint32 xml_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	return xml_write_hpuint64(super, val);
}

hpint32 xml_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	fprintf(self->f, "%llu", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}

static void write_char(FILE* fout, hpchar c)
{
	switch (c)
	{
	case '<':
		fprintf(fout, "&lt");
		break;
	case '>':
		fprintf(fout, "&gt");
		break;
	case '&':
		fprintf(fout, "&amp");
		break;
	case '\'':
		fprintf(fout, "&apos");
		break;
	case '\"':
		fprintf(fout, "&quot");
		break;
	default:
		{
			fputc(c, fout);
		}
	}
}

hpint32 xml_write_string(HPAbstractWriter *super, const hpchar* str)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);
	const hpchar *i;
	self->need_tab = hpfalse;
	for(i = str; *i ; ++i)
	{
		write_char(self->f, *i);
	}

	return E_HP_NOERROR;
}
hpint32 xml_write_enum_name(HPAbstractWriter *self, const hpchar *enum_name)
{
	return xml_write_string(self, enum_name);
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

hpint32 xml_write_hpchar(HPAbstractWriter *super, const hpchar val)
{
	HP_XML_WRITER *self = HP_CONTAINER_OF(super, HP_XML_WRITER, super);

	write_char(self->f, val);
	return E_HP_NOERROR;
}

hpint32 xml_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val)
{
	xml_write_field_begin(super, name);
	xml_write_hpuint32(super, val);
	xml_write_field_end(super, name);
	return E_HP_NOERROR;
}
