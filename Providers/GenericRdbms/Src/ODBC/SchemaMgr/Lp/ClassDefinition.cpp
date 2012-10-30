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

#include "stdafx.h"
#include "ClassDefinition.h"
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"
#include "Schema.h"
#include <Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h>
#include "../Ph/Mgr.h"
#include "../Ph/Table.h"
#include "DataPropertyDefinition.h"
#include "Sm/Lp/DataPropertyDefinitionCollection.h"


FdoSmLpOdbcClassDefinition::FdoSmLpOdbcClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpGrdClassDefinition(classReader, parent),
	FdoSmLpClassBase(classReader, parent)
{
    Init();
}

FdoSmLpOdbcClassDefinition::FdoSmLpOdbcClassDefinition(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
	FdoSmLpGrdClassDefinition(pFdoClass, bIgnoreStates, parent),
	FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
 	FdoSmLpSchemaP         pSchema = GetLogicalPhysicalSchema();

    this->SetOwner(pSchema->GetOwner());

    Init();
}

FdoSmLpOdbcClassDefinition::FdoSmLpOdbcClassDefinition(
	FdoString* name, 
	FdoString* description,
	FdoSmLpSchemaElement* parent,
	FdoSmLpClassDefinitionP pSrcClass,
	FdoSmOvTableMappingType tableMapping,
	FdoString* tableName,
	FdoSmPhDbObjectP table,
	FdoSchemaElementState elementState
) : 
	FdoSmLpGrdClassDefinition(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState),
	FdoSmLpClassBase(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState, parent->GetIsFromFdo())
{
    Init();
}


FdoSmLpOdbcClassDefinition::~FdoSmLpOdbcClassDefinition(void)
{
}

bool FdoSmLpOdbcClassDefinition::AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoOdbcOvPhysicalSchemaMapping* odbcSchemaMapping = (FdoOdbcOvPhysicalSchemaMapping*)(FdoPhysicalSchemaMapping*) schemaMapping;

    // Get the physical mappings for this class
    FdoPhysicalClassMappingP classMapping = GetSchemaMappings( bIncludeDefaults );
    FdoOdbcOvClassDefinition* odbcClassMapping = 
        dynamic_cast<FdoOdbcOvClassDefinition*>(classMapping.p);

    // Add them to the schema override set.
    if ( odbcClassMapping ) {
        FdoOdbcOvClassesP(odbcSchemaMapping->GetClasses())->Add( odbcClassMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalClassMappingP FdoSmLpOdbcClassDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoOdbcOvClassDefinition*     classMapping = FdoOdbcOvClassDefinition::Create( GetName() );
    FdoPhysicalClassMappingP        retClassMapping = (FdoOdbcOvClassDefinition*) classMapping;

    // Set class type specific mappings.
    bool bHasMappings = SetSchemaMappings( retClassMapping, bIncludeDefaults );

    // return NULL if there are no mappings. This prevents generating empty objects.
    if ( !bHasMappings ) 
        retClassMapping = NULL;

    return retClassMapping;
    
}
bool FdoSmLpOdbcClassDefinition::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    ((FdoSmLpOdbcClassDefinition*)this)->Finalize();

    const FdoSmLpOdbcSchema*         pSchema = (const FdoSmLpOdbcSchema*) RefLogicalPhysicalSchema();
    FdoOdbcOvClassDefinition*      odbcClassMapping = dynamic_cast<FdoOdbcOvClassDefinition*>( (FdoPhysicalClassMapping*) classMapping );

    // Default the table mapping to the one for the Feature Schema.
    FdoSmOvTableMappingType      defaultTableMappingType = pSchema->GetTableMapping();
    FdoSmOvTableMappingType      tableMappingType = GetTableMapping();
    FdoStringP                      database = GetDatabase();
    FdoStringP                      owner = GetOwner();
    FdoSmLpDbObjectP                dbObject = ((FdoSmLpOdbcClassDefinition*)this)->GetDbObject();
    bool                            bHasMappings = FdoSmLpGrdClassDefinition::SetSchemaMappings( FDO_SAFE_ADDREF(odbcClassMapping), bIncludeDefaults );

    // Set the table physical mappings of including default mappings or there are any
    // non-default schema mappings.
    if ( bIncludeDefaults || 
         ((tableMappingType != FdoSmOvTableMappingType_BaseTable) && GetIsFixedDbObject())
    ) {
        FdoSmPhOdbcMgrP pPhysical = ((FdoSmLpSchema*) RefLogicalPhysicalSchema())->GetPhysicalSchema()->SmartCast<FdoSmPhOdbcMgr>();

        // Include owner if not default owner and data source supports qualified table names.
        FdoStringP dbObjectName = pPhysical->CanQualifyTableNameOverride() ? GetDbObjectQName() : GetDbObjectName();

        // For foreign schemas override table name is the root (foreign) table.
        FdoOdbcOvTableP tableMapping = FdoOdbcOvTable::Create( 
            wcslen(GetRootDbObjectName()) > 0 ?
                GetRootDbObjectName() :
                dbObjectName   
        );
        
        if ( dbObject ) {
            FdoSmPhDbObjectP phDbObject = dbObject->GetDbObject();
            FdoSmPhTableP phTable = phDbObject->SmartCast<FdoSmPhTable>();

            // Get generic mappings:

            if ( phTable ) 
                tableMapping->SetPKeyName( phTable->GetPkeyName() );
        }

        odbcClassMapping->SetTable(tableMapping);
        bHasMappings = true;
    }

    return bHasMappings;
}


