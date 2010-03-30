#!/bin/sh

##
## Copyright (C) 2004-2010  Autodesk, Inc.
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


export XERCESCROOT=$PWD/xerces
export XALANCROOT=$PWD/xalan
export NLSDIR=$XALANCROOT/src/xalanc/NLS

cd xerces
./runConfigure -p linux -P "$XERCESCROOT"
make
make install
cd ..

cd xalan
./runConfigure -p linux -P "$XALANCROOT"
make
##make install
cd ..

