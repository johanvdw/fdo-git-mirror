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
#include <FdoCommonStringUtil.h>

FdoWfsConnectionInfo::FdoWfsConnectionInfo (FdoWfsConnection* connection) :
    mConnection (connection)
{
    // NOTE: we use a weak reference here to avoid a circular dependancy:
    // FDO_SAFE_ADDREF(mConnection.p);
}

FdoWfsConnectionInfo::~FdoWfsConnectionInfo (void)
{
}

void FdoWfsConnectionInfo::Dispose()
{
    delete this;
}

void FdoWfsConnectionInfo::validate ()
{
    if (mConnection == NULL)
        throw FdoException::Create (NlsMsgGet (WFS_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* FdoWfsConnectionInfo::GetProviderName ()
{
    validate ();
    return (WFS_PROVIDER_NAME);
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* FdoWfsConnectionInfo::GetProviderDisplayName()
{
    validate ();
    return NlsMsgGet(WFS_PROVIDER_DISPLAY_NAME, WFS_PROVIDER_DEFAULT_DISPLAY_NAME);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* FdoWfsConnectionInfo::GetProviderDescription ()
{
    validate ();
    return NlsMsgGet(WFS_PROVIDER_DESCRIPTION, WFS_PROVIDER_DEFAULT_DESCRIPTION);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* FdoWfsConnectionInfo::GetProviderVersion ()
{
    validate ();
    return (WFS_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* FdoWfsConnectionInfo::GetFeatureDataObjectsVersion ()
{
    validate ();
    return (WFS_FDO_VERSION);
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* FdoWfsConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary (mConnection);

        char* mbPropName = NULL;
        wide_to_multibyte(mbPropName, FdoWfsGlobals::FeatureServer);
        FdoPtr<ConnectionProperty> featureServerProperty = new ConnectionProperty (
                        FdoWfsGlobals::FeatureServer,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_FEATURESRVER, mbPropName),
                        L"", true, false, false, false, false, false, false,0, NULL);
        mPropertyDictionary->AddProperty(featureServerProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::Username);
        FdoPtr<ConnectionProperty> usernameProperty = new ConnectionProperty (
                        FdoWfsGlobals::Username,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_USERNAME, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(usernameProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::Password);
        FdoPtr<ConnectionProperty> passwordProperty = new ConnectionProperty (
                        FdoWfsGlobals::Password,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PASSWORD, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(passwordProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::ProxyServer);
        FdoPtr<ConnectionProperty> proxyServerProperty = new ConnectionProperty (
                        FdoWfsGlobals::ProxyServer,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PROXYSERVER, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(proxyServerProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::ProxyPort);
        FdoPtr<ConnectionProperty> proxyPortProperty = new ConnectionProperty (
                        FdoWfsGlobals::ProxyPort,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PROXYPORT, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(proxyPortProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::ProxyUsername);
        FdoPtr<ConnectionProperty> proxyUsernameProperty = new ConnectionProperty (
                        FdoWfsGlobals::ProxyUsername,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PROXYUSERNAME, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(proxyUsernameProperty);

        wide_to_multibyte(mbPropName, FdoWfsGlobals::ProxyPassword);
        FdoPtr<ConnectionProperty> proxyPasswordProperty = new ConnectionProperty (
                        FdoWfsGlobals::ProxyPassword,
                        NlsMsgGet(WFS_CONNECTION_PROPERTY_PROXYPASSWORD, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(proxyPasswordProperty);
    }

    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

/// <summary>Returns the provider type. A provider can be a file-based, database-based or
/// web-based provider. The valid values the function may return are defined in
/// the enumeration FdoProviderDatastoreType. The enumeration includes the following
/// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
/// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.</summary>
/// <returns>Returns the provider data store type.</returns>
FdoProviderDatastoreType FdoWfsConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_WebServer;
}

/// <summary>File-based providers depend on a various files. This function returns a list
/// of fully qualified dependend file names. The return parameter will be NULL if
/// the provider is not a file-based provider.</summary>
/// <returns>Returns the list of fully-qualified dependend file names if the provider is a
/// file-based provider, NULL otherwise.</returns>
FdoStringCollection* FdoWfsConnectionInfo::GetDependentFileNames()
{
    return NULL;
}

