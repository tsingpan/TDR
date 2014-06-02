#ifndef _H_GENERATOR_SQL_H
#define _H_GENERATOR_SQL_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct generator_sql_s generator_sql_t;
struct generator_sql_s
{
	generator_t super;
	int struct_begin;
	int first_field;
};

#define GENERATOR_SQL_SUFFIX "_tables.sql"

void generator_sql_init(generator_sql_t *self, const symbols_t *symbols, int make_rule);

#endif//_H_GENERATOR_SQL_H
