#ifndef _H_HOTDATA_REDUCE
#define _H_HOTDATA_REDUCE


#include "platform/tlibc_platform.h"

#include "parse/parser.h"

void dp_reduce_Definition_Import(DATA_PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_Import* pn_import);

void dp_reduce_Definition_Const(DATA_PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_Const* pn_const);

void dp_reduce_Definition_Typedef(DATA_PARSER *self, const YYLTYPE *yylloc, ST_DEFINITION *pn_current, const ST_TYPEDEF* pn_typedef);

void dp_reduce_Import_tok_string(DATA_PARSER *self, const YYLTYPE *yylloc, ST_Import* current, const tstring str);

void dp_reduce_ObjectType_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, ST_TYPE* current, const tbytes *tok_identifier);

void dp_reduce_ContainerType_tok_t_vector(DATA_PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current);

void dp_reduce_ContainerType_tok_t_string(DATA_PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current);

void dp_reduce_SimpleType(DATA_PARSER *self, const YYLTYPE *yylloc, ST_TYPE *current, const SN_SIMPLE_TYPE type);

void dp_reduce_Value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tbytes sn_identifier);

void dp_reduce_Value_tok_char(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tchar pn_char);

void dp_reduce_Value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tint64 i64);

void dp_reduce_Value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tint64 i64);

void dp_reduce_Value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tuint64 ui64);

void dp_reduce_Value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tuint64 ui64);

void dp_reduce_Value_tok_double(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const double d);

void dp_reduce_Value_tok_bool(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tbool b);

void dp_reduce_Value_tok_string(DATA_PARSER *self, const YYLTYPE *yylloc, ST_VALUE* current, const tstring str);


void dp_reduce_Const(DATA_PARSER *self, const YYLTYPE *yylloc, ST_Const* current, const ST_TYPE *type, const tbytes *identifier, const ST_VALUE *val);

void dp_reduce_ArgumentList_Type(DATA_PARSER *self, const YYLTYPE *yylloc, ST_ARGUMENTS* current, const ST_TYPE *pn_type);

void dp_reduce_ArgumentList_ArgumentList_Type(DATA_PARSER *self, const YYLTYPE *yylloc, ST_ARGUMENTS* current, const ST_ARGUMENTS* argument_list, const ST_TYPE *pn_type);

void dp_reduce_Typedef_Type_Arguments_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, ST_TYPEDEF *current, ST_TYPE* type, const tbytes *tok_identifier);

void dp_reduce_Condition_tok_case(DATA_PARSER *self, const YYLTYPE *yylloc, ST_CONDITION *current, const ST_VALUE *val);


#endif //_H_HOTDATA_REDUCE