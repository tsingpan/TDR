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
	hpint32 (*read_field_begin)(HPAbstractReader *self, const char *var_name);
	hpint32 (*read_field_end)(HPAbstractReader *self, const char *var_name);
	hpint32 (*read_vector_item_begin)(HPAbstractReader *self, hpuint32 index);
	hpint32 (*read_vector_item_end)(HPAbstractReader *self, hpuint32 index);

	hpint32 (*read_int8)(HPAbstractReader *self, hpint8 *val);
	hpint32 (*read_int16)(HPAbstractReader *self, hpint16 *val);
	hpint32 (*read_int32)(HPAbstractReader *self, hpint32 *val);
	hpint32 (*read_int64)(HPAbstractReader *self, hpint64 *val);
	hpint32 (*read_uint8)(HPAbstractReader *self, hpuint8 *val);
	hpint32 (*read_uint16)(HPAbstractReader *self, hpuint16 *val);
	hpint32 (*read_uint32)(HPAbstractReader *self, hpuint32 *val);
	hpint32 (*read_uint64)(HPAbstractReader *self, hpuint64 *val);

	hpint32 (*read_enum_number)(HPAbstractReader *self, hpint32 *val);
	hpint32 (*read_enum_name)(HPAbstractReader *self, hpchar *enum_name, hpuint32 enum_name_length);
	hpint32 (*read_char)(HPAbstractReader *self, hpchar *val);
	hpint32 (*read_double)(HPAbstractReader *self, hpdouble *val);
	hpint32 (*read_string)(HPAbstractReader *self, hpchar* str, hpuint32 str_length);
	hpint32 (*read_bytes)(HPAbstractReader *self, hpbytes *bytes);

	hpint32 (*read_bool)(HPAbstractReader *self, hpbool *val);
	hpint32 (*read_null)(HPAbstractReader *self);
	hpint32 (*read_counter)(HPAbstractReader *self, const char *name, hpuint32 *val);

	hpint32 (*read_semicolon)(HPAbstractReader *self);

	hpint32 (*read_type)(HPAbstractReader *self, HPType *type);

	hpuint32 stack[MAX_VECTOR_DEEP];
	hpuint32 stack_num;
};

hpint32 read_struct_begin(HPAbstractReader *self, const char *struct_name);
hpint32 read_struct_end(HPAbstractReader *self, const char *struct_name);
hpint32 read_vector_begin(HPAbstractReader *self);
hpint32 read_vector_end(HPAbstractReader *self);
hpint32 read_field_begin(HPAbstractReader *self, const char *var_name);
hpint32 read_field_end(HPAbstractReader *self, const char *var_name);
hpint32 read_int8(HPAbstractReader *self, hpint8 *val);
hpint32 read_int16(HPAbstractReader *self, hpint16 *val);
hpint32 read_int32(HPAbstractReader *self, hpint32 *val);
hpint32 read_int64(HPAbstractReader *self, hpint64 *val);
hpint32 read_uint8(HPAbstractReader *self, hpuint8 *val);
hpint32 read_uint16(HPAbstractReader *self, hpuint16 *val);
hpint32 read_uint32(HPAbstractReader *self, hpuint32 *val);
hpint32 read_uint64(HPAbstractReader *self, hpuint64 *val);

hpint32 read_enum_number(HPAbstractReader *self, hpint32 *val);
hpint32 read_enum_name(HPAbstractReader *self, hpchar *enum_name, hpuint32 enum_name_length);
hpint32 read_char(HPAbstractReader *self, hpchar *val);
hpint32 read_double(HPAbstractReader *self, hpdouble *val);
hpint32 read_string(HPAbstractReader *self, hpchar* str, hpuint32 str_length);
hpint32 read_bytes(HPAbstractReader *self, hpbytes *bytes);
hpint32 read_bool(HPAbstractReader *self, hpbool *val);
hpint32 read_null(HPAbstractReader *self);
hpint32 read_type(HPAbstractReader *self, HPType *type);
hpint32 read_counter(HPAbstractReader *self, const char *name, hpuint32 *val);
hpint32 read_vector_item_begin(HPAbstractReader *self, hpuint32 index);
hpint32 read_vector_item_end(HPAbstractReader *self, hpuint32 index);
hpuint32 reader_get_index(HPAbstractReader *self);
#endif //_H_HP_ABSTRACT_READR

