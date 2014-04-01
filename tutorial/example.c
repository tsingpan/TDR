#include <stdio.h>
#include "platform/tlibc_platform.h"
#include "core/tlibc_error_code.h"
#include "core/tlibc_string.h"

#include "protocol/tlibc_xml_reader.h"
#include "protocol/tlibc_xml_writer.h"

#include "protocol/tlibc_compact_reader.h"
#include "protocol/tlibc_compact_writer.h"

#include "protocol/tlibc_binary_reader.h"
#include "protocol/tlibc_binary_writer.h"

#include "protocol/tlibc_xlsx_reader.h"


#include "protocol/tlibc_mybind_reader.h"
#include "protocol/tlibc_mybind_writer.h"


#include "protocol_types.h"
#include "protocol_writer.h"
#include "protocol_reader.h"

#include "sql_types.h"
#include "sql_writer.h"
#include "sql_reader.h"

#include <assert.h>
#include "mysql.h"


#include <string.h>
#include <time.h>

#define MAX_BUFF_SIZE 1024
void test_compact()
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

void test_binary()
{
	char buff[MAX_BUFF_SIZE];
	tlibc_binary_writer_t writer;
	tlibc_binary_reader_t reader;
	message_t message;
	int ret;

	message.mid = E_MID_LOGIN_RSP;
	message.body.login_rsp.result = 1;
	//由于result说明登录失败， 所以session_id实际上是无效的， 并不会被读出。
	message.body.login_rsp.session_id = 123321;

	tlibc_binary_writer_init(&writer, buff, MAX_BUFF_SIZE);
	ret = tlibc_write_message(&writer.super, &message);

	memset(&message, 0, sizeof(message));
	tlibc_binary_reader_init(&reader, buff, MAX_BUFF_SIZE);
	ret = tlibc_read_message(&reader.super, &message);
}

void test_protocol()
{
	//compact型协议具有简单的压缩数据功能， 同时处理速度也非常快， 适合外网数据的传输
	test_compact();

	//binary型协议直接按照C语言默认的编码方式存放， 用小端表示， 速度最快， 不具备压缩功能， 适合内网数据传输
	test_binary();
}



void test_xml_once()
{
	tlibc_xml_reader_t xml_reader;
	tlibc_xml_writer_t xml_writer;	
	int ret;
	tconnd_config_t config;

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

	tlibc_xml_writer_init(&xml_writer, "tconnd.xml");
	ret = tlibc_write_tconnd_config(&xml_writer.super, &config);
	tlibc_xml_writer_fini(&xml_writer);
	
	memset(&config, 0, sizeof(tconnd_config_t));

	tlibc_xml_reader_init(&xml_reader);	
	tlibc_xml_reader_push_file(&xml_reader, "tconnd.xml");
	ret = tlibc_read_tconnd_config(&xml_reader.super, &config);
	tlibc_xml_reader_pop_file(&xml_reader);

	
	memset(&config, 0, sizeof(tconnd_config_t));
	//用下面这个命令可以来添加查找包含文件的目录
	tlibc_xml_add_include(&xml_reader, "./gen");
	tlibc_xml_reader_push_file(&xml_reader, "./gen/tconnd_inc.xml");
	ret = tlibc_read_tconnd_config(&xml_reader.super, &config);
	tlibc_xml_reader_pop_file(&xml_reader);
}

#define MAX_XML_FILES 10000
//it takes 5 seconds by reading 10000 xml.
void test_xml()
{
	size_t i;
	time_t start_time;
	time_t current_time;

	start_time = time(0);
	for(i = 0; i < MAX_XML_FILES; ++i)
	{
		test_xml_once();
	}
	current_time = time(0);
	printf("it takes %u seconds by reading %u xml.\n", (uint32_t)(current_time - start_time), i);
}
#define MAX_ITEM_NUM 65536
item_table_t g_item_table[MAX_ITEM_NUM];

void test_xlsx_read_once()
{
	tlibc_xlsx_reader_t xlsx_reader;
	uint32_t i;
	tlibc_error_code_t ret;

	uint32_t item_table_num, row;

	item_table_num = 0;
	//memset(&g_item_table, 0, sizeof(g_item_table));

	ret = tlibc_xlsx_reader_init(&xlsx_reader, "./gen/item.xlsx");
	ret = tlibc_xlsx_reader_open_sheet(&xlsx_reader, NULL, 2);
	row = tlibc_xlsx_reader_num_rows(&xlsx_reader);
	for(i = 3; i <= row; ++i)
	{
		tlibc_xlsx_reader_row_seek(&xlsx_reader, i);		
		ret = tlibc_read_item_table(&xlsx_reader.super, &g_item_table[item_table_num]);		

		if(ret == E_TLIBC_EMPTY)
		{
			continue;
		}
		else if(ret != E_TLIBC_NOERROR)
		{
			const char *col = tlibc_xlsx_last_col(&xlsx_reader);
			if((col != NULL) && (col[0]))
			{
				fprintf(stderr, "%s%d, %s\n", col, i, tstrerror(ret));
			}
			else
			{
				fprintf(stderr, "?%d, %s\n", i, tstrerror(ret));
			}			
		}
		++item_table_num;
	}	
	tlibc_xlsx_reader_close_sheet(&xlsx_reader);
	tlibc_xlsx_reader_fini(&xlsx_reader);
}

