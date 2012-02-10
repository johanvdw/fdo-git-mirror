 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "stdafx.h"
#include <Parse/Parse.h>
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Error.h>

#include <Src/Common/StringUtility.h>

#ifndef ASSERT
#ifdef _DEBUG
    #include <CrtDbg.h>
    #define ASSERT  _ASSERT
#else
    #define ASSERT(x) {}
#endif
#endif

FdoSmLpSchemaCollection::FdoSmLpSchemaCollection(FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextMgrP scMgr) :
	mPhysicalSchema(physicalSchema),
    mSpatialContextMgr(scMgr),
    mSchemasLoaded(false),
    mCreatePhysicalObjects(false),
    mFoundCount(0)
{
}

FdoSmLpSchemaCollection::~FdoSmLpSchemaCollection(void)
{
}

bool FdoSmLpSchemaCollection::CanCreatePhysicalObjects() const
{
    bool hasMetaSchema = false;

    FdoSmPhOwnerP owner = ((FdoSmLpSchemaCollection*) this)->GetPhysicalSchema()->FindOwner();
    if ( owner )
        hasMetaSchema = owner->GetHasMetaSchema();

    return ( GetCreatePhysicalObjects() && (hasMetaSchema || CanApplySchemaWithoutMetaSchema()) );
}

bool FdoSmLpSchemaCollection::CanApplySchemaWithoutMetaSchema() const
{
    return false;
}

void FdoSmLpSchemaCollection::SetCreatePhysicalObjects( bool createPhysicalObjects )
{
    mCreatePhysicalObjects = createPhysicalObjects;
}

bool FdoSmLpSchemaCollection::GetCreatePhysicalObjects() const
{
    return mCreatePhysicalObjects;
}

/*
FdoSmLpSchemaP CreateSchema( FdoSmPhSchemaReaderP rdr )
{
    return new (rdr, mPhysicalSchema, FDO_SAFE_ADDREF(this));
}
*/
const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass( FdoStringP schemaName, FdoStringP className ) const
{
	const FdoSmLpSchema* pSchema = NULL;
	const FdoSmLpClassDefinition* pFoundClass = NULL;

    if ( ((const wchar_t*)schemaName)[0] != '\0' )  {
		// Check the given schema if schema name specified
		pSchema = RefItem(schemaName);
		if ( pSchema ) 
            pFoundClass = pSchema->RefClass(className);
	}
	else {
		// Otherwise, find the first class in any schema.
		const FdoSmLpClassDefinition* pClass = NULL;

		for ( int i = 0; i < GetCount(); i++ ) {
			pSchema = RefItem(i);

			if ( pSchema ) {
				pClass = pSchema->RefClasses()->RefItem(className);

				if ( pClass ) {

					if ( pFoundClass ) {
						// Found a second class so exception.

                        throw FdoSchemaException::Create( 
                            FdoSmError::NLSGetMessage(
                                FDO_NLSID(FDOSM_174), 
									(FdoString*) className, 
									pFoundClass->RefLogicalPhysicalSchema()->GetName(),
									pSchema->GetName()
                            )
                        );
					}

					pFoundClass = pClass;
				}
			}
		}
	}

	return(pFoundClass);
}

const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass(FdoInt64 classId) const
{
	const FdoSmLpSchema* pSchema;
	const FdoSmLpClassDefinition* pClass = NULL;
	bool found = false;

	for (int i = 0; i < GetCount() && !found; i++)	{
		pSchema = RefItem(i);
		if (pSchema)	{
			for (int j = 0; j < pSchema->RefClasses()->GetCount() && !found; j++)	{
				pClass = pSchema->RefClasses()->RefItem(j);
				if (pClass->GetId() == classId)
					found = true;
			}
		}
	}
	return pClass;
}

FdoFeatureSchemasP FdoSmLpSchemaCollection::GetFdoSchemas( FdoStringP schemaName)
{
    return GetFdoSchemasEx(schemaName, NULL);
}

/*
FdoSmLpJoinTreeNode* FdoSmLpSchemaCollection::GetJoinTree( FdoStringP schemaName, FdoStringP className, const FdoStringsP& properties ) const
{
	const FdoSmLpClassDefinition* pClass = FindClass( schemaName, className );

	if ( !pClass ) 
		return(NULL);

	return ( pClass->GetJoinTree( properties ) );
}
*/

FdoSmLpQClassesP FdoSmLpSchemaCollection::TableToClasses( 
    FdoStringP tableName, 
    FdoStringP ownerName,
    FdoStringP databaseName,
    bool cacheOnly
) const
{
    FdoInt32            idx;
    FdoSmLpQClassesP    classes = new FdoSmLpQClassCollection();

    for ( idx = 0; idx < GetCount(); idx++ ) {
        RefItem(idx)->TableToClasses( classes, tableName, ownerName, databaseName, cacheOnly );
    }

    return classes;
}

FdoSmLpSchemaP FdoSmLpSchemaCollection::CreateSchema(
	FdoFeatureSchema* pFeatSchema,
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoSmLpSchemaP schema = NewSchema( pFeatSchema, bIgnoreStates );

    schema->Update( 
        pFeatSchema, 
        bIgnoreStates ? FdoSchemaElementState_Added : pFeatSchema->GetElementState(), 
        pOverrides,
        bIgnoreStates 
    );

    return schema;
}

void FdoSmLpSchemaCollection::Commit()
{
	for ( int i = 0; i < GetCount(); i++ ) {
		FdoSmLpSchemaP schema = GetItem(i);
		schema->Commit( true );
	}
}

FdoSchemaExceptionP FdoSmLpSchemaCollection::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);

	for ( int i = 0; i < GetCount(); i++ )
		pException = RefItem(i)->Errors2Exception(pException);

	return pException;
}


void FdoSmLpSchemaCollection::Load()
{
    int i;

	if ( (mPhysicalSchema != NULL) && !mSchemasLoaded ) {
        mSchemasLoaded = true;

        // Check if there is a config doc
        FdoFeatureSchemasP schemas = mPhysicalSchema->GetConfigSchemas();
        FdoSchemaMappingsP mappings = mPhysicalSchema->GetConfigMappings();
        FdoStringP providerName = mPhysicalSchema->GetProviderName();

        if ( schemas ) {
            // There is a config doc so load schemas from it first.
            for ( i = 0; i < schemas->GetCount(); i++ ) {
                FdoFeatureSchemaP schema = schemas->GetItem(i);
                FdoPhysicalSchemaMappingP mapping;
                
                if ( mappings ) 
                    mapping = mappings->GetItem(
                        providerName, 
                        schema->GetName() 
                    );

                FdoSmLpSchemaP lpSchema = NewSchema( schema, true );

                // Elements from config doc are never modified so mark them as Unchanged.
                lpSchema->Update(
                    schema, 
                    FdoSchemaElementState_Unchanged, 
                    mapping,
                    true
                );

                Add( lpSchema );
            }
        }

        // Next, read schemas from datastore
        FdoSmPhSchemaReaderP rdr = mPhysicalSchema->CreateSchemaReader();

        while ( rdr->ReadNext() ) {
            FdoSmLpSchemaP rdrSchema = NewSchema(rdr);
            // Skip any schemas already loaded from config doc.
            // Config doc takes precedence.
            if ( !schemas || (!FdoPtr<FdoSmLpSchema>(FindItem(rdrSchema->GetName()))) )
    		    this->Add( rdrSchema );
	    }
	}
}


