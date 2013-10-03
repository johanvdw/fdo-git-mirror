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

#############################################
## build third party libraries for WMS:    ##
## boost, libcurl, openssl, gdal           ##
#############################################

pushd . >& /dev/null
cd boost
echo Building boost
cd tools/jam/src
chmod a+x ./build.sh
sudo -E sh ./build.sh
cd ../../../
./tools/jam/src/bin.linuxx86/bjam toolset=gcc variant=release threading=multi link=static --layout=system stage
popd >& /dev/null

pushd . >& /dev/null
cd libcurl
echo Building libcurl
chmod a+x ./configure
./configure --without-libidn --disable-ldap
sudo -E make
mkdir -p lib/linux
rm -f ./lib/linux/*.*
cp -f ./lib/.libs/libcurl.a ./lib/linux
popd >& /dev/null

pushd . >& /dev/null
cd openssl
echo Building openssl
mkdir -p lib/linux
rm -f ./lib/linux/*.*
chmod a+x ./config
./config
sudo -E make
mv -f ./libssl.a ./lib/linux/libssl.a
mv -f ./libcrypto.a ./lib/linux/libcrypto.a
popd >& /dev/null

## Thirdparty_WMS/GDAL
if test "$FDOGDAL" == "$FDOTHIRDPARTY/gdal"; then
    pushd . >& /dev/null
    cd gdal
    echo Building gdal
    mkdir -p lib
    mkdir -p include
    rm -f lib/*.libgdal.a    
    rm -f lib/*.libgdal.so    
    rm -f lib/*.libgdal.so.1    
    rm -f lib/*.libgdal.so.1.16.0   
    rm -f include/*
    chmod a+x ./configure
    echo Build GDAL library with the following settings:
    echo     gif support         - internal
    echo     jpeg support        - internal
    echo     png support         - internal
    echo     tiff support        - internal
    echo     geotiff support     - internal
    echo     libz support        - internal
    echo     python support      - no
    echo     OGR support         - yes
    echo     postgreSQL support  - no
    echo     odbc support        - no
    ./configure --with-gif=internal --with-jpeg=internal --with-png=internal --with-libtiff=internal --with-geotiff=internal --with-pg=no --with-python=no --with-libz=internal --with-odbc=no
    sudo -E make
    cp -f .libs/libgdal.a lib/
    cp -f .libs/libgdal.so lib/
    cp -f .libs/libgdal.so.1 lib/
    cp -f .libs/libgdal.so.1.16.0 lib/
    cp -f port/*.h include/
    cp -f gcore/*.h include/
    cp -f alg/*.h include/
    cp -f ogr/*.h include/
    popd >& /dev/null
else
    echo "Building of the Thirdparty/GDAL libraries has been skipped. Environment variable FDOGDAL points to a previously installed version of GDAL at $FDOGDAL. This version of GDAL will be used to build the FDO WMS and GDAL providers."
fi
