#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"


#define HOT_ECHO                               0

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
	HotOperand op0;
	HotOperand op1;
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



#define MAX_FUNCTION_STACK_DEEP 1024
typedef struct _HotVM
{
	const HotOpArr *hotoparr;

	hpuint32 current_op;


	//函数调用堆栈
	hpuint32 function_stack[MAX_FUNCTION_STACK_DEEP];
	hpuint32 function_stack_num;
}HotVM;

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr);

#endif//_H_HOT_VM

