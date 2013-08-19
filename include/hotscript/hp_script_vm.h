#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include "hotscript/hp_script_op.h"
#include "hotscript/hp_error_msg_reader.h"



#define MAX_VM_HO_SIZE 1024


hpint32 hotoparr_init(HotOpArr *self);

HotOp *hotoparr_get_next_op(HotOpArr *self);

hpuint32 hotoparr_get_next_op_number(HotOpArr *self);

typedef struct _HotVM HotVM;
typedef void (*vm_user_putc)(HotVM *self, char c);

#define MAX_VECTOR_STACK_DEEP 1024
typedef hpint32 (*hotvm_execute_func)(HotVM *self, const HotOp* op);

#define HS_MAX_NAME_LENGTH 512
typedef struct _STACK_FRAME
{
	hpuint32 eax;
}STACK_FRAME;

struct _HotVM
{
	//这里搞个堆栈
	const HotOpArr *hotoparr;
	hpuint32 eip;

	//这里加入一个script_parser
	HPAbstractReader *reader;
	void *user_data;
	vm_user_putc uputc;


	hotvm_execute_func op_handler[HOT_MAX];	
	STACK_FRAME vector_stack[MAX_VECTOR_STACK_DEEP];
	hpuint32 vector_stack_num;

	hpint32 result;
	char result_str[MAX_ERROR_MSG_LENGTH];
};


hpint32 hotvm_execute(HotVM *self, const char* file_name, const char* root_dir, HPAbstractReader *reader, void *user_data, vm_user_putc uputc);

#endif//_H_HOT_VM

