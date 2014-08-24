#include <stdio.h>
#include "tlibc_error_code.h"
#include "core/tlibc_string.h"

#include "protocol/tlibc_xml_reader.h"
#include "protocol/tlibc_xml_writer.h"

#include "protocol/tlibc_compact_reader.h"
#include "protocol/tlibc_compact_writer.h"

#include "protocol/tlibc_binary_reader.h"
#include "protocol/tlibc_binary_writer.h"

#include "protocol/tlibc_xlsx_reader.h"

#include "tlibc_csv_reader.h"

#include "protocol_types.h"
#include "protocol_writer.h"
#include "protocol_reader.h"
#include <stdbool.h>

#include <assert.h>


#include <string.h>
#include <time.h>

#define MAX_BUFF_SIZE 1024
static void test_compact()
{
	char buff[MAX_BUFF_SIZE];
	tlibc_compact_writer_t compact_writer;
	tlibc_compact_reader_t compact_reader;
	message_t message;
	int ret;

	message.mid = E_MID_LOGIN_REQ;
	snprintf(message.body.login_req.name, MAX_NAME_LENGTH, "xiaoxingxing");
	snprintf(message.body.login_req.password, MAX_NAME_LENGTH, "123456");
	message.body.login_req.age = 27;

	tlibc_compact_writer_init(&compact_writer, buff, MAX_BUFF_SIZE);
	ret = tlibc_write_message(&compact_writer.super, &message);

	memset(&message, 0, sizeof(message));
	tlibc_compact_reader_init(&compact_reader, buff, MAX_BUFF_SIZE);
	ret = tlibc_read_message(&compact_reader.super, &message);
}

static void test_binary()
{
	char buff[MAX_BUFF_SIZE];
	tlibc_binary_writer_t writer;
	tlibc_binary_reader_t reader;
	message_t message;
	int ret;

	message.mid = E_MID_LOGIN_RSP;
	message.body.login_rsp.result = 1;
	//ÓÉÓÚresultËµÃ÷µÇÂ¼Ê§°Ü£¬ ËùÒÔsession_idÊµ¼ÊÉÏÊÇÎÞÐ§µÄ£¬ ²¢²»»á±»¶Á³ö¡£
	message.body.login_rsp.session_id = 123321;

	tlibc_binary_writer_init(&writer, buff, MAX_BUFF_SIZE);
	ret = tlibc_write_message(&writer.super, &message);

	memset(&message, 0, sizeof(message));
	tlibc_binary_reader_init(&reader, buff, MAX_BUFF_SIZE);
	ret = tlibc_read_message(&reader.super, &message);
}

static void test_protocol()
{
	//compactÐÍÐ­Òé¾ßÓÐ¼òµ¥µÄÑ¹ËõÊý¾Ý¹¦ÄÜ£¬ Í¬Ê±´¦ÀíËÙ¶ÈÒ²·Ç³£¿ì£¬ ÊÊºÏÍâÍøÊý¾ÝµÄ´«Êä
	test_compact();

	//binaryÐÍÐ­ÒéÖ±½Ó°´ÕÕCÓïÑÔÄ¬ÈÏµÄ±àÂë·½Ê½´æ·Å£¬ ÓÃÐ¡¶Ë±íÊ¾£¬ ËÙ¶È×î¿ì£¬ ²»¾ß±¸Ñ¹Ëõ¹¦ÄÜ£¬ ÊÊºÏÄÚÍøÊý¾Ý´«Êä
	test_binary();
}


