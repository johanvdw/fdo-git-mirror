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

#include "Pch.h"
#include "WmsTestSelect.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WmsTestSelect );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( WmsTestSelect, "WmsTestSelect");

WmsTestSelect::WmsTestSelect ()
{
}

WmsTestSelect::~WmsTestSelect ()
{
}

#ifdef _FDO_RUN_INTERNAL_TEST
void WmsTestSelect::testServer1 ()
{
//TODO: remove this when ? the bug ? on Linux is fixed
#ifdef WIN32
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://cadc-isd-jake.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmd->SetFeatureClassName (L"airports");
	    FdoPtr<FdoIdentifierCollection> selProps = cmd->GetPropertyNames ();
	    CPPUNIT_ASSERT (selProps->GetCount () == 0);
	    FdoPtr<FdoIdentifier> prop = FdoIdentifier::Create (L"FeatId");
	    selProps->Add (prop);
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

	    // test GetClassDefinition method
	    FdoPtr<FdoClassDefinition> clsDef = featReader->GetClassDefinition ();
	    FdoPtr<FdoPropertyDefinitionCollection> props = clsDef->GetProperties ();	
	    CPPUNIT_ASSERT (wcscmp (clsDef->GetName (), L"airports") == 0);
	    CPPUNIT_ASSERT (props->GetCount () == 0);
	    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = clsDef->GetBaseProperties ();
	    CPPUNIT_ASSERT (baseProps->GetCount () != 0);
    	

	    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader;

	    // Layer "airports"
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    CPPUNIT_ASSERT (wcscmp (featReader->GetString(L"FeatId"), L"airports") == 0);
	    try 
	    {
		    (featReader->GetString(L"non-exist"));		
		    CPPUNIT_ASSERT (false);
	    }	
	    catch(FdoException*)
	    {
		    // Expected exception caught.
	    }

	    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
	    raster->SetImageXSize(1000);
	    raster->SetImageYSize(1000);
	    FdoIStreamReader* streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    CPPUNIT_ASSERT (streamReader != NULL);
	    CPPUNIT_ASSERT (byteStreamReader != NULL);
	    FdoByte buff[4096];
	    FdoInt64 cntTotal = 0;
	    FdoInt32 cntRead = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    FdoPtr<FdoIRasterPropertyDictionary> x = raster->GetAuxiliaryProperties();
	    FdoDataValue* val1 = x->GetProperty(L"Palette");
	    FdoDataValue* val2 = x->GetProperty(L"NumOfPaletteEntries");

	    struct RgbColor
	    {
		    union 
		    {
			    struct { FdoByte red; FdoByte green; FdoByte blue; FdoByte alpha; } rgba;
			    FdoInt32 packed;
		    };
	    };

	    FdoPtr<FdoInt32Value> val3 = static_cast<FdoInt32Value*>(val2);
	    FdoPtr<FdoBLOBValue> val4 = static_cast<FdoBLOBValue*>(val1);
	    FdoPtr<FdoByteArray> paletteData  = val4->GetData();

	    RgbColor*	palette = (RgbColor*)paletteData->GetData();

	    CPPUNIT_ASSERT (cntTotal == 1000000);	
	    CPPUNIT_ASSERT (!featReader->ReadNext ());	

	    // Layer "dlgstln2"
	    cmd->SetFeatureClassName (L"dlgstln2");	

	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());

	    // get the raster first time
	    raster = featReader->GetRaster (L"Raster");
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    // get the raster second time
	    raster = featReader->GetRaster (L"Raster");
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "lakespy2"
	    cmd->SetFeatureClassName (L"lakespy2");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
	    streamReader = raster->GetStreamReader ();
	    byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

	    cntTotal = 0;
	    do
	    {
		    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
		    cntTotal += cntRead;
	    }
	    while (cntRead);

	    // Layer "mcd90py2"
	    cmd->SetFeatureClassName (L"mcd90py2");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());	

	    // Layer "ITASCA"
	    cmd->SetFeatureClassName (L"ITASCA");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    try
	    {
		    raster = featReader->GetRaster (L"Raster");	
		    CPPUNIT_ASSERT (false);
	    }
	    catch (FdoException* ex)
	    {
		    ex->Release ();
	    }

	    featReader->GetString (L"FeatId");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "ctybdpy2"
	    cmd->SetFeatureClassName (L"ctybdpy2");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());	
	    raster = featReader->GetRaster (L"Raster");	
	    featReader->GetString (L"FeatId");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }

