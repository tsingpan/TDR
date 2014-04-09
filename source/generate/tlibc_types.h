#ifndef _H_TLIBC_TYPES
#define _H_TLIBC_TYPES

#include "platform/tlibc_platform.h"
#include "symbols.h"

#include "generator.h"

typedef struct _TLIBC_TYPES_GENERATOR TLIBC_TYPES_GENERATOR;
struct _TLIBC_TYPES_GENERATOR
{
	generator_t super;
};

#define TLIBC_TYPES_SUFFIX "_types.h"

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self, const SYMBOLS *symbols);

#endif//_H_TLIBC_TYPES
