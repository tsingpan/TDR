#include "encoding/ddekit_xml_encoding_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"

#include <string.h>
#include <assert.h>

hpint32 ddekit_xml_encoding_reader_init(DDEKIT_XML_ENCODING_READER *self, FILE *f)
{
	self->dpr.read_struct_begin = ddekit_xml_encoding_read_struct_begin;
	self->dpr.read_struct_end = ddekit_xml_encoding_read_struct_end;
	self->dpr.read_repeat_begin = ddekit_xml_encoding_read_vector_begin;
	self->dpr.read_repeat_end = ddekit_xml_encoding_read_vector_end;
	self->dpr.read_field_begin = ddekit_xml_encoding_read_field_begin;
	self->dpr.read_field_end = ddekit_xml_encoding_read_field_end;
	self->dpr.read_enum = ddekit_xml_encoding_read_enum;
	self->dpr.read_hpchar = ddekit_xml_encoding_read_hpchar;
	self->dpr.read_hpdouble = ddekit_xml_encoding_read_hpdouble;
	self->dpr.read_hpint8 = ddekit_xml_encoding_read_hpint8;
	self->dpr.read_hpint16 = ddekit_xml_encoding_read_hpint16;
	self->dpr.read_hpint32 = ddekit_xml_encoding_read_hpint32;
	self->dpr.read_hpint64 = ddekit_xml_encoding_read_hpint64;
	self->dpr.read_hpuint8 = ddekit_xml_encoding_read_hpuint8;
	self->dpr.read_hpuint16 = ddekit_xml_encoding_read_hpuint16;
	self->dpr.read_hpuint32 = ddekit_xml_encoding_read_hpuint32;
	self->dpr.read_hpuint64 = ddekit_xml_encoding_read_hpuint64;


	self->f = f;

	self->union_level = 0;
	self->struct_level = 0;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;	

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_reader_fini(DDEKIT_XML_ENCODING_READER *self)
{
	self->dpr.read_struct_begin = NULL;
	self->dpr.read_struct_end = NULL;
	self->dpr.read_repeat_begin = NULL;
	self->dpr.read_repeat_end = NULL;
	self->dpr.read_field_begin = NULL;
	self->dpr.read_field_end = NULL;
	self->dpr.read_enum = NULL;
	self->dpr.read_hpchar = NULL;
	self->dpr.read_hpdouble = NULL;
	self->dpr.read_hpint8 = NULL;
	self->dpr.read_hpint16 = NULL;
	self->dpr.read_hpint32 = NULL;
	self->dpr.read_hpint64 = NULL;
	self->dpr.read_hpuint8 = NULL;
	self->dpr.read_hpuint16 = NULL;
	self->dpr.read_hpuint32 = NULL;
	self->dpr.read_hpuint64 = NULL;


	self->f = NULL;

	self->union_level = 0;
	self->struct_level = 0;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;

	return E_HP_NOERROR;
}
#define DDEKIT_SKIP_NCHAR(fout, n) \
	do\
{\
	hpuint32 i;\
	for(i = 0; i < n; ++i)\
{\
	fgetc(self->f);\
}\
}while(0)

#define DDEKIT_SKIP_TAB(fout) \
do\
{\
	hpuint32 i;\
	for(i = 0; i < self->union_level + self->struct_level; ++i)\
	{\
		fgetc(self->f);\
	}\
}while(0)



hpint32 ddekit_xml_encoding_read_struct_begin(DDEKIT_ENCODING_READER *super, const char *struct_name)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	//根节点
	if(self->struct_level == 0)
	{
		DDEKIT_SKIP_NCHAR(self->f, strlen(struct_name) + 3);
	}
	++self->struct_level;
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_read_struct_end(DDEKIT_ENCODING_READER *super, const char *struct_name)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	--self->struct_level;
	//根节点
	if(self->struct_level == 0)
	{
		DDEKIT_SKIP_NCHAR(self->f, strlen(struct_name) + 4);
	}

	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_vector_begin(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	self->struct_stack[self->struct_level].in_repeat = TRUE;
	self->struct_stack[self->struct_level].end_with_zero = end_with_zero;

	//对于字符串的数组进行特殊处理
	if(var_type == 0100)
	{
		DDEKIT_SKIP_TAB(self->f);
		DDEKIT_SKIP_NCHAR(self->f, strlen(var_name) + 2);
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_read_vector_end(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	//对于字符串的数组进行特殊处理
	if(var_type == 0100)
	{
		DDEKIT_SKIP_NCHAR(self->f, strlen(var_name) + 4);
	}
	self->struct_stack[self->struct_level].in_repeat = FALSE;
	self->struct_stack[self->struct_level].end_with_zero = FALSE;

	return E_HP_NOERROR;

}

hpint32 ddekit_xml_encoding_read_field_begin(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	//对于字符串数组的特殊处理
	if((!self->struct_stack[self->struct_level].in_repeat) || (var_type != 0100))
	{
		DDEKIT_SKIP_TAB(self->f);
		DDEKIT_SKIP_NCHAR(self->f, strlen(var_name) + 2);
		//对于结构体的结尾行开头需要空格
		if((var_type == 01) || (var_type == 02))
		{
			DDEKIT_SKIP_NCHAR(self->f, 1);
		}
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_read_field_end(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	//对于字符串数组的特殊处理
	if((!self->struct_stack[self->struct_level].in_repeat) || (var_type != 0100))
	{
		//对于结构体的结尾行开头需要空格
		if((var_type == 01) || (var_type == 02))
		{
			DDEKIT_SKIP_TAB(self->f);
		}
		DDEKIT_SKIP_NCHAR(self->f, strlen(var_name) + 4);
	}

	return E_HP_NOERROR;
}

hpint32 ddekit_xml_encoding_read_enum(DDEKIT_ENCODING_READER *super, const hpint32 *val, hpchar *enum_name, hpuint32 *enum_name_length)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	hpuint32 len = *enum_name_length;
	*enum_name_length = 0;

	while(1)
	{
		char c = (char)fgetc(self->f);
		if(c == '\<')
		{
			if(*enum_name_length < len)
			{
				enum_name[*enum_name_length] = 0;
			}
			ungetc('<', self->f);
			break;
		}
		else
		{
			if(*enum_name_length < len)
			{
				enum_name[*enum_name_length] = c;
				++(*enum_name_length);
			}
			else
			{
				goto ERROR_RET;
			}
		}
	}

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 ddekit_xml_encoding_read_hpchar(DDEKIT_ENCODING_READER *super, char *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	//实体引用
	char c = fgetc(self->f);

	
	//对字符串的特殊处理
	if(c == '<')
	{
		//回退一个字符
		*val = 0;
		ungetc('<', self->f);
	}
	//实体引用
	else if(c == '&')
	{
		char c2 = fgetc(self->f);
		if(c2 == 'l')
		{
			//&lt
			DDEKIT_SKIP_NCHAR(self->f, 1);
			*val = '<';
		}
		else if(c2 == 'g')
		{
			//&gt
			DDEKIT_SKIP_NCHAR(self->f, 1);
			*val = '>';
		}
		else
		{
			char c3 = fgetc(self->f);
			if(c3 == 'm')
			{
				//&amp
				DDEKIT_SKIP_NCHAR(self->f, 1);
				*val = '&';
			}
			else if(c3 == 'p')
			{
				//&apos
				DDEKIT_SKIP_NCHAR(self->f, 2);
				*val = '\'';
			}
			else if(c3 == 'u')
			{
				//&auot
				DDEKIT_SKIP_NCHAR(self->f, 2);
				*val = '\"';
			}
		}
	}
	else
	{
		*val = c;
	}	

	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpdouble(DDEKIT_ENCODING_READER *super, double *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	fscanf(self->f, "%lf", val);
	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpint8(DDEKIT_ENCODING_READER *super, hpint8 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	
	fscanf(self->f, "%hhd", val);
	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpint16(DDEKIT_ENCODING_READER *super, hpint16 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	fscanf(self->f, "%hd", val);
	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpint32(DDEKIT_ENCODING_READER *super, hpint32 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	
	fscanf(self->f, "%d", val);
	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpint64(DDEKIT_ENCODING_READER *super, hpint64 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	fscanf(self->f, "%"PRIi64, val);
	
	return E_HP_NOERROR;
}



hpint32 ddekit_xml_encoding_read_hpuint8(DDEKIT_ENCODING_READER *super, hpuint8 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);

	fscanf(self->f, "%hhu", val);
	
	return E_HP_NOERROR;
}



hpint32 ddekit_xml_encoding_read_hpuint16(DDEKIT_ENCODING_READER *super, hpuint16 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	
	fscanf(self->f, "%hu", val);

	return E_HP_NOERROR;
}


hpint32 ddekit_xml_encoding_read_hpuint32(DDEKIT_ENCODING_READER *super, hpuint32 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	
	fscanf(self->f, "%u", val);

	return E_HP_NOERROR;
}



hpint32 ddekit_xml_encoding_read_hpuint64(DDEKIT_ENCODING_READER *super, hpuint64 *val)
{
	DDEKIT_XML_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_XML_ENCODING_READER, dpr);
	
	fscanf(self->f, "%"PRIu64, val);
	return E_HP_NOERROR;
}
