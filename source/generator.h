#ifndef _H_GENERATOR
#define _H_GENERATOR

#include <stdio.h>

#include "platform/tlibc_platform.h"
#include "error/error_code.h"

#include "definition.h"
typedef struct _GENERATOR GENERATOR;
struct _GENERATOR
{
	const char *target_dir;
	FILE* fout;

	TD_ERROR_CODE (*on_document_begin)(GENERATOR *self, const char *file_name);
	TD_ERROR_CODE (*on_document_end)(GENERATOR *self, const char *file_name);
	TD_ERROR_CODE (*on_definition)(GENERATOR *self, const ST_DEFINITION *definition);
};

void generator_init(GENERATOR *self, const char *target_dir);

TD_ERROR_CODE generator_open(GENERATOR *self, const char *primary_file, const char *suffix);

TD_ERROR_CODE generator_print(GENERATOR *self, const char* fmt, ...);

TD_ERROR_CODE generator_close(GENERATOR *self);



//virtual functions
TD_ERROR_CODE generator_on_definition(GENERATOR *self, const ST_DEFINITION *definition);

TD_ERROR_CODE generator_on_document_begin(GENERATOR *self, const char *file_name);

TD_ERROR_CODE generator_on_document_end(GENERATOR *self, const char *file_name);

#endif
