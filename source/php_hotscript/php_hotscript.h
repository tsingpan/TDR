#ifndef PHP_HOTSCRIPT_H
#define PHP_HOTSCRIPT_H

extern zend_module_entry hotscript_module_entry;
#define phpext_hotscript_ptr &hotscript_module_entry

#ifdef PHP_WIN32
#	define PHP_HOTSCRIPT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HOTSCRIPT_API __attribute__ ((visibility("default")))
#else
#	define PHP_HOTSCRIPT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#include "hotpot/hp_error_msg.h"

#define PHP_HOTSCRIPT_VERSION HOTPOT_VERSION


PHP_MINIT_FUNCTION(hotscript);
PHP_MSHUTDOWN_FUNCTION(hotscript);
PHP_RINIT_FUNCTION(hotscript);
PHP_RSHUTDOWN_FUNCTION(hotscript);
PHP_MINFO_FUNCTION(hotscript);

PHP_FUNCTION(hs_execute_array);
PHP_FUNCTION(hs_execute);
  

ZEND_BEGIN_MODULE_GLOBALS(hotscript)
	char* hotpot_dir;

	int error_code;
	char error_msg[MAX_ERROR_MSG_LENGTH];
ZEND_END_MODULE_GLOBALS(hotscript)

#ifdef ZTS
#define HOTSCRIPT_G(v) TSRMG(hotscript_globals_id, zend_hotscript_globals *, v)
#else
#define HOTSCRIPT_G(v) (hotscript_globals.v)
#endif

#endif	/* PHP_HOTSCRIPT_H */
