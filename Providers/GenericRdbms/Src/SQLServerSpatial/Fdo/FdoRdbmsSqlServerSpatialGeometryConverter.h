/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#ifndef FDORDBMSSQLSERVERSPATIALGEOMETRYCONVERTER_H
#define FDORDBMSSQLSERVERSPATIALGEOMETRYCONVERTER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialGeometryConverter.h>

// Implementation class for abstract FdoSpatialGeometryConverter class.
// No X and Y ordinates swapping.
class FdoRdbmsSqlServerSpatialGeometryConverter : public FdoSpatialGeometryConverter
{
public:
   FDO_SPATIAL_API void ConvertPosition(
        double &x,
		double &y) 
   {
   }

    FDO_SPATIAL_API virtual void ConvertPosition(
        double &x,
        double &y,
		double &z) 
    {
    }
};

// Implementation class for converting geography column values.
// Swaps X and Y ordinates.
class FdoRdbmsSqlServerSpatialGeographyConverter : public FdoSpatialGeometryConverter
{
public:
   FDO_SPATIAL_API void ConvertPosition(
        double &x,
		double &y) 
   {
       double temp = y;
       y = x;
       x = temp;
   }

    FDO_SPATIAL_API virtual void ConvertPosition(
        double &x,
        double &y,
		double &z) 
    {
        ConvertPosition( x, y );
    }
};

#endif
