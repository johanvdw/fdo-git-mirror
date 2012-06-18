//
// Copyright (C) 2006 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDORDBMSPOSTGISSCHEMACAPABILITIES_H
#define FDORDBMSPOSTGISSCHEMACAPABILITIES_H

#include "../../Src/Fdo/Capability/FdoRdbmsSchemaCapabilities.h"

/// \brief
/// Implements interface to query Feature Schema capabilities
/// for PostGIS provider.
///
/// \sa FdoISchemaCapabilities - declares interface implemented by
/// this class.
///
class FdoRdbmsPostGisSchemaCapabilities :
    public FdoRdbmsSchemaCapabilities
{
public:

    /// Default constructor.
    FdoRdbmsPostGisSchemaCapabilities();

	/// Determines if the provider supports inclusive value
    /// range constraints.
	///
    /// @return true if inclusive value range constraints are supported,
    /// false otherwise. 
    ///
	virtual bool SupportsInclusiveValueRangeConstraints();

	/// Determines if the provider supports exclusive value
    /// range constraints.
    ///
	/// @return true if exclusive value range constraints are supported,
    /// false otherwise.
    ///
	virtual bool SupportsExclusiveValueRangeConstraints();

    /// <summary>Determines if the provider supports Schema Mapping Overrides (overrides to the default
    /// rules for mapping Feature Schemas to provider-specific physical schemas).</summary>
	/// <returns>Returns true if the provider supports Schema Overrides.</returns> 
    virtual bool SupportsSchemaOverrides();

    /// <summary>Determines if the provider supports object properties. Returns false if the provider
    /// supports only data and geometric properties.</summary>
	/// <returns>Returns true if the provider supports object properties.</returns> 
    virtual bool SupportsObjectProperties();

    /// <summary>Determines if the provider supports association property.</summary>
	/// <returns>Returns true if the provider support the association property.</returns>
    virtual bool SupportsAssociationProperties();

	/// Determines if the provider supports value constraints list.
	/// 
    /// @return true if value constraints list is supported,
    /// false otherwise.
    ///
	virtual bool SupportsValueConstraintsList();

    /// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
    /// <param name="length">Output the number of data types.</param>
	/// <returns>Returns the list of data types.</returns>
    virtual FdoDataType* GetSupportedAutoGeneratedTypes(FdoInt32& length);
};

#endif // FDORDBMSMYSQLSCHEMACAPABILITIES_H
