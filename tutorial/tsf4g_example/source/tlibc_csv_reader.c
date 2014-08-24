#include "tlibc_csv_reader.h"
#include "tlibc_error_code.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tlibcdef.h"

static const char *php_fgetcsv_lookup_trailing_spaces(const char *ptr, size_t len)
{
	int inc_len;
	unsigned char last_chars[2] = { 0, 0 };

	while (len > 0) {
		inc_len = (*ptr == '\0' ? 1: mblen(ptr, len));
		switch (inc_len) {
			case -2:
			case -1:
				inc_len = 1;
				break;
			case 0:
				goto quit_loop;
			case 1:
			default:
				last_chars[0] = last_chars[1];
				last_chars[1] = (unsigned char)*ptr;
				break;
		}
		ptr += inc_len;
		len -= (size_t)inc_len;
	}
quit_loop:
	switch (last_chars[1]) {
		case '\n':
			if (last_chars[0] == '\r') {
				return ptr - 2;
			}
			/* break is omitted intentionally */
		case '\r':
			return ptr - 1;
	}
	return ptr;
}

static void line_close(tlibc_csv_reader_t *self)
{
	if(self->cur_line != NULL)
	{
		free(self->cur_line);
		self->cur_line = NULL;
	}
	self->cur_line_size = 0;
	self->cur_line_fields_num = 0;
}

static void line_open(tlibc_csv_reader_t *self, const char *buf, size_t buf_len)
{
	const char escape_char = '\\';
	const char delimiter = ',';
	const char enclosure = '"';
	const char *bptr, *limit, *const_tptr, *line_end;
	char *temp, *tptr;
	size_t temp_len, line_end_len;
	int inc_len;
	int first_field = 1;


	/* Now into new section that parses buf for delimiter/enclosure fields */

	/* Strip trailing space from buf, saving end of line in case required for enclosure field */

	bptr = buf;
	const_tptr = (const char *)php_fgetcsv_lookup_trailing_spaces(buf, buf_len);
	line_end_len = buf_len - (size_t)(const_tptr - buf);
	line_end = limit = const_tptr;

	/* reserve workspace for building each individual field */
	self->cur_line_fields_num = 0;
	temp_len = buf_len;
	self->cur_line_size = temp_len + line_end_len + 1;
	self->cur_line = malloc(self->cur_line_size);
	if(self->cur_line == NULL)
	{
		return;
	}
	temp = self->cur_line;

	/* Initialize return array */

	/* Main loop to read CSV fields */
	/* NB this routine will return a single null entry for a blank line */

	do {
		const char *hunk_begin, *const_comp_end;
		char *comp_end;

		tptr = temp;

		inc_len = (bptr < limit ? (*bptr == '\0' ? 1: mblen(bptr, (size_t)(limit - bptr))): 0);
		if (inc_len == 1) {
			const char *tmp = bptr;
			while ((*tmp != delimiter) && isspace((int)*(const unsigned char *)tmp)) {
				tmp++;
			}
			if (*tmp == enclosure) {
				bptr = tmp;
			}
		}

		if (first_field && bptr == line_end) {
			break;
		}
		first_field = 0;
		/* 2. Read field, leaving bptr pointing at start of next field */
		if (inc_len != 0 && *bptr == enclosure) {
			int state = 0;

			bptr++;	/* move on to first character in field */
			hunk_begin = bptr;

			/* 2A. handle enclosure delimited field */
			for (;;) {
				switch (inc_len) {
					case 0:
						switch (state) {
							case 2:
								memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin - 1));
								tptr += (bptr - hunk_begin - 1);
								hunk_begin = bptr;
								goto quit_loop_2;

							case 1:
								memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
								tptr += (bptr - hunk_begin);
								hunk_begin = bptr;
								/* break is omitted intentionally */

							case 0: {
										if (hunk_begin != line_end) {
											memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
											tptr += (bptr - hunk_begin);
											hunk_begin = bptr;
										}

										/* add the embedded line end to the field */
										memcpy(tptr, line_end, line_end_len);
										tptr += line_end_len;

										goto quit_loop_2;
									} break;
						}
						break;

					case -2:
					case -1:
						//php_ignore_value(mblen(NULL, 0));
						/* break is omitted intentionally */
					case 1:
						/* we need to determine if the enclosure is
						 * 						 * 'real' or is it escaped */
						switch (state) {
							case 1: /* escaped */
								bptr++;
								state = 0;
								break;
							case 2: /* embedded enclosure ? let's check it */
								if (*bptr != enclosure) {
									/* real enclosure */
									memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin - 1));
									tptr += (bptr - hunk_begin - 1);
									hunk_begin = bptr;
									goto quit_loop_2;
								}
								memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
								tptr += (bptr - hunk_begin);
								bptr++;
								hunk_begin = bptr;
								state = 0;
								break;
							default:
								if (*bptr == enclosure) {
									state = 2;
								} else if (*bptr == escape_char) {
								//不处理转义字符
								//	state = 1;
								}
								bptr++;
								break;
						}
						break;

					default:
						switch (state) {
							case 2:
								/* real enclosure */
								memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin - 1));
								tptr += (bptr - hunk_begin - 1);
								hunk_begin = bptr;
								goto quit_loop_2;
							case 1:
								bptr += inc_len;
								memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
								tptr += (bptr - hunk_begin);
								hunk_begin = bptr;
								break;
							default:
								bptr += inc_len;
								break;
						}
						break;
				}
				inc_len = (bptr < limit ? (*bptr == '\0' ? 1: mblen(bptr, (size_t)(limit - bptr))): 0);
			}

