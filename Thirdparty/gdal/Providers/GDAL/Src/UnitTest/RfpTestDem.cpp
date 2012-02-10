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
 */

#include "UnitTest.h"
#include "RfpTestDem.h"

RfpTestDem::RfpTestDem()
{
}

RfpTestDem::~RfpTestDem()
{
}

void RfpTestDem::_setUp()
{
}

void RfpTestDem::_tearDown()
{
}

void RfpTestDem::testDataStream()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/DEM/dem.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
//	FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
    FdoPtr<FdoISelect> cmdSelect = (FdoISelect*)(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    CPPUNIT_ASSERT(featureReader->ReadNext());

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoInt32 xSize = raster->GetImageXSize();
    FdoInt32 ySize = raster->GetImageYSize();
    CPPUNIT_ASSERT(xSize == 337);
    CPPUNIT_ASSERT(ySize == 469); // interestingly GDAL has a different value than old RFP provider!

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();

    // old rfp provider returns 32, not sure why!
    CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 16);

    // old provider returns _Data as type, but we have no way to distinguish.
    CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_Gray);

    CPPUNIT_ASSERT(dataModel->GetTileSizeX() == raster->GetImageXSize());
    CPPUNIT_ASSERT(dataModel->GetTileSizeY() == raster->GetImageYSize());

    // retiling
    const FdoInt32 tileSizeX = 128, tileSizeY = 128;
    dataModel->SetTileSizeX(tileSizeX);
    dataModel->SetTileSizeY(tileSizeY);
    raster->SetDataModel(dataModel);

    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	
    int numTileRows = (337 - 1) / tileSizeX + 1;
    int numTileCols = (469 - 1) / tileSizeY + 1;
    const int bytesTile = tileSizeX * tileSizeY * 2;

    FdoByte* buffer = new FdoByte[bytesTile];
    for (int i = 0; i < numTileRows; i++)
    {
        // read the first tile of the tile row
        FdoInt32 numRead = reader->ReadNext(buffer, 0, bytesTile);
        CPPUNIT_ASSERT(numRead == bytesTile);
        // skip the rest tiles
        reader->Skip(bytesTile * (numTileCols - 1));
    }
    
    // no data
    CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);
    delete[] buffer;

    // scale down to 1/4 of original size and read it row by row
    raster->SetImageXSize(168);
    raster->SetImageYSize(233);
    dataModel->SetTileSizeX(raster->GetImageXSize());
    dataModel->SetTileSizeY(raster->GetImageYSize());
    raster->SetDataModel(dataModel);
    reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	

    // iterate all rows
    FdoByte buff[336]; //336=168*sizeof(int16)
    for (int row = 0; row < 233; row++)
    {
        FdoInt32 numRead = reader->ReadNext(buff, 0, sizeof(buff));
        CPPUNIT_ASSERT(numRead == sizeof(buff));
    }
    CPPUNIT_ASSERT(reader->ReadNext(buff, 0, 1) == 0);

    connection->Close();
}

void RfpTestDem::testConversion()
{
    FdoPtr<FdoIConnection> connection = CreateConnection();
    FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"../../TestData/DEM/dem.xml", L"r");
    connection->SetConfiguration(stream);
    connection->Open();

    FdoICommand* cmd = connection->CreateCommand(FdoCommandType_Select);
//	FdoPtr<FdoISelect> cmdSelect = dynamic_cast<FdoISelect*>(cmd);
    FdoPtr<FdoISelect> cmdSelect = (FdoISelect*)(cmd);
    cmdSelect->SetFeatureClassName(L"Photo");
    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

    CPPUNIT_ASSERT(featureReader->ReadNext());

    FdoPtr<FdoIRaster> raster = featureReader->GetRaster(L"Image");
    FdoInt32 xSize = raster->GetImageXSize();
    FdoInt32 ySize = raster->GetImageYSize();
    CPPUNIT_ASSERT(xSize == 337);
    CPPUNIT_ASSERT(ySize == 469);

    FdoPtr<FdoRasterDataModel> dataModel = raster->GetDataModel();
    CPPUNIT_ASSERT(dataModel->GetBitsPerPixel() == 16);
    CPPUNIT_ASSERT(dataModel->GetDataModelType() == FdoRasterDataModelType_Gray);
    CPPUNIT_ASSERT(dataModel->GetDataType() == FdoRasterDataType_Integer);

    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > reader = dynamic_cast<FdoIStreamReaderTmpl<FdoByte>*>(raster->GetStreamReader());	

    int bufferSize = xSize * ySize * 2;
    FdoByte* buffer = new FdoByte[bufferSize];

    FdoInt32 numRead = reader->ReadNext(buffer, 0, bufferSize);
    CPPUNIT_ASSERT(numRead == bufferSize);
    CPPUNIT_ASSERT(reader->ReadNext(buffer, 0, 1) == 0);

    delete[] buffer;

    connection->Close();
}

