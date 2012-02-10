/*
* 
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

#include "stdafx.h"
#include <Fdo\Commands\SpatialContext\IActivateSpatialContext.h>

#include "FDO\Commands\SpatialContext\mgIActivateSpatialContextImp.h"

FdoIActivateSpatialContext* NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp::GetImpObj()
{
    return static_cast<FdoIActivateSpatialContext*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp::Execute()
{
	EXCEPTION_HANDLER(GetImpObj()->Execute())
}
