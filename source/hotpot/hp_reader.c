#include "hotpot/hp_reader.h"
#include "hotpot/hp_error.h"


hpint32 read_struct_begin(HPAbstractReader *self, const char *struct_name)
{
	if(self->read_struct_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_struct_begin(self, struct_name);
}

hpint32 read_struct_end(HPAbstractReader *self, const char *struct_name)
{
	if(self->read_struct_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_struct_end(self, struct_name);
}

hpint32 read_vector_begin(HPAbstractReader *self)
{
	if(self->read_vector_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_vector_begin(self);
}

hpint32 read_vector_end(HPAbstractReader *self)
{
	if(self->read_vector_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_vector_end(self);
}

hpint32 read_field_begin(HPAbstractReader *self, const char *var_name, hpuint32 len)
{
	if(self->read_field_begin == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_field_begin(self, var_name, len);
}

hpint32 read_field_end(HPAbstractReader *self, const char *var_name, hpuint32 len)
{
	if(self->read_field_end == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_field_end(self, var_name, len);
}

hpint32 read_hpint8(HPAbstractReader *self, hpint8 *val)
{
	if(self->read_hpint8 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpint8(self, val);
}

hpint32 read_hpint16(HPAbstractReader *self, hpint16 *val)
{
	if(self->read_hpint16 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpint16(self, val);
}

hpint32 read_hpint32(HPAbstractReader *self, hpint32 *val)
{
	if(self->read_hpint32 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpint32(self, val);
}

hpint32 read_hpint64(HPAbstractReader *self, hpint64 *val)
{
	if(self->read_hpint64 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpint64(self, val);
}

hpint32 read_hpuint8(HPAbstractReader *self, hpuint8 *val)
{
	if(self->read_hpuint8 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpuint8(self, val);
}

hpint32 read_hpuint16(HPAbstractReader *self, hpuint16 *val)
{
	if(self->read_hpuint16 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpuint16(self, val);
}

hpint32 read_hpuint32(HPAbstractReader *self, hpuint32 *val)
{
	if(self->read_hpuint32 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpuint32(self, val);
}

hpint32 read_hpuint64(HPAbstractReader *self, hpuint64 *val)
{
	if(self->read_hpuint64 == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpuint64(self, val);
}

hpint32 read_enum(HPAbstractReader *self, hpint32 *val)
{
	if(self->read_enum == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_enum(self, val);
}

hpint32 read_enum_name(HPAbstractReader *self, hpchar *enum_name, hpuint32 *enum_name_length)
{
	if(self->read_enum_name == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_enum_name(self, enum_name, enum_name_length);
}

hpint32 read_hpchar(HPAbstractReader *self, hpchar *val)
{
	if(self->read_hpchar == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpchar(self, val);
}

hpint32 read_hpdouble(HPAbstractReader *self, hpdouble *val)
{
	if(self->read_hpdouble == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpdouble(self, val);
}

hpint32 read_hpstring(HPAbstractReader *self, hpchar* str, hpuint32 *str_length)
{
	if(self->read_hpstring == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpstring(self, str, str_length);
}

hpint32 read_bytes(HPAbstractReader *self, hpchar* buff, hpuint32 *buff_size)
{
	if(self->read_bytes == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_bytes(self, buff, buff_size);
}

hpint32 read_bool(HPAbstractReader *self, hpbool *val)
{
	if(self->read_hpbool == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_hpbool(self, val);
}

hpint32 read_null(HPAbstractReader *self)
{
	if(self->read_null == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_null(self);
}

hpint32 read_semicolon(HPAbstractReader *self)
{
	if(self->read_semicolon == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_semicolon(self);
}

hpint32 read_type(HPAbstractReader *self, HPType *type)
{
	if(self->read_type == NULL)
	{
		return E_HP_NOERROR;
	}
	return self->read_type(self, type);
}
