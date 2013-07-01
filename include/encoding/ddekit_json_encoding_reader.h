#ifndef _H_DDEKIT_JSON_ENCODING_READER
#define _H_DDEKIT_JSON_ENCODING_READER

#include "hotpot/hp_platform.h"
#include "hotpot/hp_reader.h"

typedef struct tagDDEKIT_JSON_ENCODING_READER
{
	DDEKIT_ENCODING_READER dpr;


}DDEKIT_JSON_ENCODING_READER;


HP_API hpint32 ddekit_json_encoding_reader_init(DDEKIT_JSON_ENCODING_READER *self, const void *addr, hpuint32 size);

HP_API hpint32 ddekit_json_encoding_reader_fini(DDEKIT_JSON_ENCODING_READER *self);

//virtual public functions

HP_API hpint32 ddekit_json_encoding_read_struct_begin(DDEKIT_ENCODING_READER *self, const char *struct_name);

HP_API hpint32 ddekit_json_encoding_read_struct_end(DDEKIT_ENCODING_READER *self, const char *struct_name);

HP_API hpint32 ddekit_json_encoding_read_vector_begin(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_json_encoding_read_vector_end(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);

HP_API hpint32 ddekit_json_encoding_read_field_begin(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_json_encoding_read_field_end(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);

HP_API hpint32 ddekit_json_encoding_read_enum(DDEKIT_ENCODING_READER *super, hpint32 *val);

HP_API hpint32 ddekit_json_encoding_read_enum_name(DDEKIT_ENCODING_READER *super, hpchar *enum_name, hpuint32 *enum_name_length);

HP_API hpint32 ddekit_json_encoding_read_hpchar(DDEKIT_ENCODING_READER *super, char *val);

HP_API hpint32 ddekit_json_encoding_read_hpdouble(DDEKIT_ENCODING_READER *super, double *val);

HP_API hpint32 ddekit_json_encoding_read_hpint8(DDEKIT_ENCODING_READER *super, hpint8 *val);

HP_API hpint32 ddekit_json_encoding_read_hpint16(DDEKIT_ENCODING_READER *super, hpint16 *val);

HP_API hpint32 ddekit_json_encoding_read_hpint32(DDEKIT_ENCODING_READER *super, hpint32 *val);

HP_API hpint32 ddekit_json_encoding_read_hpint64(DDEKIT_ENCODING_READER *super, hpint64 *val);

HP_API hpint32 ddekit_json_encoding_read_hpuint8(DDEKIT_ENCODING_READER *super, hpuint8 *val);

HP_API hpint32 ddekit_json_encoding_read_hpuint16(DDEKIT_ENCODING_READER *super, hpuint16 *val);

HP_API hpint32 ddekit_json_encoding_read_hpuint32(DDEKIT_ENCODING_READER *super, hpuint32 *val);

HP_API hpint32 ddekit_json_encoding_read_hpuint64(DDEKIT_ENCODING_READER *super, hpuint64 *val);

#endif//_H_DDEKIT_JSON_ENCODING_READER
