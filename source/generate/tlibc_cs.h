#ifndef _H_TLIBC_CS
#define _H_TLIBC_CS

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct _TLIBC_CS_GENERATOR TLIBC_CS_GENERATOR;
struct _TLIBC_CS_GENERATOR
{
	generator_t super;
};

#define TLIBC_CS_SUFFIX "_types.cs"

void tlibc_cs_generator_init(TLIBC_CS_GENERATOR *self, const td_symbols_t *symbols);

#endif//_H_TLIBC_CS
