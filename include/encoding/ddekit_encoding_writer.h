#ifndef _H_DDEKIT_ENCODING_WRITER
#define _H_DDEKIT_ENCODING_WRITER

#include "hotpot/hp_platform.h"

#ifndef _DEF_DDEKIT_ENCODING_WRITER
#define _DEF_DDEKIT_ENCODING_WRITER
typedef struct tagDDEKIT_ENCODING_WRITER DDEKIT_ENCODING_WRITER;
#endif//_DEF_DDEKIT_ENCODING_WRITER


struct tagDDEKIT_ENCODING_WRITER
{
	hpint32 (*write_struct_begin)(DDEKIT_ENCODING_WRITER *self, const char *struct_name);
	hpint32 (*write_struct_end)(DDEKIT_ENCODING_WRITER *self, const char *struct_name);
	hpint32 (*write_vector_begin)(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*write_vector_end)(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*write_field_begin)(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type);
	hpint32 (*write_field_end)(DDEKIT_ENCODING_WRITER *self, const char *var_name, hpint32 var_type);	

	hpint32 (*write_hpint8)(DDEKIT_ENCODING_WRITER *self, const hpint8 val);
	hpint32 (*write_hpint16)(DDEKIT_ENCODING_WRITER *self, const hpint16 val);
	hpint32 (*write_hpint32)(DDEKIT_ENCODING_WRITER *self, const hpint32 val);
	hpint32 (*write_hpint64)(DDEKIT_ENCODING_WRITER *self, const hpint64 val);
	hpint32 (*write_hpuint8)(DDEKIT_ENCODING_WRITER *self, const hpuint8 val);
	hpint32 (*write_hpuint16)(DDEKIT_ENCODING_WRITER *self, const hpuint16 val);
	hpint32 (*write_hpuint32)(DDEKIT_ENCODING_WRITER *self, const hpuint32 val);
	hpint32 (*write_hpuint64)(DDEKIT_ENCODING_WRITER *self, const hpuint64 val);

	hpint32 (*write_hpchar)(DDEKIT_ENCODING_WRITER *self, const hpchar val);	
	hpint32 (*write_hpdouble)(DDEKIT_ENCODING_WRITER *self, const hpdouble val);
	hpint32 (*write_enum)(DDEKIT_ENCODING_WRITER *self);
	hpint32 (*write_enum_name)(DDEKIT_ENCODING_WRITER *self, const hpint32 val, const hpchar *enum_name);
	hpint32 (*write_hpstring)(DDEKIT_ENCODING_WRITER *self, const hpchar* str);
	hpint32 (*write_bytes)(DDEKIT_ENCODING_WRITER *self, const hpchar* buff, const hpuint32 buff_size);
};

#endif//_H_DDEKIT_ENCODING_WRITER
