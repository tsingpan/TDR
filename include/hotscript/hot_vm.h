#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include "hotpot/hp_value.h"

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
	HOT_CALL_FIELD						   = 12,
	HOT_MAX
}HOTSCRIPT_INSTRUCT;

typedef struct _HOT_ECHO_ARG
{
	hpbytes bytes;
}HOT_ECHO_ARG;

typedef struct _HOT_FIELD_BEGIN_ARG
{	
	hpbytes name;
	hpuint32 lineno_after_field_end;
}HOT_FIELD_BEGIN_ARG;

typedef struct _HOT_CALL_FIELD_ARG
{	
	hpbytes name;
	hpuint32 lineno_after_field_end;
}HOT_CALL_FIELD_ARG;

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
	HOT_CALL_FIELD_ARG call_field_arg;
	HOT_VECTOR_BEGIN_ARG vector_begin_arg;
	HOT_VECTOR_SET_INDEX_ARG vector_set_index_arg;
	HOT_VECTOR_ITEM_BEGIN_ARG vector_item_begin_arg;
	HOT_JMP_ARG jmp_arg;
}HOTSCRIPT_ARGUMENT;

//ÐéÄâ»úÖ¸Áî
typedef struct _HotOp
{
	HOTSCRIPT_INSTRUCT instruct;
	HOTSCRIPT_ARGUMENT arg;
	hpuint32 lineno;
}HotOp;

typedef struct _HotOpArr
{
	HotOp *oparr;
	hpuint32 oparr_size;
	hpuint32 next_oparr;
}HotOpArr;

#define MAX_VM_HO_SIZE 1024


hpint32 hotoparr_init(HotOpArr *self);

HotOp *hotoparr_get_next_op(HotOpArr *self);

hpuint32 hotoparr_get_next_op_number(HotOpArr *self);

typedef struct _HotVM HotVM;
typedef void (*vm_user_putc)(HotVM *self, char c);

#define MAX_FUNCTION_STACK_DEEP 1024
typedef hpint32 (*hotvm_execute_func)(HotVM *self, const HotOp* op);

#define HS_MAX_NAME_LENGTH 512
typedef struct _STACK_FRAME
{
	hpuint32 eax;
	hpuint32 eip;
	hpuint32 return_eip;
}STACK_FRAME;

struct _HotVM
{
	const HotOpArr *hotoparr;
	HPAbstractReader *reader;
	void *user_data;
	vm_user_putc uputc;
	hotvm_execute_func op_handler[HOT_MAX];
	
	
	
	hpuint32 eip;
	hpuint32 eax;
	STACK_FRAME stack[MAX_FUNCTION_STACK_DEEP];
	hpuint32 stack_num;
};


hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader, void *user_data, vm_user_putc uputc);

#endif//_H_HOT_VM