#endif //WIN32
}

void WmsTestSelect::testNestedClip ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://cadc-isd-jake.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName (L"airports");

	    // set up clip function: CLIP(Image, 100, 100, 1500.0, 1000.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create(1500, FdoDataType_Double);
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(1000, FdoDataType_Double);
	    funcParams->Add(maxY);

	    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
	    FdoPtr<FdoExpressionCollection> outerFuncParams = FdoExpressionCollection::Create ();
	    minX = FdoDataValue::Create (500);
	    minY = FdoDataValue::Create (500);
	    maxX = FdoDataValue::Create (2000);
	    maxY = FdoDataValue::Create (1200);
	    outerFuncParams->Add (clipFunc);
	    outerFuncParams->Add (minX);
	    outerFuncParams->Add (minY);
	    outerFuncParams->Add (maxX);
	    outerFuncParams->Add (maxY);

	    FdoPtr<FdoFunction> outerClipFunc = FdoFunction::Create (L"CLIP", outerFuncParams);
	    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", outerClipFunc);
    	
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(clipIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    CPPUNIT_ASSERT (featureReader->ReadNext ());
	    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"clippedRaster");
	    CPPUNIT_ASSERT (raster.p);
	    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
	    CPPUNIT_ASSERT (stream.p);
	    CPPUNIT_ASSERT (!featureReader->ReadNext ());	
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

// http://cadc-brucejia.ads.autodesk.com:8888/cgi-bin/mapserv.exe?map=wms/wms.map
void WmsTestSelect::testHttpBasicAuthentification ()
{
	try
	{
		FdoPtr<FdoIConnection> conn = this->GetConnection ();

		// use ip address directly
		conn->SetConnectionString (L"FeatureServer=http://cadc-brucejia.ads.autodesk.com:8888/cgi-bin/mapserv.exe?map=wms/wms.map;Username=bruce;Password=brucebruce");
		CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

		FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
		FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

		FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
		cmdSelect->SetFeatureClassName(L"lakespy2");


		FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

		while (featureReader->ReadNext ())
		{
			FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
			FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
		}
	}
	catch(FdoException* ex)
	{
		CPPUNIT_FAIL ((const char*)FdoStringP(ex->GetExceptionMessage ()));
	}
}

void WmsTestSelect::testGetBounds ()
{
    FdoPtr<FdoIConnection> conn = this->GetConnection ();
    conn->SetConnectionString (L"FeatureServer=http://cadc-isd-jake.ads.autodesk.com/cgi-bin/mapserv.exe?map=wms/wms.map&");
    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
    cmd->SetFeatureClassName (L"ITASCA");	    
    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

    CPPUNIT_ASSERT (featReader->ReadNext ());
    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
    FdoPtr<FdoByteArray> gba = raster->GetBounds ();
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
    FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
    FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

    CPPUNIT_ASSERT (enve->GetMinX() == 388108.00);
    CPPUNIT_ASSERT (enve->GetMinY() == 5203120.00);
    CPPUNIT_ASSERT (enve->GetMaxX() == 500896.00);
    CPPUNIT_ASSERT (enve->GetMaxY() == 5310240.00);
#ifdef _DEBUG
    wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif
}

#endif //_FDO_RUN_INTERNAL_TEST

