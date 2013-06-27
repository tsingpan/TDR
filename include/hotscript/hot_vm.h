#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"
#include "hotpot/hp_reader.h"
#include "hotpot/hp_value.h"

#define HOT_ECHO                               0
#define HOT_ECHO_LITERAL                       1			//这玩意要进行转义
#define HOT_PUSH							   2
#define HOT_PUSH_INDEX						   3
#define HOT_POP								   4
#define HOT_ECHO_TRIE						   5
#define HOT_JMP								   6

//虚拟机操作数
typedef struct _HotOperand
{
	hpint32 num;
	const char *str;
	hpuint32 str_len;
}HotOperand;


//虚拟机指令
typedef struct _HotOp
{
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

