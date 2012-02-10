#ifndef FDOSMLPMYSQLCLASSDEFINITION_H
#define FDOSMLPMYSQLCLASSDEFINITION_H		1
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

#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>
#include <Fdo/Commands/Schema/PhysicalClassMapping.h>
#include "../Ph/Table.h"


class FdoSmLpMySqlClassDefinition : public FdoSmLpGrdClassDefinition
{
public:
    virtual ~FdoSmLpMySqlClassDefinition(void);

	//Update from class overrides
    //
    // Parameters:
    //      pFdoClass: the FDO class.
    //      elementState: the modification state for this class definition.
    //      pClassOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.

    virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );


	// Copy overrides from this class to given table:
	void GetOverridesFromClass(FdoSmPhMySqlTable* phTable);

	// Simple accessor methods for override info:
	FdoStringP                GetAutoIncrementPropertyName() const { return mAutoIncrementPropertyName; };
	FdoInt64                  GetAutoIncrementSeed() const { return mAutoIncrementSeed; };
	FdoStringP                GetTableDataDirectory() const { return mTableDataDirectory; };
	FdoStringP                GetTableIndexDirectory() const { return mTableIndexDirectory; };
	MySQLOvStorageEngineType  GetTableStorageEngine() const { return mTableStorageEngine; };

protected:
    // Constructs an instance of a ClassDefinition from a class reader
    FdoSmLpMySqlClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

    // Constructs an instance of a ClassDefinition from an FDO element
    // and Fdo to physical mapping overrides.
    //
    // Parameters:
	//		pFdoClass: the FDO class.
    //      bIgnoreStates: true if the element state on the FDO class is 
    //          to be ignored.
	//		parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpMySqlClassDefinition(
        FdoClassDefinition* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

	// Constructs an instance of a ClassDefinition from the given attributes.
	//
	// Parameters:
	//		name: class name
	//		description: class description
	//		parent: class objects parent ( usually an FdoSmLpSchema object )
	//		pSrcClass: When this is an object property class, pSrcClass represents the 
	//			object property class for the associated object property's base property.
	//		tableMapping: property to table mapping type
	//		tableName: class table name
	//		table: class table. Null if class has no table or table
	//			does not exist.

	FdoSmLpMySqlClassDefinition(
        FdoString* name,
        FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> pSrcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState elementState
    );

    // Add the physical mappings for this class to the given schema override set
    virtual bool AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const;

    // Generate the physical mappings for this class
    virtual FdoPhysicalClassMappingP GetSchemaMappings( bool bIncludeDefaults ) const;

    // Sets the MySQL-specific physical mappings.
    virtual bool SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const;

    // Get a writer for adding this class to the MetaSchema
    virtual void SetPhysicalAddWriter( FdoSmPhClassWriterP writer );

    // Common initializer for all constructors:
    void Init();

    virtual FdoSmPhDbObjectP NewTable(FdoString* tableName, FdoString* pkeyName);

	virtual void PostFinalize();

private:
    FdoStringP                  mAutoIncrementPropertyName;
    FdoInt64                    mAutoIncrementSeed;
    FdoStringP                  mTableDataDirectory;
    FdoStringP                  mTableIndexDirectory;
    MySQLOvStorageEngineType    mTableStorageEngine;
};

typedef FdoPtr<FdoSmLpMySqlClassDefinition> FdoSmLpMySqlClassDefinitionP;

#endif
