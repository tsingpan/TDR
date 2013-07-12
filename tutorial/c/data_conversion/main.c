#include <stdio.h>
#include "proto.h"
#include "proto_write.h"
#include "proto_read.h"

#include "hotpot/hp_platform.h"
#include "ddekit/encoding/ddekit_compact_encoding_writer.h"
#include "ddekit/encoding/ddekit_compact_encoding_reader.h"

#include "ddekit/encoding/ddekit_xml_encoding_writer.h"
#include "ddekit/encoding/ddekit_xml_encoding_reader.h"


#include <string.h>

#define MAX_BUFF_SIZE 1024

PROTO proto;

DDEKIT_COMPACT_ENCODING_WRITER compact_writer;
DDEKIT_COMPACT_ENCODING_READER compact_reader;

DDEKIT_XML_ENCODING_WRITER xml_writer;
DDEKIT_XML_ENCODING_READER xml_reader;

char buff[MAX_BUFF_SIZE];

typedef struct tagTEST_BLOB
{
	int a;
	double b;
}TEST_BLOB;

TEST_BLOB test_blob;
int main()
{
	FILE *fout;
	FILE *fin;

	test_blob.a = 98123;
	test_blob.b = 194132.5123;
	memset(buff, 0, sizeof(buff));

	proto.pos_size = 2;
	proto.pos[0].id = 851;
	proto.pos[0].lat = 32.1412381;
	proto.pos[0].lng = 121.1901731;
	proto.pos[1].id = 852;
	proto.pos[1].lat = 32.1412411;
	proto.pos[1].lng = 121.1901511;
	proto.list_num = 2;
	proto.list[0] = 123;
	proto.list[1] = 321;
	proto.id = 10;
	snprintf(proto.name, MAX_NAME_LENGTH, "randy");
	proto.blob_size = sizeof(TEST_BLOB);
	memcpy(proto.blob, &test_blob, sizeof(TEST_BLOB));
	proto.single_c = 'A';
	proto.message_id = MSG_LOGIN;
	proto.msg_num = 2;
	snprintf(proto.msg[0].login_name, MAX_NAME_LENGTH, "<Hello Wo\'rld0!>");
	snprintf(proto.msg[1].login_name, MAX_NAME_LENGTH, "Hello &Wor\"ld1!");
	proto.number.c = '&';
	proto.number.d = 123456.7890;
	proto.number.i8 = 0xf;
	proto.number.i16 = 0xffff;
	proto.number.i32 = 0x7fffffff;
	proto.number.i64 = 0x7fffffffffffffff;
	proto.number.ui8 = 0xff;
	proto.number.ui16 = 0xffff;
	proto.number.ui32 = 0xffffffff;
	proto.number.ui64 = 0xffffffffffffffff;
	proto.tng[0] = TNG_B;
	proto.tng[1] = TNG_A;

	

	ddekit_compact_encoding_writer_init(&compact_writer, buff, sizeof(buff));
	PROTO_write(&proto, (DDEKIT_ENCODING_WRITER*)&compact_writer);

	memset(&proto, 0, sizeof(proto));

	ddekit_compact_encoding_reader_init(&compact_reader, buff, sizeof(buff));
	PROTO_read(&proto, (DDEKIT_ENCODING_READER*)&compact_reader);

	ddekit_xml_encoding_writer_init(&xml_writer, stdout);
	PROTO_write(&proto, (DDEKIT_ENCODING_WRITER*)&xml_writer);

	fout = fopen("1.xml", "wb");
	ddekit_xml_encoding_writer_init(&xml_writer, fout);
	PROTO_write(&proto, (DDEKIT_ENCODING_WRITER*)&xml_writer);
	fclose(fout);

	memset(&proto, 0, sizeof(proto));

	fin = fopen("1.xml", "rb");
	ddekit_xml_encoding_reader_init(&xml_reader, fin);
	PROTO_read(&proto, (DDEKIT_ENCODING_READER*)&xml_reader);
	fclose(fin);

	memset(&test_blob, 0, sizeof(test_blob));
	memcpy(&test_blob, proto.blob, proto.blob_size);
	return 0;
}
