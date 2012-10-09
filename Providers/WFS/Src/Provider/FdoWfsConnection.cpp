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
#define WFS_MESSAGE_DEFINE
#include <../Message/Inc/WFSMessage.h>

#include <malloc.h>
#include <string.h>
#include "FdoWfsDelegate.h"
#include "FdoWfsServiceMetadata.h"
#include <OWS/FdoOwsCapabilities.h>
#include <OWS/FdoOwsRequestMetadata.h>
#include <OWS/FdoOwsOperationsMetadata.h>
#include "FdoWfsOgcFilterCapabilities.h"
#include "FdoWfsFeatureTypeList.h"
#include "FdoWfsFeatureType.h"
#include <FdoCommonConnStringParser.h>

#define FEATCOLLECTION      L"FeatureCollection"

// external access to connection for client services
extern "C" FDOWFS_API FdoIConnection* CreateConnection ()
{
   return (new FdoWfsConnection ());
}

FdoWfsConnection::FdoWfsConnection (void) :
    mConnectionString ((wchar_t*)NULL)
{
}

FdoWfsConnection::~FdoWfsConnection (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void FdoWfsConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* FdoWfsConnection::GetConnectionCapabilities ()
{
    return (new FdoWfsConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* FdoWfsConnection::GetSchemaCapabilities ()
{
    return (new FdoWfsSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* FdoWfsConnection::GetCommandCapabilities ()
{
    return (new FdoWfsCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* FdoWfsConnection::GetFilterCapabilities ()
{
    FdoPtr<FdoWfsOgcFilterCapabilities> ogcFilterCaps;
    if (mServiceMetadata != NULL)
        ogcFilterCaps = mServiceMetadata->GetOGCFilterCapabilities();
        
    return (new FdoWfsFilterCapabilities(ogcFilterCaps));
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* FdoWfsConnection::GetExpressionCapabilities ()
{
    return (new FdoWfsExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* FdoWfsConnection::GetRasterCapabilities (void)
{
    return (new FdoWfsRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* FdoWfsConnection::GetTopologyCapabilities ()
{
    return (new FdoWfsTopologyCapabilities ());
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoWfsConnection::GetGeometryCapabilities()
{
    return (new FdoWfsGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* FdoWfsConnection::GetConnectionString ()
{
    return (mConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConnectionString (FdoString* value)
{
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
        FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());
        dictionary->UpdateFromConnectionString (mConnectionString);
    }
    else
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_103_CONNECTION_ALREADY_OPEN)));
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* FdoWfsConnection::GetConnectionInfo ()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoWfsConnectionInfo (this);
    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState FdoWfsConnection::GetConnectionState ()
{
    // return "Open" only when the service metadata is available
    if (mServiceMetadata != NULL)
        return FdoConnectionState_Open;
    return FdoConnectionState_Closed;

}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 FdoWfsConnection::GetConnectionTimeout ()
{
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConnectionTimeout (FdoInt32 value)
{
    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_67_CONNECTION_TIMEOUT_NOT_SUPPORTED)));
}

/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState FdoWfsConnection::Open ()
{
    if (GetConnectionState() == FdoConnectionState_Open)
        return FdoConnectionState_Open;

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());

    FdoStringP mFeatureServer = dictionary->GetProperty (FdoWfsGlobals::FeatureServer);
    FdoStringP mUserName = dictionary->GetProperty (FdoWfsGlobals::Username);
    FdoStringP mPassword = dictionary->GetProperty (FdoWfsGlobals::Password);

    FdoStringP proxyHost = dictionary->GetProperty (FdoWfsGlobals::ProxyServer);
    FdoStringP proxyPort = dictionary->GetProperty (FdoWfsGlobals::ProxyPort);
    FdoStringP proxyUser = dictionary->GetProperty (FdoWfsGlobals::ProxyUsername);
    FdoStringP proxyPassword = dictionary->GetProperty (FdoWfsGlobals::ProxyPassword);
    
    if (0 == mFeatureServer.GetLength()) 
    {
        throw FdoException::Create (NlsMsgGet(WFS_CONNECTION_REQUIRED_PROPERTY_NULL, 
            "The required property '%1$ls' cannot be set to NULL.", FdoWfsGlobals::FeatureServer));
    }

    FdoCommonConnStringParser parser (NULL, GetConnectionString ());
    // check the validity of the connection string, i.e. it doesn�t contain unknown properties
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoException::Create (NlsMsgGet(WFS_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", GetConnectionString ()));
    if (parser.HasInvalidProperties(dictionary))
        throw FdoException::Create (NlsMsgGet(WFS_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName (dictionary)));

	// get the version parameter in the request string if set
	FdoStringP version = _getRequestWFSVersion(mFeatureServer);

    // set up the WFS delegate
    mDelegate = FdoWfsDelegate::Create(mFeatureServer, mUserName, mPassword, proxyHost, proxyPort, proxyUser, proxyPassword);

    // try to get the service metadata
    mServiceMetadata = mDelegate->GetCapabilities(version);

	if (wcscmp(GetVersion(),FdoWfsGlobals::WfsVersion) == 0)
	{
		FdoPtr<FdoOwsRequestMetadataCollection> requestMetadatas = 
			FdoPtr<FdoOwsCapabilities>(mServiceMetadata->GetCapabilities())->GetRequestMetadatas();
		mDelegate->SetRequestMetadatas(requestMetadatas);
	}
	else // in 1.1.0 and later version, we assume it uses OperationsMetadata defined in OWS
	{
		FdoPtr<FdoOwsOperationCollection> operationMetadatas = 
			FdoPtr<FdoOwsOperationsMetadata>(mServiceMetadata->GetOperationsMetadata())->GetOperations();
		mDelegate->SetOperationMetadatas(operationMetadatas);
	}

    return (GetConnectionState ());
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::Close ()
{
    mServiceMetadata = NULL;
    mDelegate = NULL;
    mSchemas = NULL;
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* FdoWfsConnection::BeginTransaction ()
{
    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_101_TRANSACTIONS_NOT_SUPPORTED)));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* FdoWfsConnection::CreateCommand (FdoInt32 commandType)
{
    FdoPtr<FdoICommand> ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new FdoWfsSelectCommand (this);
            break;
        case FdoCommandType_SelectAggregates:
            ret = new FdoWfsSelectAggregatesCommand (this);
            break;        
        case FdoCommandType_DescribeSchema:
            ret = new FdoWfsDescribeSchemaCommand (this);
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = new FdoWfsGetSpatialContexts (this);
            break;
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_102_COMMAND_NOT_SUPPORTED, "The command '%1$ls' is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoCommandTypeToString (commandType))));
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* FdoWfsConnection::CreateSchemaMapping()
{
    throw FdoException::Create (NlsMsgGet (WFS_SCHEMA_OVERRIDES_NOT_SUPPORTED, "WFS Provider does not support schema overrides."));
}


/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsConnection::SetConfiguration(FdoIoStream* configStream)
{
    throw FdoException::Create(NlsMsgGet(WFS_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED, "WFS Provider does not support configuration files."));
}

FdoWfsDelegate* FdoWfsConnection::GetWfsDelegate ()
{
    return (FDO_SAFE_ADDREF(mDelegate.p));
}

FdoWfsServiceMetadata* FdoWfsConnection::GetServiceMetadata()
{
    return FDO_SAFE_ADDREF(mServiceMetadata.p);
}

FdoBoolean FdoWfsConnection::IsSchemaLoadedFromServer()
{
    return (mSchemas == NULL) ? false : true;
}

FdoFeatureSchemaCollection* FdoWfsConnection::GetSchemas()
{
    if (mSchemas == NULL) {

		//get wfs version
		FdoStringP version = this->GetVersion();

        // First get the raw schemas from the schema document from the server
        mSchemas = mDelegate->DescribeFeatureType(NULL,version);

        // And then we have to make some adjustments to the raw schemas
        // to make the schema more user friendly.
        // We will iterate all the schema elements and corresponding mapping objects to:
        // 1. Remove the GML schemas which are useless for the end user
        // 2. Remove all the useless properties inherited from GML schema and XLink schema
        // 3. For those feature classes without any identity properties, make all non-null data properties as identity properties
        // 4. Update Spatial Context Association for geometry properties.
        // 5. For those object properties whose class definition only contains one or more geometry properties,
        //    change the property type from object to geometry cause it's a customized geometry property and thus shouldn't
        //    be recognized as object property.
        // 6. Set the Class capabilities
        FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = mSchemas->GetXmlSchemaMappings();
        FdoInt32 count = mSchemas->GetCount();
        for (int i = count - 1; i >= 0; i--) {
            FdoPtr<FdoFeatureSchema> schema = mSchemas->GetItem(i);
            FdoPtr<FdoXmlSchemaMapping> mapping = static_cast<FdoXmlSchemaMapping*>(mappings->GetItem(FdoXml::mGmlProviderName, schema->GetName()));
            
            // Doing #1
            if (wcscmp(schema->GetName(), FdoWfsGlobals::GMLSchemaName) == 0 ||
                wcscmp(schema->GetName(), FdoWfsGlobals::XLinkSchemaName) == 0)
            {
                // remove schema from schema collection and from schema mapping
                mappings->Remove(mapping.p);
                mSchemas->RemoveAt(i);
                continue;
            }

            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            FdoPtr<FdoXmlClassMappingCollection> classMappings = mapping->GetClassMappings();
            if (classes == NULL || classes->GetCount() == 0)
            {
                // remove schema from schema collection and from schema mapping
                mappings->Remove(mapping.p);
                mSchemas->RemoveAt(i);
                continue;
            }
            
            FdoPtr<FdoWfsFeatureTypeList> pFeatList = mServiceMetadata->GetFeatureTypeList();
            FdoPtr<FdoWfsFeatureTypeCollection> pFeatColl = pFeatList->GetFeatureTypes();
            bool bNameModified = false;
            for (int j  = classes->GetCount() - 1; j >= 0; j--) {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);
                if(FdoCommonOSUtil::wcsicmp(classDef->GetName(), FEATCOLLECTION) == 0)
                {
                    bNameModified = true;
                    classDef->SetName(FEATCOLLECTION);
                }

                // Prepare for implementation of #3
                FdoPtr<FdoClassDefinition> base = classDef->GetBaseClass();
                bool bNoIdentity = false;
                int cntGeometricProperties = 0;
                FdoPtr<FdoDataPropertyDefinitionCollection> ids;
                if (base != NULL)
                {
                    FdoPtr<FdoSchemaElement> parentElem = base->GetParent();
                    // in case base parent is from gml set base class to NULL
                    if(parentElem != NULL && (wcscmp(parentElem->GetName(), FdoWfsGlobals::GMLSchemaName) == 0))
                    {
                        classDef->SetBaseClass(NULL);
                        ids = classDef->GetIdentityProperties();
                        if (ids->GetCount() == 0)
                            bNoIdentity = true;
                    }
                }

                // Prepare for implementation of #5
                FdoPtr<FdoXmlClassMapping> classMapping;
                for (int k = classMappings->GetCount() - 1; k >= 0; k--) {
                    classMapping = classMappings->GetItem(k);
                    if (wcscmp(classDef->GetName(), classMapping->GetName()) == 0)
                        break;
                    classMapping = NULL;
                }

                FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
                FdoPtr<FdoXmlElementMappingCollection> elements;
                if (classMapping != NULL)
                    elements = classMapping->GetElementMappings();

                for (int k = props->GetCount() - 1; k >= 0; k--) {
                    FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);

                    // Doing #2 
                    FdoStringP name = prop->GetName();
                    if (name.Contains(L"/xlink") ||
                        name.Contains(L"xlink/") ||
                        name.Contains(L"/type") ||
                        name.Contains(L"/gml"))
                    {
                        FdoPtr<FdoXmlElementMapping> pElement;
                        for (int m = elements->GetCount() - 1; m >= 0; m--)
                        {
                            pElement = elements->GetItem(m);
                            if (wcscmp(name, pElement->GetName()) == 0) 
                            {
                                elements->RemoveAt(m);
                                break;
                            }
                        }
                        props->RemoveAt(k);
                        continue;
                    }

                    // Doing #3 
                    if (bNoIdentity && prop->GetPropertyType() == FdoPropertyType_DataProperty) {
                        FdoPtr<FdoDataPropertyDefinition> dataProp = static_cast<FdoDataPropertyDefinition*>(FDO_SAFE_ADDREF(prop.p));
                        if (!dataProp->GetNullable())
                            ids->Add(dataProp);
                        continue; // no need to do the left adjustments
                    }

                    // Doing #5
                    if (elements != NULL)
                    {
                        FdoPtr<FdoXmlElementMapping> element;
                        for (int m = elements->GetCount() - 1; m >= 0; m--) {
                            element = elements->GetItem(m);
                            if (wcscmp(prop->GetName(), element->GetName()) == 0) 
                                break;
                            element = NULL;
                        }
                        if (element == NULL)
                            continue;
                        if (prop->GetPropertyType() == FdoPropertyType_AssociationProperty)
                        {
                            FdoAssociationPropertyDefinition* ascProp = static_cast<FdoAssociationPropertyDefinition*>(prop.p);
                            FdoPtr<FdoClassDefinition> ascClassDef = ascProp->GetAssociatedClass();
                            // remove associations properties associated with a class from gml schema
                            if (ascClassDef->GetIsAbstract() && wcscmp(ascClassDef->GetName(), FdoWfsGlobals::AbstractFeature) == 0)
                            {
                                for (int m = elements->GetCount() - 1; m >= 0; m--)
                                {
                                    element = elements->GetItem(m);
                                    if (wcscmp(name, element->GetName()) == 0) 
                                    {
                                        elements->RemoveAt(m);
                                        break;
                                    }
                                }
                                props->RemoveAt(k);
                                continue;
                            }
                        }
                        if (prop->GetPropertyType() == FdoPropertyType_ObjectProperty)
                        {
                            FdoObjectPropertyDefinition* objProp = static_cast<FdoObjectPropertyDefinition*>(prop.p);
                            FdoPtr<FdoClassDefinition> objClassDef = objProp->GetClass();
                            FdoPtr<FdoClassDefinition> objBaseClassDef = objClassDef->GetBaseClass();
                            FdoPtr<FdoSchemaElement> pObjParent = objClassDef->GetParent();
                            FdoPtr<FdoPropertyDefinitionCollection> objClassProps = objClassDef->GetProperties();
                            // study the class if has no properties (including base classes)
                            int objPropsCount = objClassProps->GetCount();
                            while(objPropsCount == 0 && objBaseClassDef != NULL)
                            {
                                objBaseClassDef = objBaseClassDef->GetBaseClass();
                                if(objBaseClassDef != NULL)
                                {
                                    FdoPtr<FdoPropertyDefinitionCollection> objBaseClassProps = objBaseClassDef->GetProperties();
                                    objPropsCount = objBaseClassProps->GetCount();
                                }
                            }
                            // in case no property found or the parent is from gml schema remove the property (gml schema was removed)
                            if (objPropsCount == 0 || (pObjParent != NULL && wcscmp(pObjParent->GetName(), FdoWfsGlobals::GMLSchemaName) == 0))
                            {
                                for (int m = elements->GetCount() - 1; m >= 0; m--)
                                {
                                    element = elements->GetItem(m);
                                    if (wcscmp(name, element->GetName()) == 0) 
                                    {
                                        elements->RemoveAt(m);
                                        break;
                                    }
                                }
                                props->RemoveAt(k);
                                continue;
                            }
                            // we can replace the property only class if has properies and all are geometric property
                            bool allGeoProp = (objClassProps->GetCount() != 0);
                            for (int m = objClassProps->GetCount() - 1; m >= 0; m--)
                            {
                                FdoPtr<FdoPropertyDefinition> objClassProp = objClassProps->GetItem(m);
                                if (objClassProp->GetPropertyType() != FdoPropertyType_GeometricProperty)
                                {
                                    allGeoProp = false;
                                    break;
                                }
                            }
                            if (allGeoProp)
                            {
                                // first remove the original object property
                                props->RemoveAt(k);
                                // then add another new geometry property with the same name
                                FdoPtr<FdoGeometricPropertyDefinition> newGeoProp = FdoGeometricPropertyDefinition::Create(prop->GetName(), prop->GetDescription());
                                props->Add(newGeoProp);
                                // and we still have to change the element mapping's class mapping
                                FdoPtr<FdoXmlClassMapping> elementClass = FdoXmlClassMapping::Create(FdoGml212::mGeometryProperty,
                                                                                                        FdoGml212::mGeometryProperty,
                                                                                                        FdoGml212::mGeometryProperty);
                                element->SetClassMapping(elementClass);
                            }
                        }
                        else if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
                        {
                            FdoStringP pChoiceName = element->GetChoiceName();
                            if (pChoiceName.GetLength() != 0)
                                cntGeometricProperties++;
                            
                            // Remove the associated spatial context name.
                            // When connecting to a MapGuide server, issuing the DescribeFeatureType command ,
                            // the returned XML document will contain "fdo:srsName" attribute that at last will
                            // be converted as the spatial context name of the geometry. This is invalid. we should 
                            // clear the spatial context name to ignore the extended attribute.
                            FdoGeometricPropertyDefinition* geomProp = (FdoGeometricPropertyDefinition*)prop.p;
                            geomProp->SetSpatialContextAssociation(L"");
                        }
                    }
                } // end of for each property

                // in case we have a choice for geometry property replace them with only one geometry propertyy
                if (cntGeometricProperties)
                {
                    FdoPtr<FdoXmlClassMapping> elementClass = FdoXmlClassMapping::Create(L"GEOMETRY",
                                                                                            FdoGml212::mGeometryProperty,
                                                                                            FdoGml212::mGeometryProperty);
                    FdoPtr<FdoStringCollection> pAliasNames = FdoStringCollection::Create();
                    FdoPtr<FdoXmlElementMapping> element;
                    FdoInt32 pNewTypeGeom = 0;
                    for (int k = props->GetCount() - 1; k >= 0; k--)
                    {
                        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);
                        FdoStringP name = prop->GetName();
                        if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
                        {
                            for (int m = elements->GetCount() - 1; m >= 0; m--)
                            {
                                element = elements->GetItem(m);
                                if (wcscmp(name, element->GetName()) == 0) 
                                {
                                    FdoStringP pChoiceName = element->GetChoiceName();
                                    if (pChoiceName.GetLength() != 0)
                                    {
                                        pAliasNames->Add(name.Replace(L"gml/", L""));
                                        elements->RemoveAt(m);
                                        pNewTypeGeom |= ((FdoGeometricPropertyDefinition*)prop.p)->GetGeometryTypes();
                                        props->RemoveAt(k);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (element != NULL)
                    {
                        element->SetName(L"GEOMETRY");
                        element->SetClassMapping(elementClass);
                        element->SetAliasNames(pAliasNames);
                        elements->Add(element);
                    }
                    // then add another new geometry property with the same name
                    FdoPtr<FdoGeometricPropertyDefinition> newGeoProp = FdoGeometricPropertyDefinition::Create(L"GEOMETRY", FdoGml212::mGeometryProperty);
                    newGeoProp->SetGeometryTypes(pNewTypeGeom);
                    props->Add(newGeoProp);
                }

                if (bNameModified)
                {
                    for (int k = classMappings->GetCount() - 1; k >= 0; k--)
                    {
                        FdoPtr<FdoXmlClassMapping> pClassMapping = classMappings->GetItem(k);
                        if(FdoCommonOSUtil::wcsicmp(pClassMapping->GetName(), FEATCOLLECTION) == 0)
                        {
                            classMappings->RemoveAt(k);
                            pClassMapping->SetName(FEATCOLLECTION);
                            classMappings->Insert(k, pClassMapping);
                        }
                    }
                    FdoPtr<FdoXmlElementMappingCollection> elemMappings = mapping->GetElementMappings();
                    for (int k = elemMappings->GetCount() - 1; k >= 0; k--)
                    {
                        FdoPtr<FdoXmlElementMapping> pElemMapping = elemMappings->GetItem(k);
                        if(FdoCommonOSUtil::wcsicmp(pElemMapping->GetName(), FEATCOLLECTION) == 0)
                        {
                            elemMappings->RemoveAt(k);
                            pElemMapping->SetName(FEATCOLLECTION);
                            elemMappings->Insert(k, pElemMapping);
                        }
                    }
                }

                // Perform 6. Set Class capabilities
                FdoPtr<FdoClassCapabilities> classCaps = classDef->GetCapabilities();
                if (NULL == classCaps)
                {
                    // Create the class capabilities.
                    classCaps = FdoClassCapabilities::Create(*classDef.p);
                    classCaps->SetSupportsLocking(false);
                    classCaps->SetSupportsLongTransactions(false);
                    classCaps->SetSupportsWrite(false);

                    FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
                    for (int k = props->GetCount() - 1; k >= 0; k--)
                    {
                        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);
                        if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
                        {
                            classCaps->SetPolygonVertexOrderRule(prop->GetName(), FdoPolygonVertexOrderRule_CCW);
                            classCaps->SetPolygonVertexOrderStrictness(prop->GetName(), false);
                        }
                    }
                    
                    // Set the class capabilities
                    classDef->SetCapabilities(classCaps);
                }

                // Set description for class
                _setClassDescription (classDef);

            } // end of for each class

            // this step at the end - after we cleaned the class
            for (int j  = classes->GetCount() - 1; j >= 0; j--)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);
                FdoStringP className = classDef->GetName();
                if (className.Contains(FdoWfsGlobals::Dot))
                    className = className.Replace(FdoWfsGlobals::Dot, L".");
                // Prepare for implementation of #4
                FdoPtr<FdoWfsFeatureType> pFeat = pFeatColl->FindItem(className);
                if (pFeat == NULL)
                {
                    FdoStringP NameFeat = schema->GetName();
                    NameFeat += L":";
                    NameFeat += className;
                    pFeat = pFeatColl->FindItem(NameFeat);
                }
                if (pFeat != NULL)
                {
                    FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
                    for (int k = props->GetCount() - 1; k >= 0; k--)
                    {
                        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);
                        // Doing #4 
                        if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
                        {
                             FdoGeometricPropertyDefinition* pPropGeom = static_cast<FdoGeometricPropertyDefinition*>(prop.p);
                             if (pPropGeom != NULL && pPropGeom->GetSpatialContextAssociation() == NULL)
                                 pPropGeom->SetSpatialContextAssociation(pFeat->GetSRS());
                        }
                    }
                }
                else
                    classDef->SetIsAbstract(true);
            }
        }// end of for each schema
    }

    // Apply the changes on these schemas.
    for (FdoInt32 i = 0; i < mSchemas->GetCount(); ++i)
    {
        FdoPtr<FdoFeatureSchema> schema = mSchemas->GetItem(i);
        schema->AcceptChanges();
    }

    return FDO_SAFE_ADDREF(mSchemas.p);
}

void FdoWfsConnection::_setClassDescription (FdoClassDefinition* clsdef)
{
    FdoString* clsName = clsdef->GetName ();
    FdoPtr<FdoWfsServiceMetadata> serviceMetadata = GetServiceMetadata ();
    FdoPtr<FdoWfsFeatureTypeList> featTypeList = serviceMetadata->GetFeatureTypeList ();
    FdoPtr<FdoWfsFeatureTypeCollection> featTypes = featTypeList->GetFeatureTypes ();
    FdoPtr<FdoWfsFeatureType> featType = featTypes->FindItem (clsName);
    if (featType != NULL)
    {
        FdoStringP title = featType->GetTitle ();
        if(title.GetLength () != 0)
        {
            clsdef->SetDescription (title);
        }
        else
        {
            FdoStringP abstraction = featType->GetAbstract ();
            clsdef->SetDescription (abstraction);
        }
    }
}

FdoString* FdoWfsConnection::GetVersion()
{
	FdoPtr<FdoWfsServiceMetadata> meta = this->GetServiceMetadata();
	return meta->GetVersion();
}

FdoStringP FdoWfsConnection::_getRequestWFSVersion(FdoString* str)
{
    FdoStringP retStr;
    wchar_t tmpBuf[21];
    FdoString* version = L"version=";
    if (str == NULL)
        return retStr;
    int idx = 0, idxVers = 0, idxResPos = -1;
    while(*(str+idx) != L'\0' && *(version+idxVers) != L'\0')
    {
        if (towlower(*(str+idx)) == *(version+idxVers))
        {
            idxResPos = (idxResPos == -1) ? idx : idxResPos;
            idxVers++;
        }
        else
        {
            idxVers = 0;
            idx = (idxResPos != -1) ? idxResPos : idx;
            idxResPos = -1;
        }
        idx++;
    }
    if (idxResPos == -1)
        return retStr;

    idx = idxResPos+8;
    while (*(str+idx) != L'\0' && *(str+idx) != L'&') idx++;
    int szcopy = (20 < idx-idxResPos-8) ? (20) : (idx-idxResPos-8);
    wcsncpy(tmpBuf, str+idxResPos+8, szcopy );
    tmpBuf[szcopy] = L'\0';
    retStr = tmpBuf;
    return retStr;
}

