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
#include <Fdo\Clientservices\ConnectionManager.h>

#include "FDO\mgIConnectionManagerImp.h"
#include "mgObjectFactory.h"
#include "FDO\Connections\mgIConnectionImp.h"

NAMESPACE_OSGEO_FDO::IConnectionManagerImp::IConnectionManagerImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
{

}

::IConnectionManager* NAMESPACE_OSGEO_FDO::IConnectionManagerImp::GetImpObj()
{
	return static_cast<::IConnectionManager*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO::IConnectionManagerImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ NAMESPACE_OSGEO_FDO::IConnectionManagerImp::CreateConnection(System::String^ providerName)
{
	FdoIConnection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->CreateConnection(StringToUni(providerName)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO::IConnectionManagerImp::FreeLibrary(System::String^ providerName)
{
	EXCEPTION_HANDLER(GetImpObj()->FreeLibrary(StringToUni(providerName)))
}
