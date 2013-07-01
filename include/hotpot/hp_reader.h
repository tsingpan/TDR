#ifndef _H_HP_READR
#define _H_HP_READR

#include "hotpot/hp_platform.h"
#include "hotpot/hp_value.h"

typedef struct _HPAbstractReader HPAbstractReader;
typedef hpint32 (*func_hp_reader_begin)(HPAbstractReader* self, const HPVar *name);
typedef hpint32 (*func_hp_reader_read)(HPAbstractReader* self, HPVar *var);
typedef hpint32 (*func_hp_reader_end)(HPAbstractReader* self);

typedef struct _HPReaderStackNode
{
	int count;
}HPReaderStackNode;

#define READER_STACK_DEEP 1024
struct _HPAbstractReader
{
	func_hp_reader_begin begin;
	func_hp_reader_read read;
	func_hp_reader_end end;

	HPReaderStackNode stack[READER_STACK_DEEP];
	hpuint32 stack_num;

	//

	hpint32 (*read_object_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_object_end)(HPAbstractReader *self, const hpstring name);

	hpint32 (*read_field_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_field_end)(HPAbstractReader *self, const hpstring name);

	hpint32 (*read_vector_begin)(HPAbstractReader *self, const hpstring name);
	hpint32 (*read_vector_end)(HPAbstractReader *self, const hpstring name);
};

hpint32 hp_reader_init(HPAbstractReader *self);

hpint32 hp_reader_begin(HPAbstractReader *self, const HPVar *name);

hpint32 hp_reader_read(HPAbstractReader* self, HPVar *var);

hpint32 hp_reader_end(HPAbstractReader *self);






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

	hpint32 (*read_enum)(DDEKIT_ENCODING_READER *self, hpint32 *val);
	hpint32 (*read_enum_name)(DDEKIT_ENCODING_READER *self, hpchar *enum_name, hpuint32 *enum_name_length);
	hpint32 (*read_hpchar)(DDEKIT_ENCODING_READER *self, hpchar *val);
	hpint32 (*read_hpdouble)(DDEKIT_ENCODING_READER *self, hpdouble *val);
	hpint32 (*read_hpstring)(DDEKIT_ENCODING_READER *self, hpchar* str);
	hpint32 (*read_bytes)(DDEKIT_ENCODING_READER *self, hpchar* buff, const hpuint32 buff_size);
};

#endif //_H_HP_READR

