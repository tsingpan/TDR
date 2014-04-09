#ifndef _H_TD_GENERATOR_CS_H
#define _H_TD_GENERATOR_CS_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct td_generator_cs_s td_generator_cs_t;
struct td_generator_cs_s
{
	generator_t super;
};

#define TLIBC_CS_SUFFIX "_types.cs"

void tlibc_cs_generator_init(td_generator_cs_t *self, const td_symbols_t *symbols);

#endif//_H_TD_GENERATOR_CS_H
