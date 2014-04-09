#ifndef _H_SCANNER
#define _H_SCANNER

#include "platform/tlibc_platform.h"
#include "core/tlibc_hash.h"
#include "language/language.h"
#include "definition.h"
#include "error.h"
#include <stdarg.h>

typedef struct _YYLTYPE
{
	char file_name[TLIBC_MAX_PATH_LENGTH];
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;

typedef union _SCANNER_TOKEN_VALUE
{
	ST_Import sn_import;
	ST_TYPE sn_type;
	ST_SIMPLE_TYPE sn_simple_type;
	syn_value_t sn_value;	
	char sn_tok_identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	ST_Const sn_const;
	double pn_tok_double;
	const char* sn_tok_unixcomment;
	uint64_t sn_uint64;
	int64_t sn_int64;
	double sn_d;
	uint64_t sn_hex_uint64;
	int64_t sn_hex_int64;
	char* sn_string;
	char sn_char;
	SN_SIMPLE_TYPE sn_st;
	ST_ARGUMENTS sn_arguments;
	SN_CONTAINER_TYPE sn_ct;
	ST_TYPEDEF sn_typedef;
	syn_unix_comment_t sn_unix_comment;
	ST_ENUM_DEF sn_enum_def;
	ST_Parameter sn_parameter;
	ST_Parameters sn_parameters;
	ST_CONDITION sn_condition;
	ST_UNION_FIELD sn_union_field;
	ST_FIELD sn_field;
}SCANNER_TOKEN_VALUE;

#define YYSTYPE SCANNER_TOKEN_VALUE
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  scanner_top(self)->yy_cursor
#define YYLIMIT   scanner_top(self)->yy_limit
#define YYMARKER scanner_top(self)->yy_marker
#define YYGETCONDITION()  scanner_top(self)->yy_state
#define YYSETCONDITION(s) scanner_top(self)->yy_state = s
#define yytext scanner_top(self)->yy_text
#define yyleng scanner_top(self)->yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()

typedef struct scanner_file_s
{
	tlibc_hash_head_t hash_head;
	char file_name[TLIBC_MAX_PATH_LENGTH];
}scanner_file_t;

typedef struct scanner_context_s
{	
	char file_name[TLIBC_MAX_PATH_LENGTH];
	int yy_state;
	YYCTYPE *yy_last;
	YYCTYPE *yy_cursor;
	YYCTYPE *yy_limit;
	YYCTYPE *yy_text;
	YYCTYPE *yy_marker;
	YYCTYPE *yy_start;
	uint32_t yy_leng;

	uint32_t yylineno;
	uint32_t yycolumn;	
}scanner_context_t;

//最多解析65536个字符
#define MAX_SCANNER_FILE_NUM 10240
#define MAX_SCANNER_FILE_BUCKETS 65536

#define MAX_LEX_BUFF_SIZE 10000000
#define MAX_SCANNER_STACK_SIZE 1024
typedef struct scanner_s
{
	uint32_t stack_num;
	scanner_context_t stack[MAX_SCANNER_STACK_SIZE];

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_LEX_BUFF_SIZE];

	tlibc_hash_bucket_t file_hash_buckets[MAX_SCANNER_FILE_BUCKETS];
	tlibc_hash_t file_hash;
	uint32_t file_vec_num;
	scanner_file_t file_vec[MAX_SCANNER_FILE_NUM];
}scanner_t;

void scanner_init(scanner_t *self);
void scanner_locate(scanner_t *self);
int32_t scanner_scan(scanner_t *self, YYLTYPE *yylloc);
scanner_context_t *scanner_top(scanner_t *self);
int32_t scanner_push(scanner_t *self, const char *file_name, int state);
void scanner_pop(scanner_t *self);
uint32_t scanner_size(scanner_t *self);

int tdatalex(SCANNER_TOKEN_VALUE * yylval_param, YYLTYPE * yylloc_param , scanner_t *self);



//此函数不会调用exit
void scanner_error(const YYLTYPE *yylloc, language_string_t result, ...);
//此函数会调用exit
void scanner_error_halt(const YYLTYPE *yylloc, language_string_t result, ...);
//此函数会调用exit
void tdataerror(const YYLTYPE *yylloc, scanner_t *self, const char *s, ...);

#endif//_H_SCANNER
