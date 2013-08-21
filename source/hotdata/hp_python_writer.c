#include "hp_python_writer.h"
#include "hotpot/hp_error_code.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>


void python_writer_init(HP_PYTHON_WRITER *self)
{
	hp_abstract_writer_init(&self->super);


	self->super.write_struct_begin = python_write_struct_begin;
	self->super.write_vector_begin = python_write_vector_begin;
	self->super.write_field_begin = python_write_field_begin;
	self->super.write_field_end = python_write_field_end;
	self->super.write_vector_item_end = python_write_vector_item_end;

	self->super.write_hpint8 = python_write_hpint8;
	self->super.write_hpint16 = python_write_hpint16;
	self->super.write_hpint32 = python_write_hpint32;
	self->super.write_hpint64 = python_write_hpint64;
	self->super.write_hpuint8 = python_write_hpuint8;
	self->super.write_hpuint16 = python_write_hpuint16;
	self->super.write_hpuint32 = python_write_hpuint32;
	self->super.write_hpuint64 = python_write_hpuint64;

	self->super.write_enum_number = python_write_enum_number;
	self->super.write_hpchar = python_write_hpchar;
	self->super.write_hpdouble = python_write_hpdouble;
	self->super.write_string = python_write_string;
	self->super.write_hpbool = python_write_hpbool;
	self->super.write_null = python_write_null;
	self->super.write_counter = python_write_counter;

	self->stack_num = 0;
}

hpint32 python_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);

	HP_UNUSED(struct_name);

	self->stack[self->stack_num++] = PyDict_New();
	
	return E_HP_NOERROR;
}

hpint32 python_write_vector_begin(HPAbstractWriter *super)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);	
	
	
	self->stack[self->stack_num++] = PyList_New(0);
	return E_HP_NOERROR;
}

hpint32 python_write_field_begin(HPAbstractWriter *super, const char *var_name)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	
	self->stack[self->stack_num++] = PyUnicode_FromString(var_name);
	return E_HP_NOERROR;
}

hpint32 python_write_field_end(HPAbstractWriter *super, const char *var_name)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);	
	HP_UNUSED(var_name);
	PyDict_SetItem(self->stack[self->stack_num - 3], self->stack[self->stack_num - 2], self->stack[self->stack_num - 1]);	
	--self->stack_num;
	--self->stack_num;
	return E_HP_NOERROR;
}

HP_API hpint32 python_write_vector_item_end(HPAbstractWriter *super, hpuint32 index)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	HP_UNUSED(index);

	PyList_Append(self->stack[self->stack_num - 2], self->stack[self->stack_num - 1]);
	--self->stack_num;
	return E_HP_NOERROR;
}


HP_API hpint32 python_write_enum_number(HPAbstractWriter *super, const int val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);

	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	char tmp[2];
	tmp[0] = val;
	tmp[1] = 0;
	
	
	self->stack[self->stack_num++] = PyUnicode_FromString(tmp);
	return E_HP_NOERROR;
}

hpint32 python_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);

	
	self->stack[self->stack_num++] = PyFloat_FromDouble(val);
	
	return E_HP_NOERROR;
}

hpint32 python_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);	
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLongLong(val);
	return E_HP_NOERROR;
}


hpint32 python_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_HP_NOERROR;
}


hpint32 python_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLongLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_string(HPAbstractWriter *super, const hpchar* str)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	
	//self->stack[self->stack_num++] = _PyUnicode_FromASCII(str, strlen(str));
	//PyUnicode_FromString这玩意不支持中文， 会崩溃
	self->stack[self->stack_num++] = PyUnicode_FromString(str);

	return E_HP_NOERROR;
}

hpint32 python_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);
	
	self->stack[self->stack_num++] = PyBool_FromLong(val);
	return E_HP_NOERROR;
}

hpint32 python_write_null(HPAbstractWriter *super)
{
	HP_PYTHON_WRITER *self = HP_CONTAINER_OF(super, HP_PYTHON_WRITER, super);	

	self->stack[self->stack_num++] = NULL;
	return E_HP_NOERROR;
}

hpint32 python_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val)
{
	hpint32 ret;

	ret = python_write_field_begin(super, name);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}

	ret = python_write_hpuint32(super, val);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}

	ret = python_write_field_end(super, name);
	if(ret != E_HP_NOERROR)
	{
		goto done;
	}
done:
	return ret;
}
