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
#include "FdoWmsGlobals.h"
#include "FdoWmsConnectionInfo.h"
#include "FdoWmsConnection.h"

FdoWmsConnectionInfo::FdoWmsConnectionInfo (FdoWmsConnection* connection) :
    mConnection (connection)
{
}

FdoWmsConnectionInfo::~FdoWmsConnectionInfo (void)
{
}

void FdoWmsConnectionInfo::Dispose()
{
    delete this;
}

void FdoWmsConnectionInfo::validate ()
{
    if (mConnection == NULL)
        throw FdoConnectionException::Create (NlsMsgGet (FDOWMS_CONNECTION_INVALID, "Connection is invalid."));
}

/// <summary>Gets the name of the feature provider.</summary>
/// <returns>Returns the provider name</returns>
FdoString* FdoWmsConnectionInfo::GetProviderName ()
{
    validate ();
    return (WMS_PROVIDER_NAME); 
}

/// <summary>Gets the display name of the feature provider.</summary>
/// <returns>Returns the provider's display name</returns>
FdoString* FdoWmsConnectionInfo::GetProviderDisplayName()
{
    validate ();
    return NlsMsgGet(WMS_PROVIDER_DISPLAY_NAME, WMS_PROVIDER_DEFAULT_DISPLAY_NAME);
}

/// <summary>Gets the description of the feature provider.</summary>
/// <returns>Returns the provider description</returns>
FdoString* FdoWmsConnectionInfo::GetProviderDescription ()
{
    validate ();
    return NlsMsgGet(WMS_PROVIDER_DESCRIPTION, WMS_PROVIDER_DEFAULT_DESCRIPTION);
}

/// <summary>Gets the version of the feature provider.</summary>
/// <returns>Returns provider version</returns>
FdoString* FdoWmsConnectionInfo::GetProviderVersion ()
{
    validate ();
    return (WMS_PROVIDER_VERSION);
}

/// <summary>Gets the version of the Feature Data Objects specification to which this provider conforms.</summary>
/// <returns>Returns FDO version supported.</returns>
FdoString* FdoWmsConnectionInfo::GetFeatureDataObjectsVersion ()
{
    validate ();
    return (WMS_FDO_VERSION);
}

/// <summary>Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.</summary>
/// <returns>Returns the property dictionary</returns>
FdoIConnectionPropertyDictionary* FdoWmsConnectionInfo::GetConnectionProperties ()
{
    validate ();
    if (mPropertyDictionary == NULL)
    {
        // Create the connection property dictionary
        mPropertyDictionary = new FdoCommonConnPropDictionary (mConnection);

        // Define Server connection properties:
        char* mbPropName = NULL;
        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyFeatureServer);
        FdoPtr<ConnectionProperty> property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyFeatureServer,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_FEATURESERVER, mbPropName),
                        L"", true, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyUsername);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyUsername,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_USERNAME, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyPassword);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyPassword,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_PASSWORD, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

		wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyDefaultImageHeight);
		property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyDefaultImageHeight,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_DEFAULTIMAGEHEIGHT, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
		mPropertyDictionary->AddProperty (property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyProxyServer);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyProxyServer,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_PROXYSERVER, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyProxyPort);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyProxyPort,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_PROXYPORT, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyProxyUsername);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyProxyUsername,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_PROXYUSERNAME, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

        wide_to_multibyte(mbPropName, FdoWmsGlobals::ConnectionPropertyProxyPassword);
        property = new ConnectionProperty (
                        FdoWmsGlobals::ConnectionPropertyProxyPassword,
                        NlsMsgGet(FDOWMS_CONNECTION_PROPERTY_PROXYPASSWORD, mbPropName),
                        L"", false, false, false, false, false, false, false, 0, NULL);
        mPropertyDictionary->AddProperty(property);

    }

    return (FDO_SAFE_ADDREF(mPropertyDictionary.p));
}

/// <summary>Returns the provider type. A provider can be a file-based, database-based or
/// web-based provider. The valid values the function may return are defined in
/// the enumeration FdoProviderDatastoreType. The enumeration includes the following
/// values: FdoProviderDatastoreType_Unknown, FdoProviderDatastoreType_File,
/// FdoProviderDatastoreType_DatabaseServer, FdoProviderDatastoreType_WebServer.</summary>
/// <returns>Returns the provider data store type.</returns>
FdoProviderDatastoreType FdoWmsConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_WebServer;
}

/// <summary>File-based providers depend on a various files. This function returns a list
/// of fully qualified dependend file names. The return parameter will be NULL if
/// the provider is not a file-based provider.</summary>
/// <returns>Returns the list of fully-qualified dependend file names if the provider is a
/// file-based provider, NULL otherwise.</returns>
FdoStringCollection* FdoWmsConnectionInfo::GetDependentFileNames()
{
    return NULL;
}
