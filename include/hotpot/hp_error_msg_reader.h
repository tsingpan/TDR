#ifndef _H_hp_error_msg_hd_READER
#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>
#include "hotpot/hp_error.h"
#include "hotpot/hp_error_msg.h"
void read_HP_ERROR_MSG(HPAbstractReader *self, HP_ERROR_MSG *data);
void read_HP_ERROR_MSG_LIBRARY(HPAbstractReader *self, HP_ERROR_MSG_LIBRARY *data);
#endif//_H_hp_error_msg_hd_READER
