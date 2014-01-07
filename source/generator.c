#include "generator.h"
#include "error/error_code.h"

void generator_init(GENERATOR *self)
{
	self->on_definition = NULL;
}

TD_ERROR_CODE generator_on_definition(GENERATOR *self, const ST_DEFINITION *definition)
{
	if(self->on_definition != NULL)
	{
		return self->on_definition(self, definition);
	}
	return E_TD_NOERROR;
}
