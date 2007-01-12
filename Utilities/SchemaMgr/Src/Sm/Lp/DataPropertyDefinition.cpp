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
#include <Sm/Lp/ClassDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/DataPropertyDefinition.h>
#include <Sm/Lp/DataTypeMapper.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Error.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>

const FdoStringP FdoSmLpDataPropertyDefinition::mClassNamePropName(L"ClassName", true);

const FdoStringP FdoSmLpDataPropertyDefinition::mSchemaNamePropName(L"SchemaName", true);



FdoSmLpDataPropertyDefinition::FdoSmLpDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) : 
	FdoSmLpSimplePropertyDefinition(propReader, parent),
	mLength(0),
	mScale(0),
	mPrecision(0),
    mDefaultValue( propReader->GetDefaultValue() ),
    mIsAutoGenerated( propReader->GetIsAutoGenerated() ),
    mIsRevisionNumber( propReader->GetIsRevisionNumber() ),
	mDataType( FdoSmLpDataTypeMapper::String2Type(propReader->GetDataType()) )
{

	int length = propReader->GetLength();
	int scale = propReader->GetScale();

	// FDO length is column length for these 3 types
	if ( mDataType == FdoDataType_String || mDataType == FdoDataType_BLOB || mDataType == FdoDataType_CLOB )
		mLength = length;

	// Decimal is different. Column length is FDO precision.
	if ( mDataType == FdoDataType_Decimal ) {
		mPrecision = length;
		mScale = scale;
	}

	mIdPosition = propReader->GetIdPosition();

}

