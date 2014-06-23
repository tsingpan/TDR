#ifndef _H_GENERATOR_READER_C_H
#define _H_GENERATOR_READER_C_H

#include "platform/tdr_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct generator_reader_c_s generator_reader_c_t;
struct generator_reader_c_s
{
	generator_t super;
};

#define GENERATOR_READER_C_SUFFIX "_reader.c"

void generator_reader_c_init(generator_reader_c_t *self, const symbols_t *symbols);

#endif//_H_GENERATOR_READER_C_H
