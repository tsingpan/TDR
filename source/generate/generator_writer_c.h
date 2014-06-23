#ifndef _H_TD_GENERATOR_WRITER_C_H
#define _H_TD_GENERATOR_WRITER_C_H

#include "platform/tdr_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct generator_writer_c_s generator_writer_c_t;
struct generator_writer_c_s
{
	generator_t super;
};

#define GENERATOR_WRITER_C_SUFFIX "_writer.c"

void generator_writer_c_init(generator_writer_c_t *self, const symbols_t *symbols);

#endif//_H_TD_GENERATOR_WRITER_C_H
