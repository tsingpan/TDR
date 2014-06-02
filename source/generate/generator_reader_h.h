#ifndef _H_GENERATOR_READER_H_H
#define _H_GENERATOR_READER_H_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct generator_reader_h_s generator_reader_h_t;
struct generator_reader_h_s
{
	generator_t super;
};

#define GENERATOR_READER_H_SUFFIX "_reader.h"

void generator_reader_h_init(generator_reader_h_t *self, const symbols_t *symbols, int make_rule);

#endif//_H_GENERATOR_READER_H_H
