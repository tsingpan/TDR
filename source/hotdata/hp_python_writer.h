#ifndef _H_TLIBC_PYTHON_WRITER
#define _H_TLIBC_PYTHON_WRITER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_writer.h"

#include "Python.h"

#define HP_PYTHON_MAX_STACK_NUM 1024
typedef struct _TLIBC_PYTHON_WRITER
{
	TLIBC_ABSTRACT_WRITER super;
	PyObject *stack[HP_PYTHON_MAX_STACK_NUM];
	tuint32 stack_num;
}TLIBC_PYTHON_WRITER;


void python_writer_init(TLIBC_PYTHON_WRITER *self);

//virtual functions
tint32 python_write_struct_begin(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);

 tint32 python_write_vector_begin(TLIBC_ABSTRACT_WRITER *self);

 tint32 python_write_field_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name);

 tint32 python_write_field_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name);

 tint32 python_write_vector_item_end(TLIBC_ABSTRACT_WRITER *super, tuint32 index);

 tint32 python_write_enum_number(TLIBC_ABSTRACT_WRITER *super, const int val);

 tint32 python_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char val);

 tint32 python_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double val);

 tint32 python_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 val);

 tint32 python_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 val);

 tint32 python_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 val);

 tint32 python_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 val);

 tint32 python_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 val);

 tint32 python_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 val);

 tint32 python_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 val);

 tint32 python_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 val);

 tint32 python_write_string(TLIBC_ABSTRACT_WRITER *super, const tchar* str);

 tint32 python_write_tbool(TLIBC_ABSTRACT_WRITER *super, const tbool val);

 tint32 python_write_null(TLIBC_ABSTRACT_WRITER *super);

 tint32 python_write_counter(TLIBC_ABSTRACT_WRITER *super, const tchar *name, const tuint32 val);

#endif//_H_TLIBC_PYTHON_WRITER