// http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp
void WmsTestSelect::testServer2 ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=400");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoISelect> cmd = static_cast<FdoISelect *> (conn->CreateCommand (FdoCommandType_Select));
	    cmd->SetFeatureClassName (L"IBA");
	    FdoPtr<FdoIFeatureReader> featReader = cmd->Execute ();

	    // Layer "IBA"
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CBC_PT"
	    cmd->SetFeatureClassName (L"CBC_PT");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CBC_PY"
	    cmd->SetFeatureClassName (L"CBC_PY");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "MMP"
	    cmd->SetFeatureClassName (L"MMP");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "CLLS"
	    cmd->SetFeatureClassName (L"CLLS");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_SQUARE"
	    cmd->SetFeatureClassName (L"OBBA_SQUARE");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_REGION"
	    cmd->SetFeatureClassName (L"OBBA_REGION");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "OBBA_BLOCK"
	    cmd->SetFeatureClassName (L"OBBA_BLOCK");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());


	    // Layer "OWLS"
	    cmd->SetFeatureClassName (L"OWLS");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());

	    // Layer "BBS_PT"
	    cmd->SetFeatureClassName (L"BBS_PT");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
    	
	    // Layer "BSC_WMS" (non-queryable)
	    cmd->SetFeatureClassName (L"BSC_WMS");
	    featReader = cmd->Execute ();
	    CPPUNIT_ASSERT (featReader->ReadNext ());
	    raster = featReader->GetRaster (L"Raster");
	    CPPUNIT_ASSERT (!featReader->ReadNext ());
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void WmsTestSelect::testResample ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=400");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"IBA");
    	
	    // set up clip function: CLIP(Image, 100, 100, 700.0, 500.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create((FdoInt32(700)));
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(500, FdoDataType_Double);
	    funcParams->Add(maxY);
	    FdoPtr<FdoDataValue> height = FdoDataValue::Create(800, FdoDataType_Double);
	    funcParams->Add(height);
	    FdoPtr<FdoDataValue> width = FdoDataValue::Create(600, FdoDataType_Double);
	    funcParams->Add(width);

	    FdoPtr<FdoFunction> resampleFunc = FdoFunction::Create(L"RESAMPLE", funcParams);
	    FdoPtr<FdoComputedIdentifier> resampleIdentifier = FdoComputedIdentifier::Create(L"resampledRaster", resampleFunc);
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(resampleIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"resampledRaster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void WmsTestSelect::testClip ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"IBA");
    	
	    // set up clip function: CLIP(Image, 100, 100, 700.0, 500.0)
	    FdoPtr<FdoExpressionCollection> funcParams = FdoExpressionCollection::Create();
	    FdoPtr<FdoIdentifier> rasterProp = FdoIdentifier::Create(L"Image");
	    funcParams->Add(rasterProp);
	    FdoPtr<FdoDataValue> minX = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minX);
	    FdoPtr<FdoDataValue> minY = FdoDataValue::Create(100, FdoDataType_Double);
	    funcParams->Add(minY);
	    FdoPtr<FdoDataValue> maxX = FdoDataValue::Create((FdoInt32(700)));
	    funcParams->Add(maxX);
	    FdoPtr<FdoDataValue> maxY = FdoDataValue::Create(500, FdoDataType_Double);
	    funcParams->Add(maxY);

	    FdoPtr<FdoFunction> clipFunc = FdoFunction::Create(L"CLIP", funcParams);
	    FdoPtr<FdoComputedIdentifier> clipIdentifier = FdoComputedIdentifier::Create(L"clippedRaster", clipFunc);
	    FdoPtr<FdoIdentifierCollection> propsToSelect = cmdSelect->GetPropertyNames();
    	
	    // add it to the properties to select
	    propsToSelect->Add(clipIdentifier);

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"clippedRaster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

void WmsTestSelect::testDefaultHeight()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://www.bsc-eoc.org/cgi-bin/bsc_ows.asp;DefaultImageHeight=300");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoICommand* cmd = conn->CreateCommand(FdoCommandType_Select);
	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(cmd);
	    cmdSelect->SetFeatureClassName(L"BSC_WMS");


	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();		
	    }
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

//http://wms.jpl.nasa.gov/wms.cgi?
void WmsTestSelect::testNASAServer ()
{	
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"NASA_WMS_Config.xml", L"r");
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"global_mosaic");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
		    FdoIStreamReader* streamReader = raster->GetStreamReader ();
		    FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);
    		
		    CPPUNIT_ASSERT (streamReader != NULL);
		    CPPUNIT_ASSERT (byteStreamReader != NULL);
    		
		    FdoByte buff[4096];
		    FdoInt64 cntTotal = 0;
		    FdoInt32 cntRead = 0;
		    do
		    {
			    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
			    cntTotal += cntRead;
		    }
		    while (cntRead);
	    }
	}
	catch(FdoException* ex)
	{
		CPPUNIT_FAIL ((const char*)FdoStringP(ex->GetExceptionMessage ()));
	}
}

//http://wms.jpl.nasa.gov/wms.cgi?
void WmsTestSelect::testNASAServer2 ()
{	
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"us_landsat_wgs84");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    while (featureReader->ReadNext ())
	    {
		    FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
		    FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

		    CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
		    FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
		    CPPUNIT_ASSERT (byteStreamReader != NULL);
    		
		    FdoByte buff[4096];
		    FdoInt64 cntTotal = 0;
		    FdoInt32 cntRead = 0;
		    do
		    {
			    cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
			    cntTotal += cntRead;
		    }
		    while (cntRead);
	    }
	}
	catch(FdoException* ex)
	{
		CPPUNIT_FAIL ((const char*)FdoStringP(ex->GetExceptionMessage ()));
	}
}

