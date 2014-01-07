#include "generate/tlibc_types.h"
#include "generator.h"


static TD_ERROR_CODE on_definition(GENERATOR *super, const ST_DEFINITION *definition)
{
	TLIBC_TYPES_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_TYPES_GENERATOR, super);
	return E_TD_NOERROR;
}

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self)
{
	generator_init(&self->super);

	self->super.on_definition = on_definition;
}

