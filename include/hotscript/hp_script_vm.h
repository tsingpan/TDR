#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include "hotscript/hp_script_op.h"
#include "hotlib/hp_error_msg_reader.h"

#include "hotscript/hp_script_parser.h"



#define MAX_VM_HO_SIZE 1024


typedef struct _HotVM HotVM;
typedef void (*vm_user_putc)(HotVM *self, char c);

#define MAX_VECTOR_STACK_DEEP 1024
typedef tint32 (*hotvm_execute_func)(HotVM *self, const HotOp* op);

#define HS_MAX_FILE_DEEP 65536

typedef struct _STACK_FRAME
{
	//HotOpArr hotoparr;
	const HotOp *start;
	const HotOp *limit;
	tuint32 eip;
}STACK_FRAME;

#define HOTVM_MAX_OP_SIZE 65536
struct _HotVM
{
	//这里搞个堆栈
	STACK_FRAME stack[HS_MAX_FILE_DEEP];
	tuint32 stack_num;

	//这玩意真心很大
	SCRIPT_PARSER sp;

	//这里加入一个script_parser
	TLIBC_ABSTRACT_READER *reader;
	void *user_data;
	vm_user_putc uputc;

	const char *root_dir;
	const char *working_dir;


	hotvm_execute_func op_handler[HOT_MAX];	
	tuint32 vector_stack[MAX_VECTOR_STACK_DEEP];
	tuint32 vector_stack_num;

	tint32 result;
	char result_str[MAX_ERROR_MSG_LENGTH];

	HotOp oparr[HOTVM_MAX_OP_SIZE];
	size_t oparr_size;
};

tint32 hotvm_execute(HotVM *self, const char* file_name, 
					  const char* root_dir, TLIBC_ABSTRACT_READER *reader, void *user_data, vm_user_putc uputc
					  ,const char *work_dir);

void hotvm_set_eip(HotVM *self, tuint32 eip);

tuint32 hotvm_get_eip(HotVM *self);

void hotvm_push(HotVM *self, tuint32 eip, const HotOp *start_op, const HotOp *limit_op);

const HotOp *hotvm_get_op(HotVM *self);

int hotvm_get_oplimit(HotVM *self);

void hotvm_pop(HotVM *self);

#endif//_H_HOT_VM

