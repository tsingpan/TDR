#include "json_parser.h"
#include "json_y.h"
#include "hotpot/hp_error.h"



hpint32 json_open_file(yyscan_t *super, const char *file_name)
{
	JSON_PARSER *self = HP_CONTAINER_OF(super, JSON_PARSER, scanner);

	self->stack[self->stack_num].f = fopen(file_name, "r");
	if(self->stack[self->stack_num].f == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs = yyjson_create_buffer(self->stack[self->stack_num].f, YY_BUF_SIZE, self->scanner);
	if(self->stack[self->stack_num].bs == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs->yy_bs_column = 1;
	++self->stack_num;
	yyjson_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 json_close_file(yyscan_t *super)
{
	JSON_PARSER *self = HP_CONTAINER_OF(super, JSON_PARSER, scanner);

	fclose(self->stack[self->stack_num - 1].f);
	yyjson_delete_buffer(self->stack[self->stack_num - 1].bs, self->scanner);

	--self->stack_num;
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}
	yyjson_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}


hpint32 json_parser(JSON_PARSER *self, const char* file_name, HPAbstractWriter *writer)
{
	hpint32 ret;
	hpint64 data;


	self->result = HP_INVALID_ERROR_CODE;

	yyjsonlex_init_extra(&self->scanner, &self->scanner);

	json_open_file(&self->scanner, file_name);

	self->writer = writer;

	ret = yyjsonparse(&self->scanner);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}


	return self->result;
}