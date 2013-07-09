#ifndef _H_HP_ABSTRACT_READR
#define _H_HP_ABSTRACT_READR

#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _HPAbstractReader HPAbstractReader;
#define MAX_VECTOR_DEEP 1024
struct _HPAbstractReader
{
	hpint32 (*read_struct_begin)(HPAbstractReader *self, const char *struct_name);
	hpint32 (*read_struct_end)(HPAbstractReader *self, const char *struct_name);
	hpint32 (*read_vector_begin)(HPAbstractReader *self);
	hpint32 (*read_vector_end)(HPAbstractReader *self);
	hpint32 (*read_field_begin)(HPAbstractReader *self, const char *var_name, hpuint32 len);
	hpint32 (*read_field_end)(HPAbstractReader *self, const char *var_name, hpuint32 len);
	hpint32 (*read_vector_item_begin)(HPAbstractReader *self, hpuint32 index);
	hpint32 (*read_vector_item_end)(HPAbstractReader *self, hpuint32 index);

	hpint32 (*read_hpint8)(HPAbstractReader *self, hpint8 *val);
	hpint32 (*read_hpint16)(HPAbstractReader *self, hpint16 *val);
	hpint32 (*read_hpint32)(HPAbstractReader *self, hpint32 *val);
	hpint32 (*read_hpint64)(HPAbstractReader *self, hpint64 *val);
	hpint32 (*read_hpuint8)(HPAbstractReader *self, hpuint8 *val);
	hpint32 (*read_hpuint16)(HPAbstractReader *self, hpuint16 *val);
	hpint32 (*read_hpuint32)(HPAbstractReader *self, hpuint32 *val);
	hpint32 (*read_hpuint64)(HPAbstractReader *self, hpuint64 *val);

	hpint32 (*read_enum)(HPAbstractReader *self, hpint32 *val);
	hpint32 (*read_enum_name)(HPAbstractReader *self, hpchar *enum_name, hpuint32 enum_name_length);
	hpint32 (*read_hpchar)(HPAbstractReader *self, hpchar *val);
	hpint32 (*read_hpdouble)(HPAbstractReader *self, hpdouble *val);
	hpint32 (*read_hpstring)(HPAbstractReader *self, hpchar* str, hpuint32 str_length);
	hpint32 (*read_bytes)(HPAbstractReader *self, hpbytes *bytes);

	hpint32 (*read_hpbool)(HPAbstractReader *self, hpbool *val);
	hpint32 (*read_null)(HPAbstractReader *self);

	hpint32 (*read_semicolon)(HPAbstractReader *self);

	hpint32 (*read_type)(HPAbstractReader *self, HPType *type);

	hpuint32 stack[MAX_VECTOR_DEEP];
	hpuint32 stack_num;
};

hpint32 read_struct_begin(HPAbstractReader *self, const char *struct_name);
hpint32 read_struct_end(HPAbstractReader *self, const char *struct_name);
hpint32 read_vector_begin(HPAbstractReader *self);
hpint32 read_vector_end(HPAbstractReader *self);
hpint32 read_field_begin(HPAbstractReader *self, const char *var_name, hpuint32 len);
hpint32 read_field_end(HPAbstractReader *self, const char *var_name, hpuint32 len);
hpint32 read_hpint8(HPAbstractReader *self, hpint8 *val);
hpint32 read_hpint16(HPAbstractReader *self, hpint16 *val);
hpint32 read_hpint32(HPAbstractReader *self, hpint32 *val);
hpint32 read_hpint64(HPAbstractReader *self, hpint64 *val);
hpint32 read_hpuint8(HPAbstractReader *self, hpuint8 *val);
hpint32 read_hpuint16(HPAbstractReader *self, hpuint16 *val);
hpint32 read_hpuint32(HPAbstractReader *self, hpuint32 *val);
hpint32 read_hpuint64(HPAbstractReader *self, hpuint64 *val);

hpint32 read_enum(HPAbstractReader *self, hpint32 *val);
hpint32 read_enum_name(HPAbstractReader *self, hpchar *enum_name, hpuint32 enum_name_length);
hpint32 read_hpchar(HPAbstractReader *self, hpchar *val);
hpint32 read_hpdouble(HPAbstractReader *self, hpdouble *val);
hpint32 read_hpstring(HPAbstractReader *self, hpchar* str, hpuint32 str_length);
hpint32 read_bytes(HPAbstractReader *self, hpbytes *bytes);
hpint32 read_hpbool(HPAbstractReader *self, hpbool *val);
hpint32 read_null(HPAbstractReader *self);
hpint32 read_semicolon(HPAbstractReader *self);
hpint32 read_type(HPAbstractReader *self, HPType *type);
hpint32 read_vector_item_begin(HPAbstractReader *self, hpuint32 index);
hpint32 read_vector_item_end(HPAbstractReader *self, hpuint32 index);
hpuint32 reader_get_index(HPAbstractReader *self);
#endif //_H_HP_ABSTRACT_READR

