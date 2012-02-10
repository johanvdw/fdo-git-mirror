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

#pragma  once

#include "FDO\Schema\mgNetworkFeatureClass.h"
#include "FDO\Schema\mgClassType.h"

class FdoNetworkLinkFeatureClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class AssociationPropertyDefinition;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// NetworkLinkFeature derives NetworkFeature 
/// and can be used to represent a logical link feature.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
public ref class NetworkLinkFeatureClass : public NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass
{
public:
    /// \brief
    /// Constructs a default instance of an NetworkLinkFeatureClass.
    /// 
	NetworkLinkFeatureClass();

    /// \brief
    /// Constructs an instance of an NetworkLinkFeatureClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	NetworkLinkFeatureClass(System::String^ name, System::String^ description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType ClassType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get();
    }

    /// \brief
    /// Gets a reference to the start node property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
    /// \brief
    /// Sets a reference to the start node property.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ StartNodeProperty
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value);
    }

    /// \brief
    /// Gets a reference to the end node property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
    /// \brief
    /// Sets a reference to the end node property.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ EndNodeProperty
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ value);
    }

    /// \brief
    /// Constructs a NetworkLinkFeatureClass object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	NetworkLinkFeatureClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(unmanaged, autoDelete)
	{

	}

internal:
	inline FdoNetworkLinkFeatureClass* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


