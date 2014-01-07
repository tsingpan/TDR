#ifndef _H_GENERATOR
#define _H_GENERATOR

#include "platform/tlibc_platform.h"
#include "error/error_code.h"

#include "definition.h"
typedef struct _GENERATOR GENERATOR;
struct _GENERATOR
{
	TD_ERROR_CODE (*on_definition)(GENERATOR *self, const ST_DEFINITION *definition);
};

void generator_init(GENERATOR *self);

TD_ERROR_CODE generator_on_definition(GENERATOR *self, const ST_DEFINITION *definition);

#endif
