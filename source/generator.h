#ifndef _H_TLIBC_ABSTRACT_WRITER
#define _H_TLIBC_ABSTRACT_WRITER

#include "platform/tlibc_platform.h"

typedef struct _TLIBC_ABSTRACT_WRITER TLIBC_ABSTRACT_WRITER;
struct _TLIBC_ABSTRACT_WRITER
{
	tint32 (*write_struct_begin)(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
	tint32 (*write_struct_end)(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
	tint32 (*write_vector_begin)(TLIBC_ABSTRACT_WRITER *self);
	tint32 (*write_vector_end)(TLIBC_ABSTRACT_WRITER *self);
	tint32 (*write_field_begin)(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
	tint32 (*write_field_end)(TLIBC_ABSTRACT_WRITER *self, const char *var_name);	
	tint32 (*write_vector_item_begin)(TLIBC_ABSTRACT_WRITER *self, tuint32 index);
	tint32 (*write_vector_item_end)(TLIBC_ABSTRACT_WRITER *self, tuint32 index);

	tint32 (*write_tint8)(TLIBC_ABSTRACT_WRITER *self, const tint8 val);
	tint32 (*write_tint16)(TLIBC_ABSTRACT_WRITER *self, const tint16 val);
	tint32 (*write_tint32)(TLIBC_ABSTRACT_WRITER *self, const tint32 val);
	tint32 (*write_tint64)(TLIBC_ABSTRACT_WRITER *self, const tint64 val);
	tint32 (*write_tuint8)(TLIBC_ABSTRACT_WRITER *self, const tuint8 val);
	tint32 (*write_tuint16)(TLIBC_ABSTRACT_WRITER *self, const tuint16 val);
	tint32 (*write_tuint32)(TLIBC_ABSTRACT_WRITER *self, const tuint32 val);
	tint32 (*write_tuint64)(TLIBC_ABSTRACT_WRITER *self, const tuint64 val);
	tint32 (*write_tchar)(TLIBC_ABSTRACT_WRITER *self, const tchar val);	
	tint32 (*write_tdouble)(TLIBC_ABSTRACT_WRITER *self, const tdouble val);
	tint32 (*write_tbool)(TLIBC_ABSTRACT_WRITER *self, const tbool val);

	tint32 (*write_enum_number)(TLIBC_ABSTRACT_WRITER *self, const tint32 val);
	tint32 (*write_enum_name)(TLIBC_ABSTRACT_WRITER *self, const tchar *enum_name);
	tint32 (*write_counter)(TLIBC_ABSTRACT_WRITER *self, const tchar *name, const tuint32 val);
	tint32 (*write_string)(TLIBC_ABSTRACT_WRITER *self, const tchar* str);
	tint32 (*write_bytes)(TLIBC_ABSTRACT_WRITER *self, const tbytes bytes);
	tint32 (*write_null)(TLIBC_ABSTRACT_WRITER *self);	

	tint32 (*write_type)(TLIBC_ABSTRACT_WRITER *self, const HPType type);
};

void tlibc_abstract_writer_init(TLIBC_ABSTRACT_WRITER *self);

tint32 write_struct_begin(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
tint32 write_struct_end(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
tint32 write_vector_begin(TLIBC_ABSTRACT_WRITER *self);
tint32 write_vector_end(TLIBC_ABSTRACT_WRITER *self);
tint32 write_field_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
tint32 write_field_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name);

tint32 write_tint8(TLIBC_ABSTRACT_WRITER *self, const tint8 val);
tint32 write_tint16(TLIBC_ABSTRACT_WRITER *self, const tint16 val);
tint32 write_tint32(TLIBC_ABSTRACT_WRITER *self, const tint32 val);
tint32 write_tint64(TLIBC_ABSTRACT_WRITER *self, const tint64 val);
tint32 write_tuint8(TLIBC_ABSTRACT_WRITER *self, const tuint8 val);
tint32 write_tuint16(TLIBC_ABSTRACT_WRITER *self, const tuint16 val);
tint32 write_tuint32(TLIBC_ABSTRACT_WRITER *self, const tuint32 val);
tint32 write_tuint64(TLIBC_ABSTRACT_WRITER *self, const tuint64 val);
tint32 write_tchar(TLIBC_ABSTRACT_WRITER *self, const tchar val);	
tint32 write_tdouble(TLIBC_ABSTRACT_WRITER *self, const tdouble val);
tint32 write_tbool(TLIBC_ABSTRACT_WRITER *self, const tbool val);

tint32 write_enum_number(TLIBC_ABSTRACT_WRITER *self, const tint32 val);
tint32 write_enum_name(TLIBC_ABSTRACT_WRITER *self, const tchar *enum_name);
tint32 write_string(TLIBC_ABSTRACT_WRITER *self, const tchar* str);
tint32 write_bytes(TLIBC_ABSTRACT_WRITER *self, const tbytes bytes);
tint32 write_null(TLIBC_ABSTRACT_WRITER *self);
tint32 write_type(TLIBC_ABSTRACT_WRITER *self, const HPType type);
tint32 write_counter(TLIBC_ABSTRACT_WRITER *self, const tchar *name, const tuint32 val);
tint32 write_vector_item_begin(TLIBC_ABSTRACT_WRITER *self, tuint32 index);
tint32 write_vector_item_end(TLIBC_ABSTRACT_WRITER *self, tuint32 index);

#endif

