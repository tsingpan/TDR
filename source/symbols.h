#ifndef _H_SYMBOLS
#define _H_SYMBOLS

#include "definition.h"

typedef enum _SYMBOLS_TYPE
{
	EN_HST_VALUE = 0,
	EN_HST_TYPE = 1,
	EN_HST_PARAMETER = 2,
	EN_HST_FIELD = 3,
	EN_HST_ENUM = 4,
	EN_HST_STRUCT = 5,
	EN_HST_UNION = 6,
}SYMBOLS_TYPE;

typedef union _SYMBOLS_BODY
{
	ST_VALUE val;
	ST_TYPE type;
	ST_Parameter para;
	ST_FIELD field;
}SYMBOLS_BODY;

typedef struct _SYMBOLS
{
	SYMBOLS_TYPE type;
	SYMBOLS_BODY body;
}SYMBOLS;

#endif //_H_SYMBOLS
