#ifndef _H_HP_XML_WRITER
#define _H_HP_XML_WRITER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"

#include <stdio.h>


#define MAX_VECTOR_DEEP 1024
typedef struct _HP_XML_WRITER
{
	HPAbstractWriter super;
	hpuint32 count;
	FILE *f;
	hpbool need_tab;
}HP_XML_WRITER;

HP_API hpint32 xml_writer_init(HP_XML_WRITER *self, FILE *f);

HP_API hpint32 xml_writer_fini(HP_XML_WRITER *self);

//virtual functions
HP_API hpint32 xml_write_vector_begin(HPAbstractWriter *super);

HP_API hpint32 xml_write_vector_end(HPAbstractWriter *super);

HP_API hpint32 xml_write_field_begin(HPAbstractWriter *super, const char *var_name);

HP_API hpint32 xml_write_field_end(HPAbstractWriter *super, const char *var_name);

HP_API hpint32 xml_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name);

HP_API hpint32 xml_write_hpdouble(HPAbstractWriter *super, const double val);


HP_API hpint32 xml_write_hpint8(HPAbstractWriter *super, const hpint8 val);

HP_API hpint32 xml_write_hpint16(HPAbstractWriter *super, const hpint16 val);

HP_API hpint32 xml_write_hpint32(HPAbstractWriter *super, const hpint32 val);

HP_API hpint32 xml_write_hpint64(HPAbstractWriter *super, const hpint64 val);

HP_API hpint32 xml_write_hpuint8(HPAbstractWriter *super, const hpuint8 val);

HP_API hpint32 xml_write_hpuint16(HPAbstractWriter *super, const hpuint16 val);

HP_API hpint32 xml_write_hpuint32(HPAbstractWriter *super, const hpuint32 val);

HP_API hpint32 xml_write_hpuint64(HPAbstractWriter *super, const hpuint64 val);

HP_API hpint32 xml_write_string(HPAbstractWriter *super, const hpchar* str);

HP_API hpint32 xml_write_hpbool(HPAbstractWriter *super, const hpbool val);

HP_API hpint32 xml_write_null(HPAbstractWriter *super);

HP_API hpint32 xml_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 xml_write_vector_item_end(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 xml_write_struct_begin(HPAbstractWriter *super, const char *struct_name);

HP_API hpint32 xml_write_struct_end(HPAbstractWriter *super, const char *struct_name);

HP_API hpint32 xml_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val);

#endif//_H_DDEKIT_JSON_ENCODING_WRITER
