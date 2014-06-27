#include "parse/parser.h"
#include "error.h"
#include "scanner_l.h"
#include "parser_y.h"
#include "parse/scanner.h"
#include "globals.h"
#include "symbols.h"
#include "parse/scanner.h"
#include "script/script_functions.h"
#include "script/script.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>


void parser_init(PARSER *self)
{
	symbols_init(&self->symbols);
}

static void parser_push(PARSER *self, const char *file_name)
{
	int ret;
	ret = scanner_push(&self->scanner, file_name, yycINITIAL);
	if(ret != E_TD_NOERROR)
	{
		switch(ret)
		{
		case E_TD_SCANNER_CAN_NOT_OPEN_FILE:
			scanner_error_halt(NULL, E_LS_CANNOT_OPEN_FILE, file_name);
		case E_TD_SCANNER_OUT_OF_MEMORY:
			scanner_error_halt(NULL, E_LS_OUT_OF_MEMORY, file_name);			
		default:
			scanner_error_halt(NULL, E_LS_UNKNOW);
		}		
	}
}

static void parser_make_rule(PARSER *self)
{
	size_t i;
	char filename[TDR_MAX_PATH_LENGTH];
	FILE *fout;
	snprintf(filename, TDR_MAX_PATH_LENGTH, "%s.%s", self->generator->target_filename, DEP_SUFFIX);
	fout = fopen(filename, "w");
	if(fout == NULL)
	{
		scanner_error_halt(NULL, E_LS_CANNOT_OPEN_FILE, filename);
	}

	fprintf(fout, "%s: \\\n", self->generator->target_filename);
	for(i = 0; i <self->scanner.file_vec_num; ++i)
	{
		fprintf(fout, "    %s\\\n", self->scanner.file_vec[i].file_name);
	}
	fprintf(fout, "\n");
	fclose(fout);
}

int tdrparse (scanner_t *self);
int32_t parser_parse(PARSER *self, const char* file_name, generator_t *generator, int make_rule)
{
	int32_t ret;

	self->generator = generator;
	scanner_init(&self->scanner);
	
	parser_push(self, file_name);

	if(self->generator)
	{
		generator_on_document_begin(self->generator, NULL, file_name);
	}
	parser_on_document_begin(self, file_name);

	ret = tdrparse(&self->scanner);
	if(make_rule)
	{
		parser_make_rule(self);
	}	
	if(self->generator)
	{
		generator_on_document_end(self->generator, NULL, file_name);
	}
	parser_on_document_end(self);
	scanner_pop(&self->scanner);
	symbols_clear(&self->symbols);
	return E_TD_NOERROR;
}

void parser_on_generator_definition(PARSER *self, const YYLTYPE *yylloc, const syn_definition_t *pn_definition)
{
	if((scanner_size(&self->scanner) == 1) && (self->generator))
	{
		generator_on_definition(self->generator, yylloc, pn_definition);
	}

	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TDR_MAX_PATH_LENGTH];
		snprintf(file_name, TDR_MAX_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		file_name[TDR_MAX_PATH_LENGTH - 1] = 0;
		parser_push(self, file_name);
	}
}


//do
void parser_on_document_begin(PARSER *self, const char *file_name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	sf_on_document_begin(file_name);
}

void parser_on_document_error(PARSER *self)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	sf_on_document_error();
}

void parser_on_document_end(PARSER *self)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	sf_on_document_end();
}

void parser_on_import(PARSER *self, const syn_import_t* syn_import)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	sf_on_import(syn_import->package_name);
}

void parser_on_typedef(PARSER *self, const syn_typedef_t *syn_typedef)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	sf_on_typedef(&syn_typedef->type, syn_typedef->name);
}

void parser_on_const(PARSER *self, const syn_const_t *syn_const)
{
	const syn_simple_type_t *real_type = NULL;
	const syn_simple_type_t *type = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	
	type = &syn_const->type;
	real_type = symbols_get_real_type(&self->symbols, &syn_const->type);
	if(type == real_type)
	{
		real_type = NULL;
	}
	sf_on_const(type, real_type, &syn_const->val);
}

void parser_on_enum_begin(PARSER *self, const char* name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	sf_on_enum_begin(name);
}

void parser_on_enum_field(PARSER *self, const enum_def_t* enum_def)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	sf_on_enum_field(enum_def);
}

void parser_on_enum_end(PARSER *self, const char* name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	TDR_UNUSED(name);
	sf_on_enum_end();
}

void parser_on_union_begin(PARSER *self, const char* name, const char *etype)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	sf_on_union_begin(name, etype);
}

void parser_on_union_field(PARSER *self, const syn_union_field_t* union_field)
{
	const syn_simple_type_t *type = NULL;
	const syn_simple_type_t *real_type = NULL;
	const char *comment = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	type = &union_field->simple_type;
	real_type = symbols_get_real_type(&self->symbols, type);
	if(type == real_type)
	{
		real_type = NULL;
	}
	
	if(union_field->comment.text[0])
	{
		comment = union_field->comment.text;		
	}

	sf_on_union_field(union_field->key, type, real_type, union_field->name, comment);
}

void parser_on_union_end(PARSER *self, const char* name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	TDR_UNUSED(name);
	sf_on_union_end();
}

void parser_on_struct_begin(PARSER *self, const char* name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	sf_on_struct_begin(name);
}

void parser_on_struct_field(PARSER *self, const syn_field_t* struct_field)
{
	const syn_simple_type_t *type = NULL;
	const syn_simple_type_t *real_type = NULL;		
	const char *comment = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	if(struct_field->comment.text[0])
	{
		comment = struct_field->comment.text;
	}
	else
	{
		comment = NULL;
	}
	
	if(struct_field->type.type == E_SNT_CONTAINER)
	{
		if(struct_field->type.ct.ct == E_CT_VECTOR)
		{
			type = &struct_field->type.ct.vector_type;
			real_type = symbols_get_real_type(&self->symbols, type);
			if(type == real_type)
			{
				real_type = NULL;
			}

			sf_on_struct_field(&struct_field->condition, type, real_type, struct_field->type.ct.vector_length
				, struct_field->identifier, comment);
		}
	}
	else
	{
		type = &struct_field->type.st;
		real_type = symbols_get_real_type(&self->symbols, type);
		if(type == real_type)
		{
			real_type = NULL;
		}

		sf_on_struct_field(&struct_field->condition, type, real_type, NULL
			, struct_field->identifier, comment);
	}
}

void parser_on_struct_end(PARSER *self, const char* name)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	TDR_UNUSED(name);
	sf_on_struct_end();
}

void parser_on_unit_comment(PARSER *self, const syn_unix_comment_t* comment)
{
	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	if(comment->text[0])
	{
		sf_on_comment(comment->text);
	}	
}
