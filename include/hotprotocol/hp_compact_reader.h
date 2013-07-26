#ifndef _H_HP_COMPACT_READER
#define _H_HP_COMPACT_READER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"

typedef struct _HP_COMPACT_READER
{
	HPAbstractReader super;

	const char *addr;
	hpuint32 size;
	hpuint32 offset;
}HP_COMPACT_READER;


HP_API hpint32 hp_compact_reader_init(HP_COMPACT_READER *self, const void *addr, hpuint32 size);

HP_API hpint32 hp_compact_reader_fini(HP_COMPACT_READER *self);

//virtual public functions

HP_API hpint32 hp_compact_read_struct_begin(HPAbstractReader *self, const char *struct_name);

HP_API hpint32 hp_compact_read_struct_end(HPAbstractReader *self, const char *struct_name);

HP_API hpint32 hp_compact_read_vector_begin(HPAbstractReader *self);

HP_API hpint32 hp_compact_read_vector_end(HPAbstractReader *self);

HP_API hpint32 hp_compact_read_field_begin(HPAbstractReader *self, const char *var_name);

HP_API hpint32 hp_compact_read_field_end(HPAbstractReader *self, const char *var_name);

HP_API hpint32 hp_compact_read_enum(HPAbstractReader *super, hpint32 *val);

HP_API hpint32 hp_compact_read_hpchar(HPAbstractReader *super, char *val);

HP_API hpint32 hp_compact_read_hpdouble(HPAbstractReader *super, double *val);

HP_API hpint32 hp_compact_read_hpint8(HPAbstractReader *super, hpint8 *val);

HP_API hpint32 hp_compact_read_hpint16(HPAbstractReader *super, hpint16 *val);

HP_API hpint32 hp_compact_read_hpint32(HPAbstractReader *super, hpint32 *val);

HP_API hpint32 hp_compact_read_hpint64(HPAbstractReader *super, hpint64 *val);

HP_API hpint32 hp_compact_read_hpuint8(HPAbstractReader *super, hpuint8 *val);

HP_API hpint32 hp_compact_read_hpuint16(HPAbstractReader *super, hpuint16 *val);

HP_API hpint32 hp_compact_read_hpuint32(HPAbstractReader *super, hpuint32 *val);

HP_API hpint32 hp_compact_read_hpuint64(HPAbstractReader *super, hpuint64 *val);

HP_API hpint32 hp_compact_read_string(HPAbstractReader *super, hpchar* str, hpuint32 str_length);

#endif//_H_DDEKIT_COMPACT_ENCODING_READER
