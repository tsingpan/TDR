#ifndef _H_TLIBC_TYPES
#define _H_TLIBC_TYPES

#include "platform/tlibc_platform.h"

#include "generator.h"

typedef struct _TLIBC_TYPES_GENERATOR TLIBC_TYPES_GENERATOR;
struct _TLIBC_TYPES_GENERATOR
{
	GENERATOR super;

	char document_name[TLIBC_MAX_FILE_PATH_LENGTH];
};

#define TLIBC_TYPES_SUFFIX "_types.h"

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self, const char *target_dir);

#endif//_H_TLIBC_TYPES
