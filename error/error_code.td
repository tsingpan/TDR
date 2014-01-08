enum(counter = "TD_ERROR_CODE_NUM") TD_ERROR_CODE
{
	E_TD_NOERROR																= 0,
	E_TD_ERROR																	= -1, 
	E_TD_BAD_PARAMETER                                                      	= -10000,#parser
	E_TD_SYNTAX_ERROR													    	= -10001,
	E_TD_NOT_ENOUGH_BYTEBUFF_SIZE                                           	= -10002,
	E_TD_NOT_ENOUGH_STRING_SIZE                                           		= -10003,
	E_TD_CONSTANT_REDEFINITION													= -10004,
	E_TD_INTEGER_OVERFLOW														= -10005,
	E_TD_CAN_NOT_OPEN_FILE														= -10006,
	E_TD_CAN_NOT_FIND_CONSTANCE													= -10007,
	E_TD_CONSTANCE_TYPE_TOO_SMALL												= -10008,
	E_TD_INVALID_CONSTANCE_TYPE													= -10009,
	E_TD_UNKNOW_CONSTANT_VALUE													= -10010,
	E_TD_SYMBOL_REDEFINITION													= -10011,
	E_TD_TYPEDEF_ONLY_SUPPORT_SIMPLEY_TYPE_OR_OBJECCT_TYPE						= -10012,#parser
};

typedef uint32 abc;


#asdfadf
