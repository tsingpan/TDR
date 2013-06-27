#ifndef _H_DDEKIT_COMPACT_ENCODING_READER
#define _H_DDEKIT_COMPACT_ENCODING_READER

#include "hotpot/hp_platform.h"
#include "encoding/ddekit_encoding_reader.h"

typedef struct tagDDEKIT_COMPACT_ENCODING_READER
{
	DDEKIT_ENCODING_READER dpr;

	const char *addr;
	hpuint32 size;
	hpuint32 offset;
}DDEKIT_COMPACT_ENCODING_READER;


HP_API hpint32 ddekit_compact_encoding_reader_init(DDEKIT_COMPACT_ENCODING_READER *self, const void *addr, hpuint32 size);

HP_API hpint32 ddekit_compact_encoding_reader_fini(DDEKIT_COMPACT_ENCODING_READER *self);

//virtual public functions

HP_API hpint32 ddekit_compact_encoding_read_struct_begin(DDEKIT_ENCODING_READER *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_read_struct_end(DDEKIT_ENCODING_READER *self, const char *struct_name);

HP_API hpint32 ddekit_compact_encoding_read_union_begin(DDEKIT_ENCODING_READER *self, const char *union_name);

HP_API hpint32 ddekit_compact_encoding_read_union_end(DDEKIT_ENCODING_READER *self, const char *union_name);

HP_API hpint32 ddekit_compact_encoding_read_repeat_begin(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_compact_encoding_read_repeat_end(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_compact_encoding_read_var_begin(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_read_var_end(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_compact_encoding_read_enum(DDEKIT_ENCODING_READER *super, hpint32 *val, hpchar *enum_name, hpuint32 *enum_name_length);

HP_API hpint32 ddekit_compact_encoding_read_hpchar(DDEKIT_ENCODING_READER *super, char *val);

HP_API hpint32 ddekit_compact_encoding_read_hpdouble(DDEKIT_ENCODING_READER *super, double *val);

HP_API hpint32 ddekit_compact_encoding_read_hpint8(DDEKIT_ENCODING_READER *super, hpint8 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpint16(DDEKIT_ENCODING_READER *super, hpint16 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpint32(DDEKIT_ENCODING_READER *super, hpint32 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpint64(DDEKIT_ENCODING_READER *super, hpint64 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpuint8(DDEKIT_ENCODING_READER *super, hpuint8 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpuint16(DDEKIT_ENCODING_READER *super, hpuint16 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpuint32(DDEKIT_ENCODING_READER *super, hpuint32 *val);

HP_API hpint32 ddekit_compact_encoding_read_hpuint64(DDEKIT_ENCODING_READER *super, hpuint64 *val);

#endif//_H_DDEKIT_COMPACT_ENCODING_READER
