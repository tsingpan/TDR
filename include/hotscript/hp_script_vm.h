#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotplatform/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include "hotscript/hp_script_op.h"
#include "hotlib/hp_error_msg_reader.h"

#include "hotscript/hp_script_parser.h"



#define MAX_VM_HO_SIZE 1024


typedef struct _HotVM HotVM;
typedef void (*vm_user_putc)(HotVM *self, char c);

#define MAX_VECTOR_STACK_DEEP 1024
typedef hpint32 (*hotvm_execute_func)(HotVM *self, const HotOp* op);

#define HS_MAX_FILE_DEEP 65536

typedef struct _STACK_FRAME
{
	HotOpArr hotoparr;
	HotOp *start;
	HotOp *limit;
	hpuint32 eip;
}STACK_FRAME;

struct _HotVM
{
	//这里搞个堆栈
	STACK_FRAME stack[HS_MAX_FILE_DEEP];
	hpuint32 stack_num;

	//这玩意真心很大
	SCRIPT_PARSER sp;

	//这里加入一个script_parser
	HPAbstractReader *reader;
	void *user_data;
	vm_user_putc uputc;

	const char *root_dir;
	const char *working_dir;


	hotvm_execute_func op_handler[HOT_MAX];	
	hpuint32 vector_stack[MAX_VECTOR_STACK_DEEP];
	hpuint32 vector_stack_num;

	hpint32 result;
	char result_str[MAX_ERROR_MSG_LENGTH];
};

hpint32 hotvm_execute(HotVM *self, const char* file_name, 
					  const char* root_dir, HPAbstractReader *reader, void *user_data, vm_user_putc uputc
					  ,const char *work_dir);

void hotvm_set_eip(HotVM *self, hpuint32 eip);

hpuint32 hotvm_get_eip(HotVM *self);

void hotvm_push(HotVM *self, hpuint32 eip, const HotOpArr *hotoparr);

const HotOpArr *hotvm_get_op(HotVM *self);

void hotvm_pop(HotVM *self);

#endif//_H_HOT_VM

