#!/bin/sh

##
## Copyright (C) 2004-2006  Autodesk, Inc.
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
TYPECONFIGURE=configure
BUILDDOCS=no
PREFIXVAL=/usr/local/fdo-3.7.0

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
   echo " "
   echo "Help:            --h[elp]"
   echo "BuildType:       --c[onfig] release(default), debug"
   echo "Action:          --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "BuildDocs:       --d[ocs] skip(default), build"
   echo "ConfigMakefiles: --m[akefile] configure(default), noconfigure"
   echo "Prefix:          --p[refix] <fdo install location>"
   echo "************************************************************************************************************"
   exit 0
fi

### start build ###
if test "$TYPECONFIGURE" == configure ; then
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
   sudo -E make clean
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
   sudo -E make
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   sudo -E make install
fi

if test "$TYPEACTION" == uninstall ; then
   sudo -E make uninstall
fi

if test "$BUILDDOCS" == yes ; then
   echo Creating WMS provider html documentation
   rm -rf Docs/HTML/WMS
   mkdir -p Docs/HTML/WMS
   cp ../../DocResources/geospatial.js Docs/HTML/WMS
   cp ../../DocResources/osgeo.css Docs/HTML/WMS

   rm -rf Docs/HTML/WMS_managed
   mkdir -p Docs/HTML/WMS_managed
   cp ../../DocResources/geospatial.js Docs/HTML/WMS_managed
   cp ../../DocResources/osgeo.css Docs/HTML/WMS_managed

   pushd Docs/doc_src >& /dev/null
   doxygen Doxyfile_WMS >& /dev/null
   doxygen Doxyfile_WMS_managed >& /dev/null
   popd >& /dev/null
fi

if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
   rm -rf "$PREFIXVAL/docs/HTML/Providers/WMS"
   mkdir -p "$PREFIXVAL/docs/HTML/Providers"
   if test -e "Docs/HTML/WMS"; then
      cp --force --recursive "Docs/HTML/WMS" "$PREFIXVAL/docs/HTML/Providers"
      cp --force --recursive "Docs/HTML/WMS_managed" "$PREFIXVAL/docs/HTML/Providers"
   fi
fi

exit 0

