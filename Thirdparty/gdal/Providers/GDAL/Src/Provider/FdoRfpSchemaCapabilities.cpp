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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSchemaCapabilities.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpSchemaCapabilities.h"

FdoRfpSchemaCapabilities::FdoRfpSchemaCapabilities()
{
}

FdoRfpSchemaCapabilities::~FdoRfpSchemaCapabilities()
{
}

FdoClassType* FdoRfpSchemaCapabilities::GetClassTypes(int& length)
{
	static FdoClassType classType[] = { FdoClassType_FeatureClass};
	length = sizeof(classType)/sizeof(FdoClassType);
	return classType;
}

FdoDataType* FdoRfpSchemaCapabilities::GetDataTypes(int& length)
{
	// TODO: fix this later according to the definition of 'FdoDataType'.
	static FdoDataType dataTypes[] = {	FdoDataType_String,
										FdoDataType_BLOB };
	length = sizeof(dataTypes)/sizeof(FdoDataType);
	return dataTypes;
}

bool FdoRfpSchemaCapabilities::SupportsInheritance()
{
	return true;
}

bool FdoRfpSchemaCapabilities::SupportsMultipleSchemas()
{
	return true;
}

bool FdoRfpSchemaCapabilities::SupportsObjectProperties()
{
	return false;
}

bool FdoRfpSchemaCapabilities::SupportsSchemaOverrides()
{
	return true;
}

bool FdoRfpSchemaCapabilities::SupportsAssociationProperties()
{
	return false;
}

bool FdoRfpSchemaCapabilities::SupportsNetworkModel()
{
	return false;
}

void FdoRfpSchemaCapabilities::Dispose()
{
	delete this;
}

/// <summary>Determines if the provider can auto-generate unique ID values for a class.</summary>
/// <returns>Returns true if the provider supports id autogeneration.</returns> 
bool FdoRfpSchemaCapabilities::SupportsAutoIdGeneration()
{
	return false;
}

/// <summary>Determines if the provider can auto-generate unique ID values that are unique for the entire datastore, rather than just for a particular class.</summary>
/// <returns>Returns true if the provider supports id autogeneration unique across an datastore.</returns> 
bool FdoRfpSchemaCapabilities::SupportsDataStoreScopeUniqueIdGeneration()
{
	return false;
}

/// <summary>Returns an array of the FdoDataType objects the feature provider supports for auto-generation.</summary>
/// <param name="length">Output the number of data types.</param> 
/// <returns>Returns the list of data types</returns> 
FdoDataType* FdoRfpSchemaCapabilities::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
	length = 0;
	return NULL;
}

bool FdoRfpSchemaCapabilities::SupportsSchemaModification()
{
    return false;
}

/// <summary>Determines if the provider supports inclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports inclusive value range constraints.</returns> 
bool FdoRfpSchemaCapabilities::SupportsInclusiveValueRangeConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports exclusive value range constraints.</summary>
/// <returns>Returns true if the provider supports exclusive value range constraints.</returns>
bool FdoRfpSchemaCapabilities::SupportsExclusiveValueRangeConstraints()
{
    return false;
}
/// <summary>Determines if the provider supports value constraints list.</summary>
/// <returns>Returns true if the provider supports value constraints list.</returns>
bool FdoRfpSchemaCapabilities::SupportsValueConstraintsList()
{
    return false;
}

/// <summary>Determines if the provider supports nullable value constraint.</summary>
/// <returns>Returns true if the provider supports nullable value constraint.</returns>
bool FdoRfpSchemaCapabilities::SupportsNullValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports unique value constraint.</summary>
/// <returns>Returns true if the provider supports unique value constraint.</returns>
bool FdoRfpSchemaCapabilities::SupportsUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports composite unique value constraint.</summary>
/// <returns>Returns true if the provider supports composite unique value constraint.</returns>
bool FdoRfpSchemaCapabilities::SupportsCompositeUniqueValueConstraints()
{
    return false;
}

/// <summary>Determines if the provider supports multiple identity properties per class.
/// <returns>Returns true if the provider supports multiple identity properties per class.
bool FdoRfpSchemaCapabilities::SupportsCompositeId()
{
    return false;
}

FdoInt64 FdoRfpSchemaCapabilities::GetMaximumDataValueLength(FdoDataType dataType)
{
    return (FdoInt64)-1;
}

FdoInt32 FdoRfpSchemaCapabilities::GetMaximumDecimalPrecision()
{
    return (FdoInt32)-1;
}

FdoInt32 FdoRfpSchemaCapabilities::GetMaximumDecimalScale()
{
    return (FdoInt32)-1;
}

FdoInt32 FdoRfpSchemaCapabilities::GetNameSizeLimit(FdoSchemaElementNameType name)
{
    return (FdoInt32)-1;
}

FdoString* FdoRfpSchemaCapabilities::GetReservedCharactersForName()
{
    return NULL;
}

FdoDataType* FdoRfpSchemaCapabilities::GetSupportedIdentityPropertyTypes(FdoInt32& length)
{
	static FdoDataType supportedIdentityTypes[] = {FdoDataType_String};
	length = sizeof(supportedIdentityTypes)/sizeof(FdoDataType);
    return supportedIdentityTypes;
}

bool FdoRfpSchemaCapabilities::SupportsDefaultValue()
{
    return false;
}

