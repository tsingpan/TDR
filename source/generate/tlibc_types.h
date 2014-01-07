#ifndef _H_TLIBC_TYPES
#define _H_TLIBC_TYPES

#include "generator.h"

typedef struct _TLIBC_TYPES_GENERATOR TLIBC_TYPES_GENERATOR;
struct _TLIBC_TYPES_GENERATOR
{
	GENERATOR super;
};

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self);

#endif//_H_TLIBC_TYPES
