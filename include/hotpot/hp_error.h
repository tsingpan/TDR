#ifndef _H_HP_ERROR
#define _H_HP_ERROR

//一个不存在的错误码
#define HP_INVALID_ERROR_CODE 0xffffffff

//hotpot错误码区间表
typedef enum tagHP_HOTPOT_ERROR_CODE_SECTION
{
	E_HP_COMMON_SECTION														= 0,
	E_HP_NUMBER_SECTION														= 20000,
	E_HP_HOTDATA_SECTION													= 30000,
}HP_HOTPOT_ERROR_CODE_SECTION;


//公用错误码区间
typedef enum tagHP_HOTPOT_ERROR_CODE_COMMON
{
	E_HP_NOERROR                                                           = -E_HP_COMMON_SECTION,
	E_HP_ERROR                                                             = -E_HP_COMMON_SECTION - 1,
	E_HP_BAD_PARAMETER                                                     = -E_HP_COMMON_SECTION - 2,
	E_HP_SYNTAX_ERROR													   = -E_HP_COMMON_SECTION - 3,
}HP_HOTPOT_ERROR_CODE_COMMON;


//number错误区间
typedef enum tagHP_HOTPOT_ERROR_CODE_NUMBER
{
	E_HP_NOT_ENOUGH_BYTEBUFF_SIZE                                          = -E_HP_NUMBER_SECTION,
}HP_HOTPOT_ERROR_CODE_NUMBER;


//hotdata错误区间
typedef enum _HP_HOTDATA_ERROR_CODE
{	
	E_HP_CONSTANT_REDEFINITION												= -E_HP_HOTDATA_SECTION - 1,
	E_HP_INTEGER_OVERFLOW													= -E_HP_HOTDATA_SECTION - 2,
	E_HP_CAN_NOT_OPEN_FILE													= -E_HP_HOTDATA_SECTION - 3,
	E_HP_CAN_NOT_FIND_CONSTANCE												= -E_HP_HOTDATA_SECTION - 4,
	E_HP_CONSTANCE_TYPE_TOO_SMALL											= -E_HP_HOTDATA_SECTION - 5,
	E_HP_INVALID_CONSTANCE_TYPE												= -E_HP_HOTDATA_SECTION - 6,
	E_HP_UNKNOW_CONSTANT_VALUE												= -E_HP_HOTDATA_SECTION - 7,
	E_HP_SYMBOL_REDEFINITION												= -E_HP_HOTDATA_SECTION - 8,
	E_HP_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE					= -E_HP_HOTDATA_SECTION - 9,
}HP_HOTDATA_ERROR_CODE;

#endif //_H_HP_ERROR
