#ifndef _H_GENERATOR_TYPES_H_H
#define _H_GENERATOR_TYPES_H_H

#include "platform/tlibc_platform.h"
#include "symbols.h"

#include "generator.h"

typedef struct generator_types_h_s generator_types_h_t;
struct generator_types_h_s
{
	generator_t super;
};

#define GENERATOR_TYPES_H_SUFFIX "_types.h"

void generator_types_h_init(generator_types_h_t *self, const symbols_t *symbols);

#endif//_H_TD_GENERATOR_TYPES_H_H