//http://maps1.intergraph.com/wms/world/request.asp
void WmsTestSelect::testIntegraphWorld ()
{	
    try
    {
        FdoPtr<FdoIConnection> conn = this->GetConnection ();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> props = info->GetConnectionProperties();
        props->SetProperty(L"FeatureServer", L"http://maps1.intergraph.com/wms/world/request.asp");
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        CPPUNIT_ASSERT (FdoStringP(schema->GetName()) == L"WMS_Schema");

        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(L"COUNTRY");

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(classDef->GetName());

        FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
        FdoPtr<FdoClassDefinition> classDef2 = featureReader->GetClassDefinition();
        CPPUNIT_ASSERT (FdoStringP(classDef2->GetName()) == L"COUNTRY");

        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }
    }
    catch(FdoException* ex)
    {
	    CPPUNIT_FAIL ((const char*)FdoStringP(ex->GetExceptionMessage ()));
    }
}

//http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?CONFIG=main
void WmsTestSelect::testCubeServer ()
{
    try
    {
        FdoPtr<FdoIConnection> conn = this->GetConnection ();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> props = info->GetConnectionProperties();
        props->SetProperty(L"FeatureServer", L"http://demo.cubewerx.com/demo/cubeserv/cubeserv.cgi?CONFIG=main");
        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

        FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);

#ifdef _DEBUG
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoInt32 cntClass = classes->GetCount ();
        for (int i=0; i<cntClass; i++)
        {
            FdoPtr<FdoClassDefinition> classDef = classes->GetItem(i);
            printf ("Class: %s\n", (const char*)FdoStringP(classDef->GetName()));
        }
#endif//_DEBUG

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(L"Trails DEMIS World Map");

        FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();
        FdoPtr<FdoClassDefinition> classDef2 = featureReader->GetClassDefinition();
#ifdef _DEBUG
        printf ("ClassDefinition: %s\n", (const char*)FdoStringP(classDef2->GetName()));
#endif
        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Raster");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }

        conn->Close ();

        // set configuration file 
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"CubeServer_WMS_Config.xml", L"r");
        conn->SetConfiguration (fileStream);

        CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());
        cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        cmdSelect->SetFeatureClassName(L"TopographyDemisWorldMap");
        featureReader = cmdSelect->Execute();
        while (featureReader->ReadNext ())
        {
            FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");
            FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();

            CPPUNIT_ASSERT (stream->GetType() == FdoStreamReaderType_Byte);
            FdoIoByteStreamReader* byteStreamReader = static_cast<FdoIoByteStreamReader*> (stream.p);
            CPPUNIT_ASSERT (byteStreamReader != NULL);
        	
            FdoByte buff[4096];
            FdoInt64 cntTotal = 0;
            FdoInt32 cntRead = 0;
            do
            {
                cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
                cntTotal += cntRead;
            }
            while (cntRead);
        }

        conn->Close ();
    }
    catch(FdoException* ex)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(ex->GetExceptionMessage()));
    }
}

