#include "hotprotocol/hp_xml_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>


hpint32 xml_reader_init(HP_XML_READER *self, FILE *f)
{
	memset(&self->super, 0, sizeof(HPAbstractReader));

	self->f = f;
	self->count = 0;
	self->need_tab = hpfalse;
	

	self->super.read_enum_name = xml_read_enum_name;

	self->super.read_struct_begin = xml_read_struct_begin;
	self->super.read_struct_end = xml_read_struct_end;

	self->super.read_vector_begin = xml_read_vector_begin;
	self->super.read_vector_end = xml_read_vector_end;
	self->super.read_field_begin = xml_read_field_begin;
	self->super.read_field_end = xml_read_field_end;
	self->super.read_vector_item_begin = xml_read_vector_item_begin;
	self->super.read_vector_item_end= xml_read_vector_item_end;

	self->super.read_hpint8 = xml_read_hpint8;
	self->super.read_hpint16 = xml_read_hpint16;
	self->super.read_hpint32 = xml_read_hpint32;
	self->super.read_hpint64 = xml_read_hpint64;

	self->super.read_hpuint8 = xml_read_hpuint8;
	self->super.read_hpuint16 = xml_read_hpuint16;
	self->super.read_hpuint32 = xml_read_hpuint32;
	self->super.read_hpuint64 = xml_read_hpuint64;

	self->super.read_hpdouble = xml_read_hpdouble;
	self->super.read_hpbool = xml_read_hpbool;	
	self->super.read_string = xml_read_string;
	self->super.read_hpchar = xml_read_hpchar;
	self->super.read_counter = xml_read_counter;
	


	return E_HP_NOERROR;
}

hpint32 xml_reader_fini(HP_XML_READER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractReader, stack));

	return E_HP_NOERROR;
}

static void skip_tab(HP_XML_READER *self)
{
	hpuint32 i;
	for(;;)
	{
		char c = fgetc(self->f);
		if((c != '\t') && (c != '\n') && (c != '\r') && (c != ' '))
		{
			ungetc('a', self->f);
			break;
		}
	}
}
hpint32 xml_read_enum_name(HPAbstractReader *self, hpchar *enum_name, hpuint32 enum_name_length)
{
	xml_read_string(self, enum_name, enum_name_length);
	return E_HP_NOERROR;
}

hpint32 xml_read_struct_begin(HPAbstractReader *super, const char *struct_name)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	hpuint32 i;
	hpuint32 len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fgetc(self->f);
		for(i = 0;i < len; ++i)
		{
			fgetc(self->f);
		}
		fgetc(self->f);
	}
	return E_HP_NOERROR;
}

hpint32 xml_read_struct_end(HPAbstractReader *super, const char *struct_name)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	hpuint32 i;
	hpuint32 len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fgetc(self->f);
		for(i = 0;i < len; ++i)
		{
			fgetc(self->f);
		}
		fgetc(self->f);
	}
	return E_HP_NOERROR;
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

hpint32 xml_read_field_begin(HPAbstractReader *super, const char *var_name)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	const char *i;

	fgetc(self->f);
	++(self->count);
	skip_tab(self);	
	fgetc(self->f);
	for(i = var_name; *i; ++i)
	{
		fgetc(self->f);
	}
	fgetc(self->f);

	return E_HP_NOERROR;
}

hpint32 xml_read_field_end(HPAbstractReader *super, const char *var_name)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);	
	const char *i;
	
	if(self->need_tab)
	{
		fgetc(self->f);
		skip_tab(self);
	}
	--(self->count);
	self->need_tab = hptrue;
	fgetc(self->f);
	fgetc(self->f);
	for(i = var_name; *i; ++i)
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

hpint32 xml_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	fscanf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}

hpint32 xml_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	hpint64 i64;
	hpint32 ret = xml_read_hpint64(super, &i64);
	*val = i64;
	return ret;
}

hpint32 xml_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	hpint64 i64;
	hpint32 ret = xml_read_hpint64(super, &i64);
	*val = i64;
	return ret;
}

hpint32 xml_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	hpint64 i64;
	hpint32 ret = xml_read_hpint64(super, &i64);
	*val = i64;
	return ret;
}

hpint32 xml_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	fscanf(self->f, "%lld", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
}

hpint32 xml_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	hpuint64 ui64;
	hpint32 ret = xml_read_hpuint64(super, &ui64);
	*val = ui64;
	return ret;
}

hpint32 xml_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	hpuint64 ui64;
	hpint32 ret = xml_read_hpuint64(super, &ui64);
	*val = ui64;
	return ret;
}

hpint32 xml_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	hpuint64 ui64;
	hpint32 ret = xml_read_hpuint64(super, &ui64);
	*val = ui64;
	return ret;
}

hpint32 xml_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	fscanf(self->f, "%llu", val);
	self->need_tab = hpfalse;
	return E_HP_NOERROR;
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

static void read_char(FILE* fin, hpchar *ch)
{
	char c = fgetc(fin);
	
	if(c == '&')
	{
		char c2 = fgetc(fin);
		if(c2 == 'l')
		{
			//&lt
			fgetc(fin);
			*ch = '<';
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
		}
		else
		{
			char c3 = fgetc(fin);
			if(c3 == 'm')
			{
				//&amp
				*ch = '&';
			}
			else if(c3 == 'p')
			{
				//&apos
				*ch = '\'';
			}
			else if(c3 == 'u')
			{
				//&auot
				*ch = '\"';
			}
		}
	}
	else
	{
		*ch = c;
	}
}

hpint32 xml_read_hpchar(HPAbstractReader *super, char *val)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	read_char(self->f, val);
}

hpint32 xml_read_string(HPAbstractReader *super, hpchar *str, hpuint32 str_len)
{
	HP_XML_READER *self = HP_CONTAINER_OF(super, HP_XML_READER, super);
	hpuint32 i;
	hpuint32 len = 0;

	self->need_tab = hpfalse;
	for(;;)
	{
		char c;
		read_char(self->f, &c);
		if(c == '<')
		{
			ungetc('<', self->f);
			str[len++] = 0;
		}
		else
		{
			str[len++] = c;
		}
	}

	return E_HP_NOERROR;
}

HP_API hpint32 xml_read_counter(HPAbstractReader *super, const hpchar *name, hpuint32 *val)
{
	xml_read_field_begin(super, name);
	xml_read_hpuint32(super, val);
	xml_read_field_end(super, name);

	return E_HP_NOERROR;
}
