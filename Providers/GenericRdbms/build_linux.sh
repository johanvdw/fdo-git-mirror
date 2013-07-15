#!/bin/sh

##
## Copyright (C) 2013  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

TYPEACTION=buildinstall
TYPEBUILD=release
TYPEARCHITECTURE=32
TYPECONFIGURE=configure
BUILDDOCS=no
PREFIXVAL=/usr/local/fdo-3.8.1

### study parameters ###
while test $# -gt 0
do
  arg="$1"
  shift
  case $arg in
  -h | --h | --help)
    SHOWHELP=yes
    break
    ;;
  -b | --b | --build)
    if test "$1" == ""; then
        echo "$arg Invalid parameter $1"
        exit 1
    else
        TYPEARCHITECTURE="$1"
    fi
    shift
    ;;
  -a | --a | --action)
    if test "$1" == buildinstall; then
        TYPEACTION=buildinstall
    elif test "$1" == build; then
        TYPEACTION=build
    elif test "$1" == install; then
        TYPEACTION=install
    elif test "$1" == uninstall; then
        TYPEACTION=uninstall
    elif test "$1" == clean; then
        TYPEACTION=clean
    else
        echo "$arg Invalid parameter $1"
	exit 1
    fi
    shift
    ;;
  -p | --p | --prefix)
    if test "$1" == ""; then
        echo "$arg Invalid parameter $1"
        exit 1
    else
        PREFIXVAL="$1"
    fi
    shift
    ;;
  -m | --m | --makefile)
    if test "$1" == configure; then
        TYPECONFIGURE=configure
    elif test "$1" == noconfigure; then
        TYPECONFIGURE=noconfigure
    else
        echo "$arg Invalid parameter $1"
	exit 1
    fi
    shift
    ;;
  -d | --d | --docs)
    if test "$1" == skip; then
        BUILDDOCS=no
    elif test "$1" == build; then
        BUILDDOCS=yes
    else
        echo "$arg Invalid parameter $1"
	exit 1
    fi
    shift
    ;;
  -c | --c | --config)
    if test "$1" == debug; then
        TYPEBUILD=debug
    elif test "$1" == release; then
        TYPEBUILD=release
    else
        echo "$arg Invalid parameter $1"
	exit 1

    fi
    shift
    ;;
  -*)
    echo "The command option is not recognized: $arg"
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;

  *)
    echo "The command is not recognized: $arg"
    echo "Please use the format:"
    SHOWHELP=yes
    break
    ;;
  esac
done
### end of study parameters ###

if test "$SHOWHELP" == yes; then
   echo "************************************************************************************************************"
   echo "build_linux.sh [--h]"
   echo "               [--c BuildType]"
   echo "               [--a Action]"
   echo "               [--d BuildDocs]"
   echo "               [--m ConfigMakefiles]"
   echo "               [--p Prefix]"
   echo "               [--b BuildArchicture]"
   echo " "
   echo "Help:                  --h[elp]"
   echo "BuildType:             --c[onfig] release(default), debug"
   echo "Action:                --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "BuildDocs:             --d[ocs] skip(default), build"
   echo "ConfigMakefiles:       --m[akefile] configure(default), noconfigure"
   echo "BuildArchitecture:     --b[uild] 32(default), 64"
   echo "Prefix:                --p[refix] <fdo install location>"
   echo "************************************************************************************************************"
   exit 0
fi

