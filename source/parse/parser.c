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

static void get_simple_type(const syn_simple_type_t *st, const char **type, const char **arg)
{
	*arg = NULL;
	switch(st->st)
	{
	case E_ST_INT8:
		*type = "int8";
		break;
	case E_ST_INT16:
		*type = "int16";
		break;
	case E_ST_INT32:
		*type = "int32";
		break;
	case E_ST_INT64:
		*type = "int64";
		break;
	case E_ST_UINT8:
		*type = "uint8";
		break;
	case E_ST_UINT16:
		*type = "uint16";
		break;
	case E_ST_UINT32:
		*type = "uint32";
		break;
	case E_ST_UINT64:
		*type = "uint64";
		break;
	case E_ST_CHAR:
		*type = "char";
		break;
	case E_ST_BOOL:
		*type = "bool";
		break;
	case E_ST_DOUBLE:
		*type = "double";
		break;
	case E_ST_STRING:
		*type = "string";
		*arg = st->string_length;
		break;
	case E_ST_REFER:
		*type = st->st_refer;
		break;
	}
}
void parser_on_typedef(PARSER *self, const syn_typedef_t *syn_typedef)
{
	const char *type = NULL;
	const char *arg = NULL;
	const char *new_type = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}
	get_simple_type(&syn_typedef->type, &type, &arg);
	new_type = syn_typedef->name;

	sf_on_typedef(type, arg, new_type);
}

void parser_on_const(PARSER *self, const syn_const_t *syn_const)
{
	const syn_simple_type_t *real_type = NULL;
	const syn_simple_type_t *type = NULL;
	const char *type_name = NULL;
	const char *real_type_name = NULL;
	const char *arg = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	
	type = &syn_const->type;
	real_type = symbols_get_real_type(&self->symbols, &syn_const->type);
	get_simple_type(type, &type_name, &arg);
	get_simple_type(real_type, &real_type_name, &arg);

	sf_on_const(type_name, real_type_name, &syn_const->val);
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
	const syn_simple_type_t *real_type = NULL;
	const syn_simple_type_t *type = NULL;
	const char *type_name = NULL;
	const char *real_type_name = NULL;
	const char *arg = NULL;

	if((scanner_size(&self->scanner) != 1) || (g_ls == NULL))
	{
		return;
	}

	type = &union_field->simple_type;
	real_type = symbols_get_real_type(&self->symbols, type);
	get_simple_type(type, &type_name, &arg);
	get_simple_type(real_type, &real_type_name, &arg);

	if(union_field->comment.text[0])
	{
		sf_on_union_field(union_field->key, type_name, real_type_name, arg, union_field->name, union_field->comment.text);
	}
	else
	{
		sf_on_union_field(union_field->key, type_name, real_type_name, arg, union_field->name, NULL);
	}
	
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

static const char* get_op_name(syn_expression_oper_t oper)
{
	switch (oper)
	{
	case E_EO_NON:
		return NULL;
	case E_EO_AND:
		return "&";
	case E_EO_EQUAL:
		return "==";
	case E_EO_UNEQUAL:
		return "!=";
	default:
		return NULL;
	}
}

void parser_on_struct_field(PARSER *self, const syn_field_t* struct_field)
{
	const syn_simple_type_t *type = NULL;
	const char *type_name = NULL;
	const char *type_arg = NULL;
	const syn_simple_type_t *real_type = NULL;		
	const char *real_type_name = NULL;
	const char *real_type_arg = NULL;
	const char *comment = NULL;
	const char *oper = NULL;
	const char *op0 = NULL;
	const syn_value_t *op1 = NULL;

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

	oper = get_op_name(struct_field->condition.oper);
	if(oper != NULL)
	{
		op0 = struct_field->condition.op0;
		op1 = &struct_field->condition.op1;
	}

	if(struct_field->type.type == E_SNT_CONTAINER)
	{
		if(struct_field->type.ct.ct == E_CT_VECTOR)
		{
			type = &struct_field->type.ct.vector_type;
			real_type = symbols_get_real_type(&self->symbols, type);
			get_simple_type(type, &type_name, &type_arg);
			get_simple_type(real_type, &real_type_name, &real_type_arg);

			sf_on_struct_vector_field(oper, op0, op1, type_name, real_type_name, type_arg, struct_field->type.ct.vector_length
				, struct_field->identifier, comment);
		}
	}
	else
	{
		type = &struct_field->type.st;
		real_type = symbols_get_real_type(&self->symbols, type);
		get_simple_type(type, &type_name, &type_arg);
		get_simple_type(real_type, &real_type_name, &real_type_arg);

		sf_on_struct_field(oper, op0, op1, type_name, real_type_name, type_arg
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
