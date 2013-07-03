#ifndef _H_DDEKIT_COMPACT_ENCODING_WRITER
#define _H_DDEKIT_COMPACT_ENCODING_WRITER

#include "hotpot/hp_platform.h"
#include "hotpot/hp_writer.h"

typedef struct tagDDEKIT_COMPACT_ENCODING_WRITER
{
	HPAbstractWriter dpw;

	char *addr;
	hpuint32 size;
	hpuint32 offset;
}DDEKIT_COMPACT_ENCODING_WRITER;

HP_API hpint32 ddekit_compact_encoding_writer_init(DDEKIT_COMPACT_ENCODING_WRITER *self, void *addr, hpuint32 size);

HP_API hpint32 ddekit_compact_encoding_writer_fini(DDEKIT_COMPACT_ENCODING_WRITER *self);

//virtual functions
HP_API hpint32 ddekit_compact_encoding_write_struct_begin(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_write_struct_end(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_write_vector_begin(HPAbstractWriter *self);

HP_API hpint32 ddekit_compact_encoding_write_vector_end(HPAbstractWriter *self);

HP_API hpint32 ddekit_compact_encoding_write_field_begin(HPAbstractWriter *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_write_field_end(HPAbstractWriter *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_write_enum(HPAbstractWriter *super, const hpint32 val, const hpchar *enum_name);

HP_API hpint32 ddekit_compact_encoding_write_hpchar(HPAbstractWriter *super, const char val);

HP_API hpint32 ddekit_compact_encoding_write_hpdouble(HPAbstractWriter *super, const double val);

HP_API hpint32 ddekit_compact_encoding_write_hpint8(HPAbstractWriter *super, const hpint8 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint16(HPAbstractWriter *super, const hpint16 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint32(HPAbstractWriter *super, const hpint32 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint64(HPAbstractWriter *super, const hpint64 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint8(HPAbstractWriter *super, const hpuint8 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint16(HPAbstractWriter *super, const hpuint16 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint32(HPAbstractWriter *super, const hpuint32 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint64(HPAbstractWriter *super, const hpuint64 val);

#endif//_H_DDEKIT_COMPACT_ENCODING_WRITER
