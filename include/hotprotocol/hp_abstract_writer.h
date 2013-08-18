#ifndef _H_HP_ABSTRACT_WRITER
#define _H_HP_ABSTRACT_WRITER

#include "hotpot/hp_platform.h"
typedef struct _HPAbstractWriter HPAbstractWriter;
struct _HPAbstractWriter
{
	hpint32 (*write_struct_begin)(HPAbstractWriter *self, const char *struct_name);
	hpint32 (*write_struct_end)(HPAbstractWriter *self, const char *struct_name);
	hpint32 (*write_vector_begin)(HPAbstractWriter *self);
	hpint32 (*write_vector_end)(HPAbstractWriter *self);
	hpint32 (*write_field_begin)(HPAbstractWriter *self, const char *var_name);
	hpint32 (*write_field_end)(HPAbstractWriter *self, const char *var_name);	
	hpint32 (*write_vector_item_begin)(HPAbstractWriter *self, hpuint32 index);
	hpint32 (*write_vector_item_end)(HPAbstractWriter *self, hpuint32 index);

	hpint32 (*write_hpint8)(HPAbstractWriter *self, const hpint8 val);
	hpint32 (*write_hpint16)(HPAbstractWriter *self, const hpint16 val);
	hpint32 (*write_hpint32)(HPAbstractWriter *self, const hpint32 val);
	hpint32 (*write_hpint64)(HPAbstractWriter *self, const hpint64 val);
	hpint32 (*write_hpuint8)(HPAbstractWriter *self, const hpuint8 val);
	hpint32 (*write_hpuint16)(HPAbstractWriter *self, const hpuint16 val);
	hpint32 (*write_hpuint32)(HPAbstractWriter *self, const hpuint32 val);
	hpint32 (*write_hpuint64)(HPAbstractWriter *self, const hpuint64 val);
	hpint32 (*write_hpchar)(HPAbstractWriter *self, const hpchar val);	
	hpint32 (*write_hpdouble)(HPAbstractWriter *self, const hpdouble val);
	hpint32 (*write_hpbool)(HPAbstractWriter *self, const hpbool val);

	hpint32 (*write_enum_number)(HPAbstractWriter *self, const hpint32 val);
	hpint32 (*write_enum_name)(HPAbstractWriter *self, const hpchar *enum_name);
	hpint32 (*write_counter)(HPAbstractWriter *self, const hpchar *name, const hpuint32 val);
	hpint32 (*write_string)(HPAbstractWriter *self, const hpchar* str);
	hpint32 (*write_bytes)(HPAbstractWriter *self, const hpbytes bytes);
	hpint32 (*write_null)(HPAbstractWriter *self);	

	hpint32 (*write_type)(HPAbstractWriter *self, const HPType type);
};

void hp_abstract_writer_init(HPAbstractWriter *self);
hpint32 write_struct_begin(HPAbstractWriter *self, const char *struct_name);
hpint32 write_struct_end(HPAbstractWriter *self, const char *struct_name);
hpint32 write_vector_begin(HPAbstractWriter *self);
hpint32 write_vector_end(HPAbstractWriter *self);
hpint32 write_field_begin(HPAbstractWriter *self, const char *var_name);
hpint32 write_field_end(HPAbstractWriter *self, const char *var_name);

hpint32 write_hpint8(HPAbstractWriter *self, const hpint8 val);
hpint32 write_hpint16(HPAbstractWriter *self, const hpint16 val);
hpint32 write_hpint32(HPAbstractWriter *self, const hpint32 val);
hpint32 write_hpint64(HPAbstractWriter *self, const hpint64 val);
hpint32 write_hpuint8(HPAbstractWriter *self, const hpuint8 val);
hpint32 write_hpuint16(HPAbstractWriter *self, const hpuint16 val);
hpint32 write_hpuint32(HPAbstractWriter *self, const hpuint32 val);
hpint32 write_hpuint64(HPAbstractWriter *self, const hpuint64 val);
hpint32 write_hpchar(HPAbstractWriter *self, const hpchar val);	
hpint32 write_hpdouble(HPAbstractWriter *self, const hpdouble val);
hpint32 write_hpbool(HPAbstractWriter *self, const hpbool val);

hpint32 write_enum_number(HPAbstractWriter *self, const hpint32 val);
hpint32 write_enum_name(HPAbstractWriter *self, const hpchar *enum_name);
hpint32 write_string(HPAbstractWriter *self, const hpchar* str);
hpint32 write_bytes(HPAbstractWriter *self, const hpbytes bytes);
hpint32 write_null(HPAbstractWriter *self);
hpint32 write_type(HPAbstractWriter *self, const HPType type);
hpint32 write_counter(HPAbstractWriter *self, const hpchar *name, const hpuint32 val);
hpint32 write_vector_item_begin(HPAbstractWriter *self, hpuint32 index);
hpint32 write_vector_item_end(HPAbstractWriter *self, hpuint32 index);

#endif //_H_HP_ABSTRACT_WRITER

