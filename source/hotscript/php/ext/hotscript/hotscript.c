#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_hotscript.h"
#include "main/php_output.h"
#include "hotprotocol/hp_abstract_reader.h"
#include "hotlib/hp_error_code.h"
#include "hotscript/hp_script_vm.h"
#include "hotscript/hp_script_parser.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#include "ext/standard/php_smart_str.h"

static PHP_FUNCTION(hs_execute_array);
static PHP_FUNCTION(hs_execute);
static PHP_FUNCTION(hs_last_error);
static PHP_FUNCTION(hs_last_error_msg);

ZEND_DECLARE_MODULE_GLOBALS(hotscript)

/* True global resources - no need for thread safety here */
static int le_hotscript;

ZEND_BEGIN_ARG_INFO_EX(arginfo_hs_execute_array, 0, 0, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hs_execute, 0, 0, 0)
ZEND_ARG_INFO(0, value)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_hs_last_error, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_hs_last_error_msg, 0)
ZEND_END_ARG_INFO()


const zend_function_entry hotscript_functions[] = {
	PHP_FE(hs_execute_array, arginfo_hs_execute_array)
	PHP_FE(hs_execute,	arginfo_hs_execute)
	PHP_FE(hs_last_error, arginfo_hs_last_error)
	PHP_FE(hs_last_error_msg, arginfo_hs_last_error_msg)
	PHP_FE_END	/* Must be the last line in hotscript_functions[] */
};

zend_module_entry hotscript_module_entry = {
	STANDARD_MODULE_HEADER,
	"hotscript",
	hotscript_functions,
	PHP_MINIT(hotscript),
	PHP_MSHUTDOWN(hotscript),
	PHP_RINIT(hotscript),
	PHP_RSHUTDOWN(hotscript),
	PHP_MINFO(hotscript),
	PHP_HOTSCRIPT_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HOTSCRIPT
ZEND_GET_MODULE(hotscript)
#endif

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("hotpot.dir", "C:/Program Files/HotPot", PHP_INI_ALL, OnUpdateString, hotpot_dir, zend_hotscript_globals, hotscript_globals)
	STD_PHP_INI_ENTRY("hotscript.workingdir", "./", PHP_INI_ALL, OnUpdateString, workingdir, zend_hotscript_globals, hotscript_globals)
PHP_INI_END()

static void php_hotscript_init_globals(zend_hotscript_globals *hotscript_globals TSRMLS_DC)
{
	hotscript_globals->hotpot_dir = getenv("HOTPOT_DIR");
	hotscript_globals->workingdir = "./";
	hotscript_globals->error_code = E_HP_NOERROR;
	hotscript_globals->error_msg[0] = 0;
}

PHP_MINIT_FUNCTION(hotscript)
{	
	ZEND_INIT_MODULE_GLOBALS(hotscript, php_hotscript_init_globals, NULL);

	REGISTER_INI_ENTRIES();

	
	
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hotscript)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(hotscript)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hotscript)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(hotscript)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hotscript support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

#define MAX_DEEP 1024
typedef struct tagSTACK_NODE
{
	zval *val;
	HashPosition pos;
}STACK_NODE;

typedef struct _ZVALReader
{
	HPAbstractReader super;
	hpuint32 stack_num;
	STACK_NODE stack[MAX_DEEP];
#ifdef ZTS
	TSRMLS_D;
#endif//ZTS
}ZVALReader;

