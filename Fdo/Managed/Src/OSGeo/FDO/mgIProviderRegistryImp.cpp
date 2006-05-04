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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\IProviderRegistry.h>
#include <Fdo\Clientservices\ProviderCollection.h>

#include "mgIProviderRegistryImp.h"
#include "FDO\mgObjectFactory.h"
#include "ClientServices\mgProviderCollection.h"


NAMESPACE_OSGEO_FDO::IProviderRegistryImp::IProviderRegistryImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: Disposable(unmanaged, autoDelete)
{
}

::IProviderRegistry* NAMESPACE_OSGEO_FDO::IProviderRegistryImp::GetImpObj()
{
	return static_cast<::IProviderRegistry*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO::IProviderRegistryImp::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection* NAMESPACE_OSGEO_FDO::IProviderRegistryImp::GetProviders()
{
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProviderCollection(const_cast<FdoProviderCollection *>(GetImpObj()->GetProviders()), true);
}

System::Void NAMESPACE_OSGEO_FDO::IProviderRegistryImp::RegisterProvider(String * name, 
													String * displayName, 
													String * description, 
													String * version, 
													String * fdoVersion, 
													String * libraryPath,
													System::Boolean isManaged)
{
	EXCEPTION_HANDLER(GetImpObj()->RegisterProvider(StringToUni(name),
		StringToUni(displayName),
		StringToUni(description),
		StringToUni(version),
		StringToUni(fdoVersion),
		StringToUni(libraryPath),
		isManaged))
}

System::Void NAMESPACE_OSGEO_FDO::IProviderRegistryImp::UnregisterProvider(String * name)
{
	EXCEPTION_HANDLER(GetImpObj()->UnregisterProvider(StringToUni(name)))
}


