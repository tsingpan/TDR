#ifndef _H_HP_JSON_WRITER
#define _H_HP_JSON_WRITER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"

#include <stdio.h>


#define MAX_VECTOR_DEEP 1024
typedef struct _HP_JSON_WRITER
{
	HPAbstractWriter super;
	hpuint32 count;
	hpbool need_tab;
	FILE *f;
}HP_JSON_WRITER;

HP_API hpint32 ddekit_json_encoding_writer_init(HP_JSON_WRITER *self, FILE *f);

HP_API hpint32 ddekit_json_encoding_writer_fini(HP_JSON_WRITER *self);

//virtual functions
HP_API hpint32 ddekit_json_encoding_write_struct_begin(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 ddekit_json_encoding_write_struct_end(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 ddekit_json_encoding_write_vector_begin(HPAbstractWriter *self);

HP_API hpint32 ddekit_json_encoding_write_vector_end(HPAbstractWriter *self);

HP_API hpint32 ddekit_json_encoding_write_field_begin(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 ddekit_json_encoding_write_field_end(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 ddekit_json_encoding_write_enum(HPAbstractWriter *super, const int val);

HP_API hpint32 ddekit_json_encoding_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name);

HP_API hpint32 ddekit_json_encoding_write_hpchar(HPAbstractWriter *super, const char val);

HP_API hpint32 ddekit_json_encoding_write_hpdouble(HPAbstractWriter *super, const double val);

HP_API hpint32 ddekit_json_encoding_write_hpint8(HPAbstractWriter *super, const hpint8 val);

HP_API hpint32 ddekit_json_encoding_write_hpint16(HPAbstractWriter *super, const hpint16 val);

HP_API hpint32 ddekit_json_encoding_write_hpint32(HPAbstractWriter *super, const hpint32 val);

HP_API hpint32 ddekit_json_encoding_write_hpint64(HPAbstractWriter *super, const hpint64 val);

HP_API hpint32 ddekit_json_encoding_write_hpuint8(HPAbstractWriter *super, const hpuint8 val);

HP_API hpint32 ddekit_json_encoding_write_hpuint16(HPAbstractWriter *super, const hpuint16 val);

HP_API hpint32 ddekit_json_encoding_write_hpuint32(HPAbstractWriter *super, const hpuint32 val);

HP_API hpint32 ddekit_json_encoding_write_hpuint64(HPAbstractWriter *super, const hpuint64 val);

HP_API hpint32 ddekit_json_encoding_write_bytes(HPAbstractWriter *super, const hpbytes bytes);

HP_API hpint32 ddekit_json_encoding_write_string(HPAbstractWriter *self, const hpchar* str);

HP_API hpint32 ddekit_json_encoding_write_hpbool(HPAbstractWriter *self, const hpbool val);

HP_API hpint32 ddekit_json_encoding_write_null(HPAbstractWriter *self);

HP_API hpint32 ddekit_json_encoding_write_semicolon(HPAbstractWriter *super);

HP_API hpint32 ddekit_json_encoding_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index);


#endif//_H_DDEKIT_JSON_ENCODING_WRITER