void FdoSmLpOdbcClassDefinition::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoStringP ovTableName;

    // Do updates from generic physical mappings
    FdoSmLpGrdClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Extract the class and class table Physical Mapping overrides.

    FdoSmLpSchemaP pSchema = GetLogicalPhysicalSchema();

    // Extract any class overrides
    FdoOdbcOvClassDefinition* pOdbcOverrides = dynamic_cast<FdoOdbcOvClassDefinition*>(pClassOverrides);
    FdoOdbcOvTableP tableOverrides = pOdbcOverrides ? pOdbcOverrides->GetTable() : NULL;

    // Handle generic table overrides
    UpdateTable( 
        L"",
        L"",
        tableOverrides 
    );

    // Table name can be overridden to a table in an owner other than the default one. 
    // In this case the format is <owner>.<table>. Set this class's physical owner in this case.

    FdoSmPhOdbcMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema()->SmartCast<FdoSmPhOdbcMgr>();

    // When source is Text, tableName can be <filename>.csv. Don't parse out owner name in this case.
    // For text source, CanQualifyTablenameOverride returns false. 
    if ( pPhysical->CanQualifyTableNameOverride() )
    {
        FdoStringP dbObjectName = GetDbObjectName();

        if ( dbObjectName.Contains(L".") )
        {
            // table name qualified. Break out owner and unqualified table name. 
            SetOwner(dbObjectName.Left(L"."));
            SetDbObjectName(dbObjectName.Right(L"."));
        }
    }

    //TODO: do we support modifying any of the ODBC-specific overrides (filegroups, etc) ?
}

void FdoSmLpOdbcClassDefinition::SetPhysicalAddWriter( FdoSmPhClassWriterP writer )
{
    FdoSmLpGrdClassDefinition::SetPhysicalAddWriter( writer );

    writer->SetDatabase( GetDatabase() );
    writer->SetOwner( GetOwner() );
}

void FdoSmLpOdbcClassDefinition::Init()
{
    // ODBC currently never creates tables, so tables are always 'fixed':
    SetIsFixedDbObject(true);
}


FdoSmPhDbObjectP FdoSmLpOdbcClassDefinition::NewTable( 
    FdoString* tableName, 
    FdoString* pkeyName
)
{
    // Let superclass do its job:
    FdoSmPhDbObjectP phDbObject = FdoSmLpClassBase::NewTable(tableName, pkeyName);

    // Pass ODBC-specific overrides to physical object:
    FdoSmPhOdbcTable* phOdbcTable = dynamic_cast<FdoSmPhOdbcTable*>(phDbObject.p);
	GetOverridesFromClass(phOdbcTable);

    // Return it:
    return phDbObject;
}

void FdoSmLpOdbcClassDefinition::PostFinalize()
{
    FdoSmLpGrdClassDefinition::PostFinalize();

    // Grab overrides from existing table:
    FdoSmPhOdbcDbObjectP dbObj = FDO_SAFE_ADDREF(dynamic_cast<FdoSmPhOdbcDbObject*>(FindPhDbObject().p));
}



void FdoSmLpOdbcClassDefinition::GetOverridesFromClass(FdoSmPhOdbcTable* phTable)
{
}

const FdoSmLpClassBase::Capabilities* FdoSmLpOdbcClassDefinition::GetCapabilities() const
{
    FdoSmLpClassBase::Capabilities* cap = const_cast<FdoSmLpClassBase::Capabilities*>(FdoSmLpClassBase::GetCapabilities());
    PolygonVertexOrderRuleMap& vertexOrderRuleMap = cap->GetPolygonVertexOrderRule();
    for (PolygonVertexOrderRuleMap::const_iterator iter = vertexOrderRuleMap.begin(); iter != vertexOrderRuleMap.end(); iter++)
        vertexOrderRuleMap[iter->first] = FdoPolygonVertexOrderRule_None;
    return cap;
}
