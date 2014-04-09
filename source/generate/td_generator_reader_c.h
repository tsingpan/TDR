#ifndef _H_TD_GENERATOR_READER_C_H
#define _H_TD_GENERATOR_READER_C_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct td_generator_reader_c_s td_generator_reader_c_t;
struct td_generator_reader_c_s
{
	generator_t super;
};

#define TLIBC_READER_SUFFIX "_reader.c"

void tlibc_reader_generator_init(td_generator_reader_c_t *self, const td_symbols_t *symbols);

#endif//_H_TD_GENERATOR_READER_C_H
