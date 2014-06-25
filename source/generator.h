#ifndef _H_GENERATOR
#define _H_GENERATOR

#include <stdio.h>

#include "platform/tdr_platform.h"
#include "error.h"
#include "symbols.h"

#include "definition.h"


#define VEC_NUM_TYPE_STYLE "%s_num"
static const syn_simple_type_t g_vec_num_type = {E_ST_UINT32, "", ""};
static const syn_simple_type_t g_enum_type = {E_ST_INT32, "", ""};

typedef struct generator_s generator_t;
struct generator_s
{
	const symbols_t *symbols;
	FILE* fout;
	char target_filename[TDR_MAX_PATH_LENGTH];

	char document_name[TDR_MAX_PATH_LENGTH];

	error_code_t (*on_document_begin)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	error_code_t (*on_document_end)(generator_t *self, const YYLTYPE *yylloc, const char *file_name);
	error_code_t (*on_definition)(generator_t *self, const YYLTYPE *yylloc, const syn_definition_t *definition);
};

void generator_init(generator_t *self, const symbols_t *symbols);

void strncpy_notdir(char *dest, const char*src, size_t dest_len);

void strncpy_dir(char *dest, const char*src, size_t dest_len);

error_code_t generator_open(generator_t *self, const char *original_file, const char *suffix);

void generator_print(generator_t *self, size_t tabs, const char* fmt, ...);

void generator_printline(generator_t *self, size_t tabs, const char* fmt, ...);

void generator_close(generator_t *self);

error_code_t generator_print_value(generator_t *self, const syn_value_t *val);

error_code_t generator_print_type_name(generator_t *self, const syn_simple_type_t *simple_type);

error_code_t generator_print_ctype(generator_t *self, const syn_simple_type_t *simple_type);

error_code_t generator_replace_extension(char *filename, uint32_t filename_length, const char *suffix);


//virtual functions
error_code_t generator_on_definition(generator_t *self, const YYLTYPE *yylloc, const syn_definition_t *definition);

error_code_t generator_on_document_begin(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

error_code_t generator_on_document_end(generator_t *self, const YYLTYPE *yylloc, const char *file_name);

#endif
