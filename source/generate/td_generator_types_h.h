#ifndef _H_TD_GENERATOR_TYPES_H_H
#define _H_TD_GENERATOR_TYPES_H_H

#include "platform/tlibc_platform.h"
#include "symbols.h"

#include "generator.h"

typedef struct td_generator_types_h_s td_generator_types_h_t;
struct td_generator_types_h_s
{
	generator_t super;
};

#define TLIBC_TYPES_SUFFIX "_types.h"

void tlibc_types_generator_init(td_generator_types_h_t *self, const td_symbols_t *symbols);

#endif//_H_TD_GENERATOR_TYPES_H_H
