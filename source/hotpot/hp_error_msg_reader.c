#ifndef _H_hp_error_msg_hd_READER
#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>
#include "hotpot/hp_error_code.h"
#include "hotpot/hp_error_reader.h"
#include "hotpot/hp_error_msg.h"
#include "hotpot/hp_error_msg_reader.h"
void read_HP_ERROR_MSG(HPAbstractReader *self, HP_ERROR_MSG *data)
{
    read_struct_begin(self, "HP_ERROR_MSG");
    read_field_begin(self, "error_code");
    read_HP_ERROR_CODE(self, &data->error_code);
    read_field_end(self, "error_code");
    read_field_begin(self, "error_msg");
    read_string(self, data->error_msg, MAX_ERROR_MSG_LENGTH);
    read_field_end(self, "error_msg");
    read_struct_end(self, "HP_ERROR_MSG");
}
void read_HP_ERROR_MSG_LIBRARY(HPAbstractReader *self, HP_ERROR_MSG_LIBRARY *data)
{
    read_struct_begin(self, "HP_ERROR_MSG_LIBRARY");
    read_counter(self, "error_list_num", &data->error_list_num);
    read_field_begin(self, "error_list");
    {
        hpuint32 i;
        read_vector_begin(self);
        for(i = 0; i < data->error_list_num; ++i)
        {
            read_vector_item_begin(self, i);
            read_HP_ERROR_MSG(self, &data->error_list[i]);
            read_vector_item_end(self, i);
        }
        read_vector_end(self);
    }
    read_field_end(self, "error_list");
    read_struct_end(self, "HP_ERROR_MSG_LIBRARY");
}
#endif//_H_hp_error_msg_hd_READER
