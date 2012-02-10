// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfSchemaCapabilities.h"


//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// default constructor
SdfSchemaCapabilities::SdfSchemaCapabilities()
{
}


// default destructor
SdfSchemaCapabilities::~SdfSchemaCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfSchemaCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoISchemaCapabilities implementation
//-------------------------------------------------------


// Returns an array of the ClassTypes the feature provider supports.
// The length parameter gives the number of class types in the array.
FdoClassType* SdfSchemaCapabilities::GetClassTypes(FdoInt32& length)
{
    //SDF supports class with and without geometry
    static FdoClassType classTypes[] =
    {
        FdoClassType_Class,
        FdoClassType_FeatureClass
    };

    length = sizeof(classTypes) / sizeof(FdoClassType);
    return classTypes;
}


// Returns an array of the DataTypes the feature provider supports.
// The length parameter gives the number of data types in the array.
FdoDataType* SdfSchemaCapabilities::GetDataTypes(FdoInt32& length)
{
    //TODO: need to implement some of these in the BinaryReader/Writer
    static FdoDataType dataTypes[] =
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
//      FdoDataType_BLOB,
//      FdoDataType_CLOB
    };

    length = sizeof(dataTypes) / sizeof(FdoDataType);
    return dataTypes;
}


// Returns true if the feature provider supports inheritance.
bool SdfSchemaCapabilities::SupportsInheritance()
{
    // SDF supports inheritance
    return true;
}


// Returns true if the provider supports multiple schemas, and false if
// if only supports a single schema.
bool SdfSchemaCapabilities::SupportsMultipleSchemas()
{
    // each SDF dataset defines only one schema
    return false;
}


// Returns true if the provider supports object properties, and false if
// it only supports data and geometric properties.
bool SdfSchemaCapabilities::SupportsObjectProperties()
{
    // no object properties support for SDF
    return false;
}


/// <summary>Determines if the provider supports association property.</summary>
/// <returns>Returns true if the provider support the association property.</returns> 
bool SdfSchemaCapabilities::SupportsAssociationProperties()
{
    return true;
}

/// <summary>Determines if the provider supports Schema Mapping Overrides 
/// (overrides to the default rules for mapping Feature Schemas to provider-specific
/// physical schemas)</summary>
/// <returns>Returns true if the provider supports Schema Overrides</returns> 
bool SdfSchemaCapabilities::SupportsSchemaOverrides()
{
    return false;
}

/// <summary>Determines if the provider supports the network model.</summary>
/// <returns>Returns true if the provider supports the network model.</returns> 
bool SdfSchemaCapabilities::SupportsNetworkModel()
{
    return false;
}


/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns> 
bool SdfSchemaCapabilities::SupportsAutoIdGeneration()
{
    return true;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
bool SdfSchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
    //SDF ids are unique per feature class.
    return false;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* SdfSchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    static FdoDataType dataTypes[] =
    {
//        FdoDataType_Boolean,
//        FdoDataType_Byte,
//        FdoDataType_DateTime,
//        FdoDataType_Decimal,
//        FdoDataType_Double,
//        FdoDataType_Int16,
        FdoDataType_Int32,
//        FdoDataType_Int64,
//        FdoDataType_Single,
//        FdoDataType_String,
//        FdoDataType_BLOB,
//        FdoDataType_CLOB
    };

    length = sizeof(dataTypes) / sizeof(FdoDataType);
    return dataTypes;
}

bool SdfSchemaCapabilities::SupportsSchemaModification()
{
    return true;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool SdfSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool SdfSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return true;
}
/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool SdfSchemaCapabilities::SupportsValueConstraintsList()
{
    return true;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool SdfSchemaCapabilities::SupportsNullValueConstraints()
{
    return true;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool SdfSchemaCapabilities::SupportsUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool SdfSchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports multiple identity properties per class.</summary>
/// <returns>Returns true if the provider supports multiple identity properties per class.</returns>
bool SdfSchemaCapabilities::SupportsCompositeId()
{
    return true;
}


FdoInt32 SdfSchemaCapabilities::GetNameSizeLimit (FdoSchemaElementNameType nameType)
{
    switch (nameType)
    {
        case FdoSchemaElementNameType_Datastore:   return 255;  // the max filename length on linux/windows
        case FdoSchemaElementNameType_Schema:      return -1;
        case FdoSchemaElementNameType_Class:       return -1;
        case FdoSchemaElementNameType_Property:    return -1;
        case FdoSchemaElementNameType_Description: return -1;
    }
    return -1;
}

FdoString* SdfSchemaCapabilities::GetReservedCharactersForName()
{
    return L".:";
}

FdoInt64 SdfSchemaCapabilities::GetMaximumDataValueLength (FdoDataType dataType)
{
    switch (dataType)
    {
        case FdoDataType_String:   return (FdoInt64)-1;
        case FdoDataType_BLOB:     return (FdoInt64)-1;
        case FdoDataType_CLOB:     return (FdoInt64)-1;
        case FdoDataType_Decimal:  return (FdoInt64)-1;
        case FdoDataType_Boolean:  return (FdoInt64)sizeof(FdoBoolean);
        case FdoDataType_Byte:     return (FdoInt64)sizeof(FdoByte);
        case FdoDataType_DateTime: return (FdoInt64)sizeof(FdoDateTime);
        case FdoDataType_Double:   return (FdoInt64)sizeof(FdoDouble);
        case FdoDataType_Int16:    return (FdoInt64)sizeof(FdoInt16);
        case FdoDataType_Int32:    return (FdoInt64)sizeof(FdoInt32);
        case FdoDataType_Int64:    return (FdoInt64)sizeof(FdoInt64);
        case FdoDataType_Single:   return (FdoInt64)sizeof(FdoFloat);
    }
    return (FdoInt64)-1;
}

FdoInt32 SdfSchemaCapabilities::GetMaximumDecimalPrecision()
{
    return -1;
}

FdoInt32 SdfSchemaCapabilities::GetMaximumDecimalScale()
{
    return -1;
}

FdoDataType* SdfSchemaCapabilities::GetSupportedIdentityPropertyTypes(FdoInt32& length)
{
    length = 0;
    static FdoDataType supportedIdentityTypes[15];
    supportedIdentityTypes[length++] = FdoDataType_Boolean;
    supportedIdentityTypes[length++] = FdoDataType_Byte;
    supportedIdentityTypes[length++] = FdoDataType_DateTime;
    supportedIdentityTypes[length++] = FdoDataType_Decimal;
    supportedIdentityTypes[length++] = FdoDataType_Double;
    supportedIdentityTypes[length++] = FdoDataType_Int16;
    supportedIdentityTypes[length++] = FdoDataType_Int32;
    supportedIdentityTypes[length++] = FdoDataType_Int64;
    supportedIdentityTypes[length++] = FdoDataType_Single;
    supportedIdentityTypes[length++] = FdoDataType_String;

    return supportedIdentityTypes;
}

bool SdfSchemaCapabilities::SupportsDefaultValue()
{
    return false;
}
