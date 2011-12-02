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
#include <Fdo/Schema/ClassDefinition.h>
#include <Fdo/Schema/SchemaException.h>
#include <Fdo/Schema/FeatureSchema.h>
#include <Fdo/Schema/GeometricPropertyDefinition.h>
#include <Fdo/Schema/ObjectPropertyDefinition.h>
#include <Fdo/Schema/RasterPropertyDefinition.h>
#include <Fdo/Schema/AssociationPropertyDefinition.h>
#include <Fdo/Schema/UniqueConstraintCollection.h>
#include <Fdo/Schema/UniqueConstraint.h>
#include "XmlContext.h"


FdoClassDefinition::~FdoClassDefinition()
{
    FDO_SAFE_RELEASE(m_baseClass);
    FDO_SAFE_RELEASE(m_baseProperties);
    FDO_SAFE_RELEASE(m_identityProperties);
    FDO_SAFE_RELEASE(m_properties);
	FDO_SAFE_RELEASE(m_uniqueConstraints);

    FDO_SAFE_RELEASE(m_baseClassCHANGED);
    FDO_SAFE_RELEASE(m_basePropertiesCHANGED);
    FDO_SAFE_RELEASE(m_uniqueConstraintsCHANGED);
}

FdoClassDefinition::FdoClassDefinition() : FdoSchemaElement()
{
    Init();
}

FdoClassDefinition::FdoClassDefinition(FdoString* name, FdoString* description) : FdoSchemaElement(name, description)
{
    Init();
}

void FdoClassDefinition::Init()
{
    m_isAbstract = false;
    m_baseClass = NULL;
    m_baseProperties = NULL;
    m_identityProperties = FdoDataPropertyDefinitionCollection::Create(this);
    m_properties = FdoPropertyDefinitionCollection::Create(this);
	m_uniqueConstraints = FdoUniqueConstraintCollection::Create();

    m_isAbstractCHANGED = false;
    m_baseClassCHANGED = NULL;
    m_basePropertiesCHANGED = NULL;
    m_uniqueConstraintsCHANGED = NULL;

    m_bProperties = false;
    m_isComputed = false;

    m_propNameScopes = FdoStringCollection::Create();
    m_bFirstAssocSubProp = false;
}


FdoClassType FdoClassDefinition::GetClassType()
{
    return FdoClassType_Class;
}

FdoClassDefinition* FdoClassDefinition::GetBaseClass()
{
    return FDO_SAFE_ADDREF(m_baseClass);
}

void FdoClassDefinition::SetBaseClass(FdoClassDefinition* value)
{
    // Keep base properties only when current and new base class both null. 
    // m_baseProperties is ignored when there is a base
    // class so it's contents are irrelevant if the class has or will have
    // a base.
    if ( value || m_baseClass ) 
        FDO_SAFE_RELEASE(m_baseProperties);

    if (value)
    {
        // Base class must have same class type.
        if ( GetClassType() != FdoClassType_Class && GetClassType() != value->GetClassType() ) 
             throw FdoSchemaException::Create(
                FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_141_BASETYPECONFLICT),
                (FdoString*) GetQualifiedName(),
                (FdoString*) value->GetQualifiedName()
             )
        );

        // A class with identity properties cannot have a base class.
        if ( m_identityProperties->GetCount() > 0 ) 
             throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_14_SUBCLASSWITHIDENTITYPROPERTIES)));
        
        // Verify this wouldn't make element its own baseclass
        FdoClassDefinition*    base = value;
        base->AddRef();
        while (base)
        {
            if (base == this)
            {
                base->Release();
                throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_12_OWNBASECLASS)));
            }

            FdoClassDefinition*    next = base->GetBaseClass();
            base->Release();
            base = next;
        }
    }
    else 
    {
        if ( m_baseClass ) {
            // This class is changing from having a base class to not having one.
            // Preserve the system properties by adding them directly to this class.

            // The system properties are the base properties of the topmost base class.
            FdoClassDefinitionP topClass = FDO_SAFE_ADDREF(m_baseClass);
            while ( FdoClassDefinitionP(topClass->GetBaseClass()) ) 
                topClass = topClass->GetBaseClass();

            // Set the base properties from the top class base properties.
            m_baseProperties = topClass->GetBaseProperties();
        }
    }

    _StartChanges();
    FDO_SAFE_RELEASE(m_baseClass);
    m_baseClass = FDO_SAFE_ADDREF(value);
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoClassDefinition::GetIsComputed()
{
    return m_isComputed;
}

void FdoClassDefinition::SetIsComputed(bool value)
{
    m_isComputed = value;
}

bool FdoClassDefinition::GetIsAbstract()
{
   return m_isAbstract;
}

void FdoClassDefinition::SetIsAbstract(bool value)
{
    _StartChanges();
    m_isAbstract = value;
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoClassDefinition::SetBaseProperties(FdoPropertyDefinitionCollection* value)
{
    _StartChanges();
    if (m_baseProperties)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_2_BASEPROPERTIESALREADYSET)));

    m_baseProperties = FdoReadOnlyPropertyDefinitionCollection::Create(value);
}

