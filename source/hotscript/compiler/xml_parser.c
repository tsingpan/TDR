#include "xml_parser.h"
#include "xml_y.h"
#include "xml_l.h"
#include "hotpot/hp_error.h"
hpint32 xml_parser(XML_PARSER *self, FILE *fin)
{
	yylex_init_extra(&self->scanner, &self->scanner);
	self->bs = yy_create_buffer(fin, YY_BUF_SIZE, self->scanner);
	self->bs->yy_bs_column = 1;
	yy_switch_to_buffer(self->bs, self->scanner);

	yyparse(&self->scanner);

	return E_HP_NOERROR;
}
