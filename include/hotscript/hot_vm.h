#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_value.h"

typedef enum _HOTSCRIPT_INSTRUCT
{
	HOT_ECHO                               = 0,
	HOT_ECHO_LITERAL                       = 1,//即将取消
	HOT_FIELD_BEGIN						   = 2,
	HOT_FIELD_END						   = 3,
	HOT_VECTOR_BEGIN					   = 4,
	HOT_VECTOR_END						   = 5,
	HOT_VECTOR_SEEK						   = 6,
	HOT_ECHO_FIELD						   = 7,
	HOT_JMP								   = 8,
}HOTSCRIPT_INSTRUCT;

typedef struct _HOT_ECHO_ARG
{
	hpuint32 lineno;
}HOT_ECHO_ARG;

typedef struct _HOT_ECHO_LITERAL_ARG
{
	hpuint32 lineno;
}HOT_ECHO_LITERAL_ARG;

typedef struct _HOT_FIELD_BEGIN_ARG
{
	hpuint32 lineno;
}HOT_FIELD_BEGIN_ARG;

typedef struct _HOT_FIELD_END_ARG
{
	hpuint32 lineno;
}HOT_FIELD_END_ARG;

typedef struct _HOT_VECTOR_BEGIN_ARG
{
	hpuint32 lineno;
}HOT_VECTOR_BEGIN_ARG;

typedef struct _HOT_VECTOR_END_ARG
{
	hpuint32 lineno;
}HOT_VECTOR_END_ARG;

typedef struct _HOT_VECTOR_SEEK_ARG
{
	hpuint32 lineno;
}HOT_VECTOR_SEEK_ARG;

typedef struct _HOT_ECHO_FIELD_ARG
{
	hpuint32 lineno;
}HOT_ECHO_FIELD_ARG;

typedef struct _HOT_JMP_ARG
{
	hpuint32 lineno;
}HOT_JMP_ARG;

typedef union _HOTSCRIPT_ARGUMENT
{
	HOT_ECHO_ARG	echo_arg;
	HOT_ECHO_LITERAL_ARG literal_arg;
	HOT_FIELD_BEGIN_ARG field_begin_arg;
	HOT_FIELD_END_ARG field_end_arg;
	HOT_VECTOR_BEGIN_ARG vector_begin_arg;
	HOT_VECTOR_SEEK_ARG vector_seek_arg;
	HOT_VECTOR_END_ARG vector_end_arg;
	HOT_ECHO_FIELD_ARG echo_field_arg;
	HOT_JMP_ARG jmp_arg;
}HOTSCRIPT_ARGUMENT;

//虚拟机指令
typedef struct _HotOp
{
	HOTSCRIPT_INSTRUCT instruct;
	HOTSCRIPT_ARGUMENT arg;
	//以下即将取消
	hpint32 op;
	HPVar op0;
	HPVar op1;
	HPVar op2;
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
struct _HotVM
{
	const HotOpArr *hotoparr;

	hpuint32 current_op;

	void *user_data;

	vm_user_putc uputc;

	HPAbstractReader *reader;
};

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader, void *user_data, vm_user_putc uputc);

#endif//_H_HOT_VM