void FdoSmLpSchemaCollection::XMLSerialize( FdoString* sFileName ) const
{
#ifdef _WIN32
	FILE* xmlFp = _wfopen( sFileName, L"w" );
#else
	FILE* xmlFp = fopen( (const char*) FdoStringP(sFileName), "w" );
#endif

	fprintf( xmlFp, "<?xml version=\"1.0\" standalone=\"yes\"?>\n" );
    fprintf( xmlFp, "<schemas xmlns:xsi=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" >\n" );
	
	for ( int i = 0; i < GetCount(); i++ ) {
		RefItem(i)->XMLSerialize(xmlFp, 0);
	}
	
	fprintf( xmlFp, "</schemas>\n" );

	fclose(xmlFp);

}

//TODO: The following functions should be moved to the schema element class that they operate on.

FdoFeatureSchema* FdoSmLpSchemaCollection::ConvertSchema(const FdoSmLpSchema* pLpSchema, SchemaElementCollection& aReferenced)
{
    ASSERT(pLpSchema);
    FdoFeatureSchema*                   pFdoFeatureSchema = FdoFeatureSchema::Create(pLpSchema->GetName(), pLpSchema->GetDescription());
    FdoClassCollection*                 pFdoClassCollection = pFdoFeatureSchema->GetClasses();

    // Loop through classes
    const FdoSmLpClassCollection*    pLpClassDefColl = pLpSchema->RefClasses();
    for (int iClass=0; iClass < pLpClassDefColl->GetCount(); iClass++)
    {
        const FdoSmLpClassDefinition*    pLpClassDef = pLpClassDefColl->RefItem(iClass);
        FdoClassDefinition*                 pFdoClassDef = ConvertClassDefinition(pLpClassDef, aReferenced);

        if (pFdoClassDef)
        {
            pFdoClassCollection->Add(pFdoClassDef);   // Also sets the parentage for pFdoClassDef
            pFdoClassDef->Release();
        }
    }
    FDO_SAFE_RELEASE(pFdoClassCollection);

    ConvertSAD(pLpSchema, pFdoFeatureSchema);

    return pFdoFeatureSchema;
}

FdoClassDefinition* FdoSmLpSchemaCollection::ConvertClassDefinition(const FdoSmLpClassDefinition* pLpClassDef, SchemaElementCollection& aReferenced)
{
    ASSERT(pLpClassDef);
    FdoClassDefinition* pFdoClassDef = (FdoClassDefinition*) mMappingClass.Map(pLpClassDef);

    if (!aReferenced.classes.ContainsClassDefinition(pLpClassDef))
        aReferenced.classes.AddReference((FdoSmLpClassDefinition*)pLpClassDef);

    FdoSmLpClassBase* pcls =  static_cast<FdoSmLpClassBase*>((FdoSmLpClassDefinition*)pLpClassDef);
    FdoSmPhDbObjectP dbObj = pcls->FindPhDbObject();
    FdoSmPhDbObjType type = dbObj ? dbObj->GetType() : FdoSmPhDbObjType_Unknown;

    if (!pFdoClassDef)
    {
        // Create an FDO version of the class
        switch (pLpClassDef->GetClassType())
        {
        case FdoClassType_Class:
            pFdoClassDef = FdoClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_FeatureClass:
            // Create a FdoFeatureClass to match
            FdoFeatureClass*                pFdoFeatureClass;
            pFdoFeatureClass = FdoFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());

            const FdoSmLpFeatureClass*   pLpFeatureClass;
            pLpFeatureClass = (FdoSmLpFeatureClass*)pLpClassDef;
            if (pLpFeatureClass->RefGeometryProperty())
            {
                FdoGeometricPropertyDefinition*     pGeomProp = ConvertGeometricPropertyDefinition(pLpFeatureClass->RefGeometryProperty(), aReferenced.schemas);
                pFdoFeatureClass->SetGeometryProperty(pGeomProp);
                pGeomProp->Release();
            }
            if (type == FdoSmPhDbObjType_View)
                pFdoFeatureClass->SetIsComputed(true);

            pFdoClassDef = pFdoFeatureClass;
            break;

/* TODO:R2 - Support other classes as defined
        case FdoClassType_PointSegmentFeatureClass:
            pFdoClassDef = FdoPointSegmentFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            // TODO:R2 Populate pFdoPointSegFeatureClass->GetFeatureClasses();
            // TODO:R2 Populate pFdoPointSegFeatureClass->MeasureUnit(); (which doesn't exist yet)
            break;

        case FdoClassType_LineSegmentFeatureClass:
            // TODO:R2 pFdoClassDef = FdoLineSegmentFeatureClass::Create(pLpClassDef->GetName(), 
            //               pLpClassDef->GetDescription(), 
            //               FdoLineSegmentClass* value);
            // TODO:R2 Populate pFdoLineSegFeatureClass->GetFeatureClasses();
            // TODO:R2 Populate pFdoLineSegFeatureClass->MeasureUnit(); (which doesn't exist yet)
            break;

        case FdoClassType_LineSegmentClass:
            pFdoClassDef = FdoLineSegmentClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TopologyFeatureClass:
            pFdoClassDef = FdoTopologyFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TinFeatureClass:
            // TODO:R2 pFdoClassDef = FdoTinFeatureClass::Create(pLpClassDef->GetName(), 
            //              pLpClassDef->GetDescription(), 
            //              FdoTinPointClass* pointClass, 
            //              FdoTinEdgeClass* edgeClass););
            // TODO:R2 Populate the pFdoTinFeatureClass->GetCoverageProperties()->Add(...);
            break;

        case FdoClassType_TinPointClass:
            pFdoClassDef = FdoTinPointClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_TinEdgeClass:
            pFdoClassDef = FdoTinEdgeClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break;

        case FdoClassType_AssociationFeatureClass:
            pFdoClassDef = FdoAssociationFeatureClass::Create(pLpClassDef->GetName(), 
                pLpClassDef->GetDescription());
            break; 
*/


        default:
            // this should never happen unless somebody adds a new FdoClassType_xxx
            ASSERT(false);
            return NULL;
        }

        // The following MUST be performed before calling ConvertClassDefinition()
        // again (directly or indirectly) to avoid recursive infinite loops.
        //
        mMappingClass.Add(pLpClassDef, pFdoClassDef);

        // copy base class
        const FdoSmLpClassDefinition*  pLpClassDefBase = pLpClassDef->RefBaseClass();
        if (pLpClassDefBase)
        {
            FdoClassDefinition*     pBaseClase = ConvertClassDefinition(pLpClassDefBase, aReferenced);
            pFdoClassDef->SetBaseClass(pBaseClase);
            pBaseClase->Release();
        }

        // copy abstract state
        pFdoClassDef->SetIsAbstract(pLpClassDef->GetIsAbstract());

        // populate properties
        //
        const FdoSmLpPropertyDefinitionCollection*   pLpPropDefColl = pLpClassDef->RefProperties();
        FdoPropertyDefinitionCollection*                pFdoBaseProperties = FdoPropertyDefinitionCollection::Create(NULL);
        FdoPropertyDefinitionCollection*                pFdoProperties = pFdoClassDef->GetProperties();
         for (int iProp=0; iProp < pLpPropDefColl->GetCount(); iProp++)
        {
            const FdoSmLpPropertyDefinition*     pLpPropDef = pLpPropDefColl->RefItem(iProp);

            if ( (!pLpClassDefBase) || (pLpPropDef->RefDefiningClass() == pLpClassDef) )
            {
                // Create an FDO version of the property.
                FdoPropertyDefinition*                  pFdoPropDef = NULL;
                switch (pLpPropDef->GetPropertyType())
                {
                case FdoPropertyType_DataProperty:
                    pFdoPropDef = ConvertDataPropertyDefinition((const FdoSmLpDataPropertyDefinition*)pLpPropDef, aReferenced.schemas);
                    break;

                case FdoPropertyType_ObjectProperty:
                    pFdoPropDef = ConvertObjectPropertyDefinition((const FdoSmLpObjectPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                case FdoPropertyType_GeometricProperty:
                    pFdoPropDef = ConvertGeometricPropertyDefinition((const FdoSmLpGeometricPropertyDefinition*)pLpPropDef, aReferenced.schemas);
                    break;

                case FdoPropertyType_AssociationProperty:
                    pFdoPropDef = ConvertAssociationPropertyDefinition((const FdoSmLpAssociationPropertyDefinition*)pLpPropDef, aReferenced);
                    break;

                default:
                    // this should be impossible to reach
                    ASSERT(false);
                    break;
                }

                if (pFdoPropDef)
                {
                    // Is this an inherited property?
                    if (pLpPropDef->RefDefiningClass() == pLpClassDef)
                    {
                        // Not inhereited, add to Properties collection
                        pFdoProperties->Add(pFdoPropDef);
                    }
                    else
                    {
                        // Inherited, add to BaseProperties collection.

                        bool addProp = true;

                        // Some simple system properties do not have a column in the
                        // class table. This is a legitimate case. For example,
                        // a foreign table often has no column for the
                        // RevisionNumber property.

                        // Skip properties that don't have a column in the
                        // class table. 
                        const FdoSmLpSimplePropertyDefinition* pLpSimplePropDef =
                            FdoSmLpSimplePropertyDefinition::Cast(pLpPropDef);

                        if ( pLpSimplePropDef ) {
                            const FdoSmPhColumn* pColumn = pLpSimplePropDef->RefColumn();
                            if ( !pColumn || !pColumn->GetParent() ) {
                                // Skip when no column or column is orphan
                                addProp = false;
                            }
                            else {
                                if ( FdoStringP(pColumn->GetParent()->GetName()).ICompare(pLpClassDef->GetDbObjectName()) != 0 )
                                    // Skip when column not in class table.
                                    addProp = false;
                            }
                        }

                        if ( addProp ) {
                            pFdoBaseProperties->Add(pFdoPropDef);

                            // Add a reference in case the class (inherited or not) comes
                            // from is in a different schema.
                            aReferenced.schemas.AddReference(pLpPropDef->RefDefiningClass()->RefLogicalPhysicalSchema());
                        }
                    }

                    pFdoPropDef->Release(); // we're done with it, and collections should have AddRef'ed
                }
            }
        }

        // Set the base property collection
        if (pFdoBaseProperties->GetCount() > 0)
            pFdoClassDef->SetBaseProperties(pFdoBaseProperties);
        FDO_SAFE_RELEASE(pFdoBaseProperties);
        FDO_SAFE_RELEASE(pFdoProperties);

        // Populate the Identity Properties
        // SSP? Can only base classes have identity properties?
        //
        if (!pLpClassDef->RefBaseClass())
        {
            const FdoSmLpDataPropertyDefinitionCollection*   pLpIdPropDefColl = pLpClassDef->RefIdentityProperties();
            FdoDataPropertyDefinitionCollection*                pFdoIdentityProperties = pFdoClassDef->GetIdentityProperties();

            for (int iProp=0; iProp < pLpIdPropDefColl->GetCount(); iProp++)
            {
                const FdoSmLpDataPropertyDefinition* pLpIdPropDef = pLpIdPropDefColl->RefItem(iProp);

                FdoDataPropertyDefinition*  pIdProp = ConvertDataPropertyDefinition(pLpIdPropDef, aReferenced.schemas);
                pFdoIdentityProperties->Add(pIdProp);
                pIdProp->Release();
            }
            FDO_SAFE_RELEASE(pFdoIdentityProperties);
        }

        // Populate class capabilities
        const FdoSmLpClassDefinition::Capabilities* pLpCap = pLpClassDef->GetCapabilities();

        if ( pLpCap ) {
            FdoClassCapabilitiesP capabilities = FdoClassCapabilities::Create( (*pFdoClassDef) );
            capabilities->SetSupportsLocking( pLpCap->SupportsLocking() );
            capabilities->SetSupportsLongTransactions( pLpCap->SupportsLongTransactions() );

            FdoInt32 lockTypeCount;
            const FdoLockType* lockTypes = pLpCap->GetLockTypes( lockTypeCount );
            capabilities->SetLockTypes( lockTypes, lockTypeCount );
            // capabilities->SetSupportsWrite(true); // TODO - Capability: How to determine whether true or false is to be set.
            capabilities->SetSupportsWrite( pLpCap->SupportsWrite() );

            PolygonVertexOrderRuleMap::const_iterator iter1;
            PolygonVertexOrderRuleMap vertexOrderRuleMap = pLpCap->GetPolygonVertexOrderRule();
            for (iter1 = vertexOrderRuleMap.begin(); iter1 != vertexOrderRuleMap.end(); iter1++)
                capabilities->SetPolygonVertexOrderRule(iter1->first, iter1->second);

            PolygonVertexOrderStrictnessMap::const_iterator iter2;
            PolygonVertexOrderStrictnessMap vertexOrderStrictnessMap = pLpCap->GetPolygonVertexOrderStrictness();
            for (iter2 = vertexOrderStrictnessMap.begin(); iter2 != vertexOrderStrictnessMap.end(); iter2++)
                capabilities->SetPolygonVertexOrderStrictness(iter2->first, iter2->second);

            pFdoClassDef->SetCapabilities( capabilities );
        }

        ConvertSAD(pLpClassDef, pFdoClassDef);

		// Populate constraints
		ConvertConstraints(pLpClassDef, pFdoClassDef);

        aReferenced.schemas.AddReference(pLpClassDef->RefLogicalPhysicalSchema());
    }
    else {
        pFdoClassDef->AddRef();
    }

    return pFdoClassDef;
}

FdoDataPropertyDefinition* FdoSmLpSchemaCollection::ConvertDataPropertyDefinition(const FdoSmLpDataPropertyDefinition* pLpDataPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpDataPropDef);
    ASSERT(pLpDataPropDef->GetPropertyType() == FdoPropertyType_DataProperty);

    FdoDataPropertyDefinition*  pFdoDataPropDef = (FdoDataPropertyDefinition*) mMappingPropDef.Map(pLpDataPropDef);
    if (!pFdoDataPropDef)
    {
        const FdoSmLpSimplePropertyDefinition* pLpSimplePropDef = FdoSmLpSimplePropertyDefinition::Cast(pLpDataPropDef);
        FdoSmPhColumnP column = ((FdoSmLpSimplePropertyDefinition*)pLpSimplePropDef)->GetColumn();

        pFdoDataPropDef = FdoDataPropertyDefinition::Create(pLpDataPropDef->GetName(), pLpDataPropDef->GetDescription());

        pFdoDataPropDef->SetDataType(pLpDataPropDef->GetDataType());
        pFdoDataPropDef->SetReadOnly(pLpDataPropDef->GetReadOnly() || (column ? column->GetReadOnly() : false));
        pFdoDataPropDef->SetLength(pLpDataPropDef->GetLength());
        pFdoDataPropDef->SetPrecision(pLpDataPropDef->GetPrecision());
        pFdoDataPropDef->SetScale(pLpDataPropDef->GetScale());
        pFdoDataPropDef->SetNullable(pLpDataPropDef->GetNullable());
        pFdoDataPropDef->SetDefaultValue(pLpDataPropDef->GetDefaultValueString());
        pFdoDataPropDef->SetIsAutoGenerated(pLpDataPropDef->GetIsAutoGenerated());

        pFdoDataPropDef->SetIsSystem( pLpDataPropDef->GetIsSystem() );

        ConvertSAD(pLpDataPropDef, pFdoDataPropDef);

        aReferenced.AddReference(pLpDataPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpDataPropDef, pFdoDataPropDef);
    }
    else
        pFdoDataPropDef->AddRef();

    return pFdoDataPropDef;
}

FdoObjectPropertyDefinition* FdoSmLpSchemaCollection::ConvertObjectPropertyDefinition(const FdoSmLpObjectPropertyDefinition* pLpObjPropDef, SchemaElementCollection& aReferenced)
{
    ASSERT(pLpObjPropDef);
    ASSERT(pLpObjPropDef->GetPropertyType() == FdoPropertyType_ObjectProperty);

    FdoObjectPropertyDefinition*    pFdoObjPropDef = (FdoObjectPropertyDefinition*) mMappingPropDef.Map(pLpObjPropDef);
    if (!pFdoObjPropDef)
    {
        pFdoObjPropDef = FdoObjectPropertyDefinition::Create(pLpObjPropDef->GetName(), pLpObjPropDef->GetDescription());

        const FdoSmLpClassDefinition*    pLpClassDef = pLpObjPropDef->RefClass();
        FdoClassDefinition*                 pFdoClassDef = pLpClassDef ? ConvertClassDefinition(pLpClassDef, aReferenced) : NULL;
        pFdoObjPropDef->SetClass(pFdoClassDef);
        FDO_SAFE_RELEASE(pFdoClassDef);

        pFdoObjPropDef->SetObjectType(pLpObjPropDef->GetObjectType());

        pFdoObjPropDef->SetOrderType(pLpObjPropDef->GetOrderType());
        if (pLpObjPropDef->RefIdentityProperty())
        {
            // TODO: Should the following ASSERT become a part of the if()?
            ASSERT(pFdoObjPropDef->GetObjectType() == FdoObjectType_Collection || pFdoObjPropDef->GetObjectType() == FdoObjectType_OrderedCollection);
            FdoDataPropertyDefinition*  pIdentProp = ConvertDataPropertyDefinition(pLpObjPropDef->RefIdentityProperty(), aReferenced.schemas);
            pFdoObjPropDef->SetIdentityProperty(pIdentProp);
            pIdentProp->Release();
        }

        pFdoObjPropDef->SetIsSystem(pLpObjPropDef->GetIsSystem());

        ConvertSAD(pLpObjPropDef, pFdoObjPropDef);

        aReferenced.schemas.AddReference(pLpObjPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpObjPropDef, pFdoObjPropDef);
    }
    else
        pFdoObjPropDef->AddRef();

    return pFdoObjPropDef;
}



FdoGeometricPropertyDefinition* FdoSmLpSchemaCollection::ConvertGeometricPropertyDefinition(const FdoSmLpGeometricPropertyDefinition* pLpGeomPropDef, SchemaCollection& aReferenced)
{
    ASSERT(pLpGeomPropDef);
    ASSERT(pLpGeomPropDef->GetPropertyType() == FdoPropertyType_GeometricProperty);

    FdoGeometricPropertyDefinition* pFdoGeomPropDef = (FdoGeometricPropertyDefinition*) mMappingPropDef.Map(pLpGeomPropDef);

    if (!pFdoGeomPropDef)
    {
        const FdoSmLpSimplePropertyDefinition* pLpSimplePropDef = FdoSmLpSimplePropertyDefinition::Cast(pLpGeomPropDef);
        FdoSmPhColumnP column = ((FdoSmLpSimplePropertyDefinition*)pLpSimplePropDef)->GetColumn();

        pFdoGeomPropDef = FdoGeometricPropertyDefinition::Create(pLpGeomPropDef->GetName(), pLpGeomPropDef->GetDescription());

        pFdoGeomPropDef->SetGeometryTypes(pLpGeomPropDef->GetGeometryTypes());

        FdoGeometryType geomTypes[MAX_GEOMETRY_TYPE_SIZE];
        FdoInt32 geomTypeCount;

        FdoCommonGeometryUtil::GeometryTypesToArray( pLpGeomPropDef->GetSpecificGeometryTypes(), geomTypes, geomTypeCount );
        pFdoGeomPropDef->SetSpecificGeometryTypes( geomTypes, geomTypeCount );

        pFdoGeomPropDef->SetReadOnly(pLpGeomPropDef->GetReadOnly() || (column ? column->GetReadOnly() : false));
        pFdoGeomPropDef->SetHasMeasure(pLpGeomPropDef->GetHasMeasure());
        pFdoGeomPropDef->SetHasElevation(pLpGeomPropDef->GetHasElevation());
        pFdoGeomPropDef->SetSpatialContextAssociation(pLpGeomPropDef->GetSpatialContextAssociation());
        pFdoGeomPropDef->SetIsSystem(pLpGeomPropDef->GetIsSystem());
        ConvertSAD(pLpGeomPropDef, pFdoGeomPropDef);

        aReferenced.AddReference(pLpGeomPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpGeomPropDef, pFdoGeomPropDef);
    }
    else
        pFdoGeomPropDef->AddRef();

    return pFdoGeomPropDef;
}

FdoAssociationPropertyDefinition* FdoSmLpSchemaCollection::ConvertAssociationPropertyDefinition(const FdoSmLpAssociationPropertyDefinition* pLpAssocPropDef, SchemaElementCollection& aReferenced )
{
    ASSERT(pLpAssocPropDef);
    ASSERT(pLpAssocPropDef->GetPropertyType() == FdoPropertyType_AssociationProperty);

    if( pLpAssocPropDef->GetReadOnly() ) // We do not convert the readonly as they will be added automatically by FDO code.
        return NULL;

    FdoAssociationPropertyDefinition*    pFdoAssocPropDef = (FdoAssociationPropertyDefinition*) mMappingPropDef.Map(pLpAssocPropDef);
    if (!pFdoAssocPropDef)
    {
        pFdoAssocPropDef = FdoAssociationPropertyDefinition::Create(pLpAssocPropDef->GetName(), pLpAssocPropDef->GetDescription());

        const FdoSmLpClassDefinition*    pLpAssocClassDef = pLpAssocPropDef->RefAssociatedClass();
        FdoClassDefinition*                 pFdoAssocClassDef = pLpAssocClassDef ? ConvertClassDefinition(pLpAssocClassDef, aReferenced) : NULL;
        pFdoAssocPropDef->SetIsReadOnly( pLpAssocPropDef->GetReadOnly() );
        pFdoAssocPropDef->SetReverseName( pLpAssocPropDef->GetReverseName() );
        pFdoAssocPropDef->SetAssociatedClass( pFdoAssocClassDef );
        pFdoAssocPropDef->SetDeleteRule( pLpAssocPropDef->GetDeleteRule() );
        pFdoAssocPropDef->SetMultiplicity( pLpAssocPropDef->GetMultiplicity() );
        pFdoAssocPropDef->SetReverseMultiplicity( pLpAssocPropDef->GetReverseMultiplicity() );
        pFdoAssocPropDef->SetLockCascade( pLpAssocPropDef->GetCascadeLock() );

        //
        // Determine if we need to populate the Identity and reverse identity properties
        // We should not populate the 2 collection if the PH 
        if( pFdoAssocClassDef )
        {
            const FdoStringsP identProps = pLpAssocPropDef->GetIdentityProperties();
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> fdoProp = (FdoDataPropertyDefinition*)FdoPtr<FdoPropertyDefinitionCollection>(pFdoAssocClassDef->GetProperties())->FindItem( (const wchar_t *)identProps->GetString(i) );
                if ( !fdoProp ) 
                {
                    // Property might be inherited, check base properties
                    fdoProp = (FdoDataPropertyDefinition*)FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(pFdoAssocClassDef->GetBaseProperties())->GetItem( (const wchar_t *)identProps->GetString(i) );
                }
                if( fdoProp )
                    FdoPtr<FdoDataPropertyDefinitionCollection>(pFdoAssocPropDef->GetIdentityProperties())->Add( fdoProp );
            }
        }
        if( pLpAssocPropDef )
        {
            const FdoStringsP identProps = pLpAssocPropDef->GetReverseIdentityProperties();
            for(int i=0; i<identProps->GetCount(); i++ )
            {
                const FdoSmLpPropertyDefinition* pLpPropDef = pLpAssocPropDef->RefParentClass()->RefProperties()->RefItem( (const wchar_t *)identProps->GetString(i) );
                if( pLpPropDef == NULL || pLpPropDef->GetPropertyType() !=  FdoPropertyType_DataProperty )
                    continue;
                FdoPtr<FdoDataPropertyDefinition> fdoProp = ConvertDataPropertyDefinition( (const FdoSmLpDataPropertyDefinition*)pLpPropDef, aReferenced.schemas);
                if( fdoProp )
                    FdoPtr<FdoDataPropertyDefinitionCollection>(pFdoAssocPropDef->GetReverseIdentityProperties())->Add( fdoProp );
            }
        }
        
        FDO_SAFE_RELEASE(pFdoAssocClassDef);
        ConvertSAD(pLpAssocPropDef, pFdoAssocPropDef );

        aReferenced.schemas.AddReference(pLpAssocPropDef->RefLogicalPhysicalSchema());
        mMappingPropDef.Add(pLpAssocPropDef, pFdoAssocPropDef);
    }
    else
        pFdoAssocPropDef->AddRef();

    return pFdoAssocPropDef;
}