FdoSmLpDataPropertyDefinition::FdoSmLpDataPropertyDefinition(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpSimplePropertyDefinition(pFdoProp, bIgnoreStates, parent),
	mLength(pFdoProp->GetLength()),
	mScale(pFdoProp->GetScale()),
	mPrecision(pFdoProp->GetPrecision()),
	mIdPosition(0),
    mDefaultValue( pFdoProp->GetDefaultValue() ),
    mIsAutoGenerated( pFdoProp->GetIsAutoGenerated() ),
    mIsRevisionNumber( false ),
	mDataType( pFdoProp->GetDataType())
{
    SetNullable( pFdoProp->GetNullable() );
}

FdoSmLpDataPropertyDefinition::FdoSmLpDataPropertyDefinition(
    FdoSmLpDataPropertyP pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpSimplePropertyDefinition(
        FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) pBaseProperty), 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
	mLength( pBaseProperty->GetLength() ),
	mScale( pBaseProperty->GetScale() ),
	mPrecision( pBaseProperty->GetPrecision() ),
	mIdPosition( 0 ),
    mDefaultValue( pBaseProperty->GetDefaultValue() ),
    mIsAutoGenerated( pBaseProperty->GetIsAutoGenerated() ),
    mIsRevisionNumber( bInherit ? pBaseProperty->GetIsRevisionNumber() : false ),
	mDataType( pBaseProperty->GetDataType() )
{

	FdoSmPhMgrP mgr = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	const FdoSmLpClassDefinition* pCurrClass = pTargetClass;
	FdoSmPhDbObjectP pPhDbObject = mgr->FindDbObject(pTargetClass->GetDbObjectName());

	// Identity properties are inherited so inherit id position as well.
	// However, metaschema identity properties are not automatically inherited.
	if ( bInherit && (wcscmp(pBaseProperty->GetLogicalPhysicalSchema()->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0 ) )
		SetIdPosition( pBaseProperty->GetIdPosition() );

    SetContainingDbObject(pPhDbObject, pTargetClass->GetDbObjectName());
}

FdoDataType FdoSmLpDataPropertyDefinition::GetDataType() const
{
    return mDataType;
}

int FdoSmLpDataPropertyDefinition::GetLength() const
{
    return mLength;
}

int FdoSmLpDataPropertyDefinition::GetPrecision() const
{
    return mPrecision;
}

int FdoSmLpDataPropertyDefinition::GetScale() const
{
    return mScale;
}

int FdoSmLpDataPropertyDefinition::GetIdPosition() const
{
	return mIdPosition;
}

FdoString* FdoSmLpDataPropertyDefinition::GetDefaultValue() const
{
    ((FdoSmLpDataPropertyDefinition*) this)->Finalize();

    return mDefaultValue;
}

bool FdoSmLpDataPropertyDefinition::GetIsAutoGenerated() const
{
    return mIsAutoGenerated;
}

bool FdoSmLpDataPropertyDefinition::GetIsRevisionNumber() const
{
    return mIsRevisionNumber;
}

FdoString* FdoSmLpDataPropertyDefinition::GetSequenceName() const
{
    return mSequenceName;
}

FdoDataType* FdoSmLpDataPropertyDefinition::GetSupportedAutoGeneratedTypes(FdoInt32& length)
{
    static FdoDataType autogen_types[] =
    {
        FdoDataType_Int64,
    };

    length = sizeof (autogen_types) / sizeof (FdoDataType);

    return (autogen_types);
}

FdoPropertyType FdoSmLpDataPropertyDefinition::GetPropertyType() const
{
	return FdoPropertyType_DataProperty;
}

void FdoSmLpDataPropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
	bool		same = true;

	if ( (pBaseProp->GetElementState() != FdoSchemaElementState_Deleted) &&
		 (GetElementState() != FdoSchemaElementState_Deleted) ) {

		// Not deleting, so make sure base and inherited property are compatible

		const FdoSmLpDataPropertyDefinition* pBaseDataProp = 
			FdoSmLpDataPropertyDefinition::Cast( pBaseProp );

		// SuperClass SetInherited validates property type.
		if ( pBaseDataProp ) {
			FdoDataType dataType = GetDataType();
			FdoDataType baseDataType = pBaseDataProp->GetDataType();

			// Inherited property cannot redefine data type or nullability
			if ( (dataType      != pBaseDataProp->GetDataType()) ||
				(GetNullable() != pBaseDataProp->GetNullable()) )
				same = false;

			// Also can't redefine length, precision or scale when relevant
			if ( same && (dataType == FdoDataType_String || dataType == FdoDataType_CLOB || dataType == FdoDataType_BLOB) ) {
				if ( GetLength() != pBaseDataProp->GetLength() )
					same = false;
			}

			if ( same && (dataType == FdoDataType_Decimal) ) {
  				if ( GetPrecision() != pBaseDataProp->GetPrecision() )
					same = false;

				if ( same && (GetScale() != pBaseDataProp->GetScale()) )
					same = false;
			}

            if (same && GetIsRevisionNumber() != pBaseDataProp->GetIsRevisionNumber() )
                same = false;

			// Log error if inherited property redefines base property.
			if ( !same ) 
				AddRedefinedError( pBaseDataProp );

            // Special case. AutoGeneration mismatch can easily happen when
            // a class contains a property called FeatId, so add a more
            // specific error.
            if ( GetIsAutoGenerated() != pBaseDataProp->GetIsAutoGenerated() ) {
                AddRedefinedAutoGenError( pBaseDataProp );
                same = false;
            }
		}		 
	}

	if ( same ) 
		FdoSmLpSimplePropertyDefinition::SetInherited( pBaseProp );
}

void FdoSmLpDataPropertyDefinition::SetIdPosition(int idPosition)
{
	mIdPosition = idPosition;
}

void FdoSmLpDataPropertyDefinition::SetDefaultValue(bool bOverrideColumn)
{
	if ( bOverrideColumn || !GetColumn() ) {
		if ( wcscmp( GetName(), mClassNamePropName ) == 0 ) 
			mDefaultValue = GetParent()->GetName();

		if ( wcscmp( GetName(), mSchemaNamePropName ) == 0 ) 
			mDefaultValue = GetLogicalPhysicalSchema()->GetName();
	}
}

void FdoSmLpDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
	FdoSmPhTableP pPhTable;
	FdoDataType dataType;

	FdoSmLpSimplePropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

	// Superclass Update logs property type conflict so just silently skip validation
	// at this level.
	if ( pFdoProp->GetPropertyType() == FdoPropertyType_DataProperty ) {
		FdoDataPropertyDefinition* pFdoDataProp = 
			(FdoDataPropertyDefinition*) pFdoProp;

        // ReadOnly is the only thing that can be modified. Set it on Add or Modify operation,
        // or when this object came from an FDO config doc.
		if ( GetElementState() == FdoSchemaElementState_Added || GetElementState() == FdoSchemaElementState_Modified ||
             GetIsFromFdo()
        )
			SetReadOnly( pFdoDataProp->GetReadOnly() );

		// Default values not yet supported.
		if ( (GetElementState() != FdoSchemaElementState_Deleted ) &&
			(pFdoDataProp->GetDefaultValue()) &&
			(wcslen(pFdoDataProp->GetDefaultValue()) > 0) )
			AddDefaultValueError();

        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
            VldAutoGenerated();
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {
			// Log a error for each thing that cannot be modified.

			if ( GetDataType() != pFdoDataProp->GetDataType() )
				AddDataTypeChangeError( pFdoDataProp->GetDataType() );

			// TODO: support changing from not-nullable to nullable.
			if ( GetNullable() != pFdoDataProp->GetNullable() )
				AddNullableChangeError();

			dataType = GetDataType();

			// TODO: support increasing length, precision or scale.
			if ( dataType == FdoDataType_String || dataType == FdoDataType_CLOB || dataType == FdoDataType_BLOB ) {
				if ( GetLength() != pFdoDataProp->GetLength() )
					AddLengthChangeError( pFdoDataProp->GetLength() );
			}

			if ( dataType == FdoDataType_Decimal ) {
				if ( GetPrecision() != pFdoDataProp->GetPrecision() )
					AddPrecisionChangeError( pFdoDataProp->GetPrecision() );

				if ( GetScale() != pFdoDataProp->GetScale() )
					AddScaleChangeError( pFdoDataProp->GetScale() );
			}
			 
			if ( GetIsAutoGenerated() != pFdoDataProp->GetIsAutoGenerated() )
    			AddAutoGeneratedChangeError();
		}
	}
}