FdoReadOnlyPropertyDefinitionCollection* FdoClassDefinition::GetBaseProperties()
{
    // Imtermediate base property collection
    FdoPropertiesP  tempBaseProps = FdoPropertyDefinitionCollection::Create(NULL);

    // Final base property collection
    FdoReadOnlyPropertyDefinitionCollection* baseProps = NULL;

    FdoPropertyP prop;
    FdoInt32 idx;

    if ( m_baseClass ) 
    {
        // Class has a base class.
        // The base properties are the base class base properties plus the 
        // base class properties.
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseClassBaseProps = m_baseClass->GetBaseProperties();
        for ( idx = 0; idx < baseClassBaseProps->GetCount(); idx++ ) {
            prop = baseClassBaseProps->GetItem(idx);
            tempBaseProps->Add( prop );
        }

        FdoPropertiesP baseClassProps = m_baseClass->GetProperties();

        for ( idx = 0; idx < baseClassProps->GetCount(); idx++ ) {
            prop = baseClassProps->GetItem(idx);
            tempBaseProps->Add( prop );
        }

        baseProps = FdoReadOnlyPropertyDefinitionCollection::Create(tempBaseProps);

    }
    else 
    {
        if (!m_baseProperties)
            // Go ahead and create an empty BaseProperties collection
            m_baseProperties = FdoReadOnlyPropertyDefinitionCollection::Create(tempBaseProps);

        // The class has no base class so the base properties are any system
        // properties attached to this class.
        baseProps = FDO_SAFE_ADDREF(m_baseProperties);
    }

    return baseProps;
}

FdoDataPropertyDefinitionCollection* FdoClassDefinition::GetIdentityProperties()
{
    return FDO_SAFE_ADDREF(m_identityProperties);
}

FdoReadOnlyDataPropertyDefinitionCollection* FdoClassDefinition::GetBaseIdentityProperties()
{
    FdoPtr<FdoDataPropertyDefinitionCollection> dataProperties = FdoDataPropertyDefinitionCollection::Create(NULL);
    FdoPtr<FdoClassDefinition> baseClass; 
    if (IsSubClass())
    {
        baseClass = this->GetBaseClass();
        FdoPtr<FdoDataPropertyDefinitionCollection> ids;
        while (baseClass)
        {
            ids = baseClass->GetIdentityProperties();
            baseClass = baseClass->GetBaseClass();
        }
        for (int i=0; i<ids->GetCount(); i++)
        {
            FdoPtr<FdoDataPropertyDefinition> dataProperty = ids->GetItem(i);
            dataProperties->Add(dataProperty);
        }
    }
    return FdoReadOnlyDataPropertyDefinitionCollection::Create(dataProperties);

}

FdoPropertyDefinitionCollection* FdoClassDefinition::GetProperties()
{
    return FDO_SAFE_ADDREF(m_properties);
}

FdoStringP FdoClassDefinition::GetQualifiedName()
{
    FdoPtr<FdoFeatureSchema> schema = GetFeatureSchema();
    FdoStringP qName;
    
    if ( schema )
        qName += schema->GetQualifiedName() + L":";

    qName += GetName();

    return qName;
}

FdoClassCapabilities* FdoClassDefinition::GetCapabilities()
{
    return FDO_SAFE_ADDREF( (FdoClassCapabilities*) m_capabilities );
}

void FdoClassDefinition::SetCapabilities(FdoClassCapabilities* capabilities)
{
    m_capabilities = FDO_SAFE_ADDREF(capabilities);
}

FdoUniqueConstraintCollection* FdoClassDefinition::GetUniqueConstraints()
{
    return FDO_SAFE_ADDREF( (FdoUniqueConstraintCollection*) m_uniqueConstraints);
}


void FdoClassDefinition::PropertyRemoved(const FdoPropertyDefinition* property)
{
    // When a property is removed, it can no longer be an identity property
    if (m_identityProperties->Contains((FdoDataPropertyDefinition*)property))       // cast should be safe since Contains is just doing a pointer comparison
        m_identityProperties->Remove((const FdoDataPropertyDefinition*)property);
}

bool FdoClassDefinition::IsProperty(const FdoPropertyDefinition* property)
{
    return m_properties->Contains(property) 
        || m_identityProperties->Contains((FdoDataPropertyDefinition*)property);    // cast should be safe since Contains is just doing a pointer comparison
}

bool FdoClassDefinition::IsSubClass()
{
    return (m_baseClass != NULL);
}

FdoUniqueConstraintCollection* DeepCopyConstraints(FdoUniqueConstraintCollection* src)
{
    FdoPtr<FdoUniqueConstraintCollection> dest = FdoUniqueConstraintCollection::Create();
    if (src == NULL || src->GetCount() == 0)
        return FDO_SAFE_ADDREF(dest.p);
    for ( int idx = 0; idx < src->GetCount(); idx++ ) {
        FdoPtr<FdoUniqueConstraint> uc = src->GetItem(idx);
        FdoPtr<FdoUniqueConstraint> newuc = FdoUniqueConstraint::Create();
        dest->Add(newuc);
        
        FdoPtr<FdoDataPropertyDefinitionCollection> newucProps = newuc->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> ucProps = uc->GetProperties();
        for (int idx2 = 0; idx2 < ucProps->GetCount(); idx2++ ) 
            newucProps->Add( FdoPtr<FdoDataPropertyDefinition>(ucProps->GetItem(idx2)));
    }
    return FDO_SAFE_ADDREF(dest.p);
}

