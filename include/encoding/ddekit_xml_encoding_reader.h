#ifndef _H_DDEKIT_XML_ENCODING_READER
#define _H_DDEKIT_XML_ENCODING_READER

#include "hotpot/hp_platform.h"
#include "encoding/ddekit_encoding_reader.h"

#include <stdio.h>

typedef struct tagDDEKIT_XML_ENCODING_READER_STACK_ELEMENT
{
	hpint32 in_repeat;
	hpint32 end_with_zero;
}DDEKIT_XML_ENCODING_READER_STACK_ELEMENT;

#ifndef _DEF_DDEKIT_XML_ENCODING_READER
#define _DEF_DDEKIT_XML_ENCODING_READER
typedef struct tagDDEKIT_XML_ENCODING_READER DDEKIT_XML_ENCODING_READER;
#endif//_DEF_DDEKIT_XML_ENCODING_READER

#define DDEKIT_XML_MAX_DEEP_OF_READER_STACK 65536
struct tagDDEKIT_XML_ENCODING_READER
{
	DDEKIT_ENCODING_READER dpr;

	FILE *f;
	hpuint32 union_level;
	hpuint32 struct_level;
	DDEKIT_XML_ENCODING_READER_STACK_ELEMENT struct_stack[DDEKIT_XML_MAX_DEEP_OF_READER_STACK];
};

HP_API hpint32 ddekit_xml_encoding_reader_init(DDEKIT_XML_ENCODING_READER *self, FILE *f);

HP_API hpint32 ddekit_xml_encoding_reader_fini(DDEKIT_XML_ENCODING_READER *self);

//virtual public functions
HP_API hpint32 ddekit_xml_encoding_read_struct_begin(DDEKIT_ENCODING_READER *super, const char *struct_name);

HP_API hpint32 ddekit_xml_encoding_read_struct_end(DDEKIT_ENCODING_READER *super, const char *struct_name);

HP_API hpint32 ddekit_xml_encoding_read_union_begin(DDEKIT_ENCODING_READER *super, const char *union_name);

HP_API hpint32 ddekit_xml_encoding_read_union_end(DDEKIT_ENCODING_READER *super, const char *union_name);

HP_API hpint32 ddekit_xml_encoding_read_repeat_begin(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_xml_encoding_read_repeat_end(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_xml_encoding_read_var_begin(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_xml_encoding_read_var_end(DDEKIT_ENCODING_READER *super, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_xml_encoding_read_enum(DDEKIT_ENCODING_READER *super, const hpint32 *val, hpchar *enum_name, hpuint32 *enum_name_length);

HP_API hpint32 ddekit_xml_encoding_read_hpchar(DDEKIT_ENCODING_READER *super, char *val);

HP_API hpint32 ddekit_xml_encoding_read_hpdouble(DDEKIT_ENCODING_READER *super, double *val);

HP_API hpint32 ddekit_xml_encoding_read_hpint8(DDEKIT_ENCODING_READER *super, hpint8 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpint16(DDEKIT_ENCODING_READER *super, hpint16 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpint32(DDEKIT_ENCODING_READER *super, hpint32 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpint64(DDEKIT_ENCODING_READER *super, hpint64 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpuint8(DDEKIT_ENCODING_READER *super, hpuint8 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpuint16(DDEKIT_ENCODING_READER *super, hpuint16 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpuint32(DDEKIT_ENCODING_READER *super, hpuint32 *val);

HP_API hpint32 ddekit_xml_encoding_read_hpuint64(DDEKIT_ENCODING_READER *super, hpuint64 *val);

#endif//_H_DDEKIT_XML_ENCODING_READER
