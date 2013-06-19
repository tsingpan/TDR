#include "script_parser.h"

#include "hotpot/hp_error.h"

hpint32 script_parser(SCRIPT_PARSER *self, FILE *fin)
{
	hpint32 ret;
	hpint64 data;


	yyscriptlex_init_extra(&self->scanner, &self->scanner);
	self->bs = yyscript_create_buffer(fin, YY_BUF_SIZE, self->scanner);
	self->bs->yy_bs_column = 1;
	yyscript_switch_to_buffer(self->bs, self->scanner);
	ret = yyscriptparse(&self->scanner);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}

	return self->result;
}
