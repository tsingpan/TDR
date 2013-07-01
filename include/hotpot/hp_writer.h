#ifndef _H_HP_WRITER
#define _H_HP_WRITER

#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _HPAbstractWriter HPAbstractWriter;
struct _HPAbstractWriter
{
	hpint32 (*write_struct_begin)(HPAbstractWriter *self, const char *struct_name);
	hpint32 (*write_struct_end)(HPAbstractWriter *self, const char *struct_name);
	hpint32 (*write_vector_begin)(HPAbstractWriter *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*write_vector_end)(HPAbstractWriter *self, const char *var_name, hpint32 var_type, hpint32 end_with_zero);
	hpint32 (*write_field_begin)(HPAbstractWriter *self, const char *var_name, hpint32 var_type);
	hpint32 (*write_field_end)(HPAbstractWriter *self, const char *var_name, hpint32 var_type);	

	hpint32 (*write_hpint8)(HPAbstractWriter *self, const hpint8 val);
	hpint32 (*write_hpint16)(HPAbstractWriter *self, const hpint16 val);
	hpint32 (*write_hpint32)(HPAbstractWriter *self, const hpint32 val);
	hpint32 (*write_hpint64)(HPAbstractWriter *self, const hpint64 val);
	hpint32 (*write_hpuint8)(HPAbstractWriter *self, const hpuint8 val);
	hpint32 (*write_hpuint16)(HPAbstractWriter *self, const hpuint16 val);
	hpint32 (*write_hpuint32)(HPAbstractWriter *self, const hpuint32 val);
	hpint32 (*write_hpuint64)(HPAbstractWriter *self, const hpuint64 val);

	hpint32 (*write_hpchar)(HPAbstractWriter *self, const hpchar val);	
	hpint32 (*write_hpdouble)(HPAbstractWriter *self, const hpdouble val);
	hpint32 (*write_enum)(HPAbstractWriter *self);
	hpint32 (*write_enum_name)(HPAbstractWriter *self, const hpint32 val, const hpchar *enum_name);
	hpint32 (*write_hpstring)(HPAbstractWriter *self, const hpchar* str);
	hpint32 (*write_bytes)(HPAbstractWriter *self, const hpchar* buff, const hpuint32 buff_size);
};

#endif //_H_HP_WRITER

