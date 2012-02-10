/*
 * Copyright (C) 2010  SL-King d.o.o
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef _WIN32
#pragma once
#endif

#ifndef _FDOKGKMLXMLGLOBALS__H_
#define _FDOKGKMLXMLGLOBALS__H_

namespace FdoKgKmlXmlGlobals
{
    static FdoString* g_KgOraSchemaMapping                    = L"SchemaMapping";
    static FdoString* g_KgOraClassDefinitionElement           = L"ComplexType";
    static FdoString* g_KgOraFullTableNameAttribute           = L"FullTableName";
    static FdoString* g_KgOraPropertyElement                  = L"Element";
    static FdoString* g_KgOraColumnElement                    = L"Column";
    static FdoString* g_KgOraXmlnsValue                       = L"http://fdokml.osgeo.org/schemas";
};

#endif // _FDOKGKMLXMLGLOBALS__H_