quit_loop_2:
			/* look up for a delimiter */
			for (;;) {
				switch (inc_len) {
					case 0:
						goto quit_loop_3;

					case -2:
					case -1:
						inc_len = 1;
						//php_ignore_value(mblen(NULL, 0));
						/* break is omitted intentionally */
					case 1:
						if (*bptr == delimiter) {
							goto quit_loop_3;
						}
						break;
					default:
						break;
				}
				bptr += inc_len;
				inc_len = (bptr < limit ? (*bptr == '\0' ? 1: mblen(bptr, (size_t)(limit - bptr))): 0);
			}

quit_loop_3:
			memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
			tptr += (bptr - hunk_begin);
			bptr += inc_len;
			comp_end = tptr;
		} else {
			/* 2B. Handle non-enclosure field */

			hunk_begin = bptr;

			for (;;) {
				switch (inc_len) {
					case 0:
						goto quit_loop_4;
					case -2:
					case -1:
						inc_len = 1;
						//php_ignore_value(mblen(NULL, 0));
						/* break is omitted intentionally */
					case 1:
						if (*bptr == delimiter) {
							goto quit_loop_4;
						}
						break;
					default:
						break;
				}
				bptr += inc_len;
				inc_len = (bptr < limit ? (*bptr == '\0' ? 1: mblen(bptr, (size_t)(limit - bptr))): 0);
			}
quit_loop_4:
			memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
			tptr += (bptr - hunk_begin);

			const_comp_end = (const char *)php_fgetcsv_lookup_trailing_spaces(temp, (size_t)(tptr - temp));
			comp_end = temp + (const_comp_end - temp);
			if (*bptr == delimiter) {
				bptr++;
			}
		}

		/* 3. Now pass our field back to php */
		*comp_end = '\0';
		if(self->cur_line_fields_num >= TLIBC_CSV_FIELD_NUM)
		{
			break;
		}
		self->cur_line_fields[self->cur_line_fields_num] = temp;
		++self->cur_line_fields_num;
		temp = comp_end + 1;
	} while (inc_len > 0);
}


