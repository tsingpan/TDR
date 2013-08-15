dnl $Id$
dnl config.m4 for extension hotscript

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(hotscript, for hotscript support,
Make sure that the comment is aligned:
[  --with-hotscript             Include hotscript support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(hotscript, whether to enable hotscript support,
dnl Make sure that the comment is aligned:
dnl [  --enable-hotscript           Enable hotscript support])

if test "$PHP_HOTSCRIPT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-hotscript -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hotscript.h"  # you most likely want to change this
  dnl if test -r $PHP_HOTSCRIPT/$SEARCH_FOR; then # path given as parameter
  dnl   HOTSCRIPT_DIR=$PHP_HOTSCRIPT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hotscript files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HOTSCRIPT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HOTSCRIPT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hotscript distribution])
  dnl fi

  dnl # --with-hotscript -> add include path
PHP_ADD_INCLUDE($HOTSCRIPT_DIR/include)

  dnl # --with-hotscript -> check for lib and symbol presence
  dnl LIBNAME=hotscript # you may want to change this
  dnl LIBSYMBOL=hotscript # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HOTSCRIPT_DIR/lib, HOTSCRIPT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HOTSCRIPTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong hotscript lib version or lib not found])
  dnl ],[
  dnl   -L$HOTSCRIPT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HOTSCRIPT_SHARED_LIBADD)

  PHP_ADD_LIBRARY_WITH_PATH(hotprotocol, $HOTSCRIPT_DIR/lib, HOTSCRIPT_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(hotscript, $HOTSCRIPT_DIR/lib, HOTSCRIPT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(hotscript, hotscript.c, $ext_shared)
fi
