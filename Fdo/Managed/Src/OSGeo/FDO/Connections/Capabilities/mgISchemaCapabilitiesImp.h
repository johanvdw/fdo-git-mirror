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

#include "FDO\Connections\Capabilities\mgISchemaCapabilities.h"

class FdoISchemaCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \brief
/// The ISchemaCapbilities interface declares the feature provider's level 
/// of support for the Feature Schema.
private __gc class ISchemaCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                            public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities
{
public:
    /// \brief
    /// Returns an array of the ClassType objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of class types.
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassTypes() [];

    /// \brief
    /// Returns an array of the DataType objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of data types
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataTypes() [];

    /// \brief
    /// Returns the maximum supported length of String, BLOB, or CLOB data
    /// properties. For decimal, it is the combination of the maximum scale
    /// and precision. For other data types that are not variable in size,
    /// the value returned is the byte length.
    ///
    /// \param dataType
    /// The data type for which the information is to retrieved.
    ///
    /// \return
    /// Returns the maximum data value length for the identified data type.
    ///
    __property System::Int64 get_MaximumDataValueLength(NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Returns the maximum supported precision for a decimal data property.
    ///
    /// \return
    /// Returns the maximum supported precision for a decimal data property.
    ///
    __property System::Int32 get_MaximumDecimalPrecision();

    /// \brief
    /// Returns the maximum supported scale for a decimal data property.
    ///
    /// \return
    /// Returns the maximum supported scale for a decimal data property.
    ///
    __property System::Int32 get_MaximumDecimalScale();

    /// \brief
    /// Returns the maximum size of a value of the given type. It includes
    /// limits for the data store name, shema name, class name, property name
    /// and description.
    ///
    /// \param name
    /// The schema element name type identifier for which the information is to
    /// be retrieved. Can be any of the following options: SchemaElementNameType_Datastore,
    /// SchemaElementNameType_Schema, SchemaElementNameType_Class, SchemaElementNameType_Property or 
    /// SchemaElementNameType_Description
    ///
    /// \return
    /// Returns the size limitation for the identified schema element.
    ///
    __property System::Int32 get_NameSizeLimit(NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SchemaElementNameType name);

    /// \brief
    /// Returns a string that includes all the reserved characters that cannot be
    /// used for the various schema element names for the provider.
    ///
    /// \return
    /// Returns a string with the reserved characters for the identified schema element.
    ///
    __property System::String* get_ReservedCharactersForName();

    /// \brief
    /// Returns an array of the DataType objects the feature provider supports for auto-generation.
    /// 
    /// \return
    /// Returns the list of data types
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_SupportedAutoGeneratedTypes() [];

    /// \brief
    /// Returns a list of property types that can be used for identity properties.
    ///
    /// \return
    /// Returns a string with the reserved characters for the identified schema element.
    ///
    __property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_SupportedIdentityPropertyTypes() [];

    /// \brief
    /// Determines if the provider supports association property.
    /// 
    /// \return
    /// Returns true if the provider support the association property.
    /// 
	__property System::Boolean get_SupportsAssociationProperties();

    /// \brief
    /// Determines if the provider can auto-generate unique ID values for a class.
    /// 
    /// \return
    /// Returns true if the provider supports id autogeneration.
    /// 
	__property System::Boolean get_SupportsAutoIdGeneration();

    /// \brief
    /// Determines if the provider supports multiple identity properties per class.
    /// 
    /// \return
    /// Returns true if the provider supports multiple identity properties per class.
    /// 
    __property System::Boolean get_SupportsCompositeId();

    /// \brief
    /// Determines if the provider supports composite unique value constraint.
    /// 
    /// \return
    /// Returns true if the provider supports composite unique value constraint.
    /// 
	__property System::Boolean get_SupportsCompositeUniqueValueConstraints();

    /// \brief
    /// Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.
    /// 
    /// \return
    /// Returns true if the provider supports id autogeneration unique across an datastore.
    /// 
	__property System::Boolean get_SupportsDataStoreScopeUniqueIdGeneration();

    /// \brief
    /// Returns TRUE if default values can be specified for a data property
    /// definition, FALSE otherwise.
    ///
    /// \return
    /// Returns TRUE if default values can be specified for a data property
    /// definition, FALSE otherwise.
    ///
    __property System::Boolean get_SupportsDefaultValue();

    /// \brief
    /// Determines if the provider supports inclusive value range constraints.
    /// 
    /// \return
    /// Returns true if the provider supports inclusive value range constraints.
    /// 
	__property System::Boolean get_SupportsInclusiveValueRangeConstraints();

    /// \brief
    /// Determines if the provider supports exclusive value range constraints.
    /// 
    /// \return
    /// Returns true if the provider supports exclusive value range constraints.
    /// 
	__property System::Boolean get_SupportsExclusiveValueRangeConstraints();

    /// \brief
    /// Determines if the feature provider supports inheritance.
    /// 
    /// \return
    /// Returns true if the feature provider supports inheritance
    /// 
	__property System::Boolean get_SupportsInheritance();

    /// \brief
    /// Determines if the provider supports multiple schemas. Returns false if the provider supports only a single schema.
    /// 
    /// \return
    /// Returns true if the provider supports multiple schemas.
    /// 
	__property System::Boolean get_SupportsMultipleSchemas();

    /// \brief
    /// Determines if the provider supports the network model.
    /// 
    /// \return
    /// Returns true if the provider supports the network model.
    /// 
	__property System::Boolean get_SupportsNetworkModel();

    /// \brief
    /// Determines if the provider supports nullable value constraint.
    /// 
    /// \return
    /// Returns true if the provider supports nullable value constraint.
    /// 
	__property System::Boolean get_SupportsNullValueConstraints();

    /// \brief
    /// Determines if the provider supports object properties. Returns false if the provider supports only data and geometric properties.
    /// 
    /// \return
    /// Returns true if the provider supports object properties.
    /// 
	__property System::Boolean get_SupportsObjectProperties();

    /// \brief
    /// Determines if the provider support update a schema through the ApplySchema command.
    /// If False, then an application can only create a schema once for the provider. They can use the ApplySchema 
    /// command once for a given datastore. Once the schema is defined, it cannot be changed. If True, then the provider 
    /// supports the ability to modify or add to the schema after it is defined initially.
    /// 
    /// \return
    /// Returns true if the provider supports the update of the schema.
    /// 
	__property System::Boolean get_SupportsSchemaModification();

    /// \brief
    /// Determines if the provider supports Schema Mapping Overrides 
    /// (overrides to the default rules for mapping Feature Schemas to provider-specific
    /// physical schemas)
    /// 
    /// \return
    /// Returns true if the provider supports Schema Overrides
    /// 
	__property System::Boolean get_SupportsSchemaOverrides();

    /// \brief
    /// Determines if the provider supports unique value constraint.
    /// 
    /// \return
    /// Returns true if the provider supports unique value constraint.
    /// 
	__property System::Boolean get_SupportsUniqueValueConstraints();

    /// \brief
    /// Determines if the provider supports value constraints list.
    /// 
    /// \return
    /// Returns true if the provider supports value constraints list.
    /// 
	__property System::Boolean get_SupportsValueConstraintsList();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	ISchemaCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoISchemaCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


