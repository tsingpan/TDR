#ifndef _H_HOT_SCRIPT_OP
#define _H_HOT_SCRIPT_OP


#include "hotplatform/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
typedef enum _HOTSCRIPT_INSTRUCT
{
	HOT_ECHO                               = 0,
	HOT_FIELD_BEGIN						   = 1,
	HOT_FIELD_END						   = 2,
	HOT_VECTOR_BEGIN					   = 3,
	HOT_VECTOR_END						   = 4,
	HOT_VECTOR_SET_INDEX				   = 5,
	HOT_VECTOR_INC_INDEX				   = 6,
	HOT_VECTOR_ITEM_BEGIN				   = 7,
	HOT_VECTOR_ITEM_END					   = 8,
	HOT_ECHO_FIELD						   = 9,
	HOT_JMP								   = 10,
	HOT_ECHO_LITERAL                       = 11,
	HOT_IMPORT		                       = 12,
	HOT_MAX
}HOTSCRIPT_INSTRUCT;

typedef struct _HOT_ECHO_ARG
{
	hpbytes bytes;
}HOT_ECHO_ARG;

#define HS_MAX_NAME_LENGTH 512
typedef struct _HOT_IMPORT_ARG
{
	char file_name[HS_MAX_NAME_LENGTH];
}HOT_IMPORT_ARG;

typedef struct _HOT_FIELD_BEGIN_ARG
{	
	hpbytes name;
	hpuint32 lineno_after_field_end;
}HOT_FIELD_BEGIN_ARG;

typedef struct _HOT_CALL_FIELD_BEGIN_ARG
{	
	hpbytes name;
	hpuint32 lineno_after_call_field_begin;
}HOT_CALL_FIELD_BEGIN_ARG;

typedef struct _HOT_VECTOR_BEGIN_ARG
{
	hpuint32 failed_jmp_lineno;
}HOT_VECTOR_BEGIN_ARG;

typedef struct _HOT_VECTOR_SET_INDEX_ARG
{
	hpuint32 index;
}HOT_VECTOR_SET_INDEX_ARG;

typedef struct _HOT_VECTOR_SEEK_ARG
{
	hpuint32 failed_jmp_lineno;
}HOT_VECTOR_ITEM_BEGIN_ARG;

typedef struct _HOT_JMP_ARG
{
	hpuint32 lineno;
}HOT_JMP_ARG;

typedef union _HOTSCRIPT_ARGUMENT
{
	HOT_ECHO_ARG	echo_arg;
	HOT_FIELD_BEGIN_ARG field_begin_arg;
	HOT_CALL_FIELD_BEGIN_ARG call_field_begin_arg;
	HOT_VECTOR_BEGIN_ARG vector_begin_arg;
	HOT_VECTOR_SET_INDEX_ARG vector_set_index_arg;
	HOT_VECTOR_ITEM_BEGIN_ARG vector_item_begin_arg;
	HOT_JMP_ARG jmp_arg;
	HOT_IMPORT_ARG import_arg;
}HOTSCRIPT_ARGUMENT;

//ÐéÄâ»úÖ¸Áî
typedef struct _HotOp
{
	HOTSCRIPT_INSTRUCT instruct;
	HOTSCRIPT_ARGUMENT arg;
	hpuint32 lineno;
}HotOp;

#define NORMAL_OP_SIZE 1024

typedef struct _HotOpArr
{
	HotOp oparr[NORMAL_OP_SIZE];
	hpuint32 oparr_size;
	hpuint32 next_oparr;
}HotOpArr;

void hotoparr_init(HotOpArr *self);

void hotoparr_fini(HotOpArr *self);

HotOp *hotoparr_get_next_op(HotOpArr *self);

hpuint32 hotoparr_get_next_op_number(HotOpArr *self);


#endif//_H_HOT_SCRIPT_OP

