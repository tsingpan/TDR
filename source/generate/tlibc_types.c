#include "generate/tlibc_types.h"
#include "generator.h"
#include "stdio.h"

static TD_ERROR_CODE on_definition(GENERATOR *super, const ST_DEFINITION *definition)
{
	TLIBC_TYPES_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_TYPES_GENERATOR, super);
	switch(definition->type)
	{
		case E_DT_IMPORT:
			{
				printf("%s\n", definition->definition.de_import.package_name);				
			}
			break;
		case E_DT_CONST:
			{
				printf("%s\n", definition->definition.de_const.identifier);				
			}
			break;
		case E_DT_ENUM:
			{
				printf("%s\n", definition->definition.de_enum.name);			
			}
			break;
		case E_DT_STRUCT:
			{
				printf("%s\n", definition->definition.de_struct.name);
			}
			break;
		case E_DT_UNION:
			{
				printf("%s\n", definition->definition.de_union.name);
			}
			break;
		case E_DT_TYPEDEF:
			{
				printf("%s\n", definition->definition.de_typedef.name);
			}
			break;
		case E_DT_UNIX_COMMENT:
			{
				printf("%s\n", definition->definition.de_unix_comment.text);
			}
			break;
		default:
			goto ERROR_RET;
			break;
	}
	
	return E_TD_NOERROR;
ERROR_RET:
	return E_TD_ERROR;
}

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self)
{
	generator_init(&self->super);

	self->super.on_definition = on_definition;
}