//
// Test against NASA server with multi-layers.
// SRS: EPSG:4326
// Layers: srtm_mag, global_mosaic
void WmsTestSelect::testMultiLayers ()
{
    try
    {
	    FdoPtr<FdoIConnection> conn = this->GetConnection ();
	    FdoIoFileStreamP fileStream = FdoIoFileStream::Create(L"MultiLayers_Config.xml", L"r");
	    conn->SetConnectionString (L"FeatureServer=http://wms.jpl.nasa.gov/wms.cgi?");
	    conn->SetConfiguration (fileStream);
	    CPPUNIT_ASSERT (FdoConnectionState_Open == conn->Open ());

	    FdoPtr<FdoIDescribeSchema> cmdDescribeSchema = static_cast<FdoIDescribeSchema *> (conn->CreateCommand (FdoCommandType_DescribeSchema));
	    FdoPtr<FdoFeatureSchemaCollection> schemas = cmdDescribeSchema->Execute ();

	    FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
	    cmdSelect->SetFeatureClassName(L"global_mosaic");

	    FdoPtr<FdoIFeatureReader> featureReader = cmdSelect->Execute();

	    CPPUNIT_ASSERT (featureReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featureReader->GetRaster (L"Image");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

        CPPUNIT_ASSERT (enve->GetMinX() == -180.00);
        CPPUNIT_ASSERT (enve->GetMinY() == -60.00);
        CPPUNIT_ASSERT (enve->GetMaxX() == 180.00);
        CPPUNIT_ASSERT (enve->GetMaxY() == 84.00);
#ifdef _DEBUG
        wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

        FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
        FdoIStreamReader* streamReader = raster->GetStreamReader ();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

        CPPUNIT_ASSERT (streamReader != NULL);
        CPPUNIT_ASSERT (byteStreamReader != NULL);

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

	    CPPUNIT_ASSERT (!featureReader->ReadNext ());
	}
	catch(FdoException* ex)
	{
        CPPUNIT_FAIL((const char*)FdoStringP(ex->GetExceptionMessage()));
	}
}

// test against http://terraservice.net/ogccapabilities.ashx. This site uses
// different URLs for "GetCapabilities" and "GetMap" operations.
void WmsTestSelect::testTerraService ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://www.terraservice.net/ogccapabilities.ashx");
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"DOQ");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

#ifdef _DEBUG
        wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

        FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
        FdoIStreamReader* streamReader = raster->GetStreamReader ();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

        CPPUNIT_ASSERT (streamReader != NULL);
        CPPUNIT_ASSERT (byteStreamReader != NULL);

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

	    CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

// http://maps.york.ca/wmsconnector/com.esri.wms.Esrimap/WMS_RegOrtho?
void WmsTestSelect::testDefect786029()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://maps.york.ca/wmsconnector/com.esri.wms.Esrimap/WMS_RegOrtho?;DefaultImageHeight=1024");
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"Orthos95"); // or "Roads"
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

#ifdef _DEBUG
        wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

        FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
        FdoIStreamReader* streamReader = raster->GetStreamReader ();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

        CPPUNIT_ASSERT (streamReader != NULL);
        CPPUNIT_ASSERT (byteStreamReader != NULL);

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

	    CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}

// http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/OCTOP_WMS?
void WmsTestSelect::testEusoils()
{
    try
    {
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        connection->SetConnectionString(L"FeatureServer=http://eusoils.jrc.it/wmsconnector/com.esri.wms.Esrimap/OCTOP_WMS?");
        FdoConnectionState state = connection->Open ();

        FdoPtr<FdoISelect> cmdSelect = static_cast<FdoISelect*> (connection->CreateCommand (FdoCommandType_Select));
        cmdSelect->SetFeatureClassName (L"3");
        FdoPtr<FdoIFeatureReader> featReader = cmdSelect->Execute ();
        CPPUNIT_ASSERT (featReader->ReadNext ());	    
        FdoPtr<FdoIRaster> raster = featReader->GetRaster (L"Raster");

        // Test the bounds
        FdoPtr<FdoByteArray> gba = raster->GetBounds ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        FdoPtr<FdoIGeometry> geom = factory->CreateGeometryFromFgf (gba.p);
        FdoPtr<FdoIEnvelope> enve = geom->GetEnvelope ();

#ifdef _DEBUG
        wprintf (L"Extent: (%f,%f %f,%f)\n", enve->GetMinX(), enve->GetMinY(), enve->GetMaxX(), enve->GetMaxY());                
#endif

        FdoPtr<FdoIStreamReader> stream = raster->GetStreamReader ();
        FdoIStreamReader* streamReader = raster->GetStreamReader ();
        FdoPtr<FdoIStreamReaderTmpl<FdoByte> > byteStreamReader = static_cast<FdoIStreamReaderTmpl<FdoByte>*> (streamReader);

        CPPUNIT_ASSERT (streamReader != NULL);
        CPPUNIT_ASSERT (byteStreamReader != NULL);

        FdoByte buff[4096];
        FdoInt64 cntTotal = 0;
        FdoInt32 cntRead = 0;
        do
        {
            cntRead = byteStreamReader->ReadNext (buff, 0 , 4096);
            cntTotal += cntRead;
        }
        while (cntRead);

	    CPPUNIT_ASSERT (!featReader->ReadNext ());

        connection->Close ();
    }
    catch (FdoException* e)
    {
        CPPUNIT_FAIL((const char*)FdoStringP(e->GetExceptionMessage()));
    }
}