zval* zval_reade_get_zval(ZVALReader *self)
{
	return self->stack[self->stack_num - 1].val;
}
hpint32 zval_read_field_end(HPAbstractReader *super, const char *var_name)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 zval_read_field_begin(HPAbstractReader *super, const char *var_name)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	HashTable *myht;
	zval *zv = zval_reade_get_zval(self);
	zval **data;

	if (Z_TYPE_P(zv) != IS_ARRAY)
	{		
		return E_HP_ERROR;
	}
	myht = zv->value.ht;
	

	if(zend_hash_find(myht, var_name, strlen(var_name) + 1, (void **)&data) != SUCCESS)
	{			
		return E_HP_ERROR;
	}
	

	self->stack[self->stack_num].val = *data;
	++(self->stack_num);


	return E_HP_NOERROR;
}
hpint32 zval_reader_vector_item_end(HPAbstractReader *super, hpuint32 index)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	--(self->stack_num);
	return E_HP_NOERROR;
}
hpint32 zval_reader_vector_item_begin(HPAbstractReader *super, hpuint32 index)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	HashTable *myht;
	zval *zv = zval_reade_get_zval(self);
	zval **data;

	if (Z_TYPE_P(zv) != IS_ARRAY)
	{		
		return E_HP_ERROR;
	}
	myht = zv->value.ht;

	if(zend_hash_index_find(myht, index, (void **) &data) != SUCCESS)
	{
		return E_HP_ERROR;
	}
	self->stack[self->stack_num].val = *data;
	++(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 zval_reader_read_type(HPAbstractReader *super, HPType *type)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);

	switch(Z_TYPE_P(zv))
	{
	case IS_STRING:
		*type = E_HP_BYTES;
		break;
	case IS_DOUBLE:
		*type = E_HP_DOUBLE;
		break;
	case IS_LONG:
		*type = E_HP_INT64;
		break;
	default:
		return E_HP_ERROR;
	}
	return E_HP_NOERROR;
}

hpint32 zval_reader_bytes(HPAbstractReader *super, hpbytes *bytes)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_STRING)
	{
		return E_HP_ERROR;
	}
	bytes->ptr = Z_STRVAL_P(zv);
	bytes->len = Z_STRLEN_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_reader_double(HPAbstractReader *super, hpdouble *val)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_DOUBLE)
	{
		return E_HP_ERROR;
	}
	*val = Z_DVAL_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_reader_int64(HPAbstractReader *super, hpint64 *val)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_LONG)
	{
		return E_HP_ERROR;
	}
	*val = Z_LVAL_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_reader_init(ZVALReader *self TSRMLS_DC)
{
	memset(&self->super, 0, sizeof(HPAbstractReader));

	self->super.read_field_begin = zval_read_field_begin;
	self->super.read_field_end= zval_read_field_end;
	self->super.read_vector_item_begin = zval_reader_vector_item_begin;	
	self->super.read_vector_item_end= zval_reader_vector_item_end;

	self->super.read_hpint64 = zval_reader_int64;
	self->super.read_hpdouble = zval_reader_double;
	self->super.read_bytes = zval_reader_bytes;
	self->super.read_type = zval_reader_read_type;

#ifdef ZTS
	self->tsrm_ls = tsrm_ls;
#endif//ZTS

	return E_HP_NOERROR;
}

hpint32 zval_ob_read_field_end(HPAbstractReader *super, const char *var_name)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	--(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 zval_ob_read_field_begin(HPAbstractReader *super, const char *var_name)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	zval *data;

	if (Z_TYPE_P(zv) != IS_OBJECT)
	{		
		return E_HP_ERROR;
	}
#ifdef ZTS
	data = zend_read_property(zend_get_class_entry(zv, self->tsrm_ls), zv, var_name, strlen(var_name) + 1, 0, self->tsrm_ls);
#else
	data = zend_read_property(zend_get_class_entry(zv), zv, var_name, strlen(var_name) + 1, 0);
#endif

	if(Z_TYPE_P(data) == IS_NULL)
	{
		return E_HP_ERROR;
	}

	self->stack[self->stack_num].val = data;
	++(self->stack_num);

	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_vector_item_end(HPAbstractReader *super, hpuint32 index)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	--(self->stack_num);
	return E_HP_NOERROR;
}
hpint32 zval_ob_reader_vector_item_begin(HPAbstractReader *super, hpuint32 index)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	HashTable *myht;
	zval *zv = zval_reade_get_zval(self);
	zval **data;

	if (Z_TYPE_P(zv) != IS_ARRAY)
	{
		return E_HP_ERROR;
	}
	myht = zv->value.ht;

	if(zend_hash_index_find(myht, index, (void **) &data) != SUCCESS)
	{
		return E_HP_ERROR;
	}
	self->stack[self->stack_num].val = *data;
	++(self->stack_num);
	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_read_type(HPAbstractReader *super, HPType *type)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);

	switch(Z_TYPE_P(zv))
	{
	case IS_STRING:
		*type = E_HP_BYTES;
		break;
	case IS_DOUBLE:
		*type = E_HP_DOUBLE;
		break;
	case IS_LONG:
		*type = E_HP_INT64;
		break;
	default:
		return E_HP_ERROR;
	}
	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_bytes(HPAbstractReader *super, hpbytes *bytes)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_STRING)
	{
		return E_HP_ERROR;
	}
	bytes->ptr = Z_STRVAL_P(zv);
	bytes->len = Z_STRLEN_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_double(HPAbstractReader *super, hpdouble *val)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_DOUBLE)
	{
		return E_HP_ERROR;
	}
	*val = Z_DVAL_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_int64(HPAbstractReader *super, hpint64 *val)
{
	ZVALReader *self = HP_CONTAINER_OF(super, ZVALReader, super);
	zval *zv = zval_reade_get_zval(self);
	if(Z_TYPE_P(zv) != IS_LONG)
	{
		return E_HP_ERROR;
	}
	*val = Z_LVAL_P(zv);
	return E_HP_NOERROR;
}