void FdoSmLpDataPropertyDefinition::SynchPhysical(bool bRollbackOnly)
{	
	FdoSmPhMgrP pPhysical	        = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhDbObjectP pPhDbObject    = pPhysical->FindDbObject( GetContainingDbObjectName() );

    // Proceed if synchronizing everything or a change to this property has been rolled back.
	if ( pPhDbObject && (!bRollbackOnly || pPhysical->RefRollbackColumn(pPhDbObject->GetQName(), GetColumnName())) ) {
		
        // Proceed only if the property has a containing database object. Can't synchronize column if there is no table.
        // Proceed only if the property has no column if the nullabilities disagree (columns are nullified on delete).
		if ( !GetColumn() || (GetColumn()->GetNullable() != GetNullable()) ) {
            if ( GetErrors()->GetCount() == 0 )
                // Proceed only if data property is error-free.
                // Create a column for the property.
                CreateColumn( pPhDbObject );
		}
	}
}

void FdoSmLpDataPropertyDefinition::Commit( bool fromParent )
{
	FdoSmLpPropertyDefinition::Commit();

	FdoSmPhMgrP	                pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Get containing class.
	FdoSmLpClassDefinition*		pClass = (FdoSmLpClassDefinition*)(RefParentClass());
	
    // Get the highest level containing class. Different from containing class
	// when this property is nested within an object property.
	FdoSmLpClassDefinition*		pTopClass = (FdoSmLpClassDefinition*)(GetTopProperty()->RefParentClass());
    
    FdoSmPhPropertyWriterP       pWriter = pPhysical->GetPropertyWriter();

	int								columnSize = (mDataType == FdoDataType_Decimal ) ? mPrecision : mLength;
	int								columnScale = mScale;
	char*							pUser = "fdo_user"; //TODO: pConn->GetUser();
	FdoStringP				        dbUser(pUser);
	
//	delete[] pUser;

	// Format the position within the identityProperties. If position not positive then
	// this is not an identity property.

	switch ( GetElementState() ) {
	// Only Adds are handled here. Modifications and Deletions are handled
	// in FdoSmLpPropertyDefinition.
	case FdoSchemaElementState_Added:
		// Skip system properties ( ones not being added to the class table ).
		if ( FdoStringP(GetContainingDbObjectName()).ICompare(pClass->GetDbObjectName()) == 0 ) {

			pWriter->SetTableName( GetContainingDbObjectName() );
			pWriter->SetClassId( pTopClass->GetId() );
			pWriter->SetColumnName( GetColumnName() );
            pWriter->SetRootObjectName( GetRootColumnName() );
			pWriter->SetName( GetNestedName() );
            if ( mIdPosition > 0 ) 
    			pWriter->SetIdPosition( mIdPosition );
			pWriter->SetColumnType( GetColumn() ? GetColumn()->GetTypeName() : L"n/a"  );
			pWriter->SetLength(	columnSize );
			pWriter->SetScale( columnScale );
			pWriter->SetDataType( (FdoString*) FdoSmLpDataTypeMapper::Type2String(GetDataType()) );
			pWriter->SetIsNullable( GetNullable() );
			pWriter->SetIsFeatId( GetIsFeatId() );
			pWriter->SetIsSystem( GetIsSystem() );
			pWriter->SetIsReadOnly(	GetReadOnly() );
			pWriter->SetUser( dbUser );
			pWriter->SetDescription( GetDescription() );
			pWriter->SetIsFixedColumn( GetIsFixedColumn() );
			pWriter->SetIsColumnCreator( GetIsColumnCreator() );
			pWriter->SetIsAutoGenerated( GetIsAutoGenerated() );
            pWriter->SetSequenceName( FdoStringP(GetSequenceName()) );
            pWriter->SetIsRevisionNumber( GetIsRevisionNumber());
			pWriter->Add();
        }

        break;

    // Currently, the only supported modification is to change the column name,
    // which can happen when the MetaSchema is re-synched with the physical schema.
	case FdoSchemaElementState_Modified:
        pWriter->SetDescription( GetDescription() );
        pWriter->SetIsReadOnly( GetReadOnly() );
        pWriter->SetSequenceName( FdoStringP(GetSequenceName()) );
			pWriter->SetColumnName( GetColumnName() );
        pWriter->Modify( pTopClass->GetId(), GetNestedName() );

		break;

    }

}

