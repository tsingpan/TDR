#ifndef _H_TLIBC_CSV_READER_H
#define _H_TLIBC_CSV_READER_H

#include "protocol/tlibc_abstract_reader.h"
#include "tlibc_error_code.h"
#include <stdint.h>

typedef struct tlibc_csv_reader_s
{
	tlibc_abstract_reader_t super;
	char *top_line;
	char *cur_line;
}tlibc_csv_reader_t;


tlibc_error_code_t tlibc_csv_reader_init(tlibc_csv_reader_t *self, const char *top_line, uint16_t top_line_size);

tlibc_error_code_t tlibc_csv_read_int8(tlibc_abstract_reader_t *super, int8_t *val);

tlibc_error_code_t tlibc_csv_read_int16(tlibc_abstract_reader_t *super, int16_t *val);

tlibc_error_code_t tlibc_csv_read_int32(tlibc_abstract_reader_t *super, int32_t *val);

tlibc_error_code_t tlibc_csv_read_int64(tlibc_abstract_reader_t *super, int64_t *val);

tlibc_error_code_t tlibc_csv_read_uint8(tlibc_abstract_reader_t *super, uint8_t *val);

tlibc_error_code_t tlibc_csv_read_uint16(tlibc_abstract_reader_t *super, uint16_t *val);

tlibc_error_code_t tlibc_csv_read_uint32(tlibc_abstract_reader_t *super, uint32_t *val);

tlibc_error_code_t tlibc_csv_read_uint64(tlibc_abstract_reader_t *super, uint64_t *val);

tlibc_error_code_t tlibc_csv_read_bool(tlibc_abstract_reader_t *super, bool *val);

tlibc_error_code_t tlibc_csv_read_double(tlibc_abstract_reader_t *super, double *val);

tlibc_error_code_t tlibc_csv_read_char(tlibc_abstract_reader_t *super, char *val);

tlibc_error_code_t tlibc_csv_read_string(tlibc_abstract_reader_t *super, char *str, uint32_t str_len);

#endif //_H_TLIBC_CSV_READER_H

