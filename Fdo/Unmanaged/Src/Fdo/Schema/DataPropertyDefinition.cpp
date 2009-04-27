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
#include <Fdo/Schema/DataPropertyDefinition.h>
#include <Fdo/Schema/SchemaException.h>
#include "StringUtility.h"
#include "DataTypeMapper.h"
#include "XmlContext.h"
#include <Fdo/Schema/PropertyValueConstraintRange.h>
#include <Fdo/Schema/PropertyValueConstraintList.h>
#include <Fdo/Expression/DataValueCollection.h>
#include <Fdo/Expression/DataValue.h>

FdoDataPropertyDefinition* FdoDataPropertyDefinition::Create()
{
    return new FdoDataPropertyDefinition();
}

FdoDataPropertyDefinition* FdoDataPropertyDefinition::Create(FdoString* name, FdoString* description, bool system)
{
    return new FdoDataPropertyDefinition(name, description, system);
}

FdoDataPropertyDefinition::~FdoDataPropertyDefinition()
{
    if (m_defaultValueCHANGED && m_defaultValue != m_defaultValueCHANGED)
        FdoStringUtility::ClearString(m_defaultValueCHANGED);

    FdoStringUtility::ClearString(m_defaultValue);
	FDO_SAFE_RELEASE( m_propertyValueConstraint );
	FDO_SAFE_RELEASE( m_propertyValueConstraintCHANGED );
}

void FdoDataPropertyDefinition::Dispose()
{
    delete this;
}

FdoDataPropertyDefinition::FdoDataPropertyDefinition() : FdoPropertyDefinition()
{
    Init();
}

FdoDataPropertyDefinition::FdoDataPropertyDefinition(FdoString* name, FdoString* description, bool system) :
    FdoPropertyDefinition(name, description, system)
{
    Init();
}

void FdoDataPropertyDefinition::Init()
{
    m_dataType = FdoDataType_String; 
    m_readOnly = false;
    m_length = 0;
    m_precision = 0;
    m_scale = 0;
    m_nullable = true;
    m_defaultValue = NULL;
    m_autogenerated = false;
	m_propertyValueConstraint = NULL;

    m_dataTypeCHANGED = FdoDataType_String; 
    m_readOnlyCHANGED = false;
    m_lengthCHANGED = 0;
    m_precisionCHANGED = 0;
    m_scaleCHANGED = 0;
    m_nullableCHANGED = true;
    m_defaultValueCHANGED = NULL;
    m_autogeneratedCHANGED = false;
	m_propertyValueConstraintCHANGED = NULL;
}

FdoPropertyType FdoDataPropertyDefinition::GetPropertyType()
{
    return FdoPropertyType_DataProperty;
}

FdoDataType FdoDataPropertyDefinition::GetDataType()
{
    return m_dataType;
}

void FdoDataPropertyDefinition::SetDataType(FdoDataType dataType)
{
    _StartChanges();
    m_dataType = dataType;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoDataPropertyDefinition::GetReadOnly()
{
    return m_readOnly;
}

void FdoDataPropertyDefinition::SetReadOnly(bool value)
{
    // Autogenerated properties cannot be writable.
    if ( m_autogenerated && !value ) 
        throw FdoSchemaException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SCHEMA_54_AUTOGENREADONLY),
                (FdoString*) GetQualifiedName()
            )
        );

    _StartChanges();
    m_readOnly = value;
    SetElementState(FdoSchemaElementState_Modified);
}

FdoInt32 FdoDataPropertyDefinition::GetLength()
{
    return m_length;
}

void FdoDataPropertyDefinition::SetLength(FdoInt32 value)
{
    if (value < 0)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_15_NEGATIVELENGTHNOTALLOWED)));
    else
    {
        _StartChanges();
        m_length = value;
        SetElementState(FdoSchemaElementState_Modified);
    }
}

FdoInt32 FdoDataPropertyDefinition::GetPrecision()
{
    return m_precision;
}

