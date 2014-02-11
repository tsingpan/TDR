#include <stdio.h>
#include "tlibc/platform/tlibc_platform.h"

#include "tlibc/protocol/tlibc_xml_reader.h"
#include "tlibc/protocol/tlibc_xml_writer.h"

#include "tlibc/protocol/tlibc_compact_reader.h"
#include "tlibc/protocol/tlibc_compact_writer.h"

#include "tlibc/protocol/tlibc_binary_reader.h"
#include "tlibc/protocol/tlibc_binary_writer.h"

#include "tlibc/protocol/tlibc_xlsx_reader.h"

#include "protocol_types.h"
#include "protocol_writer.h"
#include "protocol_reader.h"


#include <string.h>

#define MAX_BUFF_SIZE 1024

/*
TLIBC_XML_READER xml_reader;
void test_xml()
{
	TLIBC_XML_WRITER xml_writer;	
	int ret;
	
	tlibc_xml_writer_init(&xml_writer, "t.xml");
	ret = tlibc_write_ST_DEFINITION(&xml_writer.super, &g_definition);
	tlibc_xml_writer_fini(&xml_writer);
	
	memset(&g_definition, 0, sizeof(ST_DEFINITION));

	tlibc_xml_reader_init(&xml_reader);
	tlibc_xml_add_include(&xml_reader, "D:\\build_tdata_example");
	tlibc_xml_reader_push_file(&xml_reader, "t.xml");
	ret = tlibc_read_ST_DEFINITION(&xml_reader.super, &g_definition);
	tlibc_xml_reader_pop_file(&xml_reader);
}
void test_xml2()
{
	TLIBC_XML_WRITER xml_writer;
	int ret;

	tlibc_xml_writer_init(&xml_writer, "test_xml2.xml");
	ret = tlibc_write_ST_Parameters(&xml_writer.super, &g_parameters);
	tlibc_xml_writer_fini(&xml_writer);

	memset(&g_parameters, 0, sizeof(ST_Parameters));

	tlibc_xml_reader_init(&xml_reader);
	tlibc_xml_reader_push_file(&xml_reader, "test_xml2.xml");
	ret = tlibc_read_ST_Parameters(&xml_reader.super, &g_parameters);
	tlibc_xml_reader_pop_file(&xml_reader);
}


void test_xlsx()
{
	tlibc_xlsx_reader_t xlsx_reader;
	tuint32 i, rows;
	int ret;

	memset(&g_definition, 0, sizeof(g_definition));

	tlibc_xml_reader_init(&xml_reader);
	tlibc_xml_reader_push_file(&xml_reader, "test_xml2.xml");
	ret = tlibc_read_ST_Parameters(&xml_reader.super, &g_parameters);
	tlibc_xml_reader_pop_file(&xml_reader);

	memset(&g_definition, 0, sizeof(g_definition));
	tlibc_xlsx_reader_init(&xlsx_reader, "d:/1.xlsx");
	tlibc_xlsx_reader_open_sheet(&xlsx_reader, NULL, 2, 4);
	rows = tlibc_xlsx_reader_num_rows(&xlsx_reader);
	for(i = 0; i < rows; ++i)
	{
		tlibc_xlsx_reader_row_seek(&xlsx_reader, i);
		ret = tlibc_read_ST_Parameters(&xlsx_reader.super, &g_parameters);
	}	
	tlibc_xlsx_reader_close_sheet(&xlsx_reader);
	tlibc_xlsx_reader_fini(&xlsx_reader);
}
*/

void test_compact()
{
	char buff[MAX_BUFF_SIZE];
	TLIBC_COMPACT_WRITER compact_writer;
	TLIBC_COMPACT_READER compact_reader;
	message_s message;
	int ret;

	message.mid = E_MID_LOGIN_REQ;
	snprintf(message.body.login_req.name, MAX_NAME_LENGTH, "xiaoxingxing");
	snprintf(message.body.login_req.password, MAX_NAME_LENGTH, "123456");
	message.body.login_req.age = 27;

	tlibc_compact_writer_init(&compact_writer, buff, MAX_BUFF_SIZE);
	ret = tlibc_write_message_s(&compact_writer.super, &message);

	memset(&message, 0, sizeof(message));
	tlibc_compact_reader_init(&compact_reader, buff, MAX_BUFF_SIZE);
	ret = tlibc_read_message_s(&compact_reader.super, &message);
}

void test_binary()
{
	char buff[MAX_BUFF_SIZE];
	TLIBC_BINARY_WRITER writer;
	TLIBC_BINARY_READER reader;
	message_s message;
	int ret;

	message.mid = E_MID_LOGIN_RSP;
	message.body.login_rsp.result = 1;
	//由于result说明登录失败， 所以session_id实际上是无效的， 并不会被读出。
	message.body.login_rsp.session_id = 123321;

	tlibc_binary_writer_init(&writer, buff, MAX_BUFF_SIZE);
	ret = tlibc_write_message_s(&writer.super, &message);

	memset(&message, 0, sizeof(message));
	tlibc_binary_reader_init(&reader, buff, MAX_BUFF_SIZE);
	ret = tlibc_read_message_s(&reader.super, &message);
}

void test_protocol()
{
	//compact型协议具有简单的压缩数据功能， 同时处理速度也非常快， 适合外网数据的传输
	test_compact();

	//binary型协议直接按照C语言默认的编码方式存放， 用小端表示， 速度最快， 不具备压缩功能， 适合内网数据传输
	test_binary();
}

int main()
{
	test_protocol();

	/*
	test_xml();

	test_xlsx();
	*/
	return 0;
}
