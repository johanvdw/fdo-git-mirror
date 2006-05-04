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

BEGIN_NAMESPACE_OSGEO_FDO_RASTER

/// \brief
/// The RasterDataType enumeration specifies the type of data stored in the Raster Image.
/// <param name="RasterDataType_Unknown">
/// Data is organized in an unknown or provider specific manner.
/// </param>
/// <param name="RasterDataType_UnsignedInteger">
/// Data is organized as Unsigned Integers.
/// </param>
/// <param name="RasterDataType_Integer">
/// Data is organized as Signed Integers.
/// </param>
/// <param name="RasterDataType_Float">
/// Data is organized as Floats.
/// </param>
/// <param name="RasterDataType_Double">
/// Data is organized as Doubles.
/// </param>
public __value enum RasterDataType
{
    RasterDataType_Unknown = FdoRasterDataType_Unknown,

    RasterDataType_UnsignedInteger = FdoRasterDataType_UnsignedInteger,

    RasterDataType_Integer = FdoRasterDataType_Integer,

    RasterDataType_Float = FdoRasterDataType_Float,

    RasterDataType_Double = FdoRasterDataType_Double
};

END_NAMESPACE_OSGEO_FDO_RASTER