FdoSmPhColumnP FdoSmLpDataPropertyDefinition::NewColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName )
{
    return NewColumn( dbObject, columnName, GetNullable(), GetRootColumnName() );
}

FdoSmPhColumnP FdoSmLpDataPropertyDefinition::NewColumn( 
    FdoSmPhDbObjectP dbObject, 
    FdoStringP columnName,
    bool nullable,
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column;
	bool featClass = false;
	FdoSmLpClassDefinition* parentClass = (FdoSmLpClassDefinition*)(RefParentClass());

	if (parentClass && parentClass->GetClassType() == FdoClassType_FeatureClass)
		featClass = true;

	// check for object property class
	const FdoSmLpObjectPropertyClass *objectPropertyClass = dynamic_cast<const FdoSmLpObjectPropertyClass *> (parentClass);
	
	if (objectPropertyClass)
		featClass = true;

	if (GetIsAutoGenerated() && !featClass)
	{
		// Check if there is already column defined as autoincrement.
		// If there is one, the new one cannot be set as autoincerement.
		FdoSmPhColumnsP pColumns = dbObject->GetColumns();
		bool found = false;
		for (int i=0; i<pColumns->GetCount() && !found; i++)
		{
			FdoSmPhColumnP pColumn = pColumns->GetItem(i);
			if (pColumn->GetAutoincrement() == true)
			{
				mIsAutoGenerated = false;
				found = true;
			}
		}			
	}
	// TO DO: Auto-generated column must be not NUll, 
	// but the below fix does not aways work.
	/*if (GetIsAutoGenerated() && nullable)
	{
		nullable = false;
		SetNullable(nullable);
	}*/

    // Create the appropriate physical column type based on the FDO data type.
	switch ( GetDataType() ) {
	case FdoDataType_Boolean:
		column = dbObject->CreateColumnBool(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_Byte:
		column = dbObject->CreateColumnByte(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_DateTime:
		column = dbObject->CreateColumnDate(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_Decimal:
		column = dbObject->CreateColumnDecimal(
            columnName,
            nullable,
            mPrecision,
            mScale,
            rootColumnName
        );
		break;

	case FdoDataType_Single:
		column = dbObject->CreateColumnSingle(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_Double:
		column = dbObject->CreateColumnDouble(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_Int16:
		column = dbObject->CreateColumnInt16(
            columnName,
            nullable,
			featClass ? (GetIsAutoGenerated() && GetIsFeatId()) : GetIsAutoGenerated(),
            rootColumnName
        );
		break;

	case FdoDataType_Int32:
		column = dbObject->CreateColumnInt32(
            columnName,
            nullable,
			featClass ? (GetIsAutoGenerated() && GetIsFeatId()) : GetIsAutoGenerated(),
            rootColumnName
        );
		break;

	case FdoDataType_Int64:
		column = dbObject->CreateColumnInt64(
			columnName,
			nullable,
			featClass ? (GetIsAutoGenerated() && GetIsFeatId()) : GetIsAutoGenerated(),
			rootColumnName
		);		
		break;

	case FdoDataType_String:
		column = dbObject->CreateColumnChar(
            columnName,
            nullable,
            mLength,
            rootColumnName
        );
		break;

	case FdoDataType_BLOB:
		column = dbObject->CreateColumnBLOB(
            columnName,
            nullable,
            rootColumnName
        );
		break;

	case FdoDataType_CLOB:
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_151),
				(FdoString*) GetQName()
			)
		);
		break;
		
	default:
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_152),
				(FdoString*) GetQName()
			)
		);
		break;
	}

    return column;
}

