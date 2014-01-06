#include "hp_python_writer.h"
#include "hp_error_code.h"
#include "lib/tlibc_code.h"
#include "protocol/tlibc_abstract_writer.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>


void python_writer_init(TLIBC_PYTHON_WRITER *self)
{
	tlibc_abstract_writer_init(&self->super);


	self->super.write_struct_begin = python_write_struct_begin;
	self->super.write_vector_begin = python_write_vector_begin;
	self->super.write_field_begin = python_write_field_begin;
	self->super.write_field_end = python_write_field_end;
	self->super.write_vector_item_end = python_write_vector_item_end;

	self->super.write_tint8 = python_write_tint8;
	self->super.write_tint16 = python_write_tint16;
	self->super.write_tint32 = python_write_tint32;
	self->super.write_tint64 = python_write_tint64;
	self->super.write_tuint8 = python_write_tuint8;
	self->super.write_tuint16 = python_write_tuint16;
	self->super.write_tuint32 = python_write_tuint32;
	self->super.write_tuint64 = python_write_tuint64;

	self->super.write_enum_number = python_write_enum_number;
	self->super.write_tchar = python_write_tchar;
	self->super.write_tdouble = python_write_tdouble;
	self->super.write_string = python_write_string;
	self->super.write_tbool = python_write_tbool;
	self->super.write_null = python_write_null;
	self->super.write_counter = python_write_counter;

	self->stack_num = 0;
}

tint32 python_write_struct_begin(TLIBC_ABSTRACT_WRITER *super, const char *struct_name)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);

	TLIBC_UNUSED(struct_name);

	self->stack[self->stack_num++] = PyDict_New();
	
	return E_TLIBC_NOERROR;
}

tint32 python_write_vector_begin(TLIBC_ABSTRACT_WRITER *super)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);	
	
	
	self->stack[self->stack_num++] = PyList_New(0);
	return E_TLIBC_NOERROR;
}

tint32 python_write_field_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	
	self->stack[self->stack_num++] = PyUnicode_FromString(var_name);
	return E_TLIBC_NOERROR;
}

tint32 python_write_field_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);	
	TLIBC_UNUSED(var_name);
	PyDict_SetItem(self->stack[self->stack_num - 3], self->stack[self->stack_num - 2], self->stack[self->stack_num - 1]);	
	--self->stack_num;
	--self->stack_num;
	return E_TLIBC_NOERROR;
}

 tint32 python_write_vector_item_end(TLIBC_ABSTRACT_WRITER *super, tuint32 index)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	TLIBC_UNUSED(index);

	PyList_Append(self->stack[self->stack_num - 2], self->stack[self->stack_num - 1]);
	--self->stack_num;
	return E_TLIBC_NOERROR;
}


 tint32 python_write_enum_number(TLIBC_ABSTRACT_WRITER *super, const int val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);

	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	char tmp[2];
	tmp[0] = val;
	tmp[1] = 0;
	
	
	self->stack[self->stack_num++] = PyUnicode_FromString(tmp);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);

	
	self->stack[self->stack_num++] = PyFloat_FromDouble(val);
	
	return E_TLIBC_NOERROR;
}

tint32 python_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);	
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromLongLong(val);
	return E_TLIBC_NOERROR;
}


tint32 python_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_TLIBC_NOERROR;
}


tint32 python_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	self->stack[self->stack_num++] = PyLong_FromUnsignedLongLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_string(TLIBC_ABSTRACT_WRITER *super, const tchar* str)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	
	//self->stack[self->stack_num++] = _PyUnicode_FromASCII(str, strlen(str));
	//PyUnicode_FromString这玩意不支持中文， 会崩溃
	self->stack[self->stack_num++] = PyUnicode_FromString(str);

	return E_TLIBC_NOERROR;
}

tint32 python_write_tbool(TLIBC_ABSTRACT_WRITER *super, const tbool val)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);
	
	self->stack[self->stack_num++] = PyBool_FromLong(val);
	return E_TLIBC_NOERROR;
}

tint32 python_write_null(TLIBC_ABSTRACT_WRITER *super)
{
	TLIBC_PYTHON_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_PYTHON_WRITER, super);	

	self->stack[self->stack_num++] = NULL;
	return E_TLIBC_NOERROR;
}

tint32 python_write_counter(TLIBC_ABSTRACT_WRITER *super, const tchar *name, const tuint32 val)
{
	tint32 ret;

	ret = python_write_field_begin(super, name);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	ret = python_write_tuint32(super, val);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	ret = python_write_field_end(super, name);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
done:
	return ret;
}
