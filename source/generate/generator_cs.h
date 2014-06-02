#ifndef _H_GENERATOR_CS_H
#define _H_GENERATOR_CS_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct generator_cs_s generator_cs_t;
struct generator_cs_s
{
	generator_t super;
};

#define GENERATOR_CS_SUFFIX "_types.cs"

void generator_cs_init(generator_cs_t *self, const symbols_t *symbols);

#endif//_H_TD_GENERATOR_CS_H
