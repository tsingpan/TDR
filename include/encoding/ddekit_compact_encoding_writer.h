#ifndef _H_DDEKIT_COMPACT_ENCODING_WRITER
#define _H_DDEKIT_COMPACT_ENCODING_WRITER

#include "hotpot/hp_platform.h"
#include "hotpot/hp_writer.h"

typedef struct tagDDEKIT_COMPACT_ENCODING_WRITER
{
	DDEKIT_ENCODING_WRITER dpw;

	char *addr;
	hpuint32 size;
	hpuint32 offset;
}DDEKIT_COMPACT_ENCODING_WRITER;

HP_API hpint32 ddekit_compact_encoding_writer_init(DDEKIT_COMPACT_ENCODING_WRITER *self, void *addr, hpuint32 size);

HP_API hpint32 ddekit_compact_encoding_writer_fini(DDEKIT_COMPACT_ENCODING_WRITER *self);

//virtual functions
HP_API hpint32 ddekit_compact_encoding_write_struct_begin(DDEKIT_ENCODING_WRITER *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_write_struct_end(DDEKIT_ENCODING_WRITER *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_write_vector_begin(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_compact_encoding_write_vector_end(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_compact_encoding_write_field_begin(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_write_field_end(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_write_enum(DDEKIT_ENCODING_WRITER *super, const hpint32 val, const hpchar *enum_name);

HP_API hpint32 ddekit_compact_encoding_write_hpchar(DDEKIT_ENCODING_WRITER *super, const char val);

HP_API hpint32 ddekit_compact_encoding_write_hpdouble(DDEKIT_ENCODING_WRITER *super, const double val);

HP_API hpint32 ddekit_compact_encoding_write_hpint8(DDEKIT_ENCODING_WRITER *super, const hpint8 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint16(DDEKIT_ENCODING_WRITER *super, const hpint16 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint32(DDEKIT_ENCODING_WRITER *super, const hpint32 val);

HP_API hpint32 ddekit_compact_encoding_write_hpint64(DDEKIT_ENCODING_WRITER *super, const hpint64 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint8(DDEKIT_ENCODING_WRITER *super, const hpuint8 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint16(DDEKIT_ENCODING_WRITER *super, const hpuint16 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint32(DDEKIT_ENCODING_WRITER *super, const hpuint32 val);

HP_API hpint32 ddekit_compact_encoding_write_hpuint64(DDEKIT_ENCODING_WRITER *super, const hpuint64 val);

#endif//_H_DDEKIT_COMPACT_ENCODING_WRITER
