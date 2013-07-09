#ifndef _H_HP_XML_READER
#define _H_HP_XML_READER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <stdio.h>

typedef struct _HP_XML_READER
{
	HPAbstractReader super;

	hpuint32 count;
	FILE *f;
	hpbool need_tab;
}HP_XML_READER;


HP_API hpint32 xml_reader_init(HP_XML_READER *self, FILE *f);

HP_API hpint32 xml_reader_fini(HP_XML_READER *self);

//virtual public functions

HP_API hpint32 xml_read_struct_begin(HPAbstractReader *self, const char *struct_name);

HP_API hpint32 xml_read_struct_end(HPAbstractReader *self, const char *struct_name);

HP_API hpint32 xml_read_vector_begin(HPAbstractReader *self);

HP_API hpint32 xml_read_vector_end(HPAbstractReader *self);

HP_API hpint32 xml_read_field_begin(HPAbstractReader *self, const char *var_name, hpuint32 len);

HP_API hpint32 xml_read_field_end(HPAbstractReader *self, const char *var_name, hpuint32 len);

HP_API hpint32 xml_read_enum(HPAbstractReader *super, hpint32 *val);

HP_API hpint32 xml_read_enum_name(HPAbstractReader *super, hpchar *enum_name, hpuint32 *enum_name_length);

HP_API hpint32 xml_read_hpchar(HPAbstractReader *super, char *val);

HP_API hpint32 xml_read_hpdouble(HPAbstractReader *super, double *val);

HP_API hpint32 xml_read_hpint8(HPAbstractReader *super, hpint8 *val);

HP_API hpint32 xml_read_hpint16(HPAbstractReader *super, hpint16 *val);

HP_API hpint32 xml_read_hpint32(HPAbstractReader *super, hpint32 *val);

HP_API hpint32 xml_read_hpint64(HPAbstractReader *super, hpint64 *val);

HP_API hpint32 xml_read_hpuint8(HPAbstractReader *super, hpuint8 *val);

HP_API hpint32 xml_read_hpuint16(HPAbstractReader *super, hpuint16 *val);

HP_API hpint32 xml_read_hpuint32(HPAbstractReader *super, hpuint32 *val);

HP_API hpint32 xml_read_hpuint64(HPAbstractReader *super, hpuint64 *val);


HP_API hpint32 xml_read_vector_item_end(HPAbstractReader *super, hpuint32 index);

HP_API hpint32 xml_read_vector_item_begin(HPAbstractReader *super, hpuint32 index);
HP_API hpint32 xml_read_hpbool(HPAbstractReader *super, hpbool *val);
HP_API hpint32 xml_read_bytes(HPAbstractReader *super, hpbytes *bytes);

#endif//_H_XML_READER
