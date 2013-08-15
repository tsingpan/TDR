#ifndef _H_hp_error_hd_READER
#include "hotpot/hp_platform.h"
#include "hotprotocol/hp_abstract_reader.h"
#include <string.h>
#include "hotpot/hp_error_code.h"
HP_ERROR_CODE read_HP_ERROR_CODE_name(HPAbstractReader *self, HP_ERROR_CODE *data);
HP_ERROR_CODE read_HP_ERROR_CODE_number(HPAbstractReader *self, HP_ERROR_CODE *data);
HP_ERROR_CODE read_HP_ERROR_CODE(HPAbstractReader *self, HP_ERROR_CODE *data);
#endif//_H_hp_error_hd_READER
