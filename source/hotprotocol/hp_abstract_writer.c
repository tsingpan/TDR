#include "hotprotocol/hp_abstract_writer.h"
#include "hotpot/hp_error.h"

hpint32 write_struct_begin(HPAbstractWriter *self, const char *struct_name)
{
	if(self->write_struct_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_struct_begin(self, struct_name);
}

hpint32 write_struct_end(HPAbstractWriter *self, const char *struct_name)
{
	if(self->write_struct_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_struct_end(self, struct_name);
}

hpint32 write_vector_begin(HPAbstractWriter *self)
{
	++(self->stack_num);
	self->stack[self->stack_num - 1] = 0;

	if(self->write_vector_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_vector_begin(self);
}

hpint32 write_vector_end(HPAbstractWriter *self)
{
	--(self->stack_num);
	if(self->write_vector_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_vector_end(self);
}

hpint32 write_field_begin(HPAbstractWriter *self, const char *var_name)
{
	if(self->write_field_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_field_begin(self, var_name);
}

hpint32 write_field_end(HPAbstractWriter *self, const char *var_name)
{
	if(self->write_field_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_field_end(self, var_name);
}

hpint32 write_int8(HPAbstractWriter *self, const hpint8 val)
{
	if(self->write_int8 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_int8(self, val);
}

hpint32 write_int16(HPAbstractWriter *self, const hpint16 val)
{
	if(self->write_int16 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_int16(self, val);
}

hpint32 write_int32(HPAbstractWriter *self, const hpint32 val)
{
	if(self->write_int32 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_int32(self, val);
}

hpint32 write_int64(HPAbstractWriter *self, const hpint64 val)
{
	if(self->write_int64 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_int64(self, val);
}

hpint32 write_uint8(HPAbstractWriter *self, const hpuint8 val)
{
	if(self->write_uint8 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_uint8(self, val);
}

hpint32 write_uint16(HPAbstractWriter *self, const hpuint16 val)
{
	if(self->write_uint16 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_uint16(self, val);
}

hpint32 write_uint32(HPAbstractWriter *self, const hpuint32 val)
{
	if(self->write_uint32 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_uint32(self, val);
}

hpint32 write_uint64(HPAbstractWriter *self, const hpuint64 val)
{
	if(self->write_uint64 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_uint64(self, val);
}

hpint32 write_char(HPAbstractWriter *self, const hpchar val)
{
	if(self->write_char == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_char(self, val);
}

hpint32 write_double(HPAbstractWriter *self, const hpdouble val)
{
	if(self->write_double == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_double(self, val);
}

hpint32 write_enum(HPAbstractWriter *self, const hpint32 val)
{
	if(self->write_enum == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_enum(self, val);
}

hpint32 write_enum_name(HPAbstractWriter *self, const hpchar *enum_name)
{
	if(self->write_enum_name == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_enum_name(self, enum_name);
}

hpint32 write_string(HPAbstractWriter *self, const hpchar* str)
{
	if(self->write_string == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_string(self, str);
}

hpint32 write_bytes(HPAbstractWriter *self, const hpbytes bytes)
{
	if(self->write_bytes == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_bytes(self, bytes);
}

hpint32 write_bool(HPAbstractWriter *self, const hpbool val)
{
	if(self->write_bool == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_bool(self, val);
}

hpint32 write_null(HPAbstractWriter *self)
{
	if(self->write_null == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_null(self);
}

hpint32 write_semicolon(HPAbstractWriter *self)
{
	if(self->write_semicolon == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_semicolon(self);
}

hpint32 write_type(HPAbstractWriter *self, const HPType type)
{
	if(self->write_type == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_type(self, type);
}

hpint32 write_vector_item_begin(HPAbstractWriter *self, hpuint32 index)
{
	self->stack[self->stack_num - 1] = index;

	if(self->write_vector_item_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_vector_item_begin(self, index);
}

hpint32 write_vector_item_end(HPAbstractWriter *self, hpuint32 index)
{
	self->stack[self->stack_num - 1] = index + 1;
	if(self->write_vector_item_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->write_vector_item_end(self, index);
}

hpuint32 writer_get_index(HPAbstractWriter *self)
{
	return self->stack[self->stack_num - 1];
}