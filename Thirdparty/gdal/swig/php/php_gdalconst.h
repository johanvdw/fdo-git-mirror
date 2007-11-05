/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.26
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

/*
  +----------------------------------------------------------------------+
  | PHP version 4.0                                                      |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997, 1998, 1999, 2000, 2001 The PHP Group             |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors:                                                             |
  |                                                                      |
  +----------------------------------------------------------------------+
 */


#ifndef PHP_GDALCONST_H
#define PHP_GDALCONST_H

extern zend_module_entry gdalconst_module_entry;
#define phpext_gdalconst_ptr &gdalconst_module_entry

#ifdef PHP_WIN32
# define PHP_GDALCONST_API __declspec(dllexport)
#else
# define PHP_GDALCONST_API
#endif

PHP_MINIT_FUNCTION(gdalconst);
PHP_MSHUTDOWN_FUNCTION(gdalconst);
PHP_RINIT_FUNCTION(gdalconst);
PHP_RSHUTDOWN_FUNCTION(gdalconst);
PHP_MINFO_FUNCTION(gdalconst);

#endif /* PHP_GDALCONST_H */
