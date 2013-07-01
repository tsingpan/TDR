#ifndef _H_DDEKIT_ENCODING_READER
#define _H_DDEKIT_ENCODING_READER

#include "hotpot/hp_platform.h"

#ifndef _DEF_DDEKIT_ENCODING_READER
#define _DEF_DDEKIT_ENCODING_READER
typedef struct tagDDEKIT_ENCODING_READER DDEKIT_ENCODING_READER;
#endif//_DEF_DDEKIT_ENCODING_READER

struct tagDDEKIT_ENCODING_READER
{
	hpint32 (*read_struct_begin)(DDEKIT_ENCODING_READER *self, const char *struct_name);
	hpint32 (*read_struct_end)(DDEKIT_ENCODING_READER *self, const char *struct_name);
	hpint32 (*read_repeat_begin)(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*read_repeat_end)(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*read_field_begin)(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);
	hpint32 (*read_field_end)(DDEKIT_ENCODING_READER *self, const char *var_name, hpint32 var_type);
	
	hpint32 (*read_hpint8)(DDEKIT_ENCODING_READER *self, hpint8 *val);
	hpint32 (*read_hpint16)(DDEKIT_ENCODING_READER *self, hpint16 *val);
	hpint32 (*read_hpint32)(DDEKIT_ENCODING_READER *self, hpint32 *val);
	hpint32 (*read_hpint64)(DDEKIT_ENCODING_READER *self, hpint64 *val);
	hpint32 (*read_hpuint8)(DDEKIT_ENCODING_READER *self, hpuint8 *val);
	hpint32 (*read_hpuint16)(DDEKIT_ENCODING_READER *self, hpuint16 *val);
	hpint32 (*read_hpuint32)(DDEKIT_ENCODING_READER *self, hpuint32 *val);
	hpint32 (*read_hpuint64)(DDEKIT_ENCODING_READER *self, hpuint64 *val);

	hpint32 (*read_enum)(DDEKIT_ENCODING_READER *self, hpint32 *val, hpchar *enum_name, hpuint32 *enum_name_length);
	hpint32 (*read_hpchar)(DDEKIT_ENCODING_READER *self, hpchar *val);
	hpint32 (*read_hpdouble)(DDEKIT_ENCODING_READER *self, hpdouble *val);
	hpint32 (*read_hpstring)(DDEKIT_ENCODING_READER *self, hpchar* str);
	hpint32 (*read_bytes)(DDEKIT_ENCODING_READER *self, hpchar* buff, const hpuint32 buff_size);
};

#endif//_H_DDEKIT_ENCODING_READER