void FdoClassDefinition::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoSchemaElement::_StartChanges();

        m_isAbstractCHANGED = m_isAbstract;
        m_baseClassCHANGED = FDO_SAFE_ADDREF(m_baseClass);
        m_basePropertiesCHANGED = FDO_SAFE_ADDREF(m_baseProperties);
        FDO_SAFE_RELEASE(m_uniqueConstraintsCHANGED);
        m_uniqueConstraintsCHANGED = DeepCopyConstraints(m_uniqueConstraints);
    }
}

void FdoClassDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoSchemaElement::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        m_isAbstract = m_isAbstractCHANGED;
        if (m_baseClass != m_baseClassCHANGED)
        {
            if (m_baseClass)
                m_baseClass->_RejectChanges();
            FDO_SAFE_RELEASE(m_baseClass);
            m_baseClass = FDO_SAFE_ADDREF(m_baseClassCHANGED);
        }

        if (m_baseProperties != m_basePropertiesCHANGED)
        {
            FDO_SAFE_RELEASE(m_baseProperties);
            m_baseProperties = FDO_SAFE_ADDREF(m_basePropertiesCHANGED);
        }
        if (m_uniqueConstraintsCHANGED != NULL)
        {
            FDO_SAFE_RELEASE(m_uniqueConstraints);
            m_uniqueConstraints = FDO_SAFE_ADDREF(m_uniqueConstraintsCHANGED);
        }

        // reset
        m_isAbstractCHANGED = false;
        FDO_SAFE_RELEASE(m_baseClassCHANGED);
        FDO_SAFE_RELEASE(m_basePropertiesCHANGED);
        FDO_SAFE_RELEASE(m_uniqueConstraintsCHANGED);
    }

    if (m_baseClass)
        m_baseClass->_RejectChanges();
    m_properties->_RejectChanges();
    m_identityProperties->_RejectChanges();
}

void FdoClassDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoSchemaElement::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        m_isAbstractCHANGED = false;
        if (m_baseClassCHANGED)
            m_baseClassCHANGED->_AcceptChanges();
        FDO_SAFE_RELEASE(m_baseClassCHANGED);

        // NOTE: no need to call _AcceptChanges() on m_basePropertiesCHANGED, it cannot change once set.
        FDO_SAFE_RELEASE(m_basePropertiesCHANGED);
        FDO_SAFE_RELEASE(m_uniqueConstraintsCHANGED);
    }

    if (m_baseClass)
        m_baseClass->_AcceptChanges();
    m_properties->_AcceptChanges();
    m_identityProperties->_AcceptChanges();
}

void FdoClassDefinition::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoSchemaElement::_BeginChangeProcessing();

    if (m_baseClass)
        m_baseClass->_BeginChangeProcessing();
    m_properties->_BeginChangeProcessing();
    m_identityProperties->_BeginChangeProcessing();
}

void FdoClassDefinition::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoSchemaElement::_EndChangeProcessing();
    if (m_baseClass)
        m_baseClass->_EndChangeProcessing();
    m_properties->_EndChangeProcessing();
    m_identityProperties->_EndChangeProcessing();
}

void FdoClassDefinition::Set( FdoClassDefinition* pClass, FdoSchemaMergeContext* pContext )
{
    // Class type change not supported.
    if ( GetClassType() != pClass->GetClassType() ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_24_CLASSTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    FdoInt32 idx = -1;
    FdoPropertiesP props = pClass->GetProperties();

    FdoSchemaElement::Set(pClass, pContext);

    if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pClass->GetElementState() == FdoSchemaElementState_Modified) ) {
        // Update this class from the given class. The same pattern is followed 
        // for each member:
        //
        // If new and old values differ
        //    If modification allowed (always allowed if this is a new property).
        //      If value is an object
        //        Add a reference to be resolved later (when we're sure that referenced
        //          object exists).
        //      else
        //        Perform the modification
        //      end if
        //    else
        //      log an error
        //    end if
        //  end if

        // Class Capabilities.
        // Take cautious approach for now. Copy capabilites only if this is 
        // a new schema element.

        if ( GetElementState() == FdoSchemaElementState_Added )
        {
            FdoPtr<FdoClassCapabilities> myCapabilities = GetCapabilities();
            FdoPtr<FdoClassCapabilities> otherCapabilities = pClass->GetCapabilities();

            // Do not overwrite any existing capabilities
            if ( otherCapabilities && !myCapabilities ) 
            {
                myCapabilities = FdoClassCapabilities::Create( *this );
                myCapabilities->Set( otherCapabilities );
                SetCapabilities( myCapabilities );
            }
        }

        // IsAbstract setting

        if ( GetIsAbstract() != pClass->GetIsAbstract() ) {
            if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModClassAbstract(pClass)) ) 
                SetIsAbstract( pClass->GetIsAbstract() );
            else
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(pClass->GetIsAbstract() ? SCHEMA_72_MODCLASSABSTRACT : SCHEMA_71_MODCLASSCONCRETE),
                                (FdoString*) GetQualifiedName()
                            )
                        )
                    )
                );
        }

        // Base Class

        FdoClassDefinitionP newBaseClass = pClass->GetBaseClass();
        FdoSchemaElementP newBaseParent = newBaseClass ? newBaseClass->GetParent() : (FdoSchemaElement*) NULL;
        if ( newBaseClass && !newBaseParent ) {
            // Error: new base class is an orphan.
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_51_CLASSNOSCHEMA),
                            (FdoString*) GetQualifiedName(),
                            newBaseClass->GetName()
                        )
                    )
                )
            );
        }
        else {
            FdoStringP oldBaseClassName = m_baseClass ? m_baseClass->GetQualifiedName() : FdoStringP();
            FdoStringP newBaseClassName = newBaseClass ? newBaseClass->GetQualifiedName() : FdoStringP();
            if ( oldBaseClassName != newBaseClassName ) { 
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModBaseClass(pClass)) ) {
                    pContext->AddBaseClassRef( 
                        this, 
                        newBaseParent ? newBaseParent->GetName() : L"",
                        newBaseClass ? newBaseClass->GetName() : L""
                    );
                }
                else {
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_73_MODBASECLASS),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) oldBaseClassName,
                                    (FdoString*) newBaseClassName
                                )
                            )
                        )
                    );
                }
            }
        }

        // Identity Properties

        FdoDataPropertiesP idProps = pClass->GetIdentityProperties();
            
        // Need to update all properties for this class and its base before
        // setting the identityProperties. For now, just put the id prop names
        // in a collection.
        FdoStringsP oldIdPropNames = FdoStringCollection::Create();
        for ( idx = 0; idx < m_identityProperties->GetCount(); idx++ ) 
            oldIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(m_identityProperties->GetItem(idx))->GetName() );

        FdoStringsP newIdPropNames = FdoStringCollection::Create();
        for ( idx = 0; idx < idProps->GetCount(); idx++ ) 
                newIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(idProps->GetItem(idx))->GetName() );

        if ( oldIdPropNames->ToString() != newIdPropNames->ToString() ) {
            if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModIdProps(pClass)) ) {
                pContext->AddIdPropRef( 
                    this, 
                    newIdPropNames
                );
            }
            else {
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_74_MODCLASSIDENTITY),
                                (FdoString*) GetQualifiedName(),
                                (FdoString*) oldIdPropNames->ToString(),
                                (FdoString*) newIdPropNames->ToString()
                            )
                        )
                    )
                );
            }
        }

        // FdoUniqueConstraint is not a FdoSchemaElement we cannot "merge" elements in the default way
        // We do not have an API function CanModUniqueConstr(pClass) we use CanModIdProps which
        // will do the same thing in most of data sources
        if (pClass->GetElementState() == FdoSchemaElementState_Modified)
        {
            bool changeDoneUq = false;
            FdoPtr<FdoUniqueConstraintCollection> uniqueConstrColl = pClass->GetUniqueConstraints();
            FdoStringsP oldConstPropNames = FdoStringCollection::Create();
            FdoStringsP newConstPropNames = FdoStringCollection::Create();
            // see which one needs to be deleted
            for ( idx = 0; idx < m_uniqueConstraints->GetCount(); idx++ ) {
                FdoPtr<FdoUniqueConstraint> olduc = m_uniqueConstraints->GetItem(idx);
                
                FdoPtr<FdoDataPropertyDefinitionCollection> olducProps = olduc->GetProperties();
                for (int idx2 = 0; idx2 < olducProps->GetCount(); idx2++ ) 
                    oldConstPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(olducProps->GetItem(idx2))->GetName() );
                
                bool constrFound = false;
                for ( int idx3 = 0; idx3 < uniqueConstrColl->GetCount(); idx3++ ) {
                    FdoPtr<FdoUniqueConstraint> newuc = uniqueConstrColl->GetItem(idx3);
                    
                    FdoPtr<FdoDataPropertyDefinitionCollection> newucProps = newuc->GetProperties();
                    for (int idx2 = 0; idx2 < newucProps->GetCount(); idx2++ ) 
                        newConstPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(newucProps->GetItem(idx2))->GetName() );
                    if (oldConstPropNames->ToString() == newConstPropNames->ToString())
                    {
                        constrFound = true;
                        break;
                    }
                    newConstPropNames->Clear();
                }
                if (!constrFound)
                {
                    FdoStringsP emptyStrs = FdoStringCollection::Create();
                    pContext->AddUniqueConstraintRef(this, olduc, emptyStrs);
                    changeDoneUq = true;
                }

                oldConstPropNames->Clear();
                newConstPropNames->Clear();
            }
            // see which one needs to be added
            for ( idx = 0; idx < uniqueConstrColl->GetCount(); idx++ ) {
                FdoPtr<FdoUniqueConstraint> newuc = uniqueConstrColl->GetItem(idx);
                
                FdoPtr<FdoDataPropertyDefinitionCollection> newucProps = newuc->GetProperties();
                for (int idx2 = 0; idx2 < newucProps->GetCount(); idx2++ ) 
                    newConstPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(newucProps->GetItem(idx2))->GetName() );
                
                bool constrFound = false;
                for ( int idx3 = 0; idx3 < m_uniqueConstraints->GetCount(); idx3++ ) {
                    FdoPtr<FdoUniqueConstraint> olduc = m_uniqueConstraints->GetItem(idx3);
                    
                    FdoPtr<FdoDataPropertyDefinitionCollection> olducProps = olduc->GetProperties();
                    for (int idx2 = 0; idx2 < olducProps->GetCount(); idx2++ ) 
                        oldConstPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(olducProps->GetItem(idx2))->GetName() );
                    if (newConstPropNames->ToString() == oldConstPropNames->ToString())
                    {
                        constrFound = true;
                        break;
                    }
                    oldConstPropNames->Clear();
                }
                if (!constrFound)
                {
                    pContext->AddUniqueConstraintRef(this, NULL, newConstPropNames);
                    newConstPropNames = FdoStringCollection::Create();
                    changeDoneUq = true;
                }

                oldConstPropNames->Clear();
                newConstPropNames->Clear();
            }
            if (changeDoneUq && !pContext->CanModIdProps(pClass))
            {
                // Can't change unique constraints for a class with data.
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_152_MODCLASSCONSTRAINT),
                                (FdoString*) pClass->GetQualifiedName()
                            )
                        )
                    )
                );
            }
        }

        if ( pContext->GetReplaceClass() ) {
            // If we're completely replacing classes (this is done when reading from XML)
            // then don't additively merge properties. Remove any properties not in the 
            // given class.
            for ( idx = (m_properties->GetCount() - 1); idx >= 0; idx-- ) {
                FdoPropertyP oldProp = m_properties->GetItem(idx);

                FdoPropertyP newProp = props->FindItem( oldProp->GetName() );
            
                if ( newProp == NULL ) 
                    m_properties->RemoveAt(idx);
            }
        }

    }

    // Merge each property from the given class into this class.
    for ( idx = 0; idx < props->GetCount(); idx++ ) {
        FdoPropertyP newProp = props->GetItem(idx);
        FdoPropertyP oldProp = m_properties->FindItem( newProp->GetName() );

        FdoSchemaElementState propState;

        // Determine new state of each property.
        if ( GetElementState() == FdoSchemaElementState_Deleted ) {
            // Always delete properties if this class is being deleted.
            propState = FdoSchemaElementState_Deleted;
        }
        else {
            // The following sets the right state when ignoring element states.
            // Its a modification if this class already has the property, and an add 
            // otherwise.
            propState = oldProp ? FdoSchemaElementState_Modified : FdoSchemaElementState_Added;

            if ( !pContext->GetIgnoreStates() ) 
                // Not ignoring element states so get state from the property to update from.
                propState = newProp->GetElementState();
        }

        // Handle each type of modification
        switch  ( propState ) {
        case FdoSchemaElementState_Added:
            if ( oldProp ) {
                // Can't add property that already exists.
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_75_PROPEXISTS),
                                (FdoString*) oldProp->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            else {
                if ( (GetElementState() == FdoSchemaElementState_Added) || pContext->CheckAddProperty(newProp) ) {
                    // Adding property allowed. Create a copy property to add to this class.
                    FdoPropertyType propType = newProp->GetPropertyType();
                    if ( propType == FdoPropertyType_DataProperty )
                        oldProp = (FdoPropertyDefinition*) FdoDataPropertyDefinition::Create();
                    else if ( propType == FdoPropertyType_ObjectProperty )
                        oldProp = (FdoPropertyDefinition*) FdoObjectPropertyDefinition::Create();
                    else if ( propType == FdoPropertyType_GeometricProperty )
                        oldProp = (FdoPropertyDefinition*) FdoGeometricPropertyDefinition::Create();
                    else if ( propType == FdoPropertyType_AssociationProperty )
                        oldProp = (FdoPropertyDefinition*) FdoAssociationPropertyDefinition::Create();
                    else if ( propType == FdoPropertyType_RasterProperty )
                        oldProp = (FdoPropertyDefinition*) FdoRasterPropertyDefinition::Create();
                    else
                        pContext->AddError( 
                            FdoSchemaExceptionP(
                                FdoSchemaException::Create(
                                    FdoException::NLSGetMessage(
                                        FDO_NLSID(SCHEMA_76_BADPROPTYPE),
                                        (FdoString*) newProp->GetQualifiedName(),
                                        propType
                                    )
                                )
                            )
                        );

                    if ( oldProp ) {
                        // Add copy property to this class. Name must be set first 
                        // since property collection is a named collection.
                        oldProp->SetName( newProp->GetName() );
                        m_properties->Add(oldProp);

                        // Set the copy property from the property to update from.
                        oldProp->Set(newProp, pContext);
                    }
                }
            }
            break;

        case FdoSchemaElementState_Deleted:
            if ( oldProp )
            {
                if (pContext->CheckDeleteProperty(oldProp) ) {
                    // Mark property for deletion.
                    // Silently ignore if property does not exist.
                    oldProp->Delete();
			    }
                else
                {
                    // Can't delete property.
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_125_DELPROP),
                                    (FdoString*) newProp->GetQualifiedName()
                                )
                            )
                        )
                    );
                }
            }
            break;

        case FdoSchemaElementState_Modified:
            if ( oldProp ) {
                // Update property from property to update from.
                oldProp->Set(newProp, pContext);
            }
            else {
                // Can't modify non-existent property.
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_77_PROPNOTEXISTS),
                                (FdoString*) newProp->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            break;
        }
    }
}