void FdoSmLpDataPropertyDefinition::XMLSerialize( FILE* xmlFp, int ref ) const
{
	if ( ref == 0 ) {
		fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\"\n dataType=\"%ls\" length=\"%d\" precision=\"%d\" scale=\"%d\"\n readOnly=\"%s\" featId=\"%s\" system=\"%s\" nullable=\"%s\" autogenerated=\"%s\" defaultValue=\"%ls\"\n tableName=\"%ls\" columnName=\"%ls\" colCreator=\"%s\" fixedCol=\"%s\"  >\n",
                (FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
				GetName(), GetDescription(),
				(FdoString*) FdoSmLpDataTypeMapper::Type2String(GetDataType()),
				GetLength(), GetPrecision(), GetScale(), 
				GetReadOnly() ? "True" : "False",
				GetIsFeatId() ? "True" : "False",
				GetIsSystem() ? "True" : "False",
				GetNullable() ? "True" : "False",
				GetIsAutoGenerated() ? "True" : "False",
				GetDefaultValue(),
				GetContainingDbObjectName(),
				GetColumnName(),
				GetIsColumnCreator() ? "True" : "False",
				GetIsFixedColumn() ? "True" : "False"
		);

		if ( RefDefiningClass() &&
			RefBaseProperty() ) 
			fprintf( xmlFp, "<Inherited baseClass=\"%ls\" />\n",
				RefDefiningClass()->GetName()
			);


		if ( RefColumn() ) 
			RefColumn()->XMLSerialize(xmlFp, ref);

        XMLSerializeSequence(xmlFp, ref );

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );

		fprintf( xmlFp, "</property>\n" );
	}
	else {
		fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" />\n",
                (FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
				GetName()
		);
	}
}