tlibc_error_code_t tlibc_csv_reader_init(tlibc_csv_reader_t *self, const char *top_line, size_t top_line_size)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	size_t i;

	self->super.enable_name = true;
	self->pre_read_uint32 = false;

	self->super.read_vector_begin = tlibc_csv_read_vector_begin;
	self->super.read_vector_element_begin = tlibc_csv_read_vector_element_begin;
	self->super.read_field_begin = tlibc_csv_read_field_begin;	
	self->super.read_enum_begin = tlibc_csv_read_enum_begin;

	self->super.read_int8 = tlibc_csv_read_int8;
	self->super.read_int16 = tlibc_csv_read_int16;
	self->super.read_int32 = tlibc_csv_read_int32;
	self->super.read_int64 = tlibc_csv_read_int64;

	self->super.read_uint8 = tlibc_csv_read_uint8;
	self->super.read_uint16 = tlibc_csv_read_uint16;
	self->super.read_uint32 = tlibc_csv_read_uint32;
	self->super.read_uint64 = tlibc_csv_read_uint64;

	self->super.read_double = tlibc_csv_read_double;
	self->super.read_char = tlibc_csv_read_char;
	self->super.read_bool = tlibc_csv_read_bool;
	self->super.read_string = tlibc_csv_read_string;



	self->top_line = NULL;
	self->top_line_size = 0;
	self->top_line_fields_num = 0;

	self->cur_line = NULL;
	self->cur_line_size = 0;
	self->cur_line_fields_num = 0;
	line_open(self, top_line, top_line_size);
	if(self->cur_line == NULL)
	{
		line_close(self);
		ret = E_TLIBC_NO_MEMORY;
		goto done;
	}
	self->top_line_size = self->cur_line_size;
	self->top_line = malloc(self->top_line_size);
	memcpy(self->top_line, self->cur_line, self->top_line_size);

	self->top_line_fields_num = self->cur_line_fields_num;
	for(i = 0; i < self->top_line_fields_num; ++i)
	{
		self->top_line_fields[i] = self->top_line + (self->cur_line_fields[i] - self->cur_line);
	}
	line_close(self);

done:
	return ret;
}

void tlibc_csv_reader_fini(tlibc_csv_reader_t *self)
{
	if(self->top_line != NULL)
	{
		free(self->top_line);
	}
}

tlibc_error_code_t tlibc_csv_reader_store(tlibc_csv_reader_t *self, const char *line, size_t line_size)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	line_open(self, line, line_size);
	if(self->cur_line == NULL)
	{
		line_close(self);
		goto error_ret;
	}
	return E_TLIBC_NOERROR;
error_ret:
	return ret;
}

void tlibc_csv_reader_close(tlibc_csv_reader_t *self)
{
	line_close(self);
}

tlibc_error_code_t tlibc_csv_read_vector_begin(tlibc_abstract_reader_t *super, const char* vec_name)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_vector_element_begin(tlibc_abstract_reader_t *super, const char* var_name, uint32_t index)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_field_begin(tlibc_abstract_reader_t *super, const char *var_name)
{
	tlibc_csv_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_csv_reader_t, super);

	if(self->pre_read_uint32)
	{   
		return E_TLIBC_NOERROR;
	}   

	TLIBC_UNUSED(var_name);

	return E_TLIBC_NOERROR;
}


tlibc_error_code_t tlibc_csv_read_enum_begin(tlibc_abstract_reader_t *super, const char *enum_name)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_int8(tlibc_abstract_reader_t *super, int8_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_int16(tlibc_abstract_reader_t *super, int16_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_int32(tlibc_abstract_reader_t *super, int32_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_int64(tlibc_abstract_reader_t *super, int64_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_uint8(tlibc_abstract_reader_t *super, uint8_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_uint16(tlibc_abstract_reader_t *super, uint16_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_uint32(tlibc_abstract_reader_t *super, uint32_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_uint64(tlibc_abstract_reader_t *super, uint64_t *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_bool(tlibc_abstract_reader_t *super, bool *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_double(tlibc_abstract_reader_t *super, double *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_char(tlibc_abstract_reader_t *super, char *val)
{
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_csv_read_string(tlibc_abstract_reader_t *super, char *str, uint32_t str_len)
{
	return E_TLIBC_NOERROR;
}
