#ifndef _H_CHECK
#define _H_CHECK


#include "platform/tlibc_platform.h"

#include "parse/parser.h"

void dp_check_Import(PARSER *self, const YYLTYPE *yylloc, const char* str);

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, ST_SIMPLE_TYPE* type, const tchar *tok_identifier);

void dp_check_Const(PARSER *self, const YYLTYPE *yylloc, const ST_SIMPLE_TYPE *type, const tchar *id, const ST_VALUE *val);

void dp_check_Enum(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier);

void dp_check_EnumDefList(PARSER *self, const YYLTYPE *yylloc, tuint32 enum_def_list_num);

void dp_check_EnumDef(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier, const ST_VALUE *st_value);

void dp_check_Union_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier);

void dp_check_Union_Parameters(PARSER *self, const YYLTYPE *yylloc, const ST_Parameters *parameters);

void dp_check_UnionFieldList(PARSER *self, const YYLTYPE *yylloc, tuint32 union_field_list_num);

void dp_check_ParameterList(PARSER *self, const YYLTYPE *yylloc, tuint32 par_list_num);

void dp_check_ArgumentList(PARSER *self, const YYLTYPE *yylloc, tuint32 arg_list_num);


void dp_check_Union(PARSER *self, const YYLTYPE *yylloc, const tchar *identifier, const ST_Parameters *parameters, const ST_UNION_FIELD_LIST* union_field_list);

void dp_check_UnionField(PARSER *self, const YYLTYPE *yylloc, const tchar *key, const ST_SIMPLE_TYPE *simple_type, const tchar *identifier);

void dp_check_FieldList(PARSER *self, const YYLTYPE *yylloc, tuint32 field_list_num);

void dp_check_Field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);


#endif //_H_CHECK