void FdoClassDefinition::CheckReferences( FdoSchemaMergeContext* pContext )
{
    // No need to check references if this element is going away.
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoInt32 idx = -1;
        FdoPropertiesP props = GetProperties();

        FdoSchemaElement::CheckReferences(pContext);

        // Check if base class marked for delete.        

        FdoClassDefinitionP baseClass = GetBaseClass();

        if ( baseClass ) {
            if ( baseClass->GetElementState() == FdoSchemaElementState_Deleted ) {
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_126_DELBASECLASS),
                                (FdoString*) baseClass->GetQualifiedName(),
                                (FdoString*)GetQualifiedName()
                            )
                        )
                    )
                );
            }
        }
        else {
            // Check if any identity property marked for delete.  
            // If class has base class then it has no identity properties
            // of its own.

            FdoDataPropertiesP idProps = GetIdentityProperties();

            for ( idx = 0; idx < idProps->GetCount(); idx++ ) {
                FdoDataPropertyP idProp = idProps->GetItem( idx );

                if ( idProp->GetElementState() == FdoSchemaElementState_Deleted ) {
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_128_DELCLASSID),
                                    (FdoString*) idProp->GetQualifiedName(),
                                    (FdoString*)GetQualifiedName()
                                )
                            )
                        )
                    );
                }
            }
        }

        for ( idx = 0; idx < props->GetCount(); idx++ ) {
            FdoPropertyP prop = props->GetItem(idx);
            prop->CheckReferences( pContext );
        }
    }
}