if [[ "$CFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CFLAGS="$CFLAGS -m$TYPEARCHITECTURE"
echo "Exporting CFLAGS: "$CFLAGS""
export CFLAGS
fi

if [[ "$CPPFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
CPPFLAGS="$CPPFLAGS -m$TYPEARCHITECTURE"
echo "Exporting CPPFLAGS: "$CPPFLAGS""
export CPPFLAGS
fi

if [[ "$LDFLAGS" != *"-m$TYPEARCHITECTURE"* ]]; then
LDFLAGS="$LDFLAGS -m$TYPEARCHITECTURE"
echo "Exporting LDFLAGS: "$LDFLAGS""
export LDFLAGS
fi

if test "$TYPEARCHITECTURE" == "32" ; then
if test "$HOSTTYPE" == "i686" ; then
if [[ "$CPPFLAGS" != *"-march=i686"* ]]; then
CPPFLAGS="$CPPFLAGS -march=i686"
echo "Exporting CPPFLAGS: "$CPPFLAGS""
export CPPFLAGS
fi
fi
fi

### start build ###
if test "$TYPECONFIGURE" == configure ; then
   if test -e "m4"; then
      echo "m4 directory exists"
   else
      mkdir m4
   fi

   aclocal
   libtoolize --force
   automake --add-missing --copy
   autoconf

   if test "$TYPEBUILD" == release; then
      ./configure --prefix="$PREFIXVAL"
   else
      ./configure --enable-debug=yes --prefix="$PREFIXVAL"
   fi
fi
   
if test "$TYPEACTION" == clean ; then
  make clean
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
   make
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   sudo make install
fi

if test "$TYPEACTION" == uninstall ; then
   sudo make uninstall
fi

if test "$BUILDDOCS" == yes ; then
   rm -rf Docs/HTML/MySQL
   mkdir -p Docs/HTML/MySQL
   cp ../../DocResources/geospatial.js Docs/HTML/MySQL
   cp ../../DocResources/osgeo.css Docs/HTML/MySQL
   
   rm -rf Docs/HTML/MySQL_managed
   mkdir -p Docs/HTML/MySQL_managed
   cp ../../DocResources/geospatial.js Docs/HTML/MySQL_managed
   cp ../../DocResources/osgeo.css Docs/HTML/MySQL_managed

   echo Creating MySQL Provider HTML documentation
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_MySQL >& /dev/null
   doxygen Doxyfile_MySQL_managed >& /dev/null
   popd >& /dev/null
   
   rm -rf Docs/HTML/ODBC
   mkdir -p Docs/HTML/ODBC
   cp ../../DocResources/geospatial.js Docs/HTML/ODBC
   cp ../../DocResources/osgeo.css Docs/HTML/ODBC

   rm -rf Docs/HTML/ODBC_managed
   mkdir -p Docs/HTML/ODBC_managed
   cp ../../DocResources/geospatial.js Docs/HTML/ODBC_managed
   cp ../../DocResources/osgeo.css Docs/HTML/ODBC_managed

   echo Creating ODBC Provider HTML documentation
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_ODBC >& /dev/null
   doxygen Doxyfile_ODBC_managed >& /dev/null
   popd >& /dev/null

   rm -rf Docs/HTML/SQLServerSpatial
   mkdir -p Docs/HTML/SQLServerSpatial
   cp ../../DocResources/geospatial.js Docs/HTML/SQLServerSpatial
   cp ../../DocResources/osgeo.css Docs/HTML/SQLServerSpatial

   rm -rf Docs/HTML/SQLServerSpatial_managed
   mkdir -p Docs/HTML/SQLServerSpatial_managed
   cp ../../DocResources/geospatial.js Docs/HTMLSQLServerSpatial_managed
   cp ../../DocResources/osgeo.css Docs/HTMLSQLServerSpatial_managed

   echo Creating SQLServerSpatial Provider HTML documentation
   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_SQLServerSpatial >& /dev/null
   doxygen Doxyfile_SQLServerSpatial_managed >& /dev/null
   popd >& /dev/null
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/MySQL"
   if test -e "Docs/HTML/MySQL"; then
      sudo cp --force --recursive "Docs/HTML/MySQL" "$PREFIXVAL/docs/HTML/Providers"
   fi

   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/MySQL_managed"
   if test -e "Docs/HTML/MySQL_managed"; then
      sudo cp --force --recursive "Docs/HTML/MySQL_managed" "$PREFIXVAL/docs/HTML/Providers"
   fi

   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/ODBC"
   if test -e "Docs/HTML/ODBC"; then
      sudo cp --force --recursive "Docs/HTML/ODBC" "$PREFIXVAL/docs/HTML/Providers"
   fi

   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/ODBC_managed"
   if test -e "Docs/HTML/ODBC"; then
      sudo cp --force --recursive "Docs/HTML/ODBC_managed" "$PREFIXVAL/docs/HTML/Providers"
   fi

   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/SQLServerSpatial"
   if test -e "Docs/HTML/SQLServerSpatial"; then
      sudo cp --force --recursive "Docs/HTML/SQLServerSpatial" "$PREFIXVAL/docs/HTML/Providers"
   fi

   sudo rm -rf "$PREFIXVAL/docs/HTML/Providers/SQLServerSpatial_managed"
   if test -e "Docs/HTML/SQLServerSpatial_managed"; then
      sudo cp --force --recursive "Docs/HTML/SQLServerSpatial_managed" "$PREFIXVAL/docs/HTML/Providers"
   fi
fi

exit 0