void FdoSmLpSchemaCollection::ConvertSAD(const FdoSmLpSchemaElement* pLpElement, FdoSchemaElement* pFdoElement)
{
    const FdoSmLpSAD*  pLpSAD = pLpElement->RefSAD();

    if (pLpSAD && pLpSAD->GetCount() > 0)
    {
        FdoPtr<FdoSchemaAttributeDictionary>  pFdoSAD = pFdoElement->GetAttributes();

        for (int i=0; i < pLpSAD->GetCount(); i++)
        {
            const FdoSmLpSADElement* pLpSADElement = pLpSAD->RefItem(i);

            if (pLpSADElement)
                pFdoSAD->Add(pLpSADElement->GetName(), pLpSADElement->GetValue());
        }
    }
}

void FdoSmLpSchemaCollection::ConvertConstraints(const FdoSmLpClassDefinition* pLpClassDef, FdoClassDefinition* pFdoClassDef)
{
	///////////////////////////////////////////////////////////////////////////
	// UNIQUE() constraints : Load from the database
	///////////////////////////////////////////////////////////////////////////

	FdoSmLpUniqueConstraintCollection*			    uLpUKeys = (FdoSmLpUniqueConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefUniqueConstraints();
	FdoPtr<FdoUniqueConstraintCollection>		    pFdoUKeys = pFdoClassDef->GetUniqueConstraints();
	FdoPropertiesP								    pFdoProps = pFdoClassDef->GetProperties();
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection>	pFdoBaseProps = pFdoClassDef->GetBaseProperties();

	for ( int i = 0; i < uLpUKeys->GetCount(); i++ ) {
		FdoSmLpUniqueConstraintP	pLpUniqueC = uLpUKeys->GetItem(i);

        // Skip inherited constraints since already defined on base class.
        if ( !pLpUniqueC->GetBaseConstraint() ) {
		    FdoSmLpDataPropertiesP		pLpProps = pLpUniqueC->GetProperties();

		    // New constraints
		    FdoPtr<FdoUniqueConstraint>	pFdoUniqueC = FdoUniqueConstraint::Create();
		    FdoDataPropertiesP			pFdoUniqueProps = pFdoUniqueC->GetProperties();
    	    bool                        constraintOk = true;	
    		
            for ( int j = 0; j < pLpProps->GetCount(); j++ ) {
			    FdoSmLpDataPropertyP	pLpProp = pLpProps->GetItem(j);
    			
			    // Find the Fdo data property
			    FdoDataPropertyP   pFdoProp = (FdoDataPropertyDefinition *)pFdoProps->FindItem(pLpProp->GetName());

			    // Match the names and add to collection
                if ( pFdoProp && ( wcscmp(pLpProp->GetName(), pFdoProp->GetName()) == 0 ) ) {
				    pFdoUniqueProps->Add( pFdoProp );
                }
                else {
                    try { 
                        pFdoProp = (FdoDataPropertyDefinition *)pFdoBaseProps->GetItem(pLpProp->GetName());
                    }
                    catch (...) {
                    }

                    if ( pFdoProp && ( wcscmp(pLpProp->GetName(), pFdoProp->GetName()) == 0 ) ) {
				        pFdoUniqueProps->Add( pFdoProp );
                    }
                    else {
                        constraintOk = false;
                    }
                }
		    }
		    if ( constraintOk && (pFdoUniqueProps->GetCount() != 0) )
			    pFdoUKeys->Add(pFdoUniqueC);
        }
	}

	///////////////////////////////////////////////////////////////////////////
	// CHECK() constraints : Load from the database
	///////////////////////////////////////////////////////////////////////////

	FdoSmLpCheckConstraintCollection*	pLpCKeys = (FdoSmLpCheckConstraintCollection *)((FdoSmLpClassBase *)pLpClassDef)->RefCheckConstraints();

	FdoSmPhDbObjectP					pPhObject = ((FdoSmLpClassBase *)pLpClassDef)->FindPhDbObject();
    FdoSmPhTableP						pPhTable = pPhObject.p->SmartCast<FdoSmPhTable>();


	// For each constraint, try to find a matching data property
	for ( int j = 0; j < pLpCKeys->GetCount(); j++ ) {

		FdoSmLpCheckConstraintP		pLpCKey = pLpCKeys->GetItem(j);			
		FdoDataPropertyP			pDataFdoProp = (FdoDataPropertyDefinition *)pFdoProps->FindItem( pLpCKey->GetPropertyName() );

		// Attach the constraint to the data property
		if ( pDataFdoProp && (pLpCKey->GetClause() != L"") ) {
			FdoPtr<FdoPropertyValueConstraint> pConstr;
            FdoCommonParse      			   *parser  = NULL;
			FdoPtr<FdoDataValue>			   val;

            try {
				parser = new FdoCommonParse();
				if (parser)
				{
					FdoStringP	clause = pLpCKey->GetClause();

					// The constraint parser might need help
					clause = pPhTable->FixCkeyClause( clause );

					pConstr = parser->ParseConstraint( clause );
				}
            }
            catch ( FdoExpressionException* ex ) {
                // The LogicalPhysical Schema also sends back constraints defined
                // outside of FDO, including ones not FDO-supported. Silently skip
                // the unsupported constraints.
                pConstr = NULL;
                ex->Release();
            }

			if ( pConstr ) {
				
				FdoDataType	propType = pDataFdoProp->GetDataType();

				// Check if the constraint type matches the property data type since the Parser cannot guess.
				if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range ) 
				{
					FdoPropertyValueConstraintRange*	pConstrR = static_cast<FdoPropertyValueConstraintRange*>((FdoPropertyValueConstraint*)pConstr);
					
					pConstrR->SetMinValue( FdoPtr<FdoDataValue>(FixDataValueType( FdoPtr<FdoDataValue>(pConstrR->GetMinValue()), propType )));
					pConstrR->SetMaxValue( FdoPtr<FdoDataValue>(FixDataValueType( FdoPtr<FdoDataValue>(pConstrR->GetMaxValue()), propType )));
				} 
				else if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List ) 
				{
					FdoPropertyValueConstraintList	*pConstrL = static_cast<FdoPropertyValueConstraintList*>((FdoPropertyValueConstraint*)pConstr);
					FdoPtr<FdoDataValueCollection>	vals = pConstrL->GetConstraintList();

					for ( int i = 0; i < vals->GetCount(); i++ )
					{
						vals->SetItem( i, FdoPtr<FdoDataValue>(FixDataValueType(vals->GetItem(i), propType) ));
					}	
				}

				// Attach the constraint to property
    			pDataFdoProp->SetValueConstraint( pConstr);
			}
			delete parser;
		}
	}
}