void FdoSmLpDataPropertyDefinition::XMLSerializeSequence( FILE* xmlFp, int ref ) const
{
}

void FdoSmLpDataPropertyDefinition::Finalize()
{
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmLpDataPropertyP pPrevProperty = 
        GetPrevProperty().p->SmartCast<FdoSmLpDataPropertyDefinition>();
    const FdoSmLpClassDefinition* pDefiningClass = RefDefiningClass();

	if ( GetState() != FdoSmObjectState_Final ) {

        FdoSmLpSimplePropertyDefinition::Finalize();

        // When defining class uses base table mapping, its data properties cannot
        // be null. Otherwise, objects that aren't of this class cannot be
        // added to the class table.
        // Skip this test for deleted properties since they're going away
        // Also skip for nested properties. This test is done in the corresponding
        // Object Property.
        if ( pDefiningClass && 
             (GetElementState() != FdoSchemaElementState_Deleted) &&
             (!FdoStringP(GetName()).Contains(L".")) &&
             (!GetBaseProperty()) &&
            (pDefiningClass->GetTableMapping() == FdoSmOvTableMappingType_BaseTable) &&
            (!GetNullable()) )
			AddNotNullBaseMappingError();

        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {

            if ( pPrevProperty && (FdoStringP(this->GetContainingDbObjectName()).ICompare(pPrevProperty->GetContainingDbObjectName()) == 0) ) {
    			// New property in same table as the previous property. New property
	    		// just latches onto previous property column.
		    	SetColumn( pPrevProperty->GetColumn() );
            }
            else {
			    // Tables are different or there is no previous property. Must create new column for this property.
			    FdoSmPhDbObjectP dbObject = GetContainingDbObject();
			    CreateColumn(dbObject);
            }
        }
        else {
            // Get the column only if the dataproperty's table exists.
			if ( GetContainingDbObject() )  {
				FdoSmPhColumnsP	columns = GetContainingDbObject()->GetColumns();
		        SetColumn( columns->FindItem(GetColumnName()) );
			}
 
            // If column not set and this is an inherited system property other than featid, set the
            // column and containing table back to that of the parent property.

            if ( (!GetColumn()) && (!GetSrcProperty()) && 
                 (GetElementState() != FdoSchemaElementState_Deleted) &&
                 (!GetIsFeatId()) &&
                 (wcscmp(GetName(),L"ClassId") != 0) &&
                 (wcscmp(GetName(),L"RevisionNumber") != 0) &&
                 (wcscmp(RefDefiningClass()->RefLogicalPhysicalSchema()->GetName(), FdoSmPhMgr::mMetaClassSchemaName) == 0)
            ) {
                FdoSmLpDataPropertyP pPrevProp = 
                    GetPrevProperty().p->SmartCast<FdoSmLpDataPropertyDefinition>();

                if ( pPrevProp && pPrevProp->GetColumn() ) {
                    SetColumn( pPrevProp->GetColumn() );
                    SetContainingDbObject( pPrevProp->GetContainingDbObject(), pPrevProp->GetContainingDbObjectName() );
                }
            }

            SetDefaultValue();

            // Delete column only if this property created it.
	        if ( GetColumn() && 
                 GetIsColumnCreator() && 
                 (GetElementState() == FdoSchemaElementState_Deleted) 
             ) {

                // Set the column for deletion.
		        FdoStringP dbObjectName = GetContainingDbObjectName();
		        FdoStringP baseDbObjectName;

		        if ( GetBaseProperty() ) 
			        baseDbObjectName = GetPrevProperty()->GetContainingDbObjectName();

		        // Keep the column if it is used by the base property.
		        if ( dbObjectName.ICompare(baseDbObjectName) != 0 ) 
			        ((FdoSmPhColumn*) GetColumn())->SetElementState( GetElementState() );
	        }
        }
    }

}

