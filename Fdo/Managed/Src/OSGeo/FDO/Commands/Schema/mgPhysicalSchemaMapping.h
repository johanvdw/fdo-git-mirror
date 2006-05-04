/*
* Copyright (C) 2004-2006  Autodesk, Inc.
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
*
*/

#pragma once

#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

class FdoPhysicalSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

public __gc class PhysicalSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
    __property System::String* get_Provider();

public protected:
    PhysicalSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
    PhysicalSchemaMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* schemaMapping, System::Boolean autoDelete);

public private:
    inline FdoPhysicalSchemaMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


