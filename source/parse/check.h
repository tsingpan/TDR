#ifndef _H_CHECK
#define _H_CHECK


#include "platform/tlibc_platform.h"

#include "parse/parser.h"

void dp_check_Const(PARSER *self, const YYLTYPE *yylloc, ST_Const* current, const ST_TYPE *type, const tbytes *identifier, const ST_VALUE *val);

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef);



void dp_check_Parameter_add(PARSER *self, const YYLTYPE *yylloc, const ST_Parameter *pn_parameter);

void dp_check_EnumDef(PARSER *self, const YYLTYPE *yylloc, const ST_ENUM_DEF *enum_def);

void dp_check_Union_Parameters(PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

void dp_check_Union_begin(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Union_end(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_begin(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_end(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void dp_check_Field_add(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void dp_check_Struct_Add(PARSER *self, const YYLTYPE *yylloc, const ST_STRUCT *de_struct);

void dp_check_Union_Add(PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

#endif //_H_CHECK