#define MAX_XLSX_FILES 100
//item.xlsx have 20002 rows
//it takes 12 seconds by reading 100 xlsx.
void test_xlsx()
{
	uint32_t i;
	time_t start_time;
	time_t current_time;

	start_time = time(0);
	for(i = 0; i < MAX_XLSX_FILES; ++i)
	{
		test_xlsx_read_once();
	}
	current_time = time(0);
	printf("it takes %u seconds by reading %u xlsx.\n", (uint32_t)(current_time - start_time), i);
	
}

void test_mysql_insert()
{
	tlibc_error_code_t ret;
	MYSQL *mysql = NULL;	
	const char *sql_insert = "insert into user value(?, ?, ?, ?);";
	MYSQL_STMT *stmt;
	int iret;
	user_t user;

	MYSQL_BIND   par_bind[1024];

	tlibc_mybind_reader_t bind_reader;

	mysql = mysql_init(NULL);
	if(mysql == NULL)
	{		
		printf("mysql_client_init Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	if(mysql_real_connect(mysql, "192.168.0.71", "tsqld", "tsqld", "testdb"
		, 3306, NULL, 0) == NULL)
	{
		printf("mysql_real_connect Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	stmt = mysql_stmt_init(mysql);
	if(stmt == NULL)
	{
		printf("mysql_stmt_init Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	if(mysql_stmt_prepare(stmt, sql_insert, strlen(sql_insert)))
	{
		printf("mysql_stmt_prepare Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	memset(&par_bind, 0, sizeof(par_bind));
	tlibc_mybind_reader_init(&bind_reader, par_bind, sizeof(par_bind));
	user.id = 2;
	user.exp = 123.321;
	user.gold = UINT32_MAX;
	snprintf(user.username, MAX_NAME_LENGTH, "xiaoxingxing");

	ret = tlibc_read_user(&bind_reader.super, &user);
	iret = mysql_stmt_bind_param(stmt, par_bind);
	if(iret)
	{
		printf("mysql_stmt_bind_param Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}
	
	


	iret = mysql_stmt_execute(stmt);
	if(iret != 0)
	{
		printf("mysql_real_query Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	mysql_stmt_close(stmt);
	mysql_close(mysql);
}

void test_mysql_select()
{
	tlibc_error_code_t ret;
	MYSQL *mysql = NULL;	
	const char *sql_insert = "select * from user_s;";
	MYSQL_STMT *stmt;
	int iret;
	user_t user;

	MYSQL_BIND   res_bind[1024];

	tlibc_mybind_writer_t bind_writer;

	mysql = mysql_init(NULL);
	if(mysql == NULL)
	{		
		printf("mysql_client_init Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	if(mysql_real_connect(mysql, "192.168.0.71", "tsqld", "tsqld", "testdb"
		, 3306, NULL, 0) == NULL)
	{
		printf("mysql_real_connect Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	stmt = mysql_stmt_init(mysql);
	if(stmt == NULL)
	{
		printf("mysql_stmt_init Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	if(mysql_stmt_prepare(stmt, sql_insert, strlen(sql_insert)))
	{
		printf("mysql_stmt_prepare Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	memset(&res_bind, 0, sizeof(res_bind));
	tlibc_mybind_writer_init(&bind_writer, res_bind, sizeof(res_bind));
	ret = tlibc_write_user(&bind_writer.super, &user);
	iret = mysql_stmt_bind_result(stmt, res_bind);
	if(iret)
	{
		printf("mysql_stmt_bind_param Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	iret = mysql_stmt_execute(stmt);
	if(iret != 0)
	{
		printf("mysql_real_query Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}


	iret = mysql_stmt_store_result(stmt);
	if(iret)
	{
			printf("mysql_stmt_store_result Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
			exit(1);
	}

	for(;;)
	{
		memset(&user, 0, sizeof(user));
		if(mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
		{
			break;
		}
		printf("%u\n", user.id);
	}
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
	mysql_close(mysql);
}


int main()
{
	test_protocol();
	
	//test_xml();
	
	test_xlsx();

	/*
	test_mysql_insert();

	test_mysql_select();
	*/	
	return 0;
}
