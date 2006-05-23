#!/bin/bash

echo "Begin settings"
if test ! -e "$PWD/Fdo"; then
   echo "Invalid FDO path provided"
   echo "Your default value is: $PWD/Fdo"
   echo "Please modify the script file with the right path"
elif test ! -e "$PWD/Utilities"; then
   echo "Invalid FDO Utilities path provided"
   echo "Your default value is: $PWD/Utilities"
   echo "Please modify the script file with the right path"
elif test ! -e "$PWD/Thirdparty"; then
   echo "Invalid FDO Thirdparty path provided"
   echo "Your default value is: $PWD/Thirdparty"
   echo "Please modify the script file with the right path"
else
   export FDO=$PWD/Fdo
   export FDOTHIRDPARTY=$PWD/Thirdparty
   export FDOUTILITIES=$PWD/Utilities
fi
if test ! -e "$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux"; then
   echo "Invalid path for ArcSDE SDK files"
   echo "Your default value is: $FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux"
   echo "Please modify the setenvironment.sh script with the right path"
elif test ! -e "$FDOTHIRDPARTY/mysql/rhlinux"; then
   echo "Invalid path for MySQL SDK files"
   echo "Your default value is: $FDOTHIRDPARTY/mysql/rhlinux"
   echo "Please modify the setenvironment.sh script with the right path"
elif test ! -e "/usr"; then
   echo "Invalid path for ODBC SDK files"
   echo "Your default value is: /usr"
   echo "Please modify the setenvironment.sh script with the right path"
else
   export SDEHOME=$FDOTHIRDPARTY/ESRI/ArcSDEClient91/Linux
   export FDOMYSQL=$FDOTHIRDPARTY/mysql/rhlinux
   export FDOODBC=/usr
   mkdir -p "/usr/local/fdo-3.2.0/lib"
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/fdo-3.2.0/lib:$FDOTHIRDPARTY/linux/cppunit/lib:$FDOTHIRDPARTY/apache/xml-xalan/c/lib:$FDOTHIRDPARTY/apache/xml-xerces/c/lib
   echo "Settings succeeded"
fi

echo "End settings"
