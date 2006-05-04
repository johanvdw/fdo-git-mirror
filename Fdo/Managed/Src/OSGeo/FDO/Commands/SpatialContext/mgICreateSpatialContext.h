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

#include "FDO\Commands\mgICommand.h"
#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

/// \brief
/// The FdoICreateSpatialContext interface defines the CreateSpatialContext
/// command, which creates a new spatial context. Input to the command includes
/// the name, description, coordinate system, extent type, and extent for the
/// new context. Command execution will fail if the context already exists, or
/// if the coordinate system or extent type is not supported by the provider.
/// <p><b>Note:</b> The geographic extent of a coordinate system should generally be
/// defined as an envelope (or bounding box). However, any geometry type that
/// falls into the surface geometric types can be used (e.g., polygons). If a
/// non-rectangular area is passed in and the provider supports only a
/// rectangular extent, the provider will compute the minimum box that
/// encloses the given geometry and use that for the extent value.
public __gc __interface ICreateSpatialContext : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the context to create as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the context to create as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the description of the context to create as a string.
    /// 
    /// \return
    /// Returns the description of the spatial context
    /// 
	__property System::String* get_Description();

    /// \brief
    /// Sets the description of the context to create as a string.
    /// 
    /// \param value 
    /// Input the description of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Description(System::String* value);

    /// \brief
    /// Gets the coordinate system of the context to create as a string
    /// in OpenGIS SRS WKT format.
    /// 
    /// \return
    /// Returns the coordinate system
    /// 
	__property System::String* get_CoordinateSystem();

    /// \brief
    /// Sets the coordinate system of the context to create as a string in
    /// OpenGIS SRS WKT format.
    /// 
    /// \param value 
    /// Input the coordinate system
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CoordinateSystem(System::String* value);

    /// \brief
    /// Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.
    /// 
    /// \return
    /// Returns the coordinate system description in WKT of of the spatial context.
    /// 
    __property System::String* get_CoordinateSystemWkt();

    /// \brief
    /// Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.
    /// 
    /// \param value 
    /// Input the coordinate system WKT
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_CoordinateSystemWkt(System::String* value);

    /// \brief
    /// Gets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \return
    /// Returns the extent type
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType get_ExtentType();

    /// \brief
    /// Sets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \param value 
    /// Input the extent type
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ExtentType(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value);

    /// \brief
    /// Gets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic, any value specified via this function is ignored.
    /// 
    /// \return
    /// Returns the extent of the spatial context
    /// 
	__property System::Byte get_Extent() [];

    /// \brief
    /// Sets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.
    /// 
    /// \param value 
    /// Input the extent of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Extent(System::Byte buffer[]);

    /// \brief
    /// Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	__property System::Double get_XYTolerance();

    /// \brief
    /// Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_XYTolerance(System::Double value);

    /// \brief
    /// Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	__property System::Double get_ZTolerance();
    /// \brief
    /// Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ZTolerance(System::Double value);

    /// \brief
    /// Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \return
    /// Returns true if command should update existing contexts
    /// 
	__property System::Boolean get_UpdateExisting();

    /// \brief
    /// Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \param value 
    /// Input true if existing contexts should be updated
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_UpdateExisting(System::Boolean value);

    /// \brief
    /// Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT


