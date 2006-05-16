/*
 * 
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

[!output PREFIX]ConnectionInfo::[!output PREFIX]ConnectionInfo ([!output PREFIX]Connection* connection) :
    mConnection (connection)
{
    // NOTE: we use a weak reference here to avoid a circular dependancy:
    // FDO_SAFE_ADDREF(mConnection.p);
}

[!output PREFIX]ConnectionInfo::~[!output PREFIX]ConnectionInfo (void)
{
}

void [!output PREFIX]ConnectionInfo::Dispose()
{
    delete this;
}

void [!output PREFIX]ConnectionInfo::validate ()
{
    if (mConnection == NULL)
        throw FdoException::Create (NlsMsgGet ([!output UPPER_PREFIX]_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* [!output PREFIX]ConnectionInfo::GetProviderName ()
{
    validate ();
    return ([!output PREFIX]ProviderName); // display name: L"OSGeo [!output PREFIX] FDO Provider"
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* [!output PREFIX]ConnectionInfo::GetProviderDisplayName ()
{
    validate ();
    return ([!output PREFIX]ProviderName);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* [!output PREFIX]ConnectionInfo::GetProviderDescription ()
{
    validate ();
    return ([!output PREFIX]ProviderDescription);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* [!output PREFIX]ConnectionInfo::GetProviderVersion ()
{
    validate ();
    return ([!output PREFIX]ProviderVersion);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* [!output PREFIX]ConnectionInfo::GetFeatureDataObjectsVersion ()
{
    validate ();
    return (FeatureDataObjectsVersion);
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* [!output PREFIX]ConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary (mConnection);

#pragma message ("ToDo: connection properties")
        // Define all the connection properties:
        //char* name;
        //wide_to_multibyte (name, CONNECTIONPROPERTY_DATASTORE);
        //FdoPtr<ConnectionProperty> property = new ConnectionProperty (CONNECTIONPROPERTY_DATASTORE,
        //        NlsMsgGet ([!output UPPER_PREFIX]_CONNECTION_PROPERTY_DATASTORE, name),
        //        L"", true, false, false, 0, NULL);
        //mPropertyDictionary->AddProperty (property);
    }
    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

