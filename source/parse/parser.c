#include "parse/parser.h"
#include "error.h"

#include "scanner_l.h"
#include "parser_y.h"
#include "parse/scanner.h"
#include "globals.h"

#include "symbols.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parse/scanner.h"

#include "script/script_functions.h"


void parser_init(PARSER *self)
{
	symbols_init(&self->symbols);
}

static void on_document_begin(PARSER *self, const YYLTYPE *yylloc, const char *file_name)
{
	generator_on_document_begin(self->generator, yylloc, file_name);
}

static void on_document_end(PARSER *self, const YYLTYPE *yylloc, const char *file_name)
{
	generator_on_document_end(self->generator, yylloc, file_name);
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
	char filename[TLIBC_MAX_PATH_LENGTH];
	FILE *fout;
	snprintf(filename, TLIBC_MAX_PATH_LENGTH, "%s.%s", self->generator->target_filename, DEP_SUFFIX);
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

	on_document_begin(self, NULL, file_name);
	ret = tdrparse(&self->scanner);
	if(make_rule)
	{
		parser_make_rule(self);
	}	
	on_document_end(self, NULL, file_name);
	scanner_pop(&self->scanner);
	symbols_clear(&self->symbols);
	return E_TD_NOERROR;
}

void parser_on_struct_begin_old(PARSER *self, const YYLTYPE *yylloc, const char* struct_name)
{
	if(scanner_size(&self->scanner) == 1)
	{	
		generator_on_struct_begin(self->generator, yylloc, struct_name);		
	}
}

void parser_on_field_old(PARSER *self, const YYLTYPE *yylloc, const syn_field_t *pn_field)
{
	if(scanner_size(&self->scanner) == 1)
	{
		generator_on_field(self->generator, yylloc, pn_field);
	}
}

void parser_on_struct_end_old(PARSER *self, const YYLTYPE *yylloc, const syn_struct_t *pn_struct)
{
	if(scanner_size(&self->scanner) == 1)
	{
		generator_on_struct_end(self->generator, yylloc, pn_struct);	
	}
}

//do
void parser_on_definition(PARSER *self, const YYLTYPE *yylloc, const syn_definition_t *pn_definition)
{
	if(scanner_size(&self->scanner) == 1)
	{
		generator_on_definition(self->generator, yylloc, pn_definition);
	}
	
	if(pn_definition->type == E_DT_IMPORT)
	{
		char file_name[TLIBC_MAX_PATH_LENGTH];
		snprintf(file_name, TLIBC_MAX_PATH_LENGTH, "%s", pn_definition->definition.de_import.package_name);
		file_name[TLIBC_MAX_PATH_LENGTH - 1] = 0;
		parser_push(self, file_name);
	}
}





void parser_on_import(PARSER *self, const syn_import_t* syn_import)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
	sf_on_import(syn_import->package_name);
}

void parser_on_typedef(PARSER *self, const syn_typedef_t *syn_typedef)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_const(PARSER *self, const syn_const_t *syn_const)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}

}

void parser_on_enum_begin(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_enum_field(PARSER *self, const enum_def_t* enum_def)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_enum_end(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_union_begin(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_union_field(PARSER *self, const syn_union_field_t* union_field)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_union_end(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_struct_begin(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_struct_field(PARSER *self, const syn_field_t* union_field)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_struct_end(PARSER *self, const char* name)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}
}

void parser_on_unit_comment(PARSER *self, const syn_unix_comment_t* comment)
{
	if(scanner_size(&self->scanner) != 1)
	{
		return;
	}

}