void FdoSmLpDataPropertyDefinition::VldAutoGenerated() 
{
    if ( mIsAutoGenerated ) {
        FdoInt32 typesLen;
        bool found = false;
        int idx;
        FdoDataType* autogen_types = GetSupportedAutoGeneratedTypes(typesLen);

        for ( idx = 0; idx < typesLen; idx++ ) {
            if ( GetDataType() == autogen_types[idx] ) {
                found = true;
                break;
            }
        }

        if ( !found ) 
            AddAutoGenDataTypeError();
    }
}

FdoSchemaExceptionP FdoSmLpDataPropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    FdoSchemaExceptionP pException = FdoSmLpPropertyDefinition::Errors2Exception(pFirstException);
/* TODO: Move up to Oracle Schema Manager.    
    FdoStringP seqName = GetSequenceName();
    FdoSmLpSchema* schema = ((FdoSmLpDataPropertyDefinition*) this)->Get_LogicalPhysicalSchema();
    if (seqName.GetLength() > 0)
    {
        FdoSmPhysicalSchema* pPhysical = schema->Get_PhysicalSchema();
        FdoSmPhSequence* pSeq = pPhysical->Get_Sequence(seqName);
        if (pSeq)
            pException = pSeq->Errors2Exception(pFirstException);
    }
*/
	return pException;
}

void FdoSmLpDataPropertyDefinition::AddDataTypeChangeError( 
	FdoDataType newType
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_153),
			    (FdoString*) GetQName(), 
			    (FdoString*) FdoSmLpDataTypeMapper::Type2String(mDataType),
			    (FdoString*) FdoSmLpDataTypeMapper::Type2String(newType)
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddNullableChangeError()
{
	if ( GetNullable() ) {
		GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_154),
				    (FdoString*) GetQName()
                )
			)
		);
	}
	else {
		GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_155),
				    (FdoString*) GetQName()
			    )
            )
		);
	}
}

void FdoSmLpDataPropertyDefinition::AddLengthChangeError( 
	int newLength
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_156),
			    (FdoString*) GetQName(), 
			    mLength,
			    newLength
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddPrecisionChangeError( 
	int newPrecision
)
{
    //Cannot change precision for property %1$ls from %2$d to %3$d",
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_157),
			    (FdoString*) GetQName(), 
			    mPrecision,
			    newPrecision
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddScaleChangeError( 
	int newScale
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_158),
			    (FdoString*) GetQName(), 
			    mScale,
			    newScale
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddColNameChangeError( 
	FdoStringP newColName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_299),
			    (FdoString*) GetQName(), 
			    GetColumnName(),
			    (FdoString*) newColName
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddDefaultValueError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_244),
			    (FdoString*) GetQName()
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddNotNullBaseMappingError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_197),
			    (FdoString*) GetQName(),
                (FdoString*) GetParent()->GetQName()
		    )
        )
	);
}

void FdoSmLpDataPropertyDefinition::AddAutoGeneratedChangeError()
{
	if ( GetIsAutoGenerated() ) {
		GetErrors()->Add( FdoSmErrorType_AutoGen, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_339),
				    (FdoString*) GetQName()
			    )
            )
		);
	}
	else {
		GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_338),
				    (FdoString*) GetQName()
                )
			)
		);
	}
}

void FdoSmLpDataPropertyDefinition::AddAutoGenDataTypeError()
{
	GetErrors()->Add( FdoSmErrorType_AutoGen, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_340),
			    (FdoString*) GetQName(),
                (FdoString*) FdoSmLpDataTypeMapper::Type2String(GetDataType())
            )
		)
	);
}

void FdoSmLpDataPropertyDefinition::AddRedefinedAutoGenError( const FdoSmLpDataPropertyDefinition* pBaseProp )
{
	GetErrors()->Add( FdoSmErrorType_AutoGen, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_351),
			    (FdoString*) GetQName(),
                (FdoString*) pBaseProp->GetQName()
            )
		)
	);
}
