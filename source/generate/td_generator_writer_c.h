#ifndef _H_TD_GENERATOR_WRITER_C_H
#define _H_TD_GENERATOR_WRITER_C_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct td_generator_writer_c_s td_generator_writer_c_t;
struct td_generator_writer_c_s
{
	generator_t super;
};

#define TLIBC_WRITER_SUFFIX "_writer.c"

void tlibc_writer_generator_init(td_generator_writer_c_t *self, const td_symbols_t *symbols);

#endif//_H_TD_GENERATOR_WRITER_C_H
