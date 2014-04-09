#ifndef _H_TD_GENERATOR_SQL_H
#define _H_TD_GENERATOR_SQL_H

#include "platform/tlibc_platform.h"
#include "symbols.h"
#include "generator.h"

typedef struct td_generator_sql_s td_generator_sql_t;
struct td_generator_sql_s
{
	generator_t super;
	int struct_begin;
	int first_field;
};

#define TLIBC_SQL_SUFFIX "_tables.sql"

void tlibc_sql_generator_init(td_generator_sql_t *self, const td_symbols_t *symbols);

#endif//_H_TD_GENERATOR_SQL_H