FdoPtr<FdoDataValue> FdoSmLpSchemaCollection::FixDataValueType( FdoPtr<FdoDataValue> val, FdoDataType propType )
{
	FdoInt32		valInt32;
	FdoInt64		valInt64;
	FdoDouble		valDouble;

	FdoPtr<FdoDataValue>	ret = val;

	// Fast return if nothing to do
	if ( val == NULL )
		return ret;

	FdoDataType	constrType = val->GetDataType();

	if ( constrType != propType ) 
	{
		FdoDataValue	*newData = NULL;

        switch (constrType) {

        case FdoDataType_Int32:
			valInt32 = ((FdoInt32Value*)(FdoDataValue*)val)->GetInt32();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt32 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt32 );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valInt32);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt32);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt32, propType );
                break;
            }

            break;

        case FdoDataType_Int64:
			valInt64 = ((FdoInt64Value*)(FdoDataValue*)val)->GetInt64();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valInt64 );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valInt64 );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valInt64);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valInt64);
                break;

            case FdoDataType_Double:
            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( (FdoDouble) valInt64, propType );
                break;
            }

            break;

        case FdoDataType_Double:
			valDouble = ((FdoDoubleValue*)(FdoDataValue*)val)->GetDouble();

            switch (propType) {
            case FdoDataType_Byte:
			    newData = FdoDataValue::Create( (FdoByte)valDouble );
                break;

            case FdoDataType_Int16:
			    newData = FdoDataValue::Create( (FdoInt16)valDouble );
                break;

            case FdoDataType_Int32:
			    newData = FdoDataValue::Create( (FdoInt32)valDouble );
                break;

            case FdoDataType_Int64:
			    newData = FdoDataValue::Create( (FdoInt64)valDouble);
                break;

            case FdoDataType_Single:
			    newData = FdoDataValue::Create( (FdoFloat)valDouble);
                break;

            case FdoDataType_Decimal:
    			newData = FdoDataValue::Create( valDouble, propType );
                break;
            }

            break;

		case FdoDataType_String:

            switch (propType) {
            case FdoDataType_DateTime:
				{
					newData = val;	// In case the conversion fail, return the same.

					FdoStringP	valString = ((FdoStringValue*)(FdoDataValue*)val)->GetString();
					const char	*timeStr = (const char *)valString;

					if( timeStr != NULL && *timeStr != '\0' )
					{
						const char		*fmt = valString.Contains(L":") ? "%4d-%02d-%02d %02d:%02d:%02d" : "%4d-%02d-%02d-%02d-%02d-%02d";
						int			year, month, day, hour, minute, seconds;

						year = month = day = hour = minute = seconds = 0;

						int count = sscanf(timeStr, fmt, &year, &month, &day, &hour, &minute, &seconds);     

						// Check the number of successfully read items
						if ( count >= 3 )
							newData = FdoDateTimeValue::Create(FdoDateTime((FdoInt16)year, (FdoInt8)month, (FdoInt8)day, 
																		  (FdoInt8)hour, (FdoInt8)minute, (float)seconds));
					}
				}
				break;
			// Other cases...

			default:
				;
			}

			break;
        }

        ret = newData;
	}
	return ret;
}

