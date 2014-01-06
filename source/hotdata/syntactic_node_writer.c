/**
 * Autogenerated by HotData (0.0.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#include "platform/tlibc_platform.h"
#include "hotdata/syntactic_node.h"
#include "hotdata/syntactic_node_writer.h"
#include "hotlib/hp_config_writer.h"
TLIBC_ERROR_CODE write_ST_UNIX_COMMENT(TLIBC_ABSTRACT_WRITER *self, const ST_UNIX_COMMENT *data)
{
    if(write_struct_begin(self, "ST_UNIX_COMMENT") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "empty") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_tbool(self, data->empty) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "empty") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "text") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->text) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "text") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_UNIX_COMMENT") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_VALUE_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const SN_VALUE_TYPE data)
{
    switch(data)
    {
        case E_SNVT_IDENTIFIER:
            if(write_enum_name(self, "E_SNVT_IDENTIFIER") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_CHAR:
            if(write_enum_name(self, "E_SNVT_CHAR") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_DOUBLE:
            if(write_enum_name(self, "E_SNVT_DOUBLE") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_STRING:
            if(write_enum_name(self, "E_SNVT_STRING") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_BOOL:
            if(write_enum_name(self, "E_SNVT_BOOL") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_INT64:
            if(write_enum_name(self, "E_SNVT_INT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_UINT64:
            if(write_enum_name(self, "E_SNVT_UINT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_HEX_INT64:
            if(write_enum_name(self, "E_SNVT_HEX_INT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNVT_HEX_UINT64:
            if(write_enum_name(self, "E_SNVT_HEX_UINT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_VALUE_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const SN_VALUE_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_SN_VALUE_TYPE(TLIBC_ABSTRACT_WRITER *self, const SN_VALUE_TYPE data)
{
    if(write_SN_VALUE_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_VALUE_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_UN_VALUE(TLIBC_ABSTRACT_WRITER *self, const UN_VALUE *data , SN_VALUE_TYPE s)
{
    if(write_struct_begin(self, "UN_VALUE") != E_TLIBC_NOERROR) goto ERROR_RET;
    switch(s)
    {
        case E_SNVT_INT64:
        {
            if(write_field_begin(self, "i64") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tint64(self, data->i64) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "i64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_HEX_INT64:
        {
            if(write_field_begin(self, "hex_i64") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tint64(self, data->hex_i64) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "hex_i64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_UINT64:
        {
            if(write_field_begin(self, "ui64") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tuint64(self, data->ui64) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "ui64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_HEX_UINT64:
        {
            if(write_field_begin(self, "hex_ui64") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tuint64(self, data->hex_ui64) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "hex_ui64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_STRING:
        {
            if(write_field_begin(self, "str") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_string(self, data->str) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "str") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_BOOL:
        {
            if(write_field_begin(self, "b") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tbool(self, data->b) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "b") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_DOUBLE:
        {
            if(write_field_begin(self, "d") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tdouble(self, data->d) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "d") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_CHAR:
        {
            if(write_field_begin(self, "c") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_tchar(self, data->c) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "c") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_SNVT_IDENTIFIER:
        {
            if(write_field_begin(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_string(self, data->identifier) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
    }
    if(write_struct_end(self, "UN_VALUE") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_VALUE(TLIBC_ABSTRACT_WRITER *self, const ST_VALUE *data)
{
    if(write_struct_begin(self, "ST_VALUE") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_VALUE_TYPE(self, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_UN_VALUE(self, &data->val, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_VALUE") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_SIMPLE_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const SN_SIMPLE_TYPE data)
{
    switch(data)
    {
        case E_ST_INT8:
            if(write_enum_name(self, "E_ST_INT8") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_INT16:
            if(write_enum_name(self, "E_ST_INT16") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_INT32:
            if(write_enum_name(self, "E_ST_INT32") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_INT64:
            if(write_enum_name(self, "E_ST_INT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_UINT8:
            if(write_enum_name(self, "E_ST_UINT8") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_UINT16:
            if(write_enum_name(self, "E_ST_UINT16") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_UINT32:
            if(write_enum_name(self, "E_ST_UINT32") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_UINT64:
            if(write_enum_name(self, "E_ST_UINT64") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_BOOL:
            if(write_enum_name(self, "E_ST_BOOL") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_CHAR:
            if(write_enum_name(self, "E_ST_CHAR") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_ST_DOUBLE:
            if(write_enum_name(self, "E_ST_DOUBLE") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_SIMPLE_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const SN_SIMPLE_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_SN_SIMPLE_TYPE(TLIBC_ABSTRACT_WRITER *self, const SN_SIMPLE_TYPE data)
{
    if(write_SN_SIMPLE_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_SIMPLE_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_CONTAINER_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const SN_CONTAINER_TYPE data)
{
    switch(data)
    {
        case E_CT_VECTOR:
            if(write_enum_name(self, "E_CT_VECTOR") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_CT_STRING:
            if(write_enum_name(self, "E_CT_STRING") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_CONTAINER_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const SN_CONTAINER_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_SN_CONTAINER_TYPE(TLIBC_ABSTRACT_WRITER *self, const SN_CONTAINER_TYPE data)
{
    if(write_SN_CONTAINER_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_CONTAINER_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const SN_TYPE data)
{
    switch(data)
    {
        case E_SNT_SIMPLE:
            if(write_enum_name(self, "E_SNT_SIMPLE") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNT_CONTAINER:
            if(write_enum_name(self, "E_SNT_CONTAINER") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_SNT_REFER:
            if(write_enum_name(self, "E_SNT_REFER") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_SN_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const SN_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_SN_TYPE(TLIBC_ABSTRACT_WRITER *self, const SN_TYPE data)
{
    if(write_SN_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_TYPE(TLIBC_ABSTRACT_WRITER *self, const ST_TYPE *data)
{
    if(write_struct_begin(self, "ST_TYPE") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_SN_TYPE(self, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(data->type == E_SNT_SIMPLE)
    {
        if(write_field_begin(self, "st") != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_SN_SIMPLE_TYPE(self, data->st) != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_field_end(self, "st") != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(data->type == E_SNT_CONTAINER)
    {
        if(write_field_begin(self, "ct") != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_SN_CONTAINER_TYPE(self, data->ct) != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_field_end(self, "ct") != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(data->type == E_SNT_REFER)
    {
        if(write_field_begin(self, "ot") != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_string(self, data->ot) != E_TLIBC_NOERROR) goto ERROR_RET;
        if(write_field_end(self, "ot") != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_struct_end(self, "ST_TYPE") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_Parameter(TLIBC_ABSTRACT_WRITER *self, const ST_Parameter *data)
{
    if(write_struct_begin(self, "ST_Parameter") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TYPE(self, &data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->identifier) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_Parameter") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_Parameters(TLIBC_ABSTRACT_WRITER *self, const ST_Parameters *data)
{
    if(write_struct_begin(self, "ST_Parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_counter(self, "par_list_num", data->par_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "par_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    {
        tuint32 i;
        if(write_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
        for(i = 0; i < data->par_list_num; ++i)
        {
            if(write_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_Parameter(self, &data->par_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
        }
        if(write_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_field_end(self, "par_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_Parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_ARGUMENTS(TLIBC_ABSTRACT_WRITER *self, const ST_ARGUMENTS *data)
{
    if(write_struct_begin(self, "ST_ARGUMENTS") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_counter(self, "arg_list_num", data->arg_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "arg_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    {
        tuint32 i;
        if(write_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
        for(i = 0; i < data->arg_list_num; ++i)
        {
            if(write_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_TYPE(self, &data->arg_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
        }
        if(write_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_field_end(self, "arg_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_ARGUMENTS") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_EXPRESSION_OPER_name(TLIBC_ABSTRACT_WRITER *self, const ST_EXPRESSION_OPER data)
{
    switch(data)
    {
        case E_EO_AND:
            if(write_enum_name(self, "E_EO_AND") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_EO_EQUAL:
            if(write_enum_name(self, "E_EO_EQUAL") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_EXPRESSION_OPER_number(TLIBC_ABSTRACT_WRITER *self, const ST_EXPRESSION_OPER data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_ST_EXPRESSION_OPER(TLIBC_ABSTRACT_WRITER *self, const ST_EXPRESSION_OPER data)
{
    if(write_ST_EXPRESSION_OPER_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_EXPRESSION_OPER_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_Expression(TLIBC_ABSTRACT_WRITER *self, const ST_Expression *data)
{
    if(write_struct_begin(self, "ST_Expression") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "neg") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_tbool(self, data->neg) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "neg") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "op0") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_VALUE(self, &data->op0) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "op0") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "oper") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_EXPRESSION_OPER(self, data->oper) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "oper") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "op1") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_VALUE(self, &data->op1) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "op1") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_Expression") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_CONDITION(TLIBC_ABSTRACT_WRITER *self, const ST_CONDITION *data)
{
    if(write_struct_begin(self, "ST_CONDITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "empty") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_tbool(self, data->empty) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "empty") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "exp") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_Expression(self, &data->exp) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "exp") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_CONDITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_FIELD(TLIBC_ABSTRACT_WRITER *self, const ST_FIELD *data)
{
    if(write_struct_begin(self, "ST_FIELD") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "condition") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_CONDITION(self, &data->condition) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "condition") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TYPE(self, &data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "args") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_ARGUMENTS(self, &data->args) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "args") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->identifier) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "comment") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_UNIX_COMMENT(self, &data->comment) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "comment") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_FIELD") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_TA_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const TA_TYPE data)
{
    switch(data)
    {
        case E_TA_SWITCH:
            if(write_enum_name(self, "E_TA_SWITCH") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_TA_UNIQUE:
            if(write_enum_name(self, "E_TA_UNIQUE") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_TA_LOWER_BOUND:
            if(write_enum_name(self, "E_TA_LOWER_BOUND") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_TA_UPPER_BOUND:
            if(write_enum_name(self, "E_TA_UPPER_BOUND") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_TA_COUNTER:
            if(write_enum_name(self, "E_TA_COUNTER") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_TA_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const TA_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_TA_TYPE(TLIBC_ABSTRACT_WRITER *self, const TA_TYPE data)
{
    if(write_TA_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_TA_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_TypeAnnotation(TLIBC_ABSTRACT_WRITER *self, const ST_TypeAnnotation *data)
{
    if(write_struct_begin(self, "ST_TypeAnnotation") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_TA_TYPE(self, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_VALUE(self, &data->val) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_TypeAnnotation") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_TypeAnnotations(TLIBC_ABSTRACT_WRITER *self, const ST_TypeAnnotations *data)
{
    if(write_struct_begin(self, "ST_TypeAnnotations") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_counter(self, "ta_list_num", data->ta_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "ta_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    {
        tuint32 i;
        if(write_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
        for(i = 0; i < data->ta_list_num; ++i)
        {
            if(write_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_TypeAnnotation(self, &data->ta_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
        }
        if(write_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_field_end(self, "ta_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_TypeAnnotations") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_Import(TLIBC_ABSTRACT_WRITER *self, const ST_Import *data)
{
    if(write_struct_begin(self, "ST_Import") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "package_name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->package_name) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "package_name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_Import") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_Const(TLIBC_ABSTRACT_WRITER *self, const ST_Const *data)
{
    if(write_struct_begin(self, "ST_Const") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TYPE(self, &data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->identifier) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_VALUE(self, &data->val) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_Const") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_ENUM_DEF(TLIBC_ABSTRACT_WRITER *self, const ST_ENUM_DEF *data)
{
    if(write_struct_begin(self, "ST_ENUM_DEF") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->identifier) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "identifier") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_VALUE(self, &data->val) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "val") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "comment") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_UNIX_COMMENT(self, &data->comment) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "comment") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_ENUM_DEF") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_ENUM(TLIBC_ABSTRACT_WRITER *self, const ST_ENUM *data)
{
    if(write_struct_begin(self, "ST_ENUM") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->name) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type_annotations") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TypeAnnotations(self, &data->type_annotations) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type_annotations") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_counter(self, "enum_def_list_num", data->enum_def_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "enum_def_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    {
        tuint32 i;
        if(write_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
        for(i = 0; i < data->enum_def_list_num; ++i)
        {
            if(write_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_ENUM_DEF(self, &data->enum_def_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
        }
        if(write_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_field_end(self, "enum_def_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_ENUM") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_FIELD_LIST(TLIBC_ABSTRACT_WRITER *self, const ST_FIELD_LIST *data)
{
    if(write_struct_begin(self, "ST_FIELD_LIST") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_counter(self, "field_list_num", data->field_list_num) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    {
        tuint32 i;
        if(write_vector_begin(self) != E_TLIBC_NOERROR) goto ERROR_RET;
        for(i = 0; i < data->field_list_num; ++i)
        {
            if(write_vector_item_begin(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_FIELD(self, &data->field_list[i]) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_vector_item_end(self, i) != E_TLIBC_NOERROR) goto ERROR_RET;
        }
        if(write_vector_end(self) != E_TLIBC_NOERROR) goto ERROR_RET;
    }
    if(write_field_end(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_FIELD_LIST") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_STRUCT(TLIBC_ABSTRACT_WRITER *self, const ST_STRUCT *data)
{
    if(write_struct_begin(self, "ST_STRUCT") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "ta") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TypeAnnotations(self, &data->ta) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "ta") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->name) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_Parameters(self, &data->parameters) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_FIELD_LIST(self, &data->field_list) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_STRUCT") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_UNION(TLIBC_ABSTRACT_WRITER *self, const ST_UNION *data)
{
    if(write_struct_begin(self, "ST_UNION") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "ta") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TypeAnnotations(self, &data->ta) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "ta") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->name) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_Parameters(self, &data->parameters) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "parameters") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_FIELD_LIST(self, &data->field_list) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "field_list") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_UNION") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_TYPEDEF(TLIBC_ABSTRACT_WRITER *self, const ST_TYPEDEF *data)
{
    if(write_struct_begin(self, "ST_TYPEDEF") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_ST_TYPE(self, &data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_string(self, data->name) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "name") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_TYPEDEF") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_EN_DEFINITION_TYPE_name(TLIBC_ABSTRACT_WRITER *self, const EN_DEFINITION_TYPE data)
{
    switch(data)
    {
        case E_DT_IMPORT:
            if(write_enum_name(self, "E_DT_IMPORT") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_CONST:
            if(write_enum_name(self, "E_DT_CONST") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_ENUM:
            if(write_enum_name(self, "E_DT_ENUM") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_STRUCT:
            if(write_enum_name(self, "E_DT_STRUCT") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_UNION:
            if(write_enum_name(self, "E_DT_UNION") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_TYPEDEF:
            if(write_enum_name(self, "E_DT_TYPEDEF") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        case E_DT_UNIX_COMMENT:
            if(write_enum_name(self, "E_DT_UNIX_COMMENT") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
    }
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_EN_DEFINITION_TYPE_number(TLIBC_ABSTRACT_WRITER *self, const EN_DEFINITION_TYPE data)
{
    return write_enum_number(self, data);
}
TLIBC_ERROR_CODE write_EN_DEFINITION_TYPE(TLIBC_ABSTRACT_WRITER *self, const EN_DEFINITION_TYPE data)
{
    if(write_EN_DEFINITION_TYPE_name(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_EN_DEFINITION_TYPE_number(self, data) != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_UN_DEFINITION(TLIBC_ABSTRACT_WRITER *self, const UN_DEFINITION *data , EN_DEFINITION_TYPE s)
{
    if(write_struct_begin(self, "UN_DEFINITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    switch(s)
    {
        case E_DT_IMPORT:
        {
            if(write_field_begin(self, "de_import") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_Import(self, &data->de_import) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_import") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_CONST:
        {
            if(write_field_begin(self, "de_const") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_Const(self, &data->de_const) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_const") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_UNIX_COMMENT:
        {
            if(write_field_begin(self, "de_unix_comment") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_UNIX_COMMENT(self, &data->de_unix_comment) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_unix_comment") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_ENUM:
        {
            if(write_field_begin(self, "de_enum") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_ENUM(self, &data->de_enum) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_enum") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_STRUCT:
        {
            if(write_field_begin(self, "de_struct") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_STRUCT(self, &data->de_struct) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_struct") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_UNION:
        {
            if(write_field_begin(self, "de_union") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_UNION(self, &data->de_union) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_union") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
        case E_DT_TYPEDEF:
        {
            if(write_field_begin(self, "de_typedef") != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_ST_TYPEDEF(self, &data->de_typedef) != E_TLIBC_NOERROR) goto ERROR_RET;
            if(write_field_end(self, "de_typedef") != E_TLIBC_NOERROR) goto ERROR_RET;
            break;
        }
    }
    if(write_struct_end(self, "UN_DEFINITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
TLIBC_ERROR_CODE write_ST_DEFINITION(TLIBC_ABSTRACT_WRITER *self, const ST_DEFINITION *data)
{
    if(write_struct_begin(self, "ST_DEFINITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_EN_DEFINITION_TYPE(self, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "type") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_begin(self, "definition") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_UN_DEFINITION(self, &data->definition, data->type) != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_field_end(self, "definition") != E_TLIBC_NOERROR) goto ERROR_RET;
    if(write_struct_end(self, "ST_DEFINITION") != E_TLIBC_NOERROR) goto ERROR_RET;
    return E_TLIBC_NOERROR;
ERROR_RET:
    return E_TLIBC_ERROR;
}
