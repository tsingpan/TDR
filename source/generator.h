#ifndef _H_GENERATOR
#define _H_GENERATOR

#include <stdio.h>

#include "platform/tlibc_platform.h"
#include "error.h"
#include "symbols.h"

#include "definition.h"


//定义计数器对象
#define VEC_NUM_TYPE_STYLE "%s_num"
static const syn_simple_type_t g_vec_num_type = {E_ST_UINT32, "", ""};
static const syn_simple_type_t g_enum_type = {E_ST_INT32, "", ""};

typedef struct generator_s generator_t;
struct generator_s
{
	const symbols_t *symbols;
	FILE* fout;
	char document_name[TLIBC_MAX_PATH_LENGTH];
	char file_name[TLIBC_MAX_PATH_LENGTH];

	error_code_t (*on_document_begin)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	error_code_t (*on_document_end)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	error_code_t (*on_definition)(generator_t *self, const YYLTYPE *yylloc, const ST_DEFINITION *definition);

	error_code_t (*on_struct_begin)(generator_t *self, const YYLTYPE *yylloc, const char * struct_name);
	error_code_t (*on_field)(generator_t *self, const YYLTYPE *yylloc, const ST_FIELD *field);
	error_code_t (*on_struct_end)(generator_t *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct);
};

void generator_init(generator_t *self, const symbols_t *symbols);

void strncpy_notdir(char *dest, const char*src, size_t dest_len);

error_code_t generator_open(generator_t *self, const char *primary_file, const char *suffix);

void generator_print(generator_t *self, size_t tabs, const char* fmt, ...);

void generator_printline(generator_t *self, size_t tabs, const char* fmt, ...);

error_code_t generator_close(generator_t *self);

error_code_t generator_print_value(generator_t *self, const syn_value_t *val);

error_code_t generator_print_type_name(generator_t *self, const syn_simple_type_t *simple_type);

error_code_t generator_print_ctype(generator_t *self, const syn_simple_type_t *simple_type);

error_code_t generator_print_cstype(generator_t *self, const syn_simple_type_t *simple_type);

error_code_t generator_replace_extension(char *filename, uint32_t filename_length, const char *suffix);

//virtual functions
error_code_t generator_on_definition(generator_t *self, const YYLTYPE *yylloc, const ST_DEFINITION *definition);

error_code_t generator_on_document_begin(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

error_code_t generator_on_document_end(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

error_code_t generator_on_struct_begin(generator_t *self, const YYLTYPE *yylloc, const char * struct_name);

error_code_t generator_on_field(generator_t *self, const YYLTYPE *yylloc, const ST_FIELD *field);

error_code_t generator_on_struct_end(generator_t *self, const YYLTYPE *yylloc, const ST_STRUCT *pn_struct);

#endif
