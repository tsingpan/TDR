#ifndef _H_HOTDATA_REDUCE
#define _H_HOTDATA_REDUCE


#include "hotpot/hp_platform.h"

#include "hotdata_parser.h"

void dp_reduce_Definition_Import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_IMPORT* pn_import);

void dp_reduce_Definition_Const(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_CONST* pn_const);

void dp_reduce_Definition_Typedef(DATA_PARSER *self, const YYLTYPE *yylloc, PN_DEFINITION *pn_current, const PN_TYPEDEF* pn_typedef);

void dp_reduce_Import_tok_import(DATA_PARSER *self, const YYLTYPE *yylloc, PN_IMPORT* current, const hpstring *sn_tok_import);

void dp_reduce_ObjectType_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE* current, const PN_IDENTIFIER *tok_identifier);

void dp_reduce_ContainerType_tok_t_vector(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current);

void dp_reduce_ContainerType_tok_t_string(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current);

void dp_reduce_SimpleType(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPE *current, const SN_SIMPLE_TYPE type);

void dp_reduce_Value_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbytes sn_identifier);

void dp_reduce_Value_tok_char(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const PN_CHAR pn_char);

void dp_reduce_Value_tok_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64);

void dp_reduce_Value_tok_hex_int64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpint64 i64);

void dp_reduce_Value_tok_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64);

void dp_reduce_Value_tok_hex_uint64(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpuint64 ui64);

void dp_reduce_Value_tok_double(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const double d);

void dp_reduce_Value_tok_bool(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpbool b);

void dp_reduce_Value_tok_string(DATA_PARSER *self, const YYLTYPE *yylloc, PN_VALUE* current, const hpstring str);


void dp_reduce_Const(DATA_PARSER *self, const YYLTYPE *yylloc, PN_CONST* current, const PN_TYPE *type, const PN_IDENTIFIER *identifier, const PN_VALUE *val);

void dp_reduce_ArgumentList_Type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENTS* current, const PN_TYPE *pn_type);

void dp_reduce_ArgumentList_ArgumentList_Type(DATA_PARSER *self, const YYLTYPE *yylloc, PN_ARGUMENTS* current, const PN_ARGUMENTS* argument_list, const PN_TYPE *pn_type);

void dp_reduce_Typedef_Type_Arguments_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, PN_TYPEDEF *current, PN_TYPE* type, const PN_IDENTIFIER *tok_identifier);

void dp_reduce_Condition_tok_case(DATA_PARSER *self, const YYLTYPE *yylloc, PN_CONDITION *current, const PN_VALUE *val);


#endif //_H_HOTDATA_REDUCE