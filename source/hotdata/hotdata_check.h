#ifndef _H_HOTDATA_CHECK
#define _H_HOTDATA_CHECK


#include "hotpot/hp_platform.h"

#include "hotdata_parser.h"

void dp_check_Const_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_Const_add_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier, const PN_VALUE *pn_value);

void dp_check_constant_value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPE* sn_type, const PN_IDENTIFIER* tok_identifier, const PN_VALUE* sn_value);

void dp_check_Typedef(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_TYPEDEF *sn_typedef);

void dp_check_domain_begin(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_domain_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_check_Parameter_add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_Parameter *pn_parameter);

void dp_check_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_tok_identifier_local(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_EnumDef_tok_identifier(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_EnumDef_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val);

void dp_check_TypeAnnotation_tok_unique_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val);

void dp_check_TypeAnnotation_bound_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val);

void dp_check_TypeAnnotation_tok_switch_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_VALUE *val);

void dp_check_Union_Parameters(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_UNION *de_union);

void dp_check_Union_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_check_Union_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_begin(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_check_Struct_end(DATA_PARSER *self, const YYLTYPE *yylloc);

void dp_check_FieldExpression_Value(DATA_PARSER *self, const YYLTYPE *yylloc, const ST_VALUE *val);

void dp_check_Field(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_FIELD *pn_field);

void dp_check_Field_add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_FIELD *pn_field);

void dp_check_Enum_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_Struct_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

void dp_check_Union_Add(DATA_PARSER *self, const YYLTYPE *yylloc, const PN_IDENTIFIER *tok_identifier);

#endif //_H_HOTDATA_CHECK