hpint32 zval_ob_reader_init(ZVALReader *self TSRMLS_DC)
{
	memset(&self->super, 0, sizeof(HPAbstractReader));

	self->super.read_field_begin = zval_ob_read_field_begin;
	self->super.read_field_end= zval_ob_read_field_end;
	self->super.read_vector_item_begin = zval_ob_reader_vector_item_begin;	
	self->super.read_vector_item_end= zval_ob_reader_vector_item_end;

	self->super.read_hpint64 = zval_ob_reader_int64;
	self->super.read_hpdouble = zval_ob_reader_double;
	self->super.read_bytes = zval_ob_reader_bytes;
	self->super.read_type = zval_ob_reader_read_type;
#ifdef ZTS
	self->tsrm_ls = tsrm_ls;
#endif//ZTS

	return E_HP_NOERROR;
}




static void php_putc(HotVM *self, char c)
{
	smart_str_appendc((smart_str*)self->user_data, c);
}


static PHP_FUNCTION(hs_execute_array)
{
	zval *parameter = NULL;	
	smart_str buf = {0};
	ZVALReader reader;
	HotVM hotvm;
	char *file_name = NULL;
	int file_name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "as", &parameter, &file_name, &file_name_len) == FAILURE)
	{
		ZVAL_FALSE(return_value);
	}
	HOTSCRIPT_G(error_code) = E_HP_NOERROR;

	zval_reader_init(&reader TSRMLS_CC);
	reader.stack[0].val = parameter;
	reader.stack_num = 1;
	

	if(hotvm_execute(&hotvm, file_name, HOTSCRIPT_G(hotpot_dir), &reader.super, &buf, php_putc, HOTSCRIPT_G(workingdir)) == E_HP_NOERROR)
	{
		ZVAL_STRINGL(return_value, buf.c, buf.len, 1);		
	}
	else
	{
		HOTSCRIPT_G(error_code) = hotvm.result;
		strncpy(HOTSCRIPT_G(error_msg), hotvm.result_str, MAX_ERROR_MSG_LENGTH);
		ZVAL_FALSE(return_value);
	}

	smart_str_free(&buf);
}

static PHP_FUNCTION(hs_execute)
{
	zval *parameter = NULL;	
	smart_str buf = {0};
	ZVALReader reader;
	HotVM hotvm;
	char *file_name = NULL;
	int file_name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &parameter, &file_name, &file_name_len) == FAILURE)
	{
		ZVAL_FALSE(return_value);
	}
	HOTSCRIPT_G(error_code) = E_HP_NOERROR;

	zval_ob_reader_init(&reader TSRMLS_CC);
	reader.stack[0].val = parameter;
	reader.stack_num = 1;

	if(hotvm_execute(&hotvm, file_name, HOTSCRIPT_G(hotpot_dir), &reader.super, &buf, php_putc, HOTSCRIPT_G(workingdir)) == E_HP_NOERROR)
	{
		ZVAL_STRINGL(return_value, buf.c, buf.len, 1);		
	}
	else
	{
		ZVAL_FALSE(return_value);
	}

	smart_str_free(&buf);
}

static PHP_FUNCTION(hs_last_error)
{
	if (zend_parse_parameters_none() == FAILURE)
	{
		return;
	}

	RETURN_LONG(HOTSCRIPT_G(error_code));
}

static PHP_FUNCTION(hs_last_error_msg)
{
	if (zend_parse_parameters_none() == FAILURE)
	{
		return;
	}

	if(HOTSCRIPT_G(error_code) == E_HP_NOERROR)
	{
		return;
	}
	
	RETURN_STRING(HOTSCRIPT_G(error_msg), 1);
}