typedef tlibc_error_code_t (*reader_func)(tlibc_abstract_reader_t *self, void *data);
static int read_xml_from_file(const char* file, void *ptr, reader_func reader)
{
	bool ret = true;
	tlibc_xml_reader_t xml_reader;
	tlibc_error_code_t r;

	tlibc_xml_reader_init(&xml_reader);	
	if(tlibc_xml_reader_push_file(&xml_reader, "tconnd.xml") != E_TLIBC_NOERROR)
	{
		ret = false;
		goto done;
	}

	r = reader(&xml_reader.super, ptr); 
	if(r != E_TLIBC_NOERROR)
	{
		const tlibc_xml_reader_yyltype_t *lo = tlibc_xml_current_location(&xml_reader);
		if(lo)
		{
			fprintf(stderr, "%s(%d,%d - %d,%d) %s\n"
				, lo->file_name
				, lo->first_line, lo->first_column, lo->last_line, lo->last_column
				, tstrerror(r));
		}
		else
		{
			fprintf(stderr, "%s %s", file, tstrerror(r));
		}   	

		ret = false;
		goto pop_file;
	}
pop_file:
	tlibc_xml_reader_pop_file(&xml_reader);
done:
	return ret;
}

static int read_xml_from_buff(const char* start, const char*limit, void *ptr, reader_func reader)
{
	int ret = true;
	tlibc_xml_reader_t xml_reader;
	tlibc_error_code_t r;

	tlibc_xml_reader_init(&xml_reader);	
	if(tlibc_xml_reader_push_buff(&xml_reader, start, limit) != E_TLIBC_NOERROR)
	{
		ret = false;
		goto done;
	}

	r = reader(&xml_reader.super, ptr); 
	if(r != E_TLIBC_NOERROR)
	{
		const tlibc_xml_reader_yyltype_t *lo = tlibc_xml_current_location(&xml_reader);
		if(lo)
		{
			fprintf(stderr, "(%d,%d - %d,%d) %s\n"
				, lo->first_line, lo->first_column, lo->last_line, lo->last_column
				, tstrerror(r));
		}
		else
		{
			fprintf(stderr, "%s", tstrerror(r));
		}   	

		ret = false;
		goto pop_file;
	}
pop_file:
	tlibc_xml_reader_pop_file(&xml_reader);
done:
	return ret;
}

#define XML_OUTPUT_BUF 65536
static void test_xml()
{
	tlibc_xml_reader_t xml_reader;
	tlibc_xml_writer_t xml_writer;
	int ret;
	tconnd_config_t config;
	int bret;
	FILE* fout;
	const char*ch;

	static char output_buff[XML_OUTPUT_BUF];

	snprintf(config.log_config, MAX_NAME_LENGTH, "/usr/local/tconnd/etc/tconnd_log.xml");
	config.instance_config_num = 2;
	snprintf(config.instance_config[0].ip, IP_LENGTH, "127.0.0.1");
	config.instance_config[0].level = e_low;
	config.instance_config[0].port = 7001;	
	config.instance_config[0].backlog = 5;
	config.instance_config[0].epoll_size = 65536;

	snprintf(config.instance_config[1].ip, IP_LENGTH, "localhost");
	config.instance_config[1].level = e_high;
	config.instance_config[1].port = 7002;
	config.instance_config[1].backlog = 1;
	config.instance_config[1].epoll_size = 1024;

	tlibc_xml_writer_init(&xml_writer, output_buff, output_buff + sizeof(output_buff));
	ret = tlibc_write_tconnd_config(&xml_writer.super, &config);
	assert(ret == E_TLIBC_NOERROR);
	fout = fopen("tconnd.xml", "w");
	assert(fout != NULL);
	for(ch = xml_writer.start; ch < xml_writer.cur; ++ch)
	{
		fputc(*ch, fout);
	}
	fclose(fout);
	memset(&config, 1, sizeof(tconnd_config_t));
	bret = read_xml_from_buff(output_buff, output_buff + sizeof(output_buff), &config, (reader_func)tlibc_read_tconnd_config);
	memset(&config, 1, sizeof(tconnd_config_t));
	bret = read_xml_from_file("tconnd.xml", &config, (reader_func)tlibc_read_tconnd_config);


	memset(&config, 0, sizeof(tconnd_config_t));
	//ÓÃÏÂÃæÕâ¸öÃüÁî¿ÉÒÔÀ´Ìí¼Ó²éÕÒ°üº¬ÎÄ¼þµÄÄ¿Â¼
	tlibc_xml_reader_init(&xml_reader);
	ret = tlibc_xml_add_include(&xml_reader, "./etc");
	ret = tlibc_xml_reader_push_file(&xml_reader, "./etc/tconnd_inc.xml");
	ret = tlibc_read_tconnd_config(&xml_reader.super, &config);
	tlibc_xml_reader_pop_file(&xml_reader);
}
#define MAX_ITEM_NUM 65536
item_table_t g_item_table[MAX_ITEM_NUM];
size_t g_item_table_num;
#define COL_STR_LEN 1024

