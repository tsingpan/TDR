#ifndef _H_TLIBC_READER_HEADER
#define _H_TLIBC_READER_HEADER

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct _TLIBC_READER_HEADER_GENERATOR TLIBC_READER_HEADER_GENERATOR;
struct _TLIBC_READER_HEADER_GENERATOR
{
	GENERATOR super;
};

#define TLIBC_READER_HEADER_SUFFIX "_writer.h"

void tlibc_reader_header_generator_init(TLIBC_READER_HEADER_GENERATOR *self, const SYMBOLS *symbols);

#endif//_H_TLIBC_READER_HEADER
