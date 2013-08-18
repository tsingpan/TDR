#ifndef _H_HP_PYTHON_WRITER
#define _H_HP_PYTHON_WRITER

#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "Python.h"

#define HP_PYTHON_MAX_STACK_NUM 1024
typedef struct _HP_PYTHON_WRITER
{
	HPAbstractWriter super;
	PyObject *stack[HP_PYTHON_MAX_STACK_NUM];
	hpuint32 stack_num;
}HP_PYTHON_WRITER;


HP_API void python_writer_init(HP_PYTHON_WRITER *self);

//virtual functions
HP_API hpint32 python_write_struct_begin(HPAbstractWriter *self, const char *struct_name);

HP_API hpint32 python_write_vector_begin(HPAbstractWriter *self);

HP_API hpint32 python_write_field_begin(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 python_write_field_end(HPAbstractWriter *self, const char *var_name);

HP_API hpint32 python_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 python_write_vector_item_end(HPAbstractWriter *super, hpuint32 index);

HP_API hpint32 python_write_enum_number(HPAbstractWriter *super, const int val);

HP_API hpint32 python_write_hpchar(HPAbstractWriter *super, const char val);

HP_API hpint32 python_write_hpdouble(HPAbstractWriter *super, const double val);

HP_API hpint32 python_write_hpint8(HPAbstractWriter *super, const hpint8 val);

HP_API hpint32 python_write_hpint16(HPAbstractWriter *super, const hpint16 val);

HP_API hpint32 python_write_hpint32(HPAbstractWriter *super, const hpint32 val);

HP_API hpint32 python_write_hpint64(HPAbstractWriter *super, const hpint64 val);

HP_API hpint32 python_write_hpuint8(HPAbstractWriter *super, const hpuint8 val);

HP_API hpint32 python_write_hpuint16(HPAbstractWriter *super, const hpuint16 val);

HP_API hpint32 python_write_hpuint32(HPAbstractWriter *super, const hpuint32 val);

HP_API hpint32 python_write_hpuint64(HPAbstractWriter *super, const hpuint64 val);

HP_API hpint32 python_write_string(HPAbstractWriter *super, const hpchar* str);

HP_API hpint32 python_write_hpbool(HPAbstractWriter *super, const hpbool val);

HP_API hpint32 python_write_null(HPAbstractWriter *super);

HP_API hpint32 python_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val);

#endif//_H_HP_PYTHON_WRITER