void FdoDataPropertyDefinition::SetPrecision(FdoInt32 value)
{
    if (value < 0)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_16_NEGATIVEPRECISIONNOTALLOWED)));
    else
    {
        _StartChanges();
        m_precision = value;
        SetElementState(FdoSchemaElementState_Modified);
    }
}

FdoInt32 FdoDataPropertyDefinition::GetScale()
{
    return m_scale;
}

void FdoDataPropertyDefinition::SetScale(FdoInt32 value)
{
    _StartChanges();
    m_scale = value;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoDataPropertyDefinition::GetNullable()
{
    return m_nullable;
}

void FdoDataPropertyDefinition::SetNullable(bool value)
{
    _StartChanges();
    m_nullable = value;
    SetElementState(FdoSchemaElementState_Modified);
}

FdoString* FdoDataPropertyDefinition::GetDefaultValue()
{
    return m_defaultValue;
}

void FdoDataPropertyDefinition::SetDefaultValue(FdoString* value)
{
    _StartChanges();

    if (m_defaultValue != NULL)
    {
        if (m_defaultValueCHANGED != m_defaultValue)
            FdoStringUtility::ClearString(m_defaultValue);
    }

    m_defaultValue = FdoStringUtility::MakeString(value);
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoDataPropertyDefinition::SetIsAutoGenerated(bool value)
{
    _StartChanges();
    m_autogenerated = value;

    // Automatically set autogenerated properties to be readonly
    if ( value ) 
        m_readOnly = true;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoDataPropertyDefinition::GetIsAutoGenerated()
{
    return m_autogenerated;
}

///<summary>Returns the value constraint of this data property.</summary>
/// <returns>Returns the FdoPropertyValueConstraint or NULL if not set.</returns> 
FdoPropertyValueConstraint* FdoDataPropertyDefinition::GetValueConstraint()
{
	FDO_SAFE_ADDREF( m_propertyValueConstraint );
	return m_propertyValueConstraint;
}

///<summary>Sets the value constraint of this data property.</summary>
/// <param name="value">Set the value constraint of this data property.</param> 
/// <returns>Returns nothing</returns>
void FdoDataPropertyDefinition::SetValueConstraint( FdoPropertyValueConstraint* value )
{
	_StartChanges();
	FDO_SAFE_RELEASE(m_propertyValueConstraint);
    FDO_SAFE_ADDREF(value);
    m_propertyValueConstraint = value;
	SetElementState(FdoSchemaElementState_Modified);
}

void FdoDataPropertyDefinition::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoPropertyDefinition::_StartChanges();

        m_dataTypeCHANGED = m_dataType;
        m_readOnlyCHANGED = m_readOnly;
        m_lengthCHANGED = m_length;
        m_precisionCHANGED = m_precision;
        m_scaleCHANGED = m_scale;
        m_nullableCHANGED = m_nullable;
        m_defaultValueCHANGED = m_defaultValue;
        m_autogeneratedCHANGED = m_autogenerated;
		FDO_SAFE_RELEASE(m_propertyValueConstraintCHANGED);
		m_propertyValueConstraintCHANGED = FDO_SAFE_ADDREF( m_propertyValueConstraint );
    }
}

void FdoDataPropertyDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        m_dataType = m_dataTypeCHANGED;
        m_readOnly = m_readOnlyCHANGED;
        m_length = m_lengthCHANGED;
        m_precision = m_precisionCHANGED;
        m_scale = m_scaleCHANGED;
        m_nullable = m_nullableCHANGED;
        if (m_defaultValue != m_defaultValueCHANGED)
        {
            if (m_defaultValue)
                FdoStringUtility::ClearString(m_defaultValue);
            m_defaultValue = m_defaultValueCHANGED;
        }
        m_autogenerated = m_autogeneratedCHANGED;

        // reset
        m_defaultValueCHANGED = NULL;
		FDO_SAFE_RELEASE(m_propertyValueConstraint);
		m_propertyValueConstraint = FDO_SAFE_ADDREF(m_propertyValueConstraintCHANGED);
		FDO_SAFE_RELEASE(m_propertyValueConstraintCHANGED);
		m_propertyValueConstraintCHANGED = NULL;
    }
}

void FdoDataPropertyDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        if (m_defaultValue != m_defaultValueCHANGED && m_defaultValueCHANGED)
            FdoStringUtility::ClearString(m_defaultValueCHANGED);
        m_defaultValueCHANGED = NULL;

		FDO_SAFE_RELEASE( m_propertyValueConstraintCHANGED );
        m_propertyValueConstraintCHANGED = NULL;
    }
}

void FdoDataPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaMergeContext* pContext )
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    // Base function catches property type mismatch so silently quit on type mismatch
    if ( GetPropertyType() == pProperty->GetPropertyType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pProperty->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoDataPropertyDefinition* pDataProperty = (FdoDataPropertyDefinition*) pProperty;

            // Set each member from the given data property. The same pattern is followed 
            // for each:
            //
            // If new and old values differ
            //    If modification allowed (always allowed if this is a new property).
            //      Perform the modification
            //    else
            //      log an error
            //    end if
            //  end if

            // Data Type

            if ( GetDataType() != pDataProperty->GetDataType() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataType(pDataProperty)) ) 
                    SetDataType( pDataProperty->GetDataType() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_87_MODDATATYPE),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoDataTypeMapper::Type2String(GetDataType()),
                                    (FdoString*) FdoDataTypeMapper::Type2String(pDataProperty->GetDataType())
                                )
                            )
                        )
                    );
            }

            // Default value

            if ( FdoStringP(GetDefaultValue()) != FdoStringP(pDataProperty->GetDefaultValue()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDefaultValue(pDataProperty)) ) 
                    SetDefaultValue( pDataProperty->GetDefaultValue() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_88_MODDEFAULTVALUE),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetDefaultValue()),
                                    (FdoString*) FdoStringP(pDataProperty->GetDefaultValue())
                                )
                            )
                        )
                    );
            }

            // Maximum length

            if ( GetLength() != pDataProperty->GetLength() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataLength(pDataProperty)) ) 
                    SetLength( pDataProperty->GetLength() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_89_MODDATALENGTH),
                                    (FdoString*) GetQualifiedName(),
                                    GetLength(),
                                    pDataProperty->GetLength()
                                )
                            )
                        )
                    );
            }

            // Nullability

            if ( GetNullable() != pDataProperty->GetNullable() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataNullable(pDataProperty)) ) 
                    SetNullable( pDataProperty->GetNullable() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pDataProperty->GetNullable() ? SCHEMA_90_MODPROPNULLABLE : SCHEMA_91_MODPROPNNULLABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Decimal precision

            if ( GetPrecision() != pDataProperty->GetPrecision() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataPrecision(pDataProperty)) ) 
                    SetPrecision( pDataProperty->GetPrecision() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_92_MODDATAPRECISION),
                                    (FdoString*) GetQualifiedName(),
                                    GetPrecision(),
                                    pDataProperty->GetPrecision()
                                )
                            )
                        )
                    );
            }

            // Decimal Scale

            if ( GetScale() != pDataProperty->GetScale() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataScale(pDataProperty)) ) 
                    SetScale( pDataProperty->GetScale() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_93_MODDATASCALE),
                                    (FdoString*) GetQualifiedName(),
                                    GetScale(),
                                    pDataProperty->GetScale()
                                )
                            )
                        )
                    );
            }
            // Autogenerated must be set before read only.
            // Otherwise an exception will be thrown if this property
            // is writable and pDataProperty is autogenerated.
            if ( GetIsAutoGenerated() != pDataProperty->GetIsAutoGenerated() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataAutoGenerated(pDataProperty)) ) 
                    SetIsAutoGenerated( pDataProperty->GetIsAutoGenerated() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_94_MODDATAAUTOGEN),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // ReadOnly

            if ( GetReadOnly() != pDataProperty->GetReadOnly() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModDataReadOnly(pDataProperty)) ) 
                    SetReadOnly( pDataProperty->GetReadOnly() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pDataProperty->GetReadOnly() ? SCHEMA_95_MODPROPRDONLY : SCHEMA_96_MODPROPWRITABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Constraint

            FdoPtr<FdoPropertyValueConstraint> constraint = pDataProperty->GetValueConstraint();
            
            // No change if both old and new constraints are null.
            if ( m_propertyValueConstraint || constraint ) {
                bool constraintChanged = (((m_propertyValueConstraint == NULL) != (constraint == NULL)) || (!m_propertyValueConstraint->Equals(constraint)) ); 
                if ( constraintChanged ) {
                    if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CheckModDataConstraint(this, pDataProperty)) ) {
                        SetValueConstraint( (FdoPropertyValueConstraint*) NULL );
                        if ( constraint ) {
                            if ( constraint->GetConstraintType() == FdoPropertyValueConstraintType_Range )
                                m_propertyValueConstraint = FdoPropertyValueConstraintRange::Create();
                            else if ( constraint->GetConstraintType() == FdoPropertyValueConstraintType_List ) 
                                m_propertyValueConstraint = FdoPropertyValueConstraintList::Create();
                            else
                                pContext->AddError( 
                                    FdoSchemaExceptionP(
                                        FdoSchemaException::Create(
                                            FdoException::NLSGetMessage(
                                                FDO_NLSID(SCHEMA_97_BADCONSTRAINTTYPE),
                                                (FdoString*) GetQualifiedName(),
                                                constraint->GetConstraintType()
                                            )
                                        )
                                    )
                                );

                            if ( m_propertyValueConstraint )
                                m_propertyValueConstraint->Set( constraint, GetQualifiedName(), pContext );
                        }
                    }
                }
            }
        }
    }
}

