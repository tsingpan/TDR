#include "encoding/ddekit_xml_encoding_writer.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"

#include <string.h>
#include <assert.h>

#define DDEKIT_PRINT_TAB(fout, level) \
do\
{\
	hpuint32 i;\
	for(i = 0; i < level; ++i)\
	{\
		fprintf(self->f, "\t");\
	}\
}while(0)

HP_API hpint32 hp_xml_writer_init(XML_WRITER *self, FILE *fout)
{
	self->f = fout;
	self->level = 0;

	self->super.begin = hp_xml_writer_begin;
	self->super.write = hp_xml_writer_write;
	self->super.end = hp_xml_writer_end;


	return E_HP_NOERROR;
}
static void echo_char(FILE *fout, const char val)
{
	switch(val)
	{
	case '<':
		fprintf(fout, "&lt");
		break;
	case '>':
		fprintf(fout, "&gt");
		break;
	case '&':
		fprintf(fout, "&amp");
		break;
	case '\'':
		fprintf(fout, "&apos");
		break;
	case '\"':
		fprintf(fout, "&auot");
		break;
	default:
		fprintf(fout, "%c", val);
		break;
	}
}

HP_API hpint32 hp_xml_writer_begin(HPAbstractWriter *super, const HPVar *name)
{
	const char *s;
	hpuint32 i;
	XML_WRITER *self = HP_CONTAINER_OF(super, XML_WRITER, super);

	DDEKIT_PRINT_TAB(self->f, self->level);

	fprintf(self->f, "<");
	for(s = name->val.str; *s; ++s)
	{
		echo_char(self->f, *s);
	}
	fprintf(self->f, ">");
	
	++(self->level);
}



HP_API hpint32 hp_xml_writer_write(HPAbstractWriter* super, const HPVar *var)
{
	XML_WRITER *self = HP_CONTAINER_OF(super, XML_WRITER, super);
	DDEKIT_PRINT_TAB(self->f, self->level);
	switch(var->type)
	{
	case E_HP_INT8:
		fprintf(self->f, "%d", var->val.i8);
		break;
	case E_HP_INT16:
		fprintf(self->f, "%d", var->val.i16);
		break;
	case E_HP_INT32:
		fprintf(self->f, "%d", var->val.i32);
		break;
	case E_HP_INT64:
		fprintf(self->f, "%"PRIi64, var->val.i64);
		break;

	case E_HP_UINT8:
		fprintf(self->f, "%u", var->val.ui8);
		break;
	case E_HP_UINT16:
		fprintf(self->f, "%u", var->val.ui16);
		break;
	case E_HP_UINT32:
		fprintf(self->f, "%u", var->val.ui32);
		break;
	case E_HP_UINT64:
		fprintf(self->f, "%"PRIu64, var->val.ui64);
		break;

	case E_HP_DOUBLE:
		fprintf(self->f, "%lf", var->val.d);
		break;

	case E_HP_CHAR:
		{
			echo_char(self->f, var->val.c);

			break;
		}
	case E_HP_BYTES:
		{
			hpuint32 i;
			for(i = 0;i < var->val.bytes.len; ++i)
			{
				echo_char(self->f, var->val.bytes.ptr[i]);
			}
			break;
		}
	case E_HP_STRING:
		{
			const char *s;
			for(s = var->val.str; *s; ++s)
			{
				echo_char(self->f, *s);
			}
			break;
		}
	}

	return E_HP_NOERROR;
}

HP_API hpint32 hp_xml_writer_end(HPAbstractWriter *super)
{
	const char *s;
	hpuint32 i;
	XML_WRITER *self = HP_CONTAINER_OF(super, XML_WRITER, super);
	--(self->level);
	fprintf(self->f, "</>\n");
	return E_HP_NOERROR;
}

