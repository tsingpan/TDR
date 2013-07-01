#include "encoding/ddekit_json_encoding_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotpot/hp_writer.h"
#include <string.h>
#include <assert.h>

hpint32 ddekit_json_encoding_writer_init(DDEKIT_JSON_ENCODING_WRITER *self, void *addr, hpuint32 size)
{
	self->dpw.write_struct_begin = ddekit_json_encoding_write_struct_begin;
	self->dpw.write_struct_end = ddekit_json_encoding_write_struct_end;
	self->dpw.write_vector_begin = ddekit_json_encoding_write_vector_begin;
	self->dpw.write_vector_end = ddekit_json_encoding_write_vector_end;
	self->dpw.write_field_begin = ddekit_json_encoding_write_field_begin;
	self->dpw.write_field_end = ddekit_json_encoding_write_field_end;
	self->dpw.write_enum = ddekit_json_encoding_write_enum;
	self->dpw.write_hpchar = ddekit_json_encoding_write_hpchar;
	self->dpw.write_hpdouble = ddekit_json_encoding_write_hpdouble;
	self->dpw.write_hpint8 = ddekit_json_encoding_write_hpint8;
	self->dpw.write_hpint16 = ddekit_json_encoding_write_hpint16;
	self->dpw.write_hpint32 = ddekit_json_encoding_write_hpint32;
	self->dpw.write_hpint64 = ddekit_json_encoding_write_hpint64;
	self->dpw.write_hpuint8 = ddekit_json_encoding_write_hpuint8;
	self->dpw.write_hpuint16 = ddekit_json_encoding_write_hpuint16;
	self->dpw.write_hpuint32 = ddekit_json_encoding_write_hpuint32;
	self->dpw.write_hpuint64 = ddekit_json_encoding_write_hpuint64;
	

	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_writer_fini(DDEKIT_JSON_ENCODING_WRITER *self)
{
	self->dpw.write_struct_begin = NULL;
	self->dpw.write_struct_end = NULL;
	self->dpw.write_field_begin = NULL;
	self->dpw.write_field_end = NULL;
	self->dpw.write_vector_begin = NULL;
	self->dpw.write_vector_end = NULL;
	self->dpw.write_enum = NULL;
	self->dpw.write_hpchar = NULL;
	self->dpw.write_hpdouble = NULL;
	self->dpw.write_hpint8 = NULL;
	self->dpw.write_hpint16 = NULL;
	self->dpw.write_hpint32 = NULL;
	self->dpw.write_hpint64 = NULL;
	self->dpw.write_hpuint8 = NULL;
	self->dpw.write_hpuint16 = NULL;
	self->dpw.write_hpuint32 = NULL;
	self->dpw.write_hpuint64 = NULL;


	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_vector_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_vector_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_field_begin(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_field_end(HPAbstractWriter *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_enum(HPAbstractWriter *super, const hpint32 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpchar(HPAbstractWriter *super, const char val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpdouble(HPAbstractWriter *super, const double val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	DDEKIT_JSON_ENCODING_WRITER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_WRITER, dpw);
	return E_HP_NOERROR;
}
