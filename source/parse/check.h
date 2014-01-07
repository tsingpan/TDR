#ifndef _H_CHECK
#define _H_CHECK


#include "platform/tlibc_platform.h"

#include "parse/parser.h"

void dp_check_Const_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_Const_add_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier, const ST_VALUE *pn_value);

void dp_check_constant_value(PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const tbytes* tok_identifier, const ST_VALUE* sn_value);

void dp_check_Typedef(PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef);

void dp_check_domain_begin(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_domain_end(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Parameter_add(PARSER *self, const YYLTYPE *yylloc, const ST_Parameter *pn_parameter);

void dp_check_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_tok_identifier_local(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_EnumDef_tok_identifier(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_EnumDef_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_TypeAnnotation_tok_unique_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_TypeAnnotation_bound_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_TypeAnnotation_tok_switch_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_TypeAnnotation_tok_counter_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_Union_Parameters(PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

void dp_check_Union_begin(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Union_end(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_begin(PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_end(PARSER *self, const YYLTYPE *yylloc);

void dp_check_FieldExpression_Value(PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_Field(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void dp_check_Field_add(PARSER *self, const YYLTYPE *yylloc, const ST_FIELD *pn_field);

void dp_check_Enum_Add(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier, const ST_ENUM *pn_enum);

void dp_check_Struct_Add(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

void dp_check_Union_Add(PARSER *self, const YYLTYPE *yylloc, const tbytes *tok_identifier);

#endif //_H_CHECK
