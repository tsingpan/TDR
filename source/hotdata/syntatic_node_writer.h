#ifndef _H_syntactic_node_hd_writer_header
#define _H_syntactic_node_hd_writer_header


#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "syntactic_node.h"


void write_ST_UNIX_COMMENT(HPAbstractWriter *self, const ST_UNIX_COMMENT* data);

void write_UN_VALUE(HPAbstractWriter *self, const UN_VALUE* data ,SN_VALUE_TYPE s);

void write_ST_VALUE(HPAbstractWriter *self, const ST_VALUE* data);

void write_ST_TYPE(HPAbstractWriter *self, const ST_TYPE* data);

void write_ST_Parameter(HPAbstractWriter *self, const ST_Parameter* data);

void write_ST_Parameters(HPAbstractWriter *self, const ST_Parameters* data);

void write_ST_ARGUMENTS(HPAbstractWriter *self, const ST_ARGUMENTS* data);

void write_ST_Expression(HPAbstractWriter *self, const ST_Expression* data);

void write_ST_CONDITION(HPAbstractWriter *self, const ST_CONDITION* data);

void write_ST_FIELD(HPAbstractWriter *self, const ST_FIELD* data);

void write_ST_TypeAnnotation(HPAbstractWriter *self, const ST_TypeAnnotation* data);

void write_ST_TypeAnnotations(HPAbstractWriter *self, const ST_TypeAnnotations* data);

void write_ST_Import(HPAbstractWriter *self, const ST_Import* data);

void write_ST_Const(HPAbstractWriter *self, const ST_Const* data);

void write_ST_ENUM_DEF(HPAbstractWriter *self, const ST_ENUM_DEF* data);

void write_ST_ENUM(HPAbstractWriter *self, const ST_ENUM* data);

void write_ST_STRUCT(HPAbstractWriter *self, const ST_STRUCT* data);

void write_ST_UNION(HPAbstractWriter *self, const ST_UNION* data);

void write_ST_TYPEDEF(HPAbstractWriter *self, const ST_TYPEDEF* data);

void write_UN_DEFINITION(HPAbstractWriter *self, const UN_DEFINITION* data, EN_DEFINITION_TYPE s);

void write_ST_DEFINITION(HPAbstractWriter *self, const ST_DEFINITION* data);

#endif//_H_syntactic_node_hd_writer_header
