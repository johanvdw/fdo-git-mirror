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

#include "FDO\Connections\Capabilities\mgIGeometryCapabilities.h"

class FdoIGeometryCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \brief
/// The FdoIGeometryCapabilities class delineates available support for raster processing from a provider.
private __gc class IGeometryCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities
{
public:
    /// \brief
    /// Return the list of supported geometry types. For example, if a client wanted to know if a provider supported
    /// multi-polygons, it would call GetGeometryTypes and check if the MultiPolygon type was listed.
    /// 
    /// \param length 
    /// Output the number of geometry types.
    /// 
    /// \return
    /// Returns the list of geometry types
    /// 
	__property NAMESPACE_OSGEO_COMMON::GeometryType get_GeometryTypes() [];

    /// \brief
    /// Return the list of supported component types. For example, if a client wanted to know if circular arcs were supported 
    /// by a provider, it would call GetGeometryComponentTypes and check for CircularArcSegment in the returned list.
    /// 
    /// \param length 
    /// Output the number of component types.
    /// 
    /// \return
    /// Returns the list of component types
    /// 
	__property NAMESPACE_OSGEO_COMMON::GeometryComponentType get_GeometryComponentTypes() [];

    /// \brief
    /// Returns the supported dimensionalities which are based on the bit masks defined in the Dimensionality enum. 
    /// The Z and M bits indicate if the provider supports 3d or Measure dimensions in geometry data. The XY bit should be set too.
    /// 
    /// \return
    /// Returns the dimensionalities
    /// 
	__property System::Int32 get_Dimensionalities();

protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	IGeometryCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIGeometryCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


