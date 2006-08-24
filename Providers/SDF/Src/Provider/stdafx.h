// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef STDAFX_H
#define STDAFX_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>

#else //_WIN32 not defined

#define SDF_API
#define _ASSERT(x)

#include <string.h>
#define _wcsnicmp wcsncasecmp

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#endif //_WIN32

#include <wchar.h>

#include "SDF.h"

// FdoCommon files
#include <FdoCommonMiscUtil.h>
#include <FdoCommonConnPropDictionary.h>
//globalized error messages, autogenerated include
#include "../Message/Inc/SDFMessage.h"

//Feature Class ID will be stored as an ushort in the 
//SDF+ file --> 2 bytes should be enough for anybody! :)
typedef unsigned short FCID_STORAGE;

//TODO: define these correctly
// do not translate these

#include "SDFProvider.h"

//SDF version numbers.
//Defined in Hex, with one byte for each of major and minor revision:

#define SDFPROVIDER_VERSION_MAJOR_CURRENT  SDFPROVIDER_VERSION_MAJOR_3
#define SDFPROVIDER_VERSION_MINOR_CURRENT  SDFPROVIDER_VERSION_MINOR_3_1
#define SDFPROVIDER_VERSION_MINOR_CURRENT_NLS  SDFPROVIDER_VERSION_MINOR_3_2
#define SDFPROVIDER_VERSION_MAJOR_3   0x03
#define SDFPROVIDER_VERSION_MINOR_3_0 0x00
#define SDFPROVIDER_VERSION_MINOR_3_1 0x01
#define SDFPROVIDER_VERSION_MINOR_3_2 0x02


#define SDF_GLOBAL_TOLERANCE 1.0e-10 //used to determine if two double precision numbers are about equal

//-----------------------------------
//For globalization of error messages.
//Definitions taken from ODBC provider
//-----------------------------------

#include "Inc/nls.h"

// the default message catalog filename
extern char *fdoodbc_cat;

inline FdoString* NlsMsgGetMain(int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    FdoString* ret = FdoException::NLSGetMessage (msg_num, default_msg, fdoodbc_cat, varargs);
    va_end(varargs);

    return ret;
}

inline FdoString* NlsMsgGetMain(int msg_num, char* default_msg, char* file, int line, ...)
{
    va_list varargs;
    va_start(varargs, line);
    FdoString* ret = FdoException::NLSGetMessage (msg_num, default_msg, file, line, fdoodbc_cat, varargs);
    va_end(varargs);

    return ret;
}

//------------------------------------
#endif //STDAFX_H

