#ifndef FDOSMLPGRDPROPERTYMAPPINGSINGLE_H
#define FDOSMLPGRDPROPERTYMAPPINGSINGLE_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/PropertyMappingSingle.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingSingle.h>

// Property Mapping definition for an Object Property. Determines
// that the Object Property is stored in the table for containing class.
class FdoSmLpGrdPropertyMappingSingle : public FdoSmLpPropertyMappingSingle
{
public:
	// Construct a new concrete mapping definition
	FdoSmLpGrdPropertyMappingSingle( 
        FdoSmLpObjectPropertyDefinition* pParent, 
        FdoSmLpClassDefinition* pParentType,
        FdoRdbmsOvPropertyMappingSingle* pOverrides
    );

	// Construct a concrete mapping definition from a base definition.
	FdoSmLpGrdPropertyMappingSingle( 
		const FdoSmLpPropertyMappingSingle* pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
        FdoRdbmsOvPropertyMappingSingle* pOverrides
	);

    // Adds concrete schema overrides to the given object property mapping.
	//
	// Parameters:
    //      propMapping: the object property mapping.
	//		bIncludeDefaults - 
    //          true: include all schema mappings.
    //          false: include only non-default mappings.
    // Returns:
    //      true if any overrides were added
    virtual bool AddSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const;
    
protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdPropertyMappingSingle() {}
private:
    void SetupOverrides( 
    	const FdoSmLpPropertyMappingSingle* pBase,
	    FdoSmLpObjectPropertyDefinition* pParent, 
        FdoRdbmsOvPropertyMappingSingle* pOverrides,
        bool bInherit
    );
};

#endif

