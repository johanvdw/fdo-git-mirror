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

#include <stdafx.h>
#include <OWS/FdoOwsGetCapabilities.h>
#include <OWS/FdoOwsResponse.h>
#include "FdoWmsDelegate.h"
#include "FdoWmsServiceMetadata.h"
#include "FdoWmsGetMap.h"
#include "FdoWmsXmlGlobals.h"

FdoWmsDelegate::FdoWmsDelegate() :
     FdoOwsDelegate(NULL, NULL, NULL, NULL, NULL, NULL, NULL)
{
}

FdoWmsDelegate::FdoWmsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd, FdoString* proxy_location, FdoString* proxy_port, FdoString* proxy_user, FdoString* proxy_password) :
     FdoOwsDelegate(defaultUrl, userName, passwd, proxy_location, proxy_port, proxy_user, proxy_password)
{
}


FdoWmsDelegate::~FdoWmsDelegate()
{
}

void FdoWmsDelegate::Dispose()
{ 
    delete this; 
}

FdoWmsDelegate* FdoWmsDelegate::Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd, FdoString* proxy_location, FdoString* proxy_port, FdoString* proxy_user, FdoString* proxy_password)
{
    return new FdoWmsDelegate(defaultUrl, userName, passwd, proxy_location, proxy_port, proxy_user, proxy_password);
}

FdoWmsDelegate* FdoWmsDelegate::Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd)
{
    return new FdoWmsDelegate(defaultUrl, userName, passwd);
}

FdoWmsServiceMetadata* FdoWmsDelegate::GetServiceMetadata(FdoString* pVersion)
{
    FdoPtr<FdoOwsGetCapabilities> request = FdoOwsGetCapabilities::Create(FdoWmsXmlGlobals::WMSServiceName);
    if (pVersion == NULL || wcslen(pVersion) == 0)
        request->SetVersion(FdoWmsXmlGlobals::WmsVersion);
    else
        request->SetVersion(pVersion, false); // version already in URL.
    FdoPtr<FdoOwsResponse> response = Invoke(request);
    FdoPtr<FdoIoStream> stream = response->GetStream();
    FdoWmsServiceMetadataP rv = FdoWmsServiceMetadata::Create();
    rv->ReadXml(stream);

    return FDO_SAFE_ADDREF(rv.p);
}

FdoIoStream* FdoWmsDelegate::GetMap(FdoStringCollection* layerNames,
									FdoStringCollection* styleNames,
									FdoWmsBoundingBox* bbox, 
									FdoString* imgFormat,
									FdoSize height,
									FdoSize width,
									FdoBoolean bTransparent,
									FdoString* backgroundColor,
									FdoString* timeDimension,
									FdoString* elevation,
									FdoString* version,
									FdoString* exceptionFormat)
{
	VALIDATE_ARGUMENT (layerNames);
	VALIDATE_ARGUMENT (styleNames);
	VALIDATE_ARGUMENT (bbox);
	VALIDATE_ARGUMENT (imgFormat);
	VALIDATE_ARGUMENT (backgroundColor);
	VALIDATE_ARGUMENT (timeDimension);
	VALIDATE_ARGUMENT (elevation);
	
	FdoStringP crs = bbox->GetCRS ();
	FdoDouble minX = bbox->GetMinX ();
	FdoDouble minY = bbox->GetMinY ();
	FdoDouble maxX = bbox->GetMaxX ();
	FdoDouble maxY = bbox->GetMaxY ();

	FdoPtr<FdoWmsGetMap> request = FdoWmsGetMap::Create (layerNames, styleNames, crs, imgFormat, height, width, minX, minY, maxX, maxY, version,exceptionFormat, bTransparent, backgroundColor, timeDimension, elevation);
    FdoPtr<FdoOwsResponse> response = Invoke (request);
	FdoPtr<FdoIoStream> stream = response->GetStream ();

	return FDO_SAFE_ADDREF (stream.p);
}
