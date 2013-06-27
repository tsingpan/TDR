#ifndef _H_DDEKIT_XML_ENCODING_WRITER
#define _H_DDEKIT_XML_ENCODING_WRITER

#include "hotpot/hp_platform.h"
#include "encoding/ddekit_encoding_writer.h"

#include <stdio.h>

typedef struct tagDDEKIT_XML_ENCODING_WRITER_STACK_ELEMENT
{
	hpint32 in_repeat;
	hpint32 end_with_zero;
}DDEKIT_XML_ENCODING_WRITER_STACK_ELEMENT;

#ifndef _DEF_DDEKIT_XML_ENCODING_WRITER
#define _DEF_DDEKIT_XML_ENCODING_WRITER
typedef struct tagDDEKIT_XML_ENCODING_WRITER DDEKIT_XML_ENCODING_WRITER;
#endif//_DEF_DDEKIT_XML_ENCODING_WRITER

#define DDEKIT_XML_MAX_DEEP_OF_WRITER_STACK 65536
struct tagDDEKIT_XML_ENCODING_WRITER
{
	DDEKIT_ENCODING_WRITER dpw;

	FILE *f;
	hpuint32 union_level;
	hpuint32 struct_level;
	DDEKIT_XML_ENCODING_WRITER_STACK_ELEMENT struct_stack[DDEKIT_XML_MAX_DEEP_OF_WRITER_STACK];
};

HP_API hpint32 ddekit_xml_encoding_writer_init(DDEKIT_XML_ENCODING_WRITER *self, FILE *f);

HP_API hpint32 ddekit_xml_encoding_writer_fini(DDEKIT_XML_ENCODING_WRITER *self);

//virtual functions
HP_API hpint32 ddekit_xml_encoding_write_struct_begin(DDEKIT_ENCODING_WRITER *super, const char *struct_name);

HP_API hpint32 ddekit_xml_encoding_write_struct_end(DDEKIT_ENCODING_WRITER *super, const char *struct_name);

HP_API hpint32 ddekit_xml_encoding_write_union_begin(DDEKIT_ENCODING_WRITER *super, const char *union_name);

HP_API hpint32 ddekit_xml_encoding_write_union_end(DDEKIT_ENCODING_WRITER *super, const char *union_name);

HP_API hpint32 ddekit_xml_encoding_write_repeat_begin(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_xml_encoding_write_repeat_end(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_xml_encoding_write_var_begin(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_xml_encoding_write_var_end(DDEKIT_ENCODING_WRITER *super, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_xml_encoding_write_enum(DDEKIT_ENCODING_WRITER *super, const hpint32 val, const hpchar *enum_name);

HP_API hpint32 ddekit_xml_encoding_write_hpchar(DDEKIT_ENCODING_WRITER *super, const char val);

HP_API hpint32 ddekit_xml_encoding_write_hpdouble(DDEKIT_ENCODING_WRITER *super, const double val);

HP_API hpint32 ddekit_xml_encoding_write_hpint8(DDEKIT_ENCODING_WRITER *super, const hpint8 val);

HP_API hpint32 ddekit_xml_encoding_write_hpint16(DDEKIT_ENCODING_WRITER *super, const hpint16 val);

HP_API hpint32 ddekit_xml_encoding_write_hpint32(DDEKIT_ENCODING_WRITER *super, const hpint32 val);

HP_API hpint32 ddekit_xml_encoding_write_hpint64(DDEKIT_ENCODING_WRITER *super, const hpint64 val);

HP_API hpint32 ddekit_xml_encoding_write_hpuint8(DDEKIT_ENCODING_WRITER *super, const hpuint8 val);

HP_API hpint32 ddekit_xml_encoding_write_hpuint16(DDEKIT_ENCODING_WRITER *super, const hpuint16 val);

HP_API hpint32 ddekit_xml_encoding_write_hpuint32(DDEKIT_ENCODING_WRITER *super, const hpuint32 val);

HP_API hpint32 ddekit_xml_encoding_write_hpuint64(DDEKIT_ENCODING_WRITER *super, const hpuint64 val);

#endif//_H_DDEKIT_XML_ENCODING_WRITER
