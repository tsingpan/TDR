#ifndef _H_GENERATOR
#define _H_GENERATOR

#include <stdio.h>

#include "platform/tlibc_platform.h"
#include "error.h"
#include "symbols.h"

#include "definition.h"


//定义计数器对象
#define VEC_NUM_TYPE_STYLE "%s_num"
static const ST_SIMPLE_TYPE g_vec_num_type = {E_ST_UINT32, "", ""};
static const ST_SIMPLE_TYPE g_enum_type = {E_ST_INT32, "", ""};

typedef struct generator_s generator_t;
struct generator_s
{
	const SYMBOLS *symbols;
	FILE* fout;
	char document_name[TLIBC_MAX_PATH_LENGTH];
	char file_name[TLIBC_MAX_PATH_LENGTH];

	TD_ERROR_CODE (*on_document_begin)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	TD_ERROR_CODE (*on_document_end)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	TD_ERROR_CODE (*on_definition)(generator_t *self, const YYLTYPE *yylloc, const ST_DEFINITION *definition);

	TD_ERROR_CODE (*on_struct_begin)(generator_t *self, const YYLTYPE *yylloc, const char * struct_name);
	TD_ERROR_CODE (*on_field)(generator_t *self, const YYLTYPE *yylloc, const ST_FIELD *field);
	TD_ERROR_CODE (*on_struct_end)(generator_t *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct);
};

void generator_init(generator_t *self, const SYMBOLS *symbols);

void strncpy_notdir(char *dest, const char*src, size_t dest_len);

TD_ERROR_CODE generator_open(generator_t *self, const char *primary_file, const char *suffix);

void generator_print(generator_t *self, size_t tabs, const char* fmt, ...);

void generator_printline(generator_t *self, size_t tabs, const char* fmt, ...);

TD_ERROR_CODE generator_close(generator_t *self);

TD_ERROR_CODE generator_print_value(generator_t *self, const ST_VALUE *val);

TD_ERROR_CODE generator_print_type_name(generator_t *self, const ST_SIMPLE_TYPE *simple_type);

TD_ERROR_CODE generator_print_ctype(generator_t *self, const ST_SIMPLE_TYPE *simple_type);

TD_ERROR_CODE generator_print_cstype(generator_t *self, const ST_SIMPLE_TYPE *simple_type);

TD_ERROR_CODE generator_replace_extension(char *filename, uint32_t filename_length, const char *suffix);

//virtual functions
TD_ERROR_CODE generator_on_definition(generator_t *self, const YYLTYPE *yylloc, const ST_DEFINITION *definition);

TD_ERROR_CODE generator_on_document_begin(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

TD_ERROR_CODE generator_on_document_end(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

TD_ERROR_CODE generator_on_struct_begin(generator_t *self, const YYLTYPE *yylloc, const char * struct_name);

TD_ERROR_CODE generator_on_field(generator_t *self, const YYLTYPE *yylloc, const ST_FIELD *field);

TD_ERROR_CODE generator_on_struct_end(generator_t *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct);

#endif
