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
// The following header files sequence is very important
#include <Fdo\Raster\RasterDataModel.h>
#include <Fdo\Expression\DataValue.h>
#include <Fdo\Commands\Feature\IReader.h>
#include <Fdo\Commands\Feature\IDataReader.h>

#include "FDO\Commands\Feature\mgIDataReaderImp.h"

FdoIDataReader* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetImpObj()
{
    return static_cast<FdoIDataReader*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyCount()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyCount())

	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetDataType(System::String^ name)
{
	FdoDataType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyType(System::String^ name)
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetDataType(System::Int32 index)
{
    FdoDataType result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetDataType(index))

    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp::GetPropertyType(System::Int32 index)
{
    FdoPropertyType result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType(index))

    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

