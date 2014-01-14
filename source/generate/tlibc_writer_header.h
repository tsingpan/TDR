#ifndef _H_TLIBC_WRITER_HEADER
#define _H_TLIBC_WRITER_HEADER

#include "platform/tlibc_platform.h"

#include "generator.h"

typedef struct _TLIBC_WRITER_HEADER_GENERATOR TLIBC_WRITER_HEADER_GENERATOR;
struct _TLIBC_WRITER_HEADER_GENERATOR
{
	GENERATOR super;
};

#define TLIBC_WRITER_HEADER_SUFFIX "_writer.h"

void tlibc_writer_header_generator_init(TLIBC_WRITER_HEADER_GENERATOR *self);

#endif//_H_TLIBC_WRITER_HEADER
