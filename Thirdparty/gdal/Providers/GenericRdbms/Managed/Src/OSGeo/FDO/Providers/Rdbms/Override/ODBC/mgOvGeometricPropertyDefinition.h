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

#include "FDO\Providers\Rdbms\Override\ODBC\mgIOvPropertyDefinition.h"

class FdoOdbcOvGeometricPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC

///<summary>Concrete class defining physical schema overrides for a geometric property definition.</summary>
public ref class OvGeometricPropertyDefinition : public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE::OvGeometricPropertyDefinition, public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC::IOvPropertyDefinition
{
public:
    ///<summary>Constructs a default of an FdoOdbcOvGeometricPropertyDefinition</summary>
    /// <returns>Returns FdoOdbcOvGeometricPropertyDefinition</returns>
    OvGeometricPropertyDefinition();

    ///<summary>Constructs an instance of an FdoOdbcOvGeometricPropertyDefinition using the specified arguments</summary>
    /// <param name="name">Input name</param>
    /// <returns>Returns FdoOdbcOvGeometricPropertyDefinition</returns>
	OvGeometricPropertyDefinition(System::String^ name);


internal:
	OvGeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoOdbcOvGeometricPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_ODBC