void FdoDataPropertyDefinition::InitFromXml(FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Error if FDO and XML property types are not the same.
    if ( wcscmp( propertyTypeName, L"DataProperty" ) != 0 ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    // Initialize generic parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset data property attributes to defaults.
    m_dataType = FdoDataType_String; 
    m_readOnly = false;
    m_length = 0;
    m_precision = 0;
    m_scale = 0;
    m_nullable = true;
    m_defaultValue = NULL;
    m_autogenerated = false;
	m_propertyValueConstraint = NULL;

    // Set Data Property from the XML attributes.

    FdoXmlAttributeP attr = attrs->FindItem( L"dataType" );

    if ( attr != NULL ) 
        SetDataType(FdoDataTypeMapper::String2Type( attr->GetValue() ));

    attr = attrs->FindItem( L"readOnly" );
    if ( attr != NULL )
        SetReadOnly( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"length" );
    if ( attr != NULL )
        SetLength( FdoStringP(attr->GetValue()).ToLong());

    attr = attrs->FindItem( L"precision" );
    if ( attr != NULL )
        SetPrecision( FdoStringP(attr->GetValue()).ToLong());

    attr = attrs->FindItem( L"scale" );
    if ( attr != NULL )
        SetScale( FdoStringP(attr->GetValue()).ToLong());

    attr = attrs->FindItem( L"nullable" );
    if ( attr != NULL )
        SetNullable( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"default" );
    if ( attr != NULL )
    {
        FdoStringP defaultValue = attr->GetValue();
        if ( (defaultValue == L"") || (GetDataType() == FdoDataType_String) ) 
            SetDefaultValue( defaultValue );
        else
        {
            if (GetNullable() && defaultValue == L"NULL")
                SetDefaultValue( NULL);
            else
                SetDefaultValue( FdoPtr<FdoDataValue>(FdoDataValue::Create(defaultValue, GetDataType()))->ToString() );
        }
    }

    attr = attrs->FindItem( L"autogenerated" );
    if ( attr != NULL )
        SetIsAutoGenerated( FdoStringP(attr->GetValue()).ToBoolean());

}

FdoXmlSaxHandler* FdoDataPropertyDefinition::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoSchemaElement::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;

    if ( !pRet ) {
        // Handle the elements related with Constraint
        if ( wcscmp(name, L"Constraint") == 0 ) {

			if ( m_propertyValueConstraint != NULL )
			{
				FDO_SAFE_RELEASE(m_propertyValueConstraint);
			}

			FdoXmlAttributeP attr = atts->FindItem( L"type" );
			if ( attr != NULL )
			{
				FdoStringP type = attr->GetValue();
				if ( type == L"list" )
				{
					m_propertyValueConstraint = FdoPropertyValueConstraintList::Create();
				}
				else
				{
					m_propertyValueConstraint = FdoPropertyValueConstraintRange::Create();
				}
			}
        }

        if ( wcscmp(name, L"Value") == 0 ) 
		{
            // Set up a SAX handler to read the value from the element content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }

        if ( wcscmp(name, L"Max") == 0 ) 
		{
			FdoXmlAttributeP attr = atts->FindItem( L"inclusive" );
			if ( attr != NULL )
			{
				((FdoPropertyValueConstraintRange*) m_propertyValueConstraint)->SetMaxInclusive( FdoStringP(attr->GetValue()).ToBoolean() );
			}

			attr = atts->FindItem( L"value" );
			if ( attr != NULL )
			{
				((FdoPropertyValueConstraintRange*) m_propertyValueConstraint)->SetMaxValue( FdoPtr<FdoDataValue>(FdoDataValue::Create( attr->GetValue(), GetDataType() )));
			}
        }

        if ( wcscmp(name, L"Min") == 0 ) 
		{
			FdoXmlAttributeP attr = atts->FindItem( L"inclusive" );
			if ( attr != NULL )
			{
				((FdoPropertyValueConstraintRange*) m_propertyValueConstraint)->SetMinInclusive( FdoStringP(attr->GetValue()).ToBoolean() );
			}

			attr = atts->FindItem( L"value" );
			if ( attr != NULL )
			{
				((FdoPropertyValueConstraintRange*) m_propertyValueConstraint)->SetMinValue( FdoPtr<FdoDataValue>(FdoDataValue::Create( attr->GetValue(), GetDataType() )));
			}

        }
    }

    return(pRet);
}

FdoBoolean FdoDataPropertyDefinition::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname 
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoSchemaElement::XmlEndElement(context, uri, name, qname);

    if ( wcscmp(name, L"Value") == 0 ) 
	{
		FdoDataValueP dValue = FdoDataValue::Create( m_XmlContentHandler->GetString(), GetDataType() );
		FdoDataValueCollectionP dvCol = ((FdoPropertyValueConstraintList*) m_propertyValueConstraint)->GetConstraintList();
		dvCol->Add(dValue);
    }

    return(false);
}

void FdoDataPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write a data property element
    writer->WriteStartElement( L"DataProperty" );

    // Write the data property attributes
    writer->WriteAttribute( L"dataType", FdoDataTypeMapper::Type2String( m_dataType ) );

    if ( m_readOnly ) 
        writer->WriteAttribute( L"readOnly", L"true" );

    if ( m_length > 0 ) 
        writer->WriteAttribute( L"length", FdoStringP::Format(L"%d", m_length) );

    if ( m_precision > 0 ) 
        writer->WriteAttribute( L"precision", FdoStringP::Format(L"%d", m_precision) );

    if ( m_scale != 0 ) 
        writer->WriteAttribute( L"scale", FdoStringP::Format(L"%d", m_scale) );

    writer->WriteAttribute( L"nullable", m_nullable ? L"true" : L"false" );
    
    if ( m_defaultValue )
    {
        if ( (GetDataType() == FdoDataType_String) || (wcslen(m_defaultValue) == 0) )
        {
            writer->WriteAttribute( L"default", m_defaultValue );
        }
        else
        {
            try {
                FdoPtr<FdoExpression> expr = FdoExpression::Parse( m_defaultValue );
                FdoDataValue* dv = FDO_SAFE_ADDREF(dynamic_cast<FdoDataValue*>(expr.p));

                if ( dv )
                    writer->WriteAttribute( L"default", dv->GetXmlValue() );
                else
                    writer->WriteAttribute( L"default", m_defaultValue );
            }
            catch ( FdoException* ) {
                // Parse exception message too general for this case
                // so not incorporated into error message.
                writer->WriteAttribute( L"default", m_defaultValue );
            }
        }
    }

    if ( m_autogenerated ) 
        writer->WriteAttribute( L"autogenerated", L"true");

    // Write the generic attributes and sub-elements
    FdoPropertyDefinition::_writeXml(pContext);
	
	if ( m_propertyValueConstraint != NULL )
	{
		// Write the data constraint
		writer->WriteStartElement( L"Constraint" );
		
		switch ( m_propertyValueConstraint->GetConstraintType() )
		{
		case FdoPropertyValueConstraintType_List:
			{
				writer->WriteAttribute( L"type", L"list");

				// Write list values element
				writer->WriteStartElement( L"Values" );
				
				FdoDataValueCollectionP dvCol = ((FdoPropertyValueConstraintList*) m_propertyValueConstraint)->GetConstraintList();
				for ( int i = 0; i < dvCol->GetCount(); i++ )
				{
					FdoDataValueP dv = dvCol->GetItem(i);

					writer->WriteStartElement( L"Value" );
					FdoStringP pData = dv->GetXmlValue();
                    writer->WriteCharacters(pData);
					writer->WriteEndElement();
				}

				writer->WriteEndElement();
			}
			break;
		case FdoPropertyValueConstraintType_Range:
			{
				writer->WriteAttribute( L"type", L"range");
				
				FdoPropertyValueConstraintRange* range = (FdoPropertyValueConstraintRange*) m_propertyValueConstraint;
				FdoDataValueP dv = range->GetMaxValue();
				if ( dv != NULL )
				{
					writer->WriteStartElement( L"Max" );

					// Write two attributes for Max element
					if ( range->GetMaxInclusive() )
					{
						writer->WriteAttribute( L"inclusive", L"true" );
					}
					else
					{
						writer->WriteAttribute( L"inclusive", L"false" );
					}
					FdoStringP pData = dv->GetXmlValue();
					writer->WriteAttribute( L"value", pData);

					writer->WriteEndElement();
				}

				dv = range->GetMinValue();
				if ( dv != NULL )
				{
					writer->WriteStartElement( L"Min" );

					// Write two attributes for Min element
					if ( range->GetMinInclusive() )
					{
						writer->WriteAttribute( L"inclusive", L"true" );
					}
					else
					{
						writer->WriteAttribute( L"inclusive", L"false" );
					}
					FdoStringP pData = dv->GetXmlValue();
					writer->WriteAttribute( L"value", pData );

					writer->WriteEndElement();
				}
			}
			break;
		}
		
		// close the data constraint
		writer->WriteEndElement();
	}

    // close the data property element.
    writer->WriteEndElement();
}

void FdoDataPropertyDefinition::ThrowDefaultValueError( FdoString* defaultValue )
{
    if ( GetDataType() == FdoDataType_DateTime ) {
        throw FdoSchemaException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SCHEMA_151_DEFAULTDATEVIOLATION),
                (FdoString*) GetQualifiedName(),
                defaultValue
            )
        );
    }
    else {
        throw FdoSchemaException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SCHEMA_150_DEFAULTVALUEVIOLATION),
                (FdoString*) GetQualifiedName(),
                defaultValue,
                (FdoString*) FdoDataTypeMapper::Type2String(GetDataType())
            )
        );
    }
}

