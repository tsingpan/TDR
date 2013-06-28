#ifndef _H_DDEKIT_XML_ENCODING_WRITER
#define _H_DDEKIT_XML_ENCODING_WRITER

#include "hotpot/hp_platform.h"
#include "encoding/ddekit_encoding_writer.h"

#include <stdio.h>

#include "hotpot/hp_writer.h"
#include "hotpot/hp_value.h"


typedef struct _XML_WRITER
{
	HPAbstractWriter super;
	FILE *f;
	hpuint32 level;
}XML_WRITER;

HP_API hpint32 hp_xml_writer_init(XML_WRITER *self, FILE *fout);
HP_API hpint32 hp_xml_writer_begin(HPAbstractWriter *super, const HPVar *name);
HP_API hpint32 hp_xml_writer_write(HPAbstractWriter* super, const HPVar *var);
HP_API hpint32 hp_xml_writer_end(HPAbstractWriter *super);

#endif//_H_DDEKIT_XML_ENCODING_WRITER