static int read_xlsx(const char *file, char *list, size_t unit_size, size_t *list_num, reader_func reader)
{
	bool ret = true;
	tlibc_xlsx_reader_t xlsx_reader;
	uint32_t i;
	tlibc_error_code_t r;
	uint32_t num, row;

	num = 0;
	r = tlibc_xlsx_reader_init(&xlsx_reader, file);
	if(r != E_TLIBC_NOERROR)
	{
		ret = false;
		goto done;
	}
	r = tlibc_xlsx_reader_open_sheet(&xlsx_reader, NULL, 2);
	if(r != E_TLIBC_NOERROR)
	{
		ret = false;
		goto fini;
	}
	row = tlibc_xlsx_reader_num_rows(&xlsx_reader);
	for(i = 3; i <= row; ++i)
	{
		tlibc_xlsx_reader_row_seek(&xlsx_reader, i);	
		if(num >= *list_num)
		{
			ret = false;
			goto close_sheet;
		}
		r = reader(&xlsx_reader.super, list + unit_size * num);

		if(r == E_TLIBC_EMPTY)
		{
			continue;
		}
		else if(r != E_TLIBC_NOERROR)
		{
			size_t col = tlibc_xlsx_current_col(&xlsx_reader);
			char col_str[COL_STR_LEN];
			const char* col_str_ptr = tlibc_xlsx_num2str((int)col, col_str, COL_STR_LEN);

			if(col_str_ptr != NULL)
			{
				fprintf(stderr, "%s%d, %s\n", col_str_ptr, i, tstrerror(r));
			}
			else
			{
				fprintf(stderr, "?%d, %s\n", i, tstrerror(r));
			}
			ret = false;
			goto close_sheet;
		}
		++num;
	}
	*list_num = num;

close_sheet:
	tlibc_xlsx_reader_close_sheet(&xlsx_reader);
fini:
	tlibc_xlsx_reader_fini(&xlsx_reader);
done:
	return ret;
}

static void test_xlsx()
{
	g_item_table_num = MAX_ITEM_NUM;
	read_xlsx("./etc/item.xlsx", (char*)g_item_table, sizeof(item_table_t), &g_item_table_num, (reader_func)tlibc_read_item_table);
}

static void test_csv()
{
	FILE *fin = NULL;
	char line[65536];
	size_t line_size = 0;
	tlibc_csv_reader_t reader;
	item_table_t item;
	tlibc_error_code_t ret;

	fin = fopen("etc/item.csv", "r");

	//¿¿¿¿¿¿¿
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);

	//¿¿¿¿¿¿¿¿
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tlibc_csv_reader_init(&reader, line, (uint16_t)(line_size));


	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tlibc_csv_reader_store(&reader, line, line_size);
	ret = tlibc_read_item_table(&reader.super, &item);
	tlibc_csv_reader_close(&reader);

	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tlibc_csv_reader_store(&reader, line, line_size);
	ret = tlibc_read_item_table(&reader.super, &item);
	tlibc_csv_reader_close(&reader);
}

int main()
{	
	test_csv();
	return 0;

	test_protocol();
	
	test_xml();	
	
	test_xlsx();

	return 0;
}
