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
#include "mgIMultiGeometryImp.h"
#include <FdoGeometry.h>
#include "mgIGeometry.h"
#include "mgObjectFactory.h"

FdoIMultiGeometry* NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp::GetImpObj()
{
	return static_cast<FdoIMultiGeometry*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp::IMultiGeometryImp(System::IntPtr unmanaged, System::Boolean autoDelete)
	: NAMESPACE_OSGEO_GEOMETRY::IGeometricAggregateAbstractImp(unmanaged, autoDelete)
{
}

NAMESPACE_OSGEO_GEOMETRY::IGeometry^ NAMESPACE_OSGEO_GEOMETRY::IMultiGeometryImp::default::get(System::Int32 index)
{
	FdoIGeometry* ret;
	EXCEPTION_HANDLER(ret = GetImpObj()->GetItem(index))
 	return NAMESPACE_OSGEO_GEOMETRY::ObjectFactory::CreateIGeometry(IntPtr(ret), true);
}