void FdoClassDefinition::InitFromXml(FdoString* classTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    InitFromXml( pContext, attrs );
}

void FdoClassDefinition::InitFromXml(FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) pContext;

    // Class type change not allowed.

    FdoClassDefinitionP pOldClass = fdoContext->GetMergeContext()->FindClass( 
        FdoFeatureSchemasP( fdoContext->GetMergeContext()->GetSchemas() ),
        FdoFeatureSchemaP(GetFeatureSchema())->GetName(),
        GetName()
    );

    if ( pOldClass && (pOldClass->GetClassType() != GetClassType()) ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_24_CLASSTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );
    
        return;
    }

    // Initialize generic parts
    FdoSchemaElement::InitFromXml(pContext, attrs );
    
    // Empty this class. As with Set(), a complete replacement is done
    // instead of an additive merge.
    m_identityProperties->Clear();
    m_properties->Clear();
    m_uniqueConstraints->Clear();
    SetBaseClass(NULL);
    FDO_SAFE_RELEASE(m_baseProperties);

    // Set abstract from XML attributes

    m_isAbstract = false;

    FdoXmlAttributeP isAbstract = attrs->FindItem( L"abstract" );

    if ( isAbstract != NULL ) 
        m_isAbstract = FdoStringP(isAbstract->GetValue()).ToBoolean();

    // Get the new base class.

    FdoXmlAttributeP baseSchema = attrs->FindItem( L"baseSchema" );
    FdoXmlAttributeP baseClass = attrs->FindItem( L"baseClass" );
    FdoXmlAttributeP restricted = attrs->FindItem( L"restricted" );

    // Add a reference that will be resolved after the read from XML
    // is complete. The base class might not have been read yet.

    // FDO does not handle inheritance by restriction so ignore base class
    // for restriction.
    if ( (restricted == NULL) && (baseSchema != NULL) && (baseClass != NULL) )
        pContext->GetMergeContext()->AddBaseClassRef( this, pContext->DecodeName(baseSchema->GetValue()), pContext->DecodeName(baseClass->GetValue()) );
}

