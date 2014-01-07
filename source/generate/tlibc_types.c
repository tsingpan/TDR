#include "generate/tlibc_types.h"
#include "generator.h"
#include <stdio.h>
#include <string.h>



static TD_ERROR_CODE on_document_begin(GENERATOR *super, const char *file_name)
{
	TLIBC_TYPES_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_TYPES_GENERATOR, super);
	tuint32 i, length;
	strncpy(self->tag, file_name, TLIBC_MAX_FILE_PATH_LENGTH);
	length = strlen(self->tag);
	for(i = 0;i < length; ++i)
	{
		if((self->tag[i] >= 'a') && (self->tag[i] <= 'z'))
		{
			self->tag[i] = 'A' + self->tag[i] - 'a';
		}
		else if ((self->tag[i] >= 'A') && (self->tag[i] <= 'Z'))
		{
		}
		else if ((self->tag[i] >= '0') && (self->tag[i] <= '9'))
		{
		}
		else
		{
			self->tag[i] = '_';
		}
	}

	generator_open(super, file_name, TLIBC_TYPES_SUFFIX);




	generator_print(super, "#ifndef _H_%s\n", self->tag);
	generator_print(super, "#define _H_%s\n", self->tag);
	generator_print(super, "\n");

	return E_TD_NOERROR;
}

static TD_ERROR_CODE on_document_end(GENERATOR *super, const char *file_name)
{	
	TLIBC_TYPES_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_TYPES_GENERATOR, super);
	(void)file_name;

	generator_print(super, "\n");
	generator_print(super, "#endif //_H_%s\n", self->tag);
	generator_print(super, "\n");

	generator_close(super);
	return E_TD_NOERROR;
}

static TD_ERROR_CODE _on_import(TLIBC_TYPES_GENERATOR *self, const ST_Import *de_import)
{
	char name[MAX_PACKAGE_NAME_LENGTH];
	tuint32 i, name_length;
	strncpy(name, de_import->package_name, MAX_PACKAGE_NAME_LENGTH);

	name_length = strlen(name);
	for(i = name_length - 1; i > 0; --i)
	{
		if(name[i] == '.')
		{
			name[i] = 0;
			name_length = i;
			break;
		}
		else if(name[i] == TLIBC_FILE_SEPARATOR)
		{
			break;
		}
	}
	
	generator_print(&self->super, "#include \"%s%s\"\n", name, TLIBC_TYPES_SUFFIX);

	return E_TD_NOERROR;
}

static void _print_val(TLIBC_TYPES_GENERATOR *self, const ST_VALUE *val)
{
	switch (val->type)
	{
	case E_SNVT_IDENTIFIER:
		generator_print(&self->super, "%s", val->val.identifier);
		break;
	case E_SNVT_CHAR:
		generator_print(&self->super, "\'%c\'", val->val.c);
		break;
	case E_SNVT_DOUBLE:
		generator_print(&self->super, "%d", val->val.d);
		break;
	case E_SNVT_BOOL:
		if(val->val.b)
		{
			generator_print(&self->super, "hptrue");
		}
		else
		{
			generator_print(&self->super, "hpfalse");
		}
		break;
	case E_SNVT_INT64:
		generator_print(&self->super, "%lld", val->val.i64);
		break;
	case E_SNVT_UINT64:
		generator_print(&self->super, "%llu", val->val.ui64);
		break;
	case E_SNVT_HEX_INT64:
		generator_print(&self->super, "%llx", val->val.i64);
		break;
	case E_SNVT_HEX_UINT64:
		generator_print(&self->super, "%llx", val->val.ui64);
		break;
	}
}


static TD_ERROR_CODE _on_const(TLIBC_TYPES_GENERATOR *self, const ST_Const *de_const)
{
	generator_print(&self->super, "\n");
	generator_print(&self->super, "#define %s ", de_const->identifier);
	_print_val(self, &de_const->val);
	generator_print(&self->super, "\n");
	return E_TD_NOERROR;
}

static TD_ERROR_CODE on_definition(GENERATOR *super, const ST_DEFINITION *definition)
{
	TLIBC_TYPES_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_TYPES_GENERATOR, super);
	switch(definition->type)
	{
		case E_DT_IMPORT:
			return _on_import(self, &definition->definition.de_import);				
		case E_DT_CONST:
			return _on_const(self, &definition->definition.de_const);
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

void tlibc_types_generator_init(TLIBC_TYPES_GENERATOR *self, const char *target_dir)
{
	generator_init(&self->super, target_dir);

	self->super.on_document_begin = on_document_begin;
	self->super.on_document_end = on_document_end;
	self->super.on_definition = on_definition;
}
