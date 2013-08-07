#ifndef _H_syntactic_node_hd_WRITER
#define _H_syntactic_node_hd_WRITER
#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_writer.h"
#include "syntactic_node.h"
#include "syntactic_node_writer.h"
void write_ST_UNIX_COMMENT(HPAbstractWriter *self, const ST_UNIX_COMMENT *data)
{
	write_struct_begin(self, "ST_UNIX_COMMENT");
	write_field_begin(self, "empty");
	write_bool(self, data->empty);
	write_field_end(self, "empty");
	write_field_begin(self, "text");
	write_string(self, data->text);
	write_field_end(self, "text");
	write_struct_end(self, "ST_UNIX_COMMENT");
}
void write_SN_VALUE_TYPE_name(HPAbstractWriter *self, const SN_VALUE_TYPE data)
{
	switch(data)
	{
	case E_SNVT_IDENTIFIER:
		write_enum_name(self, "E_SNVT_IDENTIFIER");
		break;
	case E_SNVT_CHAR:
		write_enum_name(self, "E_SNVT_CHAR");
		break;
	case E_SNVT_DOUBLE:
		write_enum_name(self, "E_SNVT_DOUBLE");
		break;
	case E_SNVT_STRING:
		write_enum_name(self, "E_SNVT_STRING");
		break;
	case E_SNVT_BOOL:
		write_enum_name(self, "E_SNVT_BOOL");
		break;
	case E_SNVT_INT64:
		write_enum_name(self, "E_SNVT_INT64");
		break;
	case E_SNVT_UINT64:
		write_enum_name(self, "E_SNVT_UINT64");
		break;
	case E_SNVT_HEX_INT64:
		write_enum_name(self, "E_SNVT_HEX_INT64");
		break;
	case E_SNVT_HEX_UINT64:
		write_enum_name(self, "E_SNVT_HEX_UINT64");
		break;
	}
}
void write_SN_VALUE_TYPE_number(HPAbstractWriter *self, const SN_VALUE_TYPE data)
{
	write_enum_number(self, data);
}
void write_SN_VALUE_TYPE(HPAbstractWriter *self, const SN_VALUE_TYPE data)
{
	write_SN_VALUE_TYPE_name(self, data);
	write_SN_VALUE_TYPE_number(self, data);
}
void write_UN_VALUE(HPAbstractWriter *self, const UN_VALUE *data , const SN_VALUE_TYPE s)
{
	write_struct_begin(self, "UN_VALUE");
	switch(s)
	{
	case E_SNVT_INT64:
		{
			write_field_begin(self, "i64");
			write_int64(self, data->i64);
			write_field_end(self, "i64");
			break;
		}
	case E_SNVT_HEX_INT64:
		{
			write_field_begin(self, "hex_i64");
			write_int64(self, data->hex_i64);
			write_field_end(self, "hex_i64");
			break;
		}
	case E_SNVT_UINT64:
		{
			write_field_begin(self, "ui64");
			write_uint64(self, data->ui64);
			write_field_end(self, "ui64");
			break;
		}
	case E_SNVT_HEX_UINT64:
		{
			write_field_begin(self, "hex_ui64");
			write_uint64(self, data->hex_ui64);
			write_field_end(self, "hex_ui64");
			break;
		}
	case E_SNVT_STRING:
		{
			write_field_begin(self, "str");
			write_string(self, data->str);
			write_field_end(self, "str");
			break;
		}
	case E_SNVT_BOOL:
		{
			write_field_begin(self, "b");
			write_bool(self, data->b);
			write_field_end(self, "b");
			break;
		}
	case E_SNVT_DOUBLE:
		{
			write_field_begin(self, "d");
			write_double(self, data->d);
			write_field_end(self, "d");
			break;
		}
	case E_SNVT_CHAR:
		{
			write_field_begin(self, "c");
			write_char(self, data->c);
			write_field_end(self, "c");
			break;
		}
	case E_SNVT_IDENTIFIER:
		{
			write_field_begin(self, "identifier");
			write_string(self, data->identifier);
			write_field_end(self, "identifier");
			break;
		}
	}
	write_struct_end(self, "UN_VALUE");
}
void write_ST_VALUE(HPAbstractWriter *self, const ST_VALUE *data)
{
	write_struct_begin(self, "ST_VALUE");
	write_field_begin(self, "type");
	write_SN_VALUE_TYPE(self, data->type);
	write_field_end(self, "type");
	write_field_begin(self, "val");
	write_UN_VALUE(self, &data->val, data->type);
	write_field_end(self, "val");
	write_struct_end(self, "ST_VALUE");
}
void write_SN_SIMPLE_TYPE_name(HPAbstractWriter *self, const SN_SIMPLE_TYPE data)
{
	switch(data)
	{
	case E_ST_INT8:
		write_enum_name(self, "E_ST_INT8");
		break;
	case E_ST_INT16:
		write_enum_name(self, "E_ST_INT16");
		break;
	case E_ST_INT32:
		write_enum_name(self, "E_ST_INT32");
		break;
	case E_ST_INT64:
		write_enum_name(self, "E_ST_INT64");
		break;
	case E_ST_UINT8:
		write_enum_name(self, "E_ST_UINT8");
		break;
	case E_ST_UINT16:
		write_enum_name(self, "E_ST_UINT16");
		break;
	case E_ST_UINT32:
		write_enum_name(self, "E_ST_UINT32");
		break;
	case E_ST_UINT64:
		write_enum_name(self, "E_ST_UINT64");
		break;
	case E_ST_BOOL:
		write_enum_name(self, "E_ST_BOOL");
		break;
	case E_ST_CHAR:
		write_enum_name(self, "E_ST_CHAR");
		break;
	case E_ST_DOUBLE:
		write_enum_name(self, "E_ST_DOUBLE");
		break;
	}
}
void write_SN_SIMPLE_TYPE_number(HPAbstractWriter *self, const SN_SIMPLE_TYPE data)
{
	write_enum_number(self, data);
}
void write_SN_SIMPLE_TYPE(HPAbstractWriter *self, const SN_SIMPLE_TYPE data)
{
	write_SN_SIMPLE_TYPE_name(self, data);
	write_SN_SIMPLE_TYPE_number(self, data);
}
void write_SN_CONTAINER_TYPE_name(HPAbstractWriter *self, const SN_CONTAINER_TYPE data)
{
	switch(data)
	{
	case E_CT_VECTOR:
		write_enum_name(self, "E_CT_VECTOR");
		break;
	case E_CT_STRING:
		write_enum_name(self, "E_CT_STRING");
		break;
	}
}
void write_SN_CONTAINER_TYPE_number(HPAbstractWriter *self, const SN_CONTAINER_TYPE data)
{
	write_enum_number(self, data);
}
void write_SN_CONTAINER_TYPE(HPAbstractWriter *self, const SN_CONTAINER_TYPE data)
{
	write_SN_CONTAINER_TYPE_name(self, data);
	write_SN_CONTAINER_TYPE_number(self, data);
}
void write_SN_TYPE_name(HPAbstractWriter *self, const SN_TYPE data)
{
	switch(data)
	{
	case E_SNT_SIMPLE:
		write_enum_name(self, "E_SNT_SIMPLE");
		break;
	case E_SNT_CONTAINER:
		write_enum_name(self, "E_SNT_CONTAINER");
		break;
	case E_SNT_REFER:
		write_enum_name(self, "E_SNT_REFER");
		break;
	}
}
void write_SN_TYPE_number(HPAbstractWriter *self, const SN_TYPE data)
{
	write_enum_number(self, data);
}
void write_SN_TYPE(HPAbstractWriter *self, const SN_TYPE data)
{
	write_SN_TYPE_name(self, data);
	write_SN_TYPE_number(self, data);
}
void write_ST_TYPE(HPAbstractWriter *self, const ST_TYPE *data)
{
	write_struct_begin(self, "ST_TYPE");
	write_field_begin(self, "type");
	write_SN_TYPE(self, data->type);
	write_field_end(self, "type");
	if (data->type == E_SNT_SIMPLE)
	{
		write_field_begin(self, "st");
		write_SN_SIMPLE_TYPE(self, data->st);
		write_field_end(self, "st");
	}
	if (data->type == E_SNT_CONTAINER)
	{
		write_field_begin(self, "ct");
		write_SN_CONTAINER_TYPE(self, data->ct);
		write_field_end(self, "ct");
	}
	if (data->type == E_SNT_REFER)
	{
		write_field_begin(self, "ot");
		write_string(self, data->ot);
		write_field_end(self, "ot");
	}
	write_struct_end(self, "ST_TYPE");
}
void write_ST_Parameter(HPAbstractWriter *self, const ST_Parameter *data)
{
	write_struct_begin(self, "ST_Parameter");
	write_field_begin(self, "type");
	write_ST_TYPE(self, &data->type);
	write_field_end(self, "type");
	write_field_begin(self, "identifier");
	write_string(self, data->identifier);
	write_field_end(self, "identifier");
	write_struct_end(self, "ST_Parameter");
}
void write_ST_Parameters(HPAbstractWriter *self, const ST_Parameters *data)
{
	write_struct_begin(self, "ST_Parameters");
	write_counter(self, "par_list_num", data->par_list_num);
	write_field_begin(self, "par_list");
	{
		hpuint32 i;
		write_vector_begin(self);
		for(i = 0; i < data->par_list_num; ++i)
		{
			write_vector_item_begin(self, i);
			write_ST_Parameter(self, &data->par_list[i]);
			write_vector_item_end(self, i);
		}
		write_vector_end(self);
	}
	write_field_end(self, "par_list");
	write_struct_end(self, "ST_Parameters");
}
void write_ST_ARGUMENTS(HPAbstractWriter *self, const ST_ARGUMENTS *data)
{
	write_struct_begin(self, "ST_ARGUMENTS");
	write_counter(self, "arg_list_num", data->arg_list_num);
	write_field_begin(self, "arg_list");
	{
		hpuint32 i;
		write_vector_begin(self);
		for(i = 0; i < data->arg_list_num; ++i)
		{
			write_vector_item_begin(self, i);
			write_ST_TYPE(self, &data->arg_list[i]);
			write_vector_item_end(self, i);
		}
		write_vector_end(self);
	}
	write_field_end(self, "arg_list");
	write_struct_end(self, "ST_ARGUMENTS");
}
void write_ST_EXPRESSION_OPER_name(HPAbstractWriter *self, const ST_EXPRESSION_OPER data)
{
	switch(data)
	{
	case E_EO_AND:
		write_enum_name(self, "E_EO_AND");
		break;
	case E_EO_EQUAL:
		write_enum_name(self, "E_EO_EQUAL");
		break;
	}
}
void write_ST_EXPRESSION_OPER_number(HPAbstractWriter *self, const ST_EXPRESSION_OPER data)
{
	write_enum_number(self, data);
}
void write_ST_EXPRESSION_OPER(HPAbstractWriter *self, const ST_EXPRESSION_OPER data)
{
	write_ST_EXPRESSION_OPER_name(self, data);
	write_ST_EXPRESSION_OPER_number(self, data);
}
void write_ST_Expression(HPAbstractWriter *self, const ST_Expression *data)
{
	write_struct_begin(self, "ST_Expression");
	write_field_begin(self, "neg");
	write_bool(self, data->neg);
	write_field_end(self, "neg");
	write_field_begin(self, "op0");
	write_ST_VALUE(self, &data->op0);
	write_field_end(self, "op0");
	write_field_begin(self, "oper");
	write_ST_EXPRESSION_OPER(self, data->oper);
	write_field_end(self, "oper");
	write_field_begin(self, "op1");
	write_ST_VALUE(self, &data->op1);
	write_field_end(self, "op1");
	write_struct_end(self, "ST_Expression");
}
void write_ST_CONDITION(HPAbstractWriter *self, const ST_CONDITION *data)
{
	write_struct_begin(self, "ST_CONDITION");
	write_field_begin(self, "empty");
	write_bool(self, data->empty);
	write_field_end(self, "empty");
	write_field_begin(self, "exp");
	write_ST_Expression(self, &data->exp);
	write_field_end(self, "exp");
	write_struct_end(self, "ST_CONDITION");
}
void write_ST_FIELD(HPAbstractWriter *self, const ST_FIELD *data)
{
	write_struct_begin(self, "ST_FIELD");
	write_field_begin(self, "condition");
	write_ST_CONDITION(self, &data->condition);
	write_field_end(self, "condition");
	write_field_begin(self, "type");
	write_ST_TYPE(self, &data->type);
	write_field_end(self, "type");
	write_field_begin(self, "args");
	write_ST_ARGUMENTS(self, &data->args);
	write_field_end(self, "args");
	write_field_begin(self, "identifier");
	write_string(self, data->identifier);
	write_field_end(self, "identifier");
	write_field_begin(self, "comment");
	write_ST_UNIX_COMMENT(self, &data->comment);
	write_field_end(self, "comment");
	write_struct_end(self, "ST_FIELD");
}
void write_TA_TYPE_name(HPAbstractWriter *self, const TA_TYPE data)
{
	switch(data)
	{
	case E_TA_SWITCH:
		write_enum_name(self, "E_TA_SWITCH");
		break;
	case E_TA_UNIQUE:
		write_enum_name(self, "E_TA_UNIQUE");
		break;
	case E_TA_LOWER_BOUND:
		write_enum_name(self, "E_TA_LOWER_BOUND");
		break;
	case E_TA_UPPER_BOUND:
		write_enum_name(self, "E_TA_UPPER_BOUND");
		break;
	}
}
void write_TA_TYPE_number(HPAbstractWriter *self, const TA_TYPE data)
{
	write_enum_number(self, data);
}
void write_TA_TYPE(HPAbstractWriter *self, const TA_TYPE data)
{
	write_TA_TYPE_name(self, data);
	write_TA_TYPE_number(self, data);
}
void write_ST_TypeAnnotation(HPAbstractWriter *self, const ST_TypeAnnotation *data)
{
	write_struct_begin(self, "ST_TypeAnnotation");
	write_field_begin(self, "type");
	write_TA_TYPE(self, data->type);
	write_field_end(self, "type");
	write_field_begin(self, "val");
	write_ST_VALUE(self, &data->val);
	write_field_end(self, "val");
	write_struct_end(self, "ST_TypeAnnotation");
}
void write_ST_TypeAnnotations(HPAbstractWriter *self, const ST_TypeAnnotations *data)
{
	write_struct_begin(self, "ST_TypeAnnotations");
	write_counter(self, "ta_list_num", data->ta_list_num);
	write_field_begin(self, "ta_list");
	{
		hpuint32 i;
		write_vector_begin(self);
		for(i = 0; i < data->ta_list_num; ++i)
		{
			write_vector_item_begin(self, i);
			write_ST_TypeAnnotation(self, &data->ta_list[i]);
			write_vector_item_end(self, i);
		}
		write_vector_end(self);
	}
	write_field_end(self, "ta_list");
	write_struct_end(self, "ST_TypeAnnotations");
}
void write_ST_Import(HPAbstractWriter *self, const ST_Import *data)
{
	write_struct_begin(self, "ST_Import");
	write_field_begin(self, "package_name");
	write_string(self, data->package_name);
	write_field_end(self, "package_name");
	write_struct_end(self, "ST_Import");
}
void write_ST_Const(HPAbstractWriter *self, const ST_Const *data)
{
	write_struct_begin(self, "ST_Const");
	write_field_begin(self, "type");
	write_ST_TYPE(self, &data->type);
	write_field_end(self, "type");
	write_field_begin(self, "identifier");
	write_string(self, data->identifier);
	write_field_end(self, "identifier");
	write_field_begin(self, "val");
	write_ST_VALUE(self, &data->val);
	write_field_end(self, "val");
	write_struct_end(self, "ST_Const");
}
void write_ST_ENUM_DEF(HPAbstractWriter *self, const ST_ENUM_DEF *data)
{
	write_struct_begin(self, "ST_ENUM_DEF");
	write_field_begin(self, "identifier");
	write_string(self, data->identifier);
	write_field_end(self, "identifier");
	write_field_begin(self, "val");
	write_ST_VALUE(self, &data->val);
	write_field_end(self, "val");
	write_field_begin(self, "comment");
	write_ST_UNIX_COMMENT(self, &data->comment);
	write_field_end(self, "comment");
	write_struct_end(self, "ST_ENUM_DEF");
}
void write_ST_ENUM(HPAbstractWriter *self, const ST_ENUM *data)
{
	write_struct_begin(self, "ST_ENUM");
	write_field_begin(self, "name");
	write_string(self, data->name);
	write_field_end(self, "name");
	write_field_begin(self, "type_annotations");
	write_ST_TypeAnnotations(self, &data->type_annotations);
	write_field_end(self, "type_annotations");
	write_counter(self, "enum_def_list_num", data->enum_def_list_num);
	write_field_begin(self, "enum_def_list");
	{
		hpuint32 i;
		write_vector_begin(self);
		for(i = 0; i < data->enum_def_list_num; ++i)
		{
			write_vector_item_begin(self, i);
			write_ST_ENUM_DEF(self, &data->enum_def_list[i]);
			write_vector_item_end(self, i);
		}
		write_vector_end(self);
	}
	write_field_end(self, "enum_def_list");
	write_struct_end(self, "ST_ENUM");
}
void write_ST_FIELD_LIST(HPAbstractWriter *self, const ST_FIELD_LIST *data)
{
	write_struct_begin(self, "ST_FIELD_LIST");
	write_counter(self, "field_list_num", data->field_list_num);
	write_field_begin(self, "field_list");
	{
		hpuint32 i;
		write_vector_begin(self);
		for(i = 0; i < data->field_list_num; ++i)
		{
			write_vector_item_begin(self, i);
			write_ST_FIELD(self, &data->field_list[i]);
			write_vector_item_end(self, i);
		}
		write_vector_end(self);
	}
	write_field_end(self, "field_list");
	write_struct_end(self, "ST_FIELD_LIST");
}
void write_ST_STRUCT(HPAbstractWriter *self, const ST_STRUCT *data)
{
	write_struct_begin(self, "ST_STRUCT");
	write_field_begin(self, "ta");
	write_ST_TypeAnnotations(self, &data->ta);
	write_field_end(self, "ta");
	write_field_begin(self, "name");
	write_string(self, data->name);
	write_field_end(self, "name");
	write_field_begin(self, "parameters");
	write_ST_Parameters(self, &data->parameters);
	write_field_end(self, "parameters");
	write_field_begin(self, "field_list");
	write_ST_FIELD_LIST(self, &data->field_list);
	write_field_end(self, "field_list");
	write_struct_end(self, "ST_STRUCT");
}
void write_ST_UNION(HPAbstractWriter *self, const ST_UNION *data)
{
	write_struct_begin(self, "ST_UNION");
	write_field_begin(self, "ta");
	write_ST_TypeAnnotations(self, &data->ta);
	write_field_end(self, "ta");
	write_field_begin(self, "name");
	write_string(self, data->name);
	write_field_end(self, "name");
	write_field_begin(self, "parameters");
	write_ST_Parameters(self, &data->parameters);
	write_field_end(self, "parameters");
	write_field_begin(self, "field_list");
	write_ST_FIELD_LIST(self, &data->field_list);
	write_field_end(self, "field_list");
	write_struct_end(self, "ST_UNION");
}
void write_ST_TYPEDEF(HPAbstractWriter *self, const ST_TYPEDEF *data)
{
	write_struct_begin(self, "ST_TYPEDEF");
	write_field_begin(self, "type");
	write_ST_TYPE(self, &data->type);
	write_field_end(self, "type");
	write_field_begin(self, "name");
	write_string(self, data->name);
	write_field_end(self, "name");
	write_struct_end(self, "ST_TYPEDEF");
}
void write_EN_DEFINITION_TYPE_name(HPAbstractWriter *self, const EN_DEFINITION_TYPE data)
{
	switch(data)
	{
	case E_DT_IMPORT:
		write_enum_name(self, "E_DT_IMPORT");
		break;
	case E_DT_CONST:
		write_enum_name(self, "E_DT_CONST");
		break;
	case E_DT_ENUM:
		write_enum_name(self, "E_DT_ENUM");
		break;
	case E_DT_STRUCT:
		write_enum_name(self, "E_DT_STRUCT");
		break;
	case E_DT_UNION:
		write_enum_name(self, "E_DT_UNION");
		break;
	case E_DT_TYPEDEF:
		write_enum_name(self, "E_DT_TYPEDEF");
		break;
	case E_DT_UNIX_COMMENT:
		write_enum_name(self, "E_DT_UNIX_COMMENT");
		break;
	}
}
void write_EN_DEFINITION_TYPE_number(HPAbstractWriter *self, const EN_DEFINITION_TYPE data)
{
	write_enum_number(self, data);
}
void write_EN_DEFINITION_TYPE(HPAbstractWriter *self, const EN_DEFINITION_TYPE data)
{
	write_EN_DEFINITION_TYPE_name(self, data);
	write_EN_DEFINITION_TYPE_number(self, data);
}
void write_UN_DEFINITION(HPAbstractWriter *self, const UN_DEFINITION *data , const EN_DEFINITION_TYPE s)
{
	write_struct_begin(self, "UN_DEFINITION");
	switch(s)
	{
	case E_DT_IMPORT:
		{
			write_field_begin(self, "de_import");
			write_ST_Import(self, &data->de_import);
			write_field_end(self, "de_import");
			break;
		}
	case E_DT_CONST:
		{
			write_field_begin(self, "de_const");
			write_ST_Const(self, &data->de_const);
			write_field_end(self, "de_const");
			break;
		}
	case E_DT_UNIX_COMMENT:
		{
			write_field_begin(self, "de_unix_comment");
			write_ST_UNIX_COMMENT(self, &data->de_unix_comment);
			write_field_end(self, "de_unix_comment");
			break;
		}
	case E_DT_ENUM:
		{
			write_field_begin(self, "de_enum");
			write_ST_ENUM(self, &data->de_enum);
			write_field_end(self, "de_enum");
			break;
		}
	case E_DT_STRUCT:
		{
			write_field_begin(self, "de_struct");
			write_ST_STRUCT(self, &data->de_struct);
			write_field_end(self, "de_struct");
			break;
		}
	case E_DT_UNION:
		{
			write_field_begin(self, "de_union");
			write_ST_UNION(self, &data->de_union);
			write_field_end(self, "de_union");
			break;
		}
	case E_DT_TYPEDEF:
		{
			write_field_begin(self, "de_typedef");
			write_ST_TYPEDEF(self, &data->de_typedef);
			write_field_end(self, "de_typedef");
			break;
		}
	}
	write_struct_end(self, "UN_DEFINITION");
}
void write_ST_DEFINITION(HPAbstractWriter *self, const ST_DEFINITION *data)
{
	write_struct_begin(self, "ST_DEFINITION");
	write_field_begin(self, "type");
	write_EN_DEFINITION_TYPE(self, data->type);
	write_field_end(self, "type");
	write_field_begin(self, "definition");
	write_UN_DEFINITION(self, &data->definition, data->type);
	write_field_end(self, "definition");
	write_struct_end(self, "ST_DEFINITION");
}
#endif//_H_syntactic_node_hd_WRITER
