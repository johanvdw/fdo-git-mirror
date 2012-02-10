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

##########################################
## build third party libraries for fdo: ##
## apache, linux/cppunit                ##
##########################################

pushd . >& /dev/null
cd apache
echo Building apche
chmod a+x ./build2.sh
sudo -E sh ./build2.sh
popd >& /dev/null

pushd . >& /dev/null
cd linux/cppunit
echo Building linux/cppunit
chmod a+x ./build
sudo -E sh ./build
popd >& /dev/null

