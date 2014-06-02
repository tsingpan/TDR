#ifndef _H_GENERATOR_WRITER_H_H
#define _H_GENERATOR_WRITER_H_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct td_generator_writer_h_s td_generator_writer_h_t;
struct td_generator_writer_h_s
{
	generator_t super;
};

#define GENERATOR_WRITER_H_SUFFIX "_writer.h"

void generator_writer_h_init(td_generator_writer_h_t *self, const symbols_t *symbols);

#endif//_H_GENERATOR_WRITER_H_H
