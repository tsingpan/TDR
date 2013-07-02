#include "encoding/ddekit_json_encoding_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotpot/hp_reader.h"

#include <string.h>
#include <assert.h>

hpint32 ddekit_json_encoding_reader_init(DDEKIT_JSON_ENCODING_READER *self, const void *addr, hpuint32 size)
{
	self->dpr.read_struct_begin = ddekit_json_encoding_read_struct_begin;
	self->dpr.read_struct_end = ddekit_json_encoding_read_struct_end;
	self->dpr.read_vector_begin = ddekit_json_encoding_read_vector_begin;
	self->dpr.read_vector_end = ddekit_json_encoding_read_vector_end;
	self->dpr.read_field_begin = ddekit_json_encoding_read_field_begin;
	self->dpr.read_field_end = ddekit_json_encoding_read_field_end;
	self->dpr.read_enum = ddekit_json_encoding_read_enum;
	self->dpr.read_hpchar = ddekit_json_encoding_read_hpchar;
	self->dpr.read_hpdouble = ddekit_json_encoding_read_hpdouble;
	self->dpr.read_hpint8 = ddekit_json_encoding_read_hpint8;
	self->dpr.read_hpint16 = ddekit_json_encoding_read_hpint16;
	self->dpr.read_hpint32 = ddekit_json_encoding_read_hpint32;
	self->dpr.read_hpint64 = ddekit_json_encoding_read_hpint64;
	self->dpr.read_hpuint8 = ddekit_json_encoding_read_hpuint8;
	self->dpr.read_hpuint16 = ddekit_json_encoding_read_hpuint16;
	self->dpr.read_hpuint32 = ddekit_json_encoding_read_hpuint32;
	self->dpr.read_hpuint64 = ddekit_json_encoding_read_hpuint64;




	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_reader_fini(DDEKIT_JSON_ENCODING_READER *self)
{
	self->dpr.read_struct_begin = NULL;
	self->dpr.read_struct_end = NULL;
	self->dpr.read_vector_begin = NULL;
	self->dpr.read_vector_end = NULL;
	self->dpr.read_field_begin = NULL;
	self->dpr.read_field_end = NULL;
	self->dpr.read_enum = NULL;
	self->dpr.read_hpchar = NULL;
	self->dpr.read_hpdouble = NULL;
	self->dpr.read_hpint8 = NULL;
	self->dpr.read_hpint16 = NULL;
	self->dpr.read_hpint32 = NULL;
	self->dpr.read_hpint64 = NULL;
	self->dpr.read_hpuint8 = NULL;
	self->dpr.read_hpuint16 = NULL;
	self->dpr.read_hpuint32 = NULL;
	self->dpr.read_hpuint64 = NULL;


	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_struct_begin(HPAbstractReader *super, const char *struct_name)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_struct_end(HPAbstractReader *super, const char *struct_name)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_vector_begin(HPAbstractReader *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_vector_end(HPAbstractReader *super, const char *var_name, hpint32 var_type, hpint32 end_with_zero)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_field_begin(HPAbstractReader *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_field_end(HPAbstractReader *super, const char *var_name, hpint32 var_type)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_read_enum(HPAbstractReader *super, hpint32 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

HP_API hpint32 ddekit_json_encoding_read_enum_name(HPAbstractReader *super, hpchar *enum_name, hpuint32 *enum_name_length)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_json_encoding_read_hpchar(HPAbstractReader *super, char *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpdouble(HPAbstractReader *super, double *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}



hpint32 ddekit_json_encoding_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}



hpint32 ddekit_json_encoding_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}


hpint32 ddekit_json_encoding_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}



hpint32 ddekit_json_encoding_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	DDEKIT_JSON_ENCODING_READER *self = HP_CONTAINER_OF(super, DDEKIT_JSON_ENCODING_READER, dpr);
	return E_HP_NOERROR;
}
