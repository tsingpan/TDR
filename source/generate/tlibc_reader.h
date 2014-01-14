#ifndef _H_TLIBC_READER
#define _H_TLIBC_READER

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct _TLIBC_READER_GENERATOR TLIBC_READER_GENERATOR;
struct _TLIBC_READER_GENERATOR
{
	GENERATOR super;
};

#define TLIBC_READER_SUFFIX "_writer.c"

void tlibc_writer_generator_init(TLIBC_READER_GENERATOR *self, const SYMBOLS *symbols);

#endif//_H_TLIBC_READER
