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
#include "FdoWmsRasterCapabilities.h"

FdoWmsRasterCapabilities::FdoWmsRasterCapabilities ()
{
}

FdoWmsRasterCapabilities::~FdoWmsRasterCapabilities ()
{
}

void FdoWmsRasterCapabilities::Dispose()
{
    delete this;
}

/// <summary>Returns true if the provider has raster capability.
/// This implies the capability to define new raster properties on a class
/// and store and retrieve raster images.</summary>
/// <returns>Returns true if raster images are supported, false otherwise.</returns>
bool FdoWmsRasterCapabilities::SupportsRaster ()
{
    return (true);
}

/// <summary>Returns true if the provider has the capability to stitch images from a
/// select command (query) together into a single image.</summary>
/// <returns>Returns false if this capability is not supported, and hence the
/// FdoIFeatureReader object returned from a select command will have one feature
/// for each of the FdoIRaster objects that match the select filter.
/// Returns true if this capability is supported. If the MOSAIC function
/// is specified in the select command, then a single image that is a combination
/// of the selected images is returned.</returns>
bool FdoWmsRasterCapabilities::SupportsStitching ()
{
    return (false);
}

/// <summary>Returns true if the provider can reduce the resolution of an image.</summary>
/// <returns>Returns true if this capability is supported, and hence the FdoIRaster
/// object returned by the FdoIFeatureReader from a query will honour the
/// SetImageXSize() and SetImageYSize() methods and reduce the resolution of the
/// image by one of several means of subsampling. Returns false if attempting to set
/// the image size of an FdoIRaster object returned by a FdoIFeatureReader will
/// throw an FdoException.</returns>
bool FdoWmsRasterCapabilities::SupportsSubsampling ()
{
    return (true);
}

/// <summary>Returns true if the provider can handle the provided data model.
/// It is expected that when used to transform between data models, the matrix defined
/// by input and output data models is not sparse for every data model that is supported.</summary>
/// <param name="model">The model to check for validity with this provider.</param> 
/// <returns>Returns true if the specified data model is supported, and hence the
/// FdoIRaster object will dynamically transform an image to the specified data model.
/// Returns false if the specified data model is not supported.</returns>
bool FdoWmsRasterCapabilities::SupportsDataModel (FdoRasterDataModel* model)
{
	if (model == NULL)
		return false;

	// We are using ATIL as our underlying image library, so we just support those data models supported by ATIL. 
    // ATIL can only support data interleave organization by pixel

    if (model->GetOrganization() != FdoRasterDataOrganization_Pixel)
		return false;

	FdoInt32 bpp = model->GetBitsPerPixel();

	switch (model->GetDataModelType())
	{
	case FdoRasterDataModelType_Bitonal:
		if (bpp == 1)
			return true;
		break;
	case FdoRasterDataModelType_Gray:
		if (bpp == 8)
			return true;
		break;
	case FdoRasterDataModelType_RGB:
		if (bpp == 24)
			return true;
		break;
	case FdoRasterDataModelType_RGBA:
		if (bpp == 32)
			return true;
		break;
	case FdoRasterDataModelType_Palette:
		if (bpp == 8)
			return true;
		break;
	};
	
	return false;
}