FdoStringCollection* FdoSmLpSchemaCollection::GetClassNames(FdoStringP schemaName)
{
    SchemaCollection            aTodo;

    /////////////////////////////////////////////
    // Setup

    mMappingClass.Clear();
    mMappingPropDef.Clear();

    if (schemaName != L"")
    {
        // Just want a specific schema (and referenced schemas)
        const FdoSmLpSchema*     pLpSchema = RefItem(schemaName);

        if (pLpSchema)
        {
            aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
        }
        else
        {
            throw FdoSchemaException::Create( 
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_26), 
					(FdoString*) schemaName
                )
            );
        }
    }
    else
    {
        // Want all schemas
        for (int iSchema=0; iSchema < GetCount(); iSchema++)
        {
            const FdoSmLpSchema*     pLpSchema = RefItem(iSchema);
            // retrieve metaclass schema only if explicitly asked for.
            if ( wcscmp(pLpSchema->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0 )
                aTodo.Add((FdoSmLpSchema*)pLpSchema);    // drop const
        }
    }

    /////////////////////////////////////////////
    // Process schemas

    FdoPtr<FdoStringCollection> featureClasses = FdoStringCollection::Create();

    FdoFeatureSchemasP pfscResult = FdoFeatureSchemaCollection::Create(NULL);
    bool bProcessFromRdClassReader = false;
    bool hasSchOv = false;
    for (int iSchema=0; iSchema < aTodo.GetCount(); iSchema++)
    {
        FdoPtr<FdoSmLpSchema> pLpSchema = aTodo.GetItem(iSchema);

        // Determine the type of the inner class reader
        FdoSmPhMgrP pPhSchemaMgr = pLpSchema->GetPhysicalSchema();
        FdoFeatureSchemasP configSchemas = pPhSchemaMgr->GetConfigSchemas();
        FdoSchemaMappingsP configMappings = pPhSchemaMgr->GetConfigMappings();
        FdoSmPhOwnerP pOwner = pPhSchemaMgr->GetOwner();
        if ( configMappings || configSchemas )
        {
            // Inner reader is a FdoSmPhCfgClassReader.
            bProcessFromRdClassReader = false;
            hasSchOv = true;
        }
        else
        {
            if (pOwner->GetHasClassMetaSchema())
            {
                // Inner reader is a FdoSmPhMtClassReader.
                bProcessFromRdClassReader = false;
            }
            else
            {
                // Inner reader is a FdoSmPhRdClassReader.
                bProcessFromRdClassReader = true;
            }
        }

        if (bProcessFromRdClassReader)
        {
            // Some providers (e.g. ODBC with SQL Server data source) map LogicalPhysical
            // Schemas and Physical Owners to Physical Schemas. In this case, we need to 
            // get the classes from the owner for LogicalPhysical Schema, rather than
            // from the default owner. GetPhysicalOwner() takes care of that.
            FdoSmPhOwnerP pSchemaOwner = pLpSchema->GetPhysicalOwner();

            // Bulk fetch the table and views, but skip bulk loading of columns and keys.
            pSchemaOwner->CacheDbObjects(false);

            FdoInt32 indexDbObject = 0;
            FdoSmPhDbObjectP pDbObject = pSchemaOwner->GetCachedDbObject(indexDbObject);
            FdoStringP className = L"";
            FdoStringP schemaName = L"";
            FdoStringP qname = L"";
            while (pDbObject != NULL)
            {
                className = pDbObject->GetBestClassName(pLpSchema->GetName());
                if ( ((const wchar_t*)className)[0] != '\0' )
                {
                    schemaName = pDbObject->GetBestSchemaName();
                    if (schemaName.GetLength() > 0)
                    {
                        qname = schemaName + L":" + className;
                    }
                    else
                    {
                        qname = className;
                    }
                    featureClasses->Add(qname);
                }
                pDbObject = pSchemaOwner->GetCachedDbObject(++indexDbObject);
            }
        }
        else
        {
            if (!hasSchOv)
                pLpSchema->GetFdoSmLpClassNames(featureClasses);
            else
            {
                const FdoSmLpClassCollection* pLpClassDefColl = pLpSchema->RefClasses();
            
                for (FdoInt32 index = 0; index < pLpClassDefColl->GetCount(); index++)
                {
                    const FdoSmLpClassDefinition* pLpClassDefinition = (FdoSmLpClassDefinition*)pLpClassDefColl->RefItem(index);
                    FdoStringP qname = pLpClassDefinition->GetQName();
                    featureClasses->Add(qname);
                }
            }
        }
    }

    /////////////////////////////////////////////
    // Done!

    return FDO_SAFE_ADDREF(featureClasses.p);
}

