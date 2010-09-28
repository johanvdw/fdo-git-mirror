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
#include "FdoCommonPropertyIndex.h"

FdoCommonPropertyIndex::FdoCommonPropertyIndex(FdoClassDefinition* clas, unsigned int fcid, FdoIdentifierCollection *selectedIds)
{
    //inherited properties 
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> bpdc = clas->GetBaseProperties();

    //class properties 
    FdoPtr<FdoPropertyDefinitionCollection> pdc = clas->GetProperties();

    // if caller gives us a NULL or empty selectedIds collection, it implicitly means to select all properties:
    bool bSelectAllIds = ((NULL==selectedIds) || (0==selectedIds->GetCount()));

    m_bHasAutoGen = false;

    m_numProps = bSelectAllIds ? bpdc->GetCount() + pdc->GetCount() : selectedIds->GetCount();
    m_vProps = new FdoCommonPropertyStub[m_numProps];

    int index = 0;

    //now iterate over inherited properties 
    for (int i=0; i<bpdc->GetCount(); i++)
    {
        FdoPtr<FdoSchemaElement> pd = bpdc->GetItem(i);

        if (bSelectAllIds || (FdoPtr<FdoIdentifier>(selectedIds->FindItem(pd->GetName())) != NULL))
        {
            FdoDataPropertyDefinition* dpd = dynamic_cast<FdoDataPropertyDefinition*>(pd.p);

            FdoCommonPropertyStub* ps = &m_vProps[index];
            ps->m_name = (wchar_t*)pd->GetName();
            ps->m_recordIndex = index++;
            
            if (dpd)
            {
                ps->m_dataType = dpd->GetDataType();
				ps->m_propertyType = dpd->GetPropertyType();
                ps->m_isAutoGen = dpd->GetIsAutoGenerated();
                if (ps->m_isAutoGen) 
                    m_bHasAutoGen = true;
            }
            else
            {
                //TODO:
                //save data type, or -1 for geometry. -1 is a little risky
                ps->m_dataType = (FdoDataType)-1;
				ps->m_propertyType = FdoPropertyType_GeometricProperty;
                ps->m_isAutoGen = false;
            }
        }        
    }


    //class properties
    for (int i=0; i<pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);

        if (bSelectAllIds || (FdoPtr<FdoIdentifier>(selectedIds->FindItem(pd->GetName())) != NULL))
        {
            FdoDataPropertyDefinition* dpd = dynamic_cast<FdoDataPropertyDefinition*>(pd.p);
            
            FdoCommonPropertyStub* ps = &m_vProps[index];
            ps->m_name = (wchar_t*)pd->GetName();
            ps->m_recordIndex = index++;

            if (dpd)
            {
				ps->m_propertyType = dpd->GetPropertyType();
                ps->m_dataType = dpd->GetDataType();
                ps->m_isAutoGen = dpd->GetIsAutoGenerated();
                if (ps->m_isAutoGen) 
                    m_bHasAutoGen = true;
            }
            else
            {
                //TODO:
                //save data type, or -1 for geometry. -1 is a little risky
                ps->m_dataType = (FdoDataType)-1;
                ps->m_propertyType = FdoPropertyType_GeometricProperty;
                ps->m_isAutoGen = false;
            }
        }
    }


    //find base classes
    m_baseClass = clas;
    
    m_baseFeatureClass = (clas->GetClassType() == FdoClassType_FeatureClass) 
        ? (FdoFeatureClass*)clas : NULL;
    
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(clas);

    while ((base = base->GetBaseClass()) != NULL)
    {
        m_baseClass = base;
        m_baseFeatureClass = (base->GetClassType() == FdoClassType_FeatureClass)
            ? (FdoFeatureClass*)(base.p) : NULL;
    }
    
    FDO_SAFE_ADDREF(m_baseClass);
    FDO_SAFE_ADDREF(m_baseFeatureClass);

    m_fcid = fcid;
}


FdoCommonPropertyIndex::~FdoCommonPropertyIndex()
{
    FDO_SAFE_RELEASE(m_baseClass);
    FDO_SAFE_RELEASE(m_baseFeatureClass);
    
    delete [] m_vProps;
}


FdoCommonPropertyStub* FdoCommonPropertyIndex::GetPropInfo(FdoString* name)
{
    FdoCommonPropertyStub* ps = NULL;

    //search for the correct FdoCommonPropertyStub -- iterate linearly
    //since we have relatively few properties. Using a hash_map
    //was slow for this
    for (int i=0; i<m_numProps; i++)
    {
        ps = &m_vProps[i];

        if (wcscmp((wchar_t*)name, ps->m_name) == 0)
            return ps;
    }

    return NULL;
}

bool FdoCommonPropertyIndex::IsPropAutoGen(FdoString* name)
{
    FdoCommonPropertyStub* ps = GetPropInfo(name);

    return ps->m_isAutoGen;
}

int FdoCommonPropertyIndex::GetNumProps()
{
    return m_numProps;
}


FdoClassDefinition* FdoCommonPropertyIndex::GetBaseClass()
{
    return m_baseClass;
}

FdoFeatureClass* FdoCommonPropertyIndex::GetBaseFeatureClass()
{
    return m_baseFeatureClass;
}

unsigned int FdoCommonPropertyIndex::GetFCID()
{
    return m_fcid;
}

bool FdoCommonPropertyIndex::HasAutoGen()
{
    return m_bHasAutoGen;
}

FdoCommonPropertyStub* FdoCommonPropertyIndex::GetPropInfo(int index)
{
    if (index < 0 || index >= m_numProps)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), index));

    return &m_vProps[index];
}

FdoGeometricPropertyDefinition* FdoCommonPropertyIndex::FindGeomProp(FdoClassDefinition* clas)
{
    if (clas->GetClassType() != FdoClassType_FeatureClass)
        return NULL;

    FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)clas)->GetGeometryProperty();
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(clas);

    //navigate up class hierarchy until we find a geom property or 
    //until we reach a NULL base class, in which case the geom prop is NULL
    while (    gpd.p == NULL 
           && (base = base->GetBaseClass()) != NULL )
        gpd = ((FdoFeatureClass*)base.p)->GetGeometryProperty();

    return FDO_SAFE_ADDREF(gpd.p);
}


