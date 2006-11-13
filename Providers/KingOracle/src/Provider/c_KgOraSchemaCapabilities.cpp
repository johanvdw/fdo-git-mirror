/*
* Copyright (C) 2006  SL-King d.o.o
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

#include "stdafx.h"

c_KgOraSchemaCapabilities::c_KgOraSchemaCapabilities ()
{
}

c_KgOraSchemaCapabilities::~c_KgOraSchemaCapabilities ()
{
}

void c_KgOraSchemaCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of the FdoClassType objects the feature provider supports.</summary>
/// <param name="length">Output the number of class types.</param> 
/// <returns>Returns the list of class types.</returns> 
FdoClassType* c_KgOraSchemaCapabilities::GetClassTypes (FdoInt32& Length)
{
    static FdoClassType classes[] =
    {
        FdoClassType_FeatureClass,
        FdoClassType_Class
    };

    Length = sizeof (classes) / sizeof (FdoClassType);

    return (classes);
}//end of c_KgOraSchemaCapabilities::GetClassTypes

/// <summary>Returns an array of the FdoDataType objects the feature provider supports.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* c_KgOraSchemaCapabilities::GetDataTypes (FdoInt32& Length)
{
    static FdoDataType types[] =
    {
        FdoDataType_Boolean,
        FdoDataType_Byte,
        FdoDataType_DateTime,
        FdoDataType_Decimal,
        FdoDataType_Double,
        FdoDataType_Int16,
        FdoDataType_Int32,
        FdoDataType_Int64,
        FdoDataType_Single,
        FdoDataType_String,
    };

    Length = sizeof (types) / sizeof (FdoDataType);

    return (types);
}//end of c_KgOraSchemaCapabilities::GetDataTypes


/// <summary>Determines if the feature provider supports inheritance.</summary>
/// <returns>Returns true if the feature provider supports inheritance</returns> 
bool c_KgOraSchemaCapabilities::SupportsInheritance ()
{
    return false;
}


/// <summary>Determines if the provider supports multiple schemas. Returns false if the provider supports only a single schema.</summary>
/// <returns>Returns true if the provider supports multiple schemas.</returns> 
bool c_KgOraSchemaCapabilities::SupportsMultipleSchemas ()
{
    return true;
}


/// <summary>Determines if the provider supports object properties. Returns false if the provider supports only data and geometric properties.</summary>
/// <returns>Returns true if the provider supports object properties.</returns> 
bool c_KgOraSchemaCapabilities::SupportsObjectProperties ()
{
    return false;
}


/// <summary>Determines if the provider supports Schema Mapping Overrides 
/// (overrides to the default rules for mapping Feature Schemas to provider-specific
/// physical schemas)</summary>
/// <returns>Returns true if the provider supports Schema Overrides</returns> 
bool c_KgOraSchemaCapabilities::SupportsSchemaOverrides ()
{
    return true;
}


/// <summary>Determines if the provider supports association property.</summary>
/// <returns>Returns true if the provider support the association property.</returns> 
bool c_KgOraSchemaCapabilities::SupportsAssociationProperties (void)
{
    return false;
}

/// <summary>Determines if the provider supports the network model.</summary>
/// <returns>Returns true if the provider supports the network model.</returns> 
bool c_KgOraSchemaCapabilities::SupportsNetworkModel (void)
{
    return false;
}

/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns> 
bool c_KgOraSchemaCapabilities::SupportsAutoIdGeneration ()
{
    return true;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
bool c_KgOraSchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration ()
{
    return false;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* c_KgOraSchemaCapabilities::GetSupportedAutoGeneratedTypes (FdoInt32& Length)
{
    static FdoDataType autogen_types[] =
    {
        FdoDataType_Int32,
    };

    Length = sizeof (autogen_types) / sizeof (FdoDataType);

    return (autogen_types);
}

/// <summary>Determines if the provider support update a schema through the ApplySchema command.
/// If False, then an application can only create a schema once for the provider.
/// They can use the ApplySchema command once for a given datastore.
/// Once the schema is defined, it cannot be changed.
/// If True, then the provider supports the ability to modify or add to the schema after it is defined initially.</summary>
/// <returns>Returns true if the provider supports the update of the schema.</returns>
bool c_KgOraSchemaCapabilities::SupportsSchemaModification ()
{
    return true;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool c_KgOraSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool c_KgOraSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return false;
}
/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool c_KgOraSchemaCapabilities::SupportsValueConstraintsList()
{
    return false;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool c_KgOraSchemaCapabilities::SupportsNullValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool c_KgOraSchemaCapabilities::SupportsUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool c_KgOraSchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports multiple identity properties per class.</summary>
/// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
bool c_KgOraSchemaCapabilities::SupportsCompositeId()
{
    return false;
}
