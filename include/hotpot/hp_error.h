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
}HP_HOTDATA_ERROR_CODE;

#endif //_H_HP_ERROR
