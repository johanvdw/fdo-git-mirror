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
 */

#ifndef SHPSCHEMACAPABILITIES_H
#define SHPSCHEMACAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class ShpSchemaCapabilities : public FdoISchemaCapabilities
{
public:
    ShpSchemaCapabilities ();

protected:
    virtual ~ShpSchemaCapabilities ();

protected:
    virtual void Dispose ();

public:
    /// <summary>Returns an array of the FdoClassType objects the feature provider supports.</summary>
    /// <param name="length">Output the number of class types.</param> 
    /// <returns>Returns the list of class types.</returns> 
    FdoClassType* GetClassTypes (FdoInt32& length);

    /// <summary>Returns an array of the FdoDataType objects the feature provider supports.</summary>
    /// <param name="length">Output the number of data types.</param> 
    /// <returns>Returns the list of data types</returns> 
    FdoDataType* GetDataTypes (FdoInt32& length);

    /// <summary>Determines if the feature provider supports inheritance.</summary>
    /// <returns>Returns true if the feature provider supports inheritance</returns> 
    bool SupportsInheritance ();

    /// <summary>Determines if the provider supports multiple schemas. Returns false if the provider supports only a single schema.</summary>
    /// <returns>Returns true if the provider supports multiple schemas.</returns> 
    bool SupportsMultipleSchemas ();

    /// <summary>Determines if the provider supports object properties. Returns false if the provider supports only data and geometric properties.</summary>
    /// <returns>Returns true if the provider supports object properties.</returns> 
    bool SupportsObjectProperties ();

    /// <summary>Determines if the provider supports Schema Mapping Overrides 
    /// (overrides to the default rules for mapping Feature Schemas to provider-specific
    /// physical schemas)</summary>
    /// <returns>Returns true if the provider supports Schema Overrides</returns> 
    bool SupportsSchemaOverrides();

    /// <summary>Determines if the provider supports association property.</summary>
    /// <returns>Returns true if the provider support the association property.</returns> 
    bool SupportsAssociationProperties(void);

    /// <summary>Determines if the provider supports the network model.</summary>
    /// <returns>Returns true if the provider supports the network model.</returns> 
    bool SupportsNetworkModel(void);

    /// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
    /// <returns>Returns true if the provider supports id autogeneration.</returns> 
    bool SupportsAutoIdGeneration();
    
    /// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
    /// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
    bool SupportsDataStoreScopeUniqueIdGeneration();

    /// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
    /// <param name="length">Output the number of data types.</param> 
    /// <returns>Returns the list of data types</returns> 
    FdoDataType* GetSupportedAutoGeneratedTypes(FdoInt32& length);

    /// <summary>Determines if the provider support update a schema through the ApplySchema command.
    /// If False, then an application can only create a schema once for the provider. They can use the ApplySchema 
    /// command once for a given datastore. Once the schema is defined, it cannot be changed. If True, then the provider 
    /// supports the ability to modify or add to the schema after it is defined initially.</summary>
    /// <returns>Returns true if the provider supports the update of the schema.</returns> 
    virtual bool  SupportsSchemaModification();

	/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
	virtual bool SupportsInclusiveValueRangeConstraints();

	/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
	/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
	virtual bool SupportsExclusiveValueRangeConstraints();

	/// <summary>Determines if the provider supports value constraints list.</summary>
	/// <returns>Returns true if the provider supports value constraints list.</returns>
	virtual bool SupportsValueConstraintsList();

	/// <summary>Determines if the provider supports nullable value constraint.</summary>
	/// <returns>Returns true if the provider supports nullable value constraint.</returns>
	virtual bool SupportsNullValueConstraints();

	/// <summary>Determines if the provider supports unique value constraint.</summary>
	/// <returns>Returns true if the provider supports unique value constraint.</returns>
	virtual bool SupportsUniqueValueConstraints();

	/// <summary>Determines if the provider supports composite unique value constraint.</summary>
	/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
	virtual bool SupportsCompositeUniqueValueConstraints();

    /// <summary>Determines if the provider supports multiple identity properties per class.</summary>
    /// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
    virtual bool SupportsCompositeId();

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
    virtual FdoInt64 GetMaximumDataValueLength(FdoDataType dataType);

    /// \brief
    /// Returns the maximum supported precision for a decimal data property.
    ///
    /// \param
    /// None.
    ///
    /// \return
    /// Returns the maximum supported precision for a decimal data property.
    ///
    virtual FdoInt32 GetMaximumDecimalPrecision();

    /// \brief
    /// Returns the maximum supported scale for a decimal data property.
    ///
    /// \param
    /// None.
    ///
    /// \return
    /// Returns the maximum supported scale for a decimal data property.
    ///
    virtual FdoInt32 GetMaximumDecimalScale();

    /// \brief
    /// Returns the maximum size of a value of the given type. It includes
    /// limits for the data store name, shema name, class name, property name
    /// and description.
    ///
    /// \param name
    /// The schema element name type identifier for which the information is to
    /// be retrieved. Can be any of the following options: FdoSchemaElementNameType_Datastore,
    /// FdoSchemaElementNameType_Schema, FdoSchemaElementNameType_Class, FdoSchemaElementNameType_Property or 
    /// FdoSchemaElementNameType_Description
    ///
    /// \return
    /// Returns the size limitation for the identified schema element.
    ///
    virtual FdoInt32 GetNameSizeLimit(FdoSchemaElementNameType name);

    /// \brief
    /// Returns a string that includes all the reserved characters that cannot be
    /// used for the various schema element names for the provider.
    ///
    /// \param
    /// None
    ///
    /// \return
    /// Returns a string with the reserved characters for the identified schema element.
    ///
    virtual FdoString* GetReservedCharactersForName();

    /// \brief
    /// Returns a list of property types that can be used for identity properties.
    ///
    /// \param length
    /// Output the number of data types.
    ///
    /// \return
    /// Returns a string with the reserved characters for the identified schema element.
    ///
    virtual FdoDataType* GetSupportedIdentityPropertyTypes(FdoInt32& length);

    /// \brief
    /// Returns TRUE if default values can be specified for a data property
    /// definition, FALSE otherwise.
    ///
    /// \param
    /// None
    ///
    /// \return
    /// Returns TRUE if default values can be specified for a data property
    /// definition, FALSE otherwise.
    ///
    virtual bool SupportsDefaultValue();
};

#endif // SHPSCHEMACAPABILITIES_H

