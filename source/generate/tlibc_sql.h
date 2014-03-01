#ifndef _H_TLIBC_SQL_HEADER
#define _H_TLIBC_SQL_HEADER

#include "tlibc/platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct _TLIBC_SQL_GENERATOR TLIBC_SQL_GENERATOR;
struct _TLIBC_SQL_GENERATOR
{
	GENERATOR super;
};

#define TLIBC_SQL_SUFFIX "_tables.sql"

void tlibc_sql_generator_init(TLIBC_SQL_GENERATOR *self, const SYMBOLS *symbols);

#endif//_H_TLIBC_SQL_HEADER
