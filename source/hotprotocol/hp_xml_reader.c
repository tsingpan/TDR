#include "hotprotocol/hp_xml_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>


hpint32 xml_reader_init(HP_XML_READER *self, FILE *f)
{
	self->f = f;
	self->count = 0;
	self->need_tab = hpfalse;

	self->super.read_vector_begin = xml_read_vector_begin;
	self->super.read_vector_end = xml_read_vector_end;
	self->super.read_field_begin = xml_read_field_begin;
	self->super.read_field_end = xml_read_field_end;

	self->super.read_hpint64 = xml_read_hpint64;

	self->super.read_hpdouble = xml_read_hpdouble;
	self->super.read_bytes = xml_read_bytes;
	self->super.read_hpbool = xml_read_hpbool;
	self->super.read_vector_item_begin = xml_read_vector_item_begin;
	self->super.read_vector_item_end= xml_read_vector_item_end;



	return E_HP_NOERROR;
}

hpint32 xml_reader_fini(HP_XML_READER *self)
{
	self->super.read_struct_begin = NULL;
	self->super.read_struct_end = NULL;
	self->super.read_field_begin = NULL;
	self->super.read_field_end = NULL;
	self->super.read_vector_begin = NULL;
	self->super.read_vector_end = NULL;
	self->super.read_enum = NULL;
	self->super.read_hpchar = NULL;
	self->super.read_hpdouble = NULL;
	self->super.read_hpint8 = NULL;
	self->super.read_hpint16 = NULL;
	self->super.read_hpint32 = NULL;
	self->super.read_hpint64 = NULL;
	self->super.read_hpuint8 = NULL;
	self->super.read_hpuint16 = NULL;
	self->super.read_hpuint32 = NULL;
	self->super.read_hpuint64 = NULL;


	return E_HP_NOERROR;
}

static void skip_tab(HP_XML_READER *self)
{
	hpuint32 i;
	for(;;)
	{
		char c = fgetc(self->f);
		if(c != '\t')
		{
			ungetc('a', self->f);
			break;
		}
	}
}

hpint32 xml_read_vector_begin(HPAbstractReader *super)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	++self->count;
	return E_HP_NOERROR;
}

hpint32 xml_read_vector_end(HPAbstractReader *super)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	--self->count;

	return E_HP_NOERROR;
}

hpint32 xml_read_field_begin(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	hpuint32 i;
	if(self->count != 0)
	{
		fgetc(self->f);
	}

	skip_tab(self);
	++(self->count);	
	fgetc(self->f);
	for(i = 0;i < len; ++i)
	{
		fgetc(self->f);
	}
	fgetc(self->f);

	return E_HP_NOERROR;
}

hpint32 xml_read_field_end(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	hpuint32 i;
	--(self->count);
	if(self->need_tab)
	{
		fgetc(self->f);
		skip_tab(self);
	}
	self->need_tab = hptrue;
	fgetc(self->f);
	fgetc(self->f);
	for(i = 0;i < len; ++i)
	{
		fgetc(self->f);
	}
	fgetc(self->f);
	return E_HP_NOERROR;
}

HP_API hpint32 xml_read_vector_item_begin(HPAbstractReader *super, hpuint32 index)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_begin(super, str, strlen(str));
}

HP_API hpint32 xml_read_vector_item_end(HPAbstractReader *super, hpuint32 index)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_end(super, str, strlen(str));
}

HP_API hpint32 xml_read_enum_name(HPAbstractReader *super, const hpchar *enum_name)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	return E_HP_NOERROR;
}


hpint32 xml_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	fscanf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}


hpint32 xml_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	fscanf(self->f, "%lld", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}


hpint32 xml_read_bytes(HPAbstractReader *super, hpbytes *bytes)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	hpuint32 i;
	self->need_tab = hpfalse;/*
	for(i = 0;i < bytes.len; ++i)
		switch (bytes.ptr[i])
	{
		case '<':
			fprintf(self->f, "&lt");
			continue;
		case '>':
			fprintf(self->f, "&gt");
			continue;
		case '&':
			fprintf(self->f, "&amp");
			continue;
		case '\'':
			fprintf(self->f, "&apos");
			continue;
		case '\"':
			fprintf(self->f, "&quot");
			continue;						
		default:
			{
				fputc(bytes.ptr[i], self->f);
			}
	}
	*/
	return E_HP_NOERROR;
}

hpint32 xml_read_string(HPAbstractReader *super, hpchar* str)
{
	hpbytes bytes;
	bytes.ptr = str;
	bytes.len = strlen(str);
	return xml_read_bytes(super, &bytes);
}

hpint32 xml_read_hpbool(HPAbstractReader *super, hpbool *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	if(*val == hptrue)
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