FdoXmlSaxHandler* FdoClassDefinition::XmlStartElement(
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
        if (wcscmp(name, L"Properties") == 0)
            m_bProperties = true;
        // Not generic, so handle here.
        if (m_bProperties) {
            if ( (wcscmp(name, L"DataProperty") == 0) || 
                (wcscmp(name, L"GeometricProperty") == 0) || 
                (wcscmp(name, L"ObjectProperty") == 0) ||
                (wcscmp(name, L"RasterProperty") == 0) ||
                (wcscmp(name, L"AssociationProperty") == 0) ||
                (wcscmp(name, L"GeometricAssociation") == 0) ||
                (wcscmp(name, L"GeometricSubProperties") == 0) ||
                (wcscmp(name, L"ElementMapping") == 0))
            {
                // Sub-element is a property
                FdoStringP propName = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"name"))->GetValue() );
                
                // Geometric or GeometricAssociation sub-properties are
                // scoped by the property name. The exception is the  
                // first sub-property of a GeometricAssociation, which is always
                // a Geometric Property.
                // In this case the sub-property replaces and takes on the
                // name of the Geometric Association.
                // Note: in the future we might represent a GeometricAssocation
                // by a TopoGeometry Property.
                if ( m_bFirstAssocSubProp )
                    m_propNameScopes->Add( L".:" );
                else
                    m_propNameScopes->Add(propName);

                FdoStringP scopedPropName = m_propNameScopes->ToString( L"/" ).Replace( L"/.:", L"" );

                // Create a property of the right type
                if ( wcscmp(name, L"DataProperty") == 0 ) {
                    pProp = FdoDataPropertyDefinition::Create( scopedPropName, L"" );
                }
                else {
                    if ( wcscmp(name, L"GeometricProperty") == 0 ) {
                        pProp = FdoGeometricPropertyDefinition::Create( scopedPropName, L"" );
                    }
                    else {
                        if ( wcscmp(name, L"ObjectProperty") == 0 )
                            pProp = FdoObjectPropertyDefinition::Create( scopedPropName, L"" );
                        else {
                            if ( wcscmp(name, L"RasterProperty") == 0 )
                                pProp = FdoRasterPropertyDefinition::Create(scopedPropName, L"");
                            else {
                                if (wcscmp(name, L"AssociationProperty") == 0 ) 
                                    pProp = FdoAssociationPropertyDefinition::Create(scopedPropName, L"");
                                else {
                                    if (wcscmp(name, L"GeometricAssociation") == 0 ) 
                                        // Set flag for promoting the 1st sub-property
                                        m_bFirstAssocSubProp = true;
                                    else {
                                        // Handle an element mapping
                                        // Skip if this is the first geometric association sub-element, since
                                        // this element is collapsed up into the association.
                                        // Also skip if this element has no parent (no schema).
                                        if ( (!m_bFirstAssocSubProp) && FdoSchemaElementP(GetParent()) && wcscmp(name, L"ElementMapping") == 0 ) {
                                            FdoStringP classSchema = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"classSchema"))->GetValue() );
                                            FdoStringP className = fdoContext->DecodeName( FdoXmlAttributeP(atts->GetItem(L"className"))->GetValue() );
                                            FdoStringP gmlUri;
                                            FdoStringP gmlLocalName;
                                            FdoStringP choiceName;

                                            FdoXmlAttributeP att = atts->FindItem( L"gmlUri" );
                                            if ( att ) 
                                                gmlUri = att->GetValue();

                                            att = atts->FindItem( L"gmlLocalName" );
                                            if ( att ) 
                                                gmlLocalName = att->GetValue();

                                            att = atts->FindItem(L"choiceName");
                                            if( att != NULL)
                                                choiceName = att->GetValue();

                                            fdoContext->AddSubElementMapping( 
                                                FdoSchemaElementP(GetParent())->GetName(), 
                                                GetName(), 
                                                scopedPropName, 
                                                classSchema, 
                                                className,
                                                gmlUri,
                                                gmlLocalName,
                                                choiceName
                                            );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if ( pProp != NULL ) {
                    m_properties->Add(pProp);
                    // Initialize the property from its XML attributes.
                    pProp->InitFromXml( name, fdoContext, atts );
                    // Make the property the SAX Handler.
                    pRet = pProp;
                }
            }
        }

        if ( wcscmp(name, L"IdentityProperties") == 0 ) {
            //Sub-element is a list of identity property names.
            //Initialize the id prop name collection.
            m_idPropNames = FdoStringCollection::Create();
        }

        if ( wcscmp(name, L"IdentityProperty") == 0 ) {
            // Sub-element contains is an identity property name.
            // Set up a SAX handler to read the content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }

        if ( wcscmp(name, L"UniqueConstraint") == 0 ) {
            m_consPropNames = FdoStringCollection::Create();
        }

        if ( wcscmp(name, L"ConstraintProperty") == 0 ) {
            // Sub-element contains is an property name.
            // Set up a SAX handler to read the content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }
    }

    return(pRet);
}

FdoBoolean FdoClassDefinition::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoSchemaElement::XmlEndElement(context, uri, name, qname);

    if ( m_bProperties ) {
        if ( (wcscmp(name, L"DataProperty") == 0) || 
            (wcscmp(name, L"GeometricProperty") == 0) || 
            (wcscmp(name, L"ObjectProperty") == 0) ||
            (wcscmp(name, L"RasterProperty") == 0) ||
            (wcscmp(name, L"AssociationProperty") == 0) ||
            (wcscmp(name, L"GeometricAssociation") == 0) ||
            (wcscmp(name, L"GeometricSubProperties") == 0) ||
            (wcscmp(name, L"ElementMapping") == 0))
        {
            if ( wcscmp(name, L"ElementMapping") != 0 )
                m_bFirstAssocSubProp = false;
            m_propNameScopes->RemoveAt( m_propNameScopes->GetCount() - 1 );
        }
    }

    if ( wcscmp(name, L"IdentityProperties") == 0 ) {
        //End the id property list. Add a reference that is 
        //resolved when the read from XML is complete.
        if ( m_idPropNames->GetCount() > 0 ) 
            fdoContext->GetMergeContext()->AddIdPropRef( this, m_idPropNames );
    }

    if ( wcscmp(name, L"IdentityProperty") == 0 ) {
        //Sub-element is an id property name. Get the name from the 
        //content handler and add it to the id property list.
        m_idPropNames->Add( fdoContext->DecodeName(m_XmlContentHandler->GetString()) );
    }

    if ( wcscmp(name, L"UniqueConstraint") == 0 ) {
        FdoUniqueConstraintP pUniCons = FdoUniqueConstraint::Create();
        m_uniqueConstraints->Add(pUniCons);

        fdoContext->GetMergeContext()->AddUniqueConstraintRef(this, pUniCons, m_consPropNames);
    }
    
    if ( wcscmp(name, L"ConstraintProperty") == 0 ) {
        //Sub-element is an id property name. Get the name from the 
        //content handler and add it to the id property list.
        m_consPropNames->Add( fdoContext->DecodeName(m_XmlContentHandler->GetString()) );
    }

    if (wcscmp(name, L"Properties") == 0)
        m_bProperties = false;

    return(false);
}

void FdoClassDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    int i;
    FDO_SAFE_ADDREF(this);
    FdoClassDefinitionP topClass = this;
    FdoClassDefinitionP nextClass = GetBaseClass();
    FdoDataPropertiesP  idProps;
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    // Write abstract and base class info

    writer->WriteAttribute( L"abstract", m_isAbstract ? L"true" : L"false" );

    if ( m_baseClass ) {
        writer->WriteAttribute( 
            L"baseSchema", 
            pContext->EncodeName(pContext->RefClass2SchemaName(this, m_baseClass))
        );
        writer->WriteAttribute( 
            L"baseClass",
            pContext->EncodeName(m_baseClass->GetName())
        );
    }

    // Write generic attributes and sub-elements.

    FdoSchemaElement::_writeXml(pContext);

    // Find the top base class (has the identity properties if any

    while ( nextClass != NULL ) {
        topClass = nextClass;
        nextClass = topClass->GetBaseClass();
    }

    // Write the identity properties

    idProps = topClass->GetIdentityProperties();

    if ( idProps->GetCount() > 0 ) {
        writer->WriteStartElement( L"IdentityProperties" );    
        for ( i = 0; i < idProps->GetCount(); i++ ) {
            writer->WriteStartElement( L"IdentityProperty" );

            FdoString* pPropName = FdoDataPropertyP(idProps->GetItem(i))->GetName();

            writer->WriteCharacters( pContext->EncodeName(pPropName) );

            writer->WriteEndElement();
        }
        writer->WriteEndElement();
    }

    // Write the properties

    if ( m_properties->GetCount() > 0 ) {
        writer->WriteStartElement( L"Properties" );    
        for ( i = 0; i < m_properties->GetCount(); i++ ) {
            FdoPropertyP pProp = m_properties->GetItem(i);

            // System properties are not written to XML.
            if ( !pProp->GetIsSystem() ) 
                pProp->_writeXml(pContext);
        }

        writer->WriteEndElement();
    }

    // Write the unique constraints

    if ( m_uniqueConstraints->GetCount() > 0 ) {
        writer->WriteStartElement( L"UniqueConstraints" );    
        for ( i = 0; i < m_uniqueConstraints->GetCount(); i++ ) {
            writer->WriteStartElement( L"UniqueConstraint" );
			writer->WriteAttribute( L"name", pContext->EncodeName(GetName()) + FdoStringP::Format(L"Key_%d", i) );
			FdoUniqueConstraintP uniqueConstraint = m_uniqueConstraints->GetItem(i);
			FdoDataPropertiesP dpCol = uniqueConstraint->GetProperties();
			for ( int j = 0; j < dpCol->GetCount(); j++ )
			{
				FdoDataPropertyP dp = dpCol->GetItem(j);
				writer->WriteStartElement( L"ConstraintProperty" );
				writer->WriteAttribute( L"name", pContext->EncodeName(dp->GetName()));
				writer->WriteEndElement();
			}
				
            writer->WriteEndElement();
        }
        writer->WriteEndElement();
    }
}

void FdoClassDefinition::writeXmlBaseProperties( FdoSchemaXmlContext* pContext )
{
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    if ( m_baseClass ) {
        writer->WriteStartElement( L"BaseProperties" );
        _writeXmlBaseProperties( pContext );
        writer->WriteEndElement();
    }
}

void FdoClassDefinition::_writeXmlBaseProperties( FdoSchemaXmlContext* pContext )
{
    FdoInt32 i;

    if ( m_baseClass ) {
        m_baseClass->writeXmlBaseProperties( pContext );

        FdoPropertiesP baseProps = m_baseClass->GetProperties();
        for ( i = 0; i < baseProps->GetCount(); i++ ) 
            FdoPropertyP(baseProps->GetItem(i))->_writeXml( pContext );
    }
}

