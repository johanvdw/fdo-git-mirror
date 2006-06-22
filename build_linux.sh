#!/bin/sh

TYPEACTION=buildinstall
TYPEBUILD=release
TYPECONFIGURE=configure
BUILDDOCS=skip

DEFMODIFY=no
FDOCOREENABLE=yes
FDOENABLE=no
UTILENABLE=no
SHPENABLE=yes
SDFENABLE=yes
WFSENABLE=yes
WMSENABLE=yes
ARCENABLE=yes
RDBMSENABLE=yes
SHOWHELP=no


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
        BUILDDOCS=skip
    elif test "$1" == build; then
        BUILDDOCS=build
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
  -w | --w | --with)
    if test "$DEFMODIFY" == no; then
       DEFMODIFY=yes
       FDOCOREENABLE=no
       FDOENABLE=no
       SHPENABLE=no
       SDFENABLE=no
       WFSENABLE=no
       WMSENABLE=no
       ARCENABLE=no
       RDBMSENABLE=no
       UTILENABLE=no
    fi
    if test -z "$1"; then
       echo "$arg Invalid parameter $1"
    exit 1
    elif test "$1" == all; then
        FDOCOREENABLE=yes
        SHPENABLE=yes
        SDFENABLE=yes
        WFSENABLE=yes
        WMSENABLE=yes
        ARCENABLE=yes
        RDBMSENABLE=yes
	elif test "$1" == fdocore; then
		FDOCOREENABLE=yes
    elif test "$1" == fdo; then
        FDOENABLE=yes
    elif test "$1" == utilities; then
        UTILENABLE=yes
    elif test "$1" == providers; then
        SHPENABLE=yes
        SDFENABLE=yes
        WFSENABLE=yes
        WMSENABLE=yes
        ARCENABLE=yes
        RDBMSENABLE=yes
    elif test "$1" == shp; then
        SHPENABLE=yes
    elif test "$1" == sdf; then
        SDFENABLE=yes
    elif test "$1" == wfs; then
        WFSENABLE=yes
    elif test "$1" == wms; then
        WMSENABLE=yes
    elif test "$1" == arcsde; then
        ARCENABLE=yes
    elif test "$1" == rdbms; then
        RDBMSENABLE=yes
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
   echo "build_linux.sh [--h] [--c BuildType] [--a Action] [--w WithModule] [--d BuildDocs] [--m ConfigMakefiles]"
   echo "*"
   echo "Help:            --h[elp]"
   echo "BuildType:       --c[onfig] release(default), debug"
   echo "Action:          --a[ction] buildinstall(default), build, install, uninstall, clean"
   echo "BuildDocs:       --d[ocs] skip(default), build"
   echo "ConfigMakefiles: --m[akefile] configure(default), noconfigure"

   HELPSTRINGWITH="WithModule:      --w[ith] all(default), fdocore, fdo, utilities, providers"
   if test -e "Providers/SHP/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, shp"
   fi
   if test -e "Providers/SDF/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, sdf"
   fi
   if test -e "Providers/WFS/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, wfs"
   fi
   if test -e "Providers/WMS/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, wms"
   fi
   if test -e "Providers/ArcSDE/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, arcsde"
   fi
   if test -e "Providers/GenericRdbms/build_linux.sh"; then
   HELPSTRINGWITH="$HELPSTRINGWITH, rdbms"
   fi
   
   echo "$HELPSTRINGWITH"
   echo "************************************************************************************************************"

   exit 0
fi

### configure build ###
if test "$TYPECONFIGURE" == configure ; then
   if test "$FDOCOREENABLE" == yes || test "$THRPENABLE" == yes || test "$FDOENABLE" == yes || test "$UTILENABLE" == yes; then
      echo "configuring fdocore"
      aclocal
      libtoolize --force
      automake --add-missing --copy
      autoconf

      if test "$TYPEBUILD" == release; then
         ./configure
      else
         ./configure --enable-debug=yes
      fi
   fi
fi

### start build ###

CMDEX="--c $TYPEBUILD --a $TYPEACTION --d $BUILDDOCS --m $TYPECONFIGURE"

#build all of fdocore
if test "$FDOCOREENABLE" == yes; then
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
      make
   fi
   if test "$TYPEACTION" == clean ; then
      make clean
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
      make install
   fi
   if test "$TYPEACTION" == uninstall ; then
      make uninstall
   fi
fi

#build Fdo
if test "$FDOENABLE" == yes; then
   pushd Fdo >& /dev/null
   
   if test "$TYPEACTION" == clean ; then
      make clean
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
      make
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
      make install
   fi
   if test "$TYPEACTION" == uninstall ; then
      make uninstall
   fi
   
   if test "$BUILDDOCS" == build ; then
      echo Creating Fdo unmanaged html documentation
      rm -rf Docs/HTML/FDO_API
      mkdir -p Docs/HTML/FDO_API

      pushd Docs/doc_src >& /dev/null
      doxygen Doxyfile_FDOunmanaged >& /dev/null
      popd >& /dev/null
   fi
   
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
      rm -rf "/usr/local/fdo-3.2.0/docs/HTML/FDO_API"
      mkdir -p "/usr/local/fdo-3.2.0/docs/HTML"
      if test -e "Docs/HTML/FDO_API"; then
         cp --force --recursive "Docs/HTML/FDO_API" "/usr/local/fdo-3.2.0/docs/HTML"
      fi
   fi
   
   popd >& /dev/null
fi

#build Utilities
if test "$UTILENABLE" == yes; then
   pushd Utilities >& /dev/null
   
   if test "$TYPEACTION" == clean ; then
      make clean
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == build ; then
      make
   fi
   if test "$TYPEACTION" == buildinstall || test "$TYPEACTION" == install ; then
      make install
   fi
   if test "$TYPEACTION" == uninstall ; then
      make uninstall
   fi
   
   popd >& /dev/null
fi

#build SHP Provider
if test "$SHPENABLE" == yes; then
   if test ! -e "Providers/SHP/build_linux.sh"; then
      echo "Providers/SHP/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/SHP >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build SDF Provider
if test "$SDFENABLE" == yes; then
   if test ! -e "Providers/SDF/build_linux.sh"; then
      echo "Providers/SDF/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/SDF >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build WFS Provider
if test "$WFSENABLE" == yes; then
   if test ! -e "Providers/WFS/build_linux.sh"; then
      echo "Providers/WFS/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/WFS >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build WMS Provider
if test "$WMSENABLE" == yes; then
   if test ! -e "Providers/WMS/build_linux.sh"; then
      echo "Providers/WMS/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/WMS >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build ArcSDE Provider
if test "$ARCENABLE" == yes; then
   if test ! -e "Providers/ArcSDE/build_linux.sh"; then
      echo "Providers/ArcSDE/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/ArcSDE >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

#build GenericRdbms Providers
if test "$RDBMSENABLE" == yes; then
   if test ! -e "Providers/GenericRdbms/build_linux.sh"; then
      echo "Providers/GenericRdbms/build_linux.sh is missing!"
      exit 1;
   fi

   pushd Providers/GenericRdbms >& /dev/null
   ./build_linux.sh $CMDEX
   popd >& /dev/null
fi

exit 0

