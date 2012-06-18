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
#include "stdafx.h"
#include "DataPropertyDefinition.h"
#include <Rdbms/Override/PostGis/PostGisOvDataPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>


FdoSmLpPostGisDataPropertyDefinition::FdoSmLpPostGisDataPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent)
        : FdoSmLpGrdDataPropertyDefinition(propReader, parent)
{
    FdoStringP defaultValue = Get_DefaultValueString();

    size_t defaultLen = defaultValue.GetLength();  
    if ( defaultLen > 0)
    {
        FdoStringP sequenceName = GetSequenceName();
        int idPos = this->GetIdPosition();
        
        if ( (idPos >= 0) && (sequenceName.GetLength() == 0) ) 
        {
            if ( defaultValue.Lower().Contains(L"nextval") ) 
            {
                FdoString* p = (FdoString*) defaultValue;
                
                size_t pos1;
                size_t pos2;

                for ( pos1 = 0; pos1 < defaultLen; pos1++ )
                {
                    if ( p[pos1] == '\'' )
                    {
                        pos1++;
                        break;
                    }
                }

                for ( pos2 = defaultLen - 1; pos2 >= 0; pos2-- )
                {
                    if ( p[pos2] == '\'' )
                        break;
                }

                if ( (pos2 - pos1) > 0 )
                {
                    sequenceName = defaultValue.Mid( pos1, pos2 - pos1 );
                    SetSequenceName(sequenceName);
                    SetIsAutoGenerated(true);
                    SetReadOnly(true);
                    SetDefaultValue( FdoPtr<FdoDataValue>() );
                }
            } 
        }
    }
}

FdoSmLpPostGisDataPropertyDefinition::FdoSmLpPostGisDataPropertyDefinition(
    FdoDataPropertyDefinition* fdoProp,
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpGrdDataPropertyDefinition(fdoProp, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisDataPropertyDefinition::FdoSmLpPostGisDataPropertyDefinition(
    FdoSmLpDataPropertyP baseProperty,
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool inherit,
    FdoPhysicalPropertyMapping* propOverrides)
    : FdoSmLpGrdDataPropertyDefinition(
        baseProperty,
        targetClass,
        logicalName,
        physicalName,
        inherit,
        propOverrides)
{
    // idle
}

FdoSmLpPropertyP FdoSmLpPostGisDataPropertyDefinition::NewInherited(
    FdoSmLpClassDefinition* subClass) const
{
    // Remove const qualifier
    FdoSmLpPostGisDataPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisDataPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpDataPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpDataPropertyDefinition*>(thisProp);

    return (new FdoSmLpPostGisDataPropertyDefinition(
            FDO_SAFE_ADDREF(propDef), subClass, L"", L"", true));
}

FdoSmLpPropertyP FdoSmLpPostGisDataPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* propOverrides) const
{
    // Remove const qualifier
    FdoSmLpPostGisDataPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisDataPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpDataPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpDataPropertyDefinition*>(thisProp);

    return (new FdoSmLpPostGisDataPropertyDefinition(
            FDO_SAFE_ADDREF(propDef),
            targetClass, logicalName, physicalName,
            false, propOverrides));
}

bool FdoSmLpPostGisDataPropertyDefinition::AddSchemaMappings(
    FdoPhysicalClassMappingP classMapping,
    bool includeDefaults) const
{
    bool hasMappings = false;

    FdoPostGISOvClassDefinition* pgClassMapping = NULL;
    pgClassMapping = static_cast<FdoPostGISOvClassDefinition*>(classMapping.p);

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping(GetSchemaMappings(includeDefaults));
    
    FdoPostGISOvPropertyDefinition* pgPropMapping = NULL;
    pgPropMapping = 
        dynamic_cast<FdoPostGISOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if (pgPropMapping)
    {
        FdoPostGISOvPropertiesP properties(pgClassMapping->GetProperties());
        properties->Add(pgPropMapping);
        hasMappings = true;
    }

    return hasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpPostGisDataPropertyDefinition::GetSchemaMappings(
    bool includeDefaults) const
{
    bool hasMappings = false;

    FdoPostGISOvDataPropertyDefinition* propMapping = NULL;
    propMapping = FdoPostGISOvDataPropertyDefinition::Create(GetName());

    FdoPhysicalPropertyMappingP retPropMapping;
    retPropMapping = static_cast<FdoPhysicalPropertyMapping*>(propMapping);

    FdoSmLpPostGisDataPropertyDefinition* thisClassDef = NULL;
    thisClassDef = const_cast<FdoSmLpPostGisDataPropertyDefinition*>(this);
    thisClassDef->Finalize();


    // Add column name mapping if including default mappings or mapping is 
    // non-default (property and column names are different).
    if (includeDefaults || GetIsFixedColumn())
    {
        // For foreign schemas, column is root column name, which is the 
        // name of the column in the foreign table.

        FdoPostGISOvColumnP columnMapping(
            FdoPostGISOvColumn::Create(
                (includeDefaults || GetIsFixedColumn())
                ? (wcslen(GetRootColumnName()) > 0)
                    ? GetRootColumnName() : GetColumnName() : L""));

        propMapping->SetColumn(columnMapping);
        hasMappings = true;
    }

    // return NULL if no mappings.
    if (!hasMappings) 
        retPropMapping = NULL;

    return retPropMapping;
}

void FdoSmLpPostGisDataPropertyDefinition::Update(
    FdoPropertyDefinition* fdoProp,
    FdoSchemaElementState state,
    FdoPhysicalPropertyMapping* propOverrides,
    bool ignoreStates)
{
    // Update according to Generic RDBMS overrides.
    FdoSmLpGrdDataPropertyDefinition::Update(fdoProp, state,
        propOverrides, ignoreStates);

}

FdoDataType* FdoSmLpPostGisDataPropertyDefinition::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    static FdoDataType autogen_types[] =
    {
        FdoDataType_Int32,
        FdoDataType_Int64,
    };

    length = sizeof (autogen_types) / sizeof (FdoDataType);

    return (autogen_types);
}
