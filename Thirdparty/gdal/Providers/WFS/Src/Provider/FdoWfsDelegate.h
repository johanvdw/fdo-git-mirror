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

#ifndef FDOWFSDELEGATE_H
#define FDOWFSDELEGATE_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsDelegate.h>

class FdoWfsServiceMetadata;

class FdoWfsDelegate : public FdoOwsDelegate
{
protected:
	FdoWfsDelegate() {}
    ///
    /// Create the WFS delegate with proxy server setting. By default, no proxy server is used.
    ///
    FdoWfsDelegate(
        FdoString* defaultUrl,
        FdoString* userName,
        FdoString* passwd,
		FdoString* proxyHost = NULL,
		FdoString* proxyPort = NULL,
		FdoString* proxyUsername = NULL,
        FdoString* proxyPassword = NULL
        );
    virtual ~FdoWfsDelegate();
    virtual void Dispose() { delete this; }

public:
    typedef bool (FdoWfsCancelExecutationHander)(void*);
    static FdoWfsDelegate* Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);
    static FdoWfsDelegate* Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd, FdoString* proxy_location, FdoString* proxy_port, FdoString* proxy_user, FdoString* proxy_password);

    FdoWfsServiceMetadata* GetCapabilities(FdoString* version);
    FdoFeatureSchemaCollection* DescribeFeatureType(FdoStringCollection* typeNames,FdoString* version);
    FdoIFeatureReader* GetFeature(FdoFeatureSchemaCollection* schemas, 
                                    FdoPhysicalSchemaMappingCollection* schemaMappings, 
                                    FdoString* targetNamespace, FdoString* srsName,
                                    FdoStringCollection* propertiesToSelect,
                                    FdoString* from,
                                    FdoFilter* where,
                                    FdoString* schemaName,
                                    FdoString* version,
                                    FdoWfsCancelExecutationHander handler,
                                    void* handleData);

private:

	// pre-scan the stream
	// some stream may contain invalid characters which doesn' defined in XML spec.
	// and Xerces will stop the parse process and throw exceptions
	// to allow resume read the main part information, a workaround is to 
	// scan the XML before parsing it, and changing all the invalid characters with empty char.
	//
	// we are not going to do very restrictive replacement here and only 
	// replace some found invalid characters reported by Xerces,current the list is
	// [0x1 - 0x7] 
	// [0x11 - 0x19]
	// we trust Xerces can handle most of the characters now or in the future. 
	//
	FdoIoStream* preProcessStream(FdoIoStream* streamm, FdoWfsCancelExecutationHander handler, void* handleData);

};

typedef FdoPtr<FdoWfsDelegate> FdoWfsDelegateP;

#endif

