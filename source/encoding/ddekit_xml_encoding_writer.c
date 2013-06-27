#include "encoding/ddekit_xml_encoding_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"

#include <string.h>
#include <assert.h>

hpint32 ddekit_xml_encoding_writer_init(DDEKIT_XML_ENCODING_WRITER *self, FILE *f)
{
	self->dpw.write_struct_begin = ddekit_xml_encoding_write_struct_begin;
	self->dpw.write_struct_end = ddekit_xml_encoding_write_struct_end;
	self->dpw.write_union_begin = ddekit_xml_encoding_write_union_begin;
	self->dpw.write_union_end = ddekit_xml_encoding_write_union_end;
	self->dpw.write_repeat_begin = ddekit_xml_encoding_write_repeat_begin;
	self->dpw.write_repeat_end = ddekit_xml_encoding_write_repeat_end;
	self->dpw.write_var_begin = ddekit_xml_encoding_write_var_begin;
	self->dpw.write_var_end = ddekit_xml_encoding_write_var_end;
	self->dpw.write_enum = ddekit_xml_encoding_write_enum;
	self->dpw.write_hpchar = ddekit_xml_encoding_write_hpchar;
	self->dpw.write_hpdouble = ddekit_xml_encoding_write_hpdouble;
	self->dpw.write_hpint8 = ddekit_xml_encoding_write_hpint8;
	self->dpw.write_hpint16 = ddekit_xml_encoding_write_hpint16;
	self->dpw.write_hpint32 = ddekit_xml_encoding_write_hpint32;
	self->dpw.write_hpint64 = ddekit_xml_encoding_write_hpint64;
	self->dpw.write_hpuint8 = ddekit_xml_encoding_write_hpuint8;
	self->dpw.write_hpuint16 = ddekit_xml_encoding_write_hpuint16;
	self->dpw.write_hpuint32 = ddekit_xml_encoding_write_hpuint32;
	self->dpw.write_hpuint64 = ddekit_xml_encoding_write_hpuint64;
	

	self->f = f;
	
	self->union_level = 0;
	self->struct_level = 0;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;	
	

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_writer_fini(DDEKIT_XML_ENCODING_WRITER *self)
{
	self->dpw.write_struct_begin = NULL;
	self->dpw.write_struct_end = NULL;
	self->dpw.write_union_begin = NULL;
	self->dpw.write_union_end = NULL;
	self->dpw.write_repeat_begin = NULL;
	self->dpw.write_repeat_end = NULL;
	self->dpw.write_var_begin = NULL;
	self->dpw.write_var_end = NULL;
	self->dpw.write_enum = NULL;
	self->dpw.write_hpchar = NULL;
	self->dpw.write_hpdouble = NULL;
	self->dpw.write_hpint8 = NULL;
	self->dpw.write_hpint16 = NULL;
	self->dpw.write_hpint32 = NULL;
	self->dpw.write_hpint64 = NULL;
	self->dpw.write_hpuint8 = NULL;
	self->dpw.write_hpuint16 = NULL;
	self->dpw.write_hpuint32 = NULL;
	self->dpw.write_hpuint64 = NULL;

	self->f = NULL;

	self->union_level = 0;
	self->struct_level = 0;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;

	return E_HP_NOERROR;
}

#define DDEKIT_PRINT_TAB(fout) \
do\
{\
	hpuint32 i;\
	for(i = 0; i < self->union_level + self->struct_level; ++i)\
	{\
		fprintf(self->f, "\t");\
	}\
}while(0)

hpint32 ddekit_xml_encoding_write_struct_begin(DDEKIT_ENCODING_WRITER *super, const char *struct_name)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	//根节点
	if(self->struct_level == 0)
	{
		fprintf(self->f, "<%s>\n", struct_name);
	}
	++self->struct_level;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_struct_end(DDEKIT_ENCODING_WRITER *super, const char *struct_name)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	--self->struct_level;
	//根节点
	if(self->struct_level == 0)
	{
		fprintf(self->f, "</%s>\n", struct_name);	
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_union_begin(DDEKIT_ENCODING_WRITER *super, const char *union_name)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	++self->union_level;

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_union_end(DDEKIT_ENCODING_WRITER *super, const char *union_name)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	--self->union_level;
	return E_HP_NOERROR;
}


HP_API hpint32 ddekit_xml_encoding_write_repeat_begin(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	self->struct_stack[self->struct_level].in_repeat = TRUE;
	self->struct_stack[self->struct_level].end_with_zero = end_with_zero;

	//对于字符串的数组进行特殊处理
	if(var_type == 0100)
	{
		DDEKIT_PRINT_TAB(self->f);
		fprintf(self->f, "<%s>", var_name);
	}

	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_xml_encoding_write_repeat_end(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	//对于字符串的数组进行特殊处理
	if(var_type == 0100)
	{
		fprintf(self->f, "</%s>\n", var_name);
	}
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;
	return E_HP_NOERROR;
}



hpint32 ddekit_xml_encoding_write_var_begin(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	
	//对于字符串数组的特殊处理
	if((!self->struct_stack[self->struct_level].in_repeat) || (var_type != 0100))
	{
		DDEKIT_PRINT_TAB(self->f);		
		fprintf(self->f, "<%s>", var_name);
		//对于结构体的结尾行开头需要空格
		if((var_type == 01) || (var_type == 02))
		{
			fprintf(self->f, "\n");
		}
	}
	
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_var_end(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);

	//对于字符串数组的特殊处理
	if((!self->struct_stack[self->struct_level].in_repeat) || (var_type != 0100))
	{
		//对于结构体的结尾行开头需要空格
		if((var_type == 01) || (var_type == 02))
		{
			DDEKIT_PRINT_TAB(self->f);
		}
		fprintf(self->f, "</%s>\n", var_name);
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_enum(DDEKIT_ENCODING_WRITER *super, const hpint32 val, const hpchar *enum_name)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%s", enum_name);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpchar(DDEKIT_ENCODING_WRITER *super, const char val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	//实体引用
	switch(val)
	{
	case '\0':
		//对于blob数据的处理
		if(self->struct_stack[self->struct_level].end_with_zero == FALSE)
		{
			fprintf(self->f, "%c", '\0');
		}
		break;
	case '<':
		fprintf(self->f, "&lt");
		break;
	case '>':
		fprintf(self->f, "&gt");
		break;
	case '&':
		fprintf(self->f, "&amp");
		break;
	case '\'':
		fprintf(self->f, "&apos");
		break;
	case '\"':
		fprintf(self->f, "&auot");
		break;
	default:
		fprintf(self->f, "%c", val);
		break;
	}
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpdouble(DDEKIT_ENCODING_WRITER *super, const double val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%lf", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpint8(DDEKIT_ENCODING_WRITER *super, const hpint8 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	
	fprintf(self->f, "%d", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpint16(DDEKIT_ENCODING_WRITER *super, const hpint16 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%d", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpint32(DDEKIT_ENCODING_WRITER *super, const hpint32 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%d", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpint64(DDEKIT_ENCODING_WRITER *super, const hpint64 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%"PRIi64, val);
	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_write_hpuint8(DDEKIT_ENCODING_WRITER *super, const hpuint8 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%u", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpuint16(DDEKIT_ENCODING_WRITER *super, const hpuint16 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%u", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpuint32(DDEKIT_ENCODING_WRITER *super, const hpuint32 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%u", val);
	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_write_hpuint64(DDEKIT_ENCODING_WRITER *super, const hpuint64 val)
{
	DDEKIT_XML_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_WRITER, dpw);
	fprintf(self->f, "%"PRIu64, val);
	return E_HP_NOERROR;
}