FdoFeatureSchemasP FdoSmLpSchemaCollection::GetFdoSchemasEx(FdoStringP schemaName, FdoStringCollection* featureClassNames)
{
    SchemaElementCollection   aTodo;

    /////////////////////////////////////////////
    // Setup

    mMappingClass.Clear();
    mMappingPropDef.Clear();
    mMappingSchema.Clear();

    // Get the schemas to look at from the input parameters 
    bool bIncludeAllSchemas = false;
    FdoPtr<FdoStringCollection> schemaNames = FdoStringCollection::Create();

    if (schemaName != L"")
    {
        schemaNames->Add(schemaName);
    }

    if (featureClassNames != NULL)
    {
        for (int iClassName = 0; iClassName < featureClassNames->GetCount(); iClassName++)
        {
            FdoPtr<FdoStringElement> elem = featureClassNames->GetItem(iClassName);
            FdoStringP className = elem->GetString();

            FdoStringP delimiter = L":";
            FdoStringP parsedSchemaName = className.Left(delimiter);

            if (parsedSchemaName.GetLength() < className.GetLength())
            {
                // Check that the schema name from the fully qualified class name matches the
                // passed in schema name (if any)
                if (schemaName != L"" && (schemaName != parsedSchemaName))
                {
                    // Throw exception.  Input schema name mismatch
                    throw FdoSchemaException::Create( 
                        FdoSmError::NLSGetMessage(
                            FDO_NLSID(FDOSM_423), 
					        (FdoString*) schemaName,
                            (FdoString*) className
                        )
                    );
                }

                if (schemaNames->IndexOf(parsedSchemaName) == -1)
                {
                    schemaNames->Add(parsedSchemaName);
                }

                // Get the unqualifed classname
                className = className.Right(delimiter);

                // Add table to the fetch candidates list
                FdoStringP tableName = GetPhysicalSchema()->ClassName2DbObjectName(parsedSchemaName, className);
                GetPhysicalSchema()->GetOwner()->AddCandDbObject(tableName);
            }
            else if ( ((const wchar_t*)schemaName)[0] != '\0' && parsedSchemaName == className )
            {
                // The classname is not qualified, so we'll use the passed in schema name and the passed in classname.
                FdoStringP tableName = GetPhysicalSchema()->ClassName2DbObjectName(schemaName, className);
                GetPhysicalSchema()->GetOwner()->AddCandDbObject(tableName);
            }
            else
            {
                // The schema name is not specified, so need to look at all schemas
                bIncludeAllSchemas = true;
            }
        }
    }


    FdoInt32 schemaCount = schemaNames->GetCount();
    for (int iSchema=0; iSchema < GetCount(); iSchema++)
    {
        const FdoSmLpSchema*     pLpSchema = RefItem(iSchema);
        FdoStringP currSchemaName = pLpSchema->GetName();

        if (featureClassNames)
        {
            for (int iClassNames = 0; iClassNames < featureClassNames->GetCount(); iClassNames++)
            {
                FdoPtr<FdoStringElement> elem = featureClassNames->GetItem(iClassNames);
                FdoStringP className = elem->GetString();
                FdoStringP delimiter = L":";
                FdoStringP tempName = className.Right(delimiter);

                if ( ((const wchar_t*)tempName)[0] == '\0' )
                {
                    if ( ((const wchar_t*)className)[0] != '\0' && ((const wchar_t*)currSchemaName)[0] != '\0' )
                    {
                        if ( (schemaName != L"") && (schemaName != currSchemaName) ) 
                            continue;
                        className = currSchemaName + L":" + className;
                    }
                }
                else 
                {
                    if ( currSchemaName != className.Left(delimiter) ) 
                        continue;
                }

                const FdoSmLpClassDefinition* pLpClassDef = pLpSchema->FindClass(className, false);
                if (pLpClassDef)
                {
                    mFoundCount++;
                    aTodo.classes.AddClassDefinition((FdoSmLpClassDefinition*)pLpClassDef);
                }
            }
        }

        if (schemaCount > 0 && !bIncludeAllSchemas)
        {
            // Only look at specific schemas
            for (int iSchemaName = 0; iSchemaName < schemaCount; iSchemaName++)
            {
                if (aTodo.schemas.GetCount() == schemaCount)
                {
                    continue;
                }
                if (FdoStringUtility::StringCompare(pLpSchema->GetName(), schemaNames->GetString(iSchemaName)) == 0)
                {
                    aTodo.schemas.Add((FdoSmLpSchema*)pLpSchema);
                }
            }
        }
        else
        {
            // Want all schemas but retrieve metaclass schema only if explicitly asked for.
            if ( wcscmp(pLpSchema->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0 )
                aTodo.schemas.Add((FdoSmLpSchema*)pLpSchema);   // drop const
        }
    }

    if ( (schemaCount > 0) && (aTodo.schemas.GetCount() != schemaCount) )
    {
        // A requested schema does not exist in the feature source
        // Identify the invalid schema name and throw an exception

        FdoPtr<FdoStringCollection> foundSchemaNames = FdoStringCollection::Create();

        // Gather the schema names that were found into a string collection
        int schemaIndex = 0;
        for (schemaIndex = 0; schemaIndex < aTodo.schemas.GetCount(); schemaIndex++)
        {
            FdoPtr<FdoSmLpSchema> pLpSchema  = aTodo.schemas.GetItem(schemaIndex);
            FdoStringP foundName = pLpSchema->GetName();
            foundSchemaNames->Add(foundName);
        }

        // Compare the collection of found names with the requested schema names
        for (schemaIndex = 0; schemaIndex < schemaNames->GetCount(); schemaIndex++)
        {
            FdoStringElementP schemaElement = schemaNames->GetItem(schemaIndex);
            FdoStringP requestedSchemaName = schemaElement->GetString();
            if (foundSchemaNames->IndexOf(requestedSchemaName) == -1)
            {
                // The requested schema does not exist
                // Throw exception
                throw FdoSchemaException::Create( 
                    FdoSmError::NLSGetMessage(
                        FDO_NLSID(FDOSM_26), 
			            (FdoString*) requestedSchemaName
                    )
                );
            }
        }
    }

    /////////////////////////////////////////////
    // Process schemas

    FdoFeatureSchemasP pfscResult = FdoFeatureSchemaCollection::Create(NULL);
    SchemaElementCollection aReferenced;

    if (!featureClassNames || featureClassNames->GetCount() == 0)
    {
        for (int iSchema = 0; iSchema < aTodo.schemas.GetCount(); iSchema++)
        {
            FdoPtr<FdoSmLpSchema> pLpSchema = aTodo.schemas.GetItem(iSchema);
            FdoFeatureSchema* pFdoFeatureSchema = ConvertSchema(pLpSchema, NULL, aReferenced);
            if (pFdoFeatureSchema)
            {
                pfscResult->Add(pFdoFeatureSchema);
                pFdoFeatureSchema->AcceptChanges();
            }

            // Add any new schemas referenced by this
            // one to the list of schemas to convert.
            aTodo.Merge(aReferenced);
            pFdoFeatureSchema->Release();
        }
    }
    else
    {
        for (int iClass = 0; iClass < aTodo.classes.GetCount(); iClass++)
        {
            const FdoSmLpClassDefinition* pLpClassDef = aTodo.classes.RefClassDefinition(iClass);
            FdoFeatureSchema* pFdoFeatureSchema = ConvertSchema(pLpClassDef->RefLogicalPhysicalSchema(), pLpClassDef, aReferenced);

            if (pFdoFeatureSchema)
            {
                if (!pfscResult->Contains(pFdoFeatureSchema))
                {
                    pfscResult->Add(pFdoFeatureSchema);
                    pFdoFeatureSchema->AcceptChanges(); // resets all ElementStates to Unchanged
                }

                // Add any new schema's referenced by this 
                // one to the list of schemas to convert.
                // NOTE: New schemas are added to the end 
                //       of the list, so this is safe 
                //       during a for-loop.
                aTodo.Merge(aReferenced);
            }
            pFdoFeatureSchema->Release();
        }
    }

    if (featureClassNames && (featureClassNames->GetCount() != mFoundCount))
    {
        // A requested class does not exist in the schema
        // Identify the invalid class and throw an exception

        FdoPtr<FdoStringCollection> foundClassNames = FdoStringCollection::Create();
        FdoPtr<FdoStringCollection> foundQClassNames = FdoStringCollection::Create();

        int classIndex = 0;
        // Gather the class names that were found into a string collection.
        for (int schemaIndex = 0; schemaIndex < pfscResult->GetCount(); schemaIndex++)
        {
            FdoPtr<FdoFeatureSchema> foundSchema = pfscResult->GetItem(schemaIndex);
            FdoPtr<FdoClassCollection> foundClasses = foundSchema->GetClasses();

            for (classIndex = 0; classIndex < foundClasses->GetCount(); classIndex++)
            {
                FdoPtr<FdoClassDefinition> foundClassDef = foundClasses->GetItem(classIndex);
                FdoStringP foundQname = foundClassDef->GetQualifiedName();
                FdoStringP foundName = foundClassDef->GetName();
                foundQClassNames->Add(foundQname);
                foundClassNames->Add(foundName);
            }
        }

        // Compare the collection of found names with the requested classnames.
        for (classIndex = 0; classIndex < featureClassNames->GetCount(); classIndex++)
        {
            FdoPtr<FdoStringElement> elem = featureClassNames->GetItem(classIndex);
            FdoStringP requestedClassName = elem->GetString();
            if (foundQClassNames->IndexOf(requestedClassName) == -1 &&
                foundClassNames->IndexOf(requestedClassName) == -1)
            {
                // The requested class does not exist
                // Throw exception.
                throw FdoSchemaException::Create( 
                    FdoSmError::NLSGetMessage(
                        FDO_NLSID(FDOSM_424), 
				        (FdoString*) requestedClassName
                    )
                );
            }
        }
    }
    
    /////////////////////////////////////////////
    // Done!

    return pfscResult;    // returned with refcount == 1

}

FdoStringCollection* FdoSmLpSchemaCollection::GetSchemaNames()
{
    FdoPtr<FdoStringCollection> schemaNames = FdoStringCollection::Create();

    for (int iSchema=0; iSchema < GetCount(); iSchema++)
    {
        const FdoSmLpSchema*     pLpSchema = RefItem(iSchema);
        // retrieve metaclass schema only if explicitly asked for.
        if ( wcscmp(pLpSchema->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0 )
            schemaNames->Add(pLpSchema->GetName());
    }

    return FDO_SAFE_ADDREF(schemaNames.p);
}

FdoFeatureSchema* FdoSmLpSchemaCollection::ConvertSchema(const FdoSmLpSchema *pLpSchema, const FdoSmLpClassDefinition *pLpClassDef, SchemaElementCollection &aReferenced)
{
    ASSERT(pLpSchema);

    if (!pLpClassDef)
    {
        return ConvertSchema(pLpSchema, aReferenced);
    }

    FdoFeatureSchema* pFdoFeatureSchema = (FdoFeatureSchema*) mMappingSchema.Map(pLpSchema);
    FdoClassCollection* pFdoClassCollection;

    if (!pFdoFeatureSchema)
    {
        pFdoFeatureSchema = FdoFeatureSchema::Create(pLpSchema->GetName(), pLpSchema->GetDescription());
        ConvertSAD(pLpSchema, pFdoFeatureSchema);    
        mMappingSchema.Add(pLpSchema, pFdoFeatureSchema);
    }
    else
    {
        pFdoFeatureSchema->AddRef();
    }

    pFdoClassCollection = pFdoFeatureSchema->GetClasses();

    // Add the single class to the schema.
    // The requested classes must exist in the schema.  Otherwise a class not found exception is thrown.
    if (pLpClassDef)
    {
        FdoClassDefinition* pFdoClassDef = ConvertClassDefinition(pLpClassDef, aReferenced);
        pFdoClassCollection->Add(pFdoClassDef);
        pFdoClassDef->Release();
    }

    FDO_SAFE_RELEASE(pFdoClassCollection);

    return pFdoFeatureSchema;
}



