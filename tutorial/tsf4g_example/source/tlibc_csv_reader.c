#include "tlibc_csv_reader.h"
#include "tlibc_error_code.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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

//移植php中的php_fgetcsv函数
static void parser_line(tlibc_csv_reader_t *self, const char *buf, size_t buf_len)
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
	temp_len = buf_len;
	temp = malloc(temp_len + line_end_len + 1);

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
			//add_next_index_null(return_value);
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
								/*
										char *new_buf;
										size_t new_len;
										char *new_temp;
										*/

										if (hunk_begin != line_end) {
											memcpy(tptr, hunk_begin, (size_t)(bptr - hunk_begin));
											tptr += (bptr - hunk_begin);
											hunk_begin = bptr;
										}

										/* add the embedded line end to the field */
										memcpy(tptr, line_end, line_end_len);
										tptr += line_end_len;

										goto quit_loop_2;
										/*
										if (stream == NULL) {
											goto quit_loop_2;
										} else if ((new_buf = php_stream_get_line(stream, NULL, 0, &new_len)) == NULL) {
											 we've got an unterminated enclosure,
											 * 									 * assign all the data from the start of
											 * 									 									 * the enclosure to end of data to the
											 * 									 									 									 * last element 
											if ((size_t)temp_len > (size_t)(limit - buf)) {
												goto quit_loop_2;
											}
											zval_dtor(return_value);
											RETVAL_FALSE;
											goto out;
										}
										temp_len += new_len;
										new_temp = erealloc(temp, temp_len);
										tptr = new_temp + (size_t)(tptr - temp);
										temp = new_temp;

										efree(buf);
										buf_len = new_len;
										bptr = buf = new_buf;
										hunk_begin = buf;

										line_end = limit = (char *)php_fgetcsv_lookup_trailing_spaces(buf, buf_len, delimiter TSRMLS_CC);
										line_end_len = buf_len - (size_t)(limit - buf);

										state = 0;
										*/
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
									state = 1;
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
		//add_next_index_stringl(return_value, temp, comp_end - temp, 1);
		printf("%s\n", temp);
	} while (inc_len > 0);

	free(temp);
}


tlibc_error_code_t tlibc_csv_reader_init(tlibc_csv_reader_t *self, const char *top_line, uint16_t top_line_size)
{
	self->cur_line = NULL;
	parser_line(self, top_line, top_line_size);

	return E_TLIBC_NOERROR;
}

