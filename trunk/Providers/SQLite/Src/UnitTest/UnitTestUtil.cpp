// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include <ctime>
#include "UnitTestUtil.h"
#include "FdoCommonFile.h"
#ifndef _WIN32
#include <unistd.h>
#endif

const wchar_t* SOURCE_FILE = L"../../TestData/PARCEL_Source.sqlite";


FdoFeatureSchema* UnitTestUtil::CreateSLTSchema(FdoGeometryType singleType)
{
    FdoFeatureSchema* schema = FdoFeatureSchema::Create(L"Default",L"Schema of the Spatial Data File format");
    FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(L"Parcel",L"");    
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();

    FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create(
                                                        L"Data",
                                                        L"The geometry of the object");
    if (singleType != FdoGeometryType_None)
        geometry->SetSpecificGeometryTypes(&singleType, 1);
    else
        geometry->SetGeometryTypes(7); // Point, Line, Polygon
    properties->Add(geometry);

    clas->SetGeometryProperty(geometry);

    // Add a second geometry property (to test defect 670427).
    FdoPtr<FdoGeometricPropertyDefinition> geometry2 = FdoGeometricPropertyDefinition::Create(
                                                        L"Data2",
                                                        L"The geometry of the object");
    if (singleType != FdoGeometryType_None)
        geometry2->SetSpecificGeometryTypes(&singleType, 1);
    else
        geometry2->SetGeometryTypes(7); // Point, Line, Polygon
	geometry2->SetSpatialContextAssociation( L"World Geodetic Coordinate System, 1984" );
    properties->Add(geometry2);

    FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"Name",
                                                    L"The name of the object");
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"Url", L"The url of the object");
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);



    dpd = FdoDataPropertyDefinition::Create(L"Key", L"The key of the object");
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);
    // TODO we don't support composed primary key
    //FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"ID", L"The autogenerated sequence ID of the object");
    dpd->SetDataType(FdoDataType_Int32);
    dpd->SetIsAutoGenerated(true);
    properties->Add(dpd);
    FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);


    FdoPtr<FdoFeatureClass> clas2 = FdoFeatureClass::Create(L"ParcelChild",L"");    
    clas2->SetBaseClass(clas);
	properties = clas2->GetProperties();

	dpd = FdoDataPropertyDefinition::Create(L"Numb", L"A number property");
    dpd->SetDataType(FdoDataType_Int32);
    properties->Add(dpd);

	dpd = FdoDataPropertyDefinition::Create(L"datetime", L"A date time property");
    dpd->SetDataType(FdoDataType_DateTime);
    properties->Add(dpd);

	dpd = FdoDataPropertyDefinition::Create(L"DblNumb1", L"A double number property(1)");
    dpd->SetDataType(FdoDataType_Double);
    properties->Add(dpd);

	dpd = FdoDataPropertyDefinition::Create(L"DblNumb2", L"A double number property(2)");
    dpd->SetDataType(FdoDataType_Double);
    properties->Add(dpd);

    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas2);

	clas = FdoFeatureClass::Create(L"AbsClass",L"");    
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    clas->SetIsAbstract( true );
    properties = clas->GetProperties();
	dpd = FdoDataPropertyDefinition::Create(L"Numb", L"A number property");
    dpd->SetDataType(FdoDataType_Int32);
    properties->Add(dpd);

    return schema;
}


FdoIConnection* UnitTestUtil::CreateConnection()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    return (manager->CreateConnection (L"OSGeo.SQLite"));
}

FdoIConnection* UnitTestUtil::OpenMemoryConnection( bool add_spc, FdoIConnection  *inConn)
{
    FdoIConnection *conn = inConn;
	if( conn == NULL )
		conn = UnitTestUtil::CreateConnection();
	// Open the connection
    std::wstring connStr = L"File=\":memory:\";UseFdoMetadata=TRUE;";
	conn->SetConnectionString(connStr.c_str());
	FdoPtr<FdoIConnectionInfo>info = conn->GetConnectionInfo();
	FdoPtr<FdoIConnectionPropertyDictionary> prop = info->GetConnectionProperties();
	conn->Open();

    if (add_spc)
    {
	    // Create spatial context
	    FdoPtr<FdoICreateSpatialContext> pCreateCreateSpatialContext = (FdoICreateSpatialContext*) conn->CreateCommand(FdoCommandType_CreateSpatialContext);
	    pCreateCreateSpatialContext->SetCoordinateSystemWkt(L"LL84");
	    pCreateCreateSpatialContext->SetDescription(L"World Coordinate System, Degrees, what else do you need to know?" );
	    pCreateCreateSpatialContext->SetName( L"LL84" );
	    pCreateCreateSpatialContext->SetXYTolerance( 17.0 );
	    pCreateCreateSpatialContext->SetZTolerance(3.14159);
	    pCreateCreateSpatialContext->Execute();
    }
	return conn;
}

FdoIConnection* UnitTestUtil::OpenConnection( FdoString* fileName, bool re_create, bool add_spc, FdoIConnection *inConn )
{
#ifdef _WIN32
	wchar_t fullpath[1024];
	_wfullpath(fullpath, fileName, 1024);
#else
	char cpath[1024];
	char cfullpath[1024];
	wcstombs(cpath, fileName, 1024);
	realpath(cpath, cfullpath);
	wchar_t fullpath[1024];
	mbstowcs(fullpath, cfullpath, 1024);
#endif
    FdoIConnection *conn = inConn;
	if( conn == NULL )
		conn = UnitTestUtil::CreateConnection();
	if( re_create )
	{
		// Clean up the existing file
        FdoCommonFile::Delete(fullpath, true);

		// Crate the SQLite file
		FdoPtr<FdoICreateDataStore>	pCreateCmd = (FdoICreateDataStore*) conn->CreateCommand(FdoCommandType_CreateDataStore);
		FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();
		int	count;
		FdoString **names = dictionary->GetPropertyNames(count);
		CPPUNIT_ASSERT_MESSAGE("Wrong number of properties for create cmd", count==2 );
		dictionary->SetProperty( names[0], fullpath );
		dictionary->SetProperty( names[1], L"TRUE" );
		pCreateCmd->Execute();	

		// Open the connection
		std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath) + L";UseFdoMetadata=TRUE;";
		conn->SetConnectionString(connStr.c_str());
		FdoPtr<FdoIConnectionInfo>info = conn->GetConnectionInfo();
		FdoPtr<FdoIConnectionPropertyDictionary> prop = info->GetConnectionProperties();
		conn->Open();

        if (add_spc)
        {
		    // Create spatial context
		    FdoPtr<FdoICreateSpatialContext> pCreateCreateSpatialContext = (FdoICreateSpatialContext*) conn->CreateCommand(FdoCommandType_CreateSpatialContext);
		    pCreateCreateSpatialContext->SetCoordinateSystemWkt(L"GEOGCS[\"LL84\",DATUM[\"WGS84\",SPHEROID[\"WGS84\",6378137.000,298.25722293]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.01745329251994]]");
		    pCreateCreateSpatialContext->SetDescription(L"World Coordinate System, Degrees, what else do you need to know?" );
		    pCreateCreateSpatialContext->SetName( L"LL84" );
		    pCreateCreateSpatialContext->Execute();
        }
		return conn;
	}

	// Just open the connection
    std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath) + L";UseFdoMetadata=TRUE;";
    conn->SetConnectionString(connStr.c_str());
    FdoPtr<FdoIConnectionInfo>info = conn->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> prop = info->GetConnectionProperties();
    conn->Open();

    return conn;
}

// Exports spatial contexts and feature schemas from datastore to XML.
void UnitTestUtil::ExportDb( FdoIConnection* connection, FdoIoStream* stream, FdoXmlSpatialContextFlags* flags )
{
    stream->Reset();
    FdoXmlWriterP writer = FdoXmlWriter::Create(stream, true, FdoXmlWriter::LineFormat_Indent);
    // Get the feature schemas from the Datastore
  
    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoFeatureSchemasP schemas = pDescCmd->Execute();
    FdoIoMemoryStreamP tempStream = FdoIoMemoryStream::Create();

    // Serialize the feature schemas.
    schemas->WriteXml( tempStream );
    tempStream->Reset();
    FdoXmlReaderP reader = FdoXmlReader::Create( tempStream );

    FdoXmlCopyHandlerP copier = FdoXmlCopyHandler::Create( writer );
    reader->Parse( copier );
}

void UnitTestUtil::PrintException( FdoException* e, FILE* fp, FdoBoolean stripLineNo )
{
    FdoPtr<FdoException> currE = e;
    // Add ref to prevent smart pointer from destroying exception.
    FDO_SAFE_ADDREF(e);

    while ( currE ) {
        FdoStringP message = currE->GetExceptionMessage();
        FdoString* pMessage = message;
        if ( stripLineNo ) {
#ifdef _WIN32
            // The following removes the function and line number info from error messages.
            FdoString* nextPos = pMessage;
            FdoString* nextPos2;
            FdoString* nextPos3;
            FdoString* nextPos4;
            FdoStringP subStr;
            int state = 0;
            bool done = false;

            while ( !done ) {
                switch (state) {

                case 0: // Initial state
                    // Look for cpp file name enclosed in brackets
                    nextPos = wcschr( pMessage, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;

                case 1: // process next cpp file in brackets
                    if ( (nextPos == NULL) || (nextPos2 == NULL) || (nextPos3 == NULL) ) { 
                        // no more cpp file in brackets
                        state = 2;
                    }
                    else {
                        if ( nextPos2 < nextPos3 ) {
                            // theres another one.
                            
                            // It might be in nested '()' some move ahead
                            // to innermost '(' before file name.
                            while (true) {
                                nextPos4 = wcschr( nextPos + 1, '(' );
                                if ( nextPos4 && (nextPos4 < nextPos2) )
                                    nextPos = nextPos4;
                                else
                                    break;
                            }
                            state = 3;
                        }
                        else {
                            // next ')' is before '.cpp', skip current '()'
                            state = 4;
                        }
                    }
                    break;

                case 2: // end state
                    // print rest of message
                    fprintf( fp, "%ls\n", pMessage );
                    done = true;
                    break;

                case 3: // found '( ... .cpp ... )'
                    // Get part to the left of '( ... .cpp ... )' and print it
                    subStr = FdoStringP(pMessage).Mid(0, nextPos - pMessage);
                    fprintf( fp, "%ls", (FdoString*) subStr );

                    // Move current ptr to right of '( ... .cpp ... )'
                    pMessage = nextPos3 + 1;
                    state = 0;
                    break;

                case 4: // Move on to next '( ... .cpp ... )'..
                    nextPos = wcschr( nextPos3, '(' );
                    nextPos2 = nextPos ? wcsstr( nextPos, L".cpp" ) : NULL;
                    nextPos3 = nextPos ? wcschr( nextPos, ')' ) : NULL;
                    state = 1;
                    break;
                }
            }
#else
		fprintf( fp, " %ls \n", pMessage );
#endif
        }
        else {
            fprintf( fp, "%ls\n", pMessage );
        }

        currE = currE->GetCause();
    }
}

void UnitTestUtil::PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo )
{
    FILE* fp = fopen( fileName, "w" );

    if ( !fp ) {
        char buffer[500];
        sprintf( buffer, "Failed to open file %s", fileName );
        CPPUNIT_FAIL( buffer );
    }

    PrintException( e, fp, stripLineNo );

    fclose( fp );
}

void UnitTestUtil::CreateData( bool create, FdoIConnection  *inConn, int featCount, FdoString* className, int threadId  )
{
    //first delete the destination file if it exists already -- cleanup that is
	FdoPtr<FdoIConnection> conn;
	if( inConn != NULL )
	{
		inConn->AddRef();
		conn = inConn;
	}

    if( create )
	{
		try
		{
			if( conn == NULL )
				conn = UnitTestUtil::CreateConnection();

			UnitTestUtil::OpenConnection( DESTINATION_FILE, true, true, conn );
			 //apply schema
			FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
			FdoPtr<FdoFeatureSchema> schema = CreateSLTSchema();
			applyschema->SetFeatureSchema(schema);
			applyschema->Execute();
		}
		catch(FdoException *exp )
		{
			PrintException( exp, stdout, false);
			CPPUNIT_FAIL("Insert failed");
		}
	}
    else if( inConn == NULL || inConn->GetConnectionState() != FdoConnectionState_Open )
        conn = UnitTestUtil::OpenConnection(DESTINATION_FILE, false);    

    //try out describe schema
    FdoPtr<FdoIDescribeSchema> desc = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = desc->Execute();
    FdoPtr<FdoFeatureSchema> schema2 = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> ccol = schema2->GetClasses();
    FdoPtr<FdoClassDefinition> cdef = ccol->GetItem(L"Parcel");
    FdoPtr<FdoPropertyDefinitionCollection> pdc = cdef->GetProperties();
    FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(L"ID");
    CPPUNIT_ASSERT(((FdoDataPropertyDefinition*)(pd.p))->GetReadOnly());
    FdoPtr<FdoClassCapabilities> caps = cdef->GetCapabilities();
    CPPUNIT_ASSERT(caps->SupportsLocking() == false);
    CPPUNIT_ASSERT(caps->SupportsLongTransactions() == false);
    CPPUNIT_ASSERT (caps->GetPolygonVertexOrderRule (L"Data") == FdoPolygonVertexOrderRule_None);
    CPPUNIT_ASSERT (caps->GetPolygonVertexOrderStrictness (L"Data") == false);
    CPPUNIT_ASSERT (caps->GetPolygonVertexOrderRule (L"Data2") == FdoPolygonVertexOrderRule_None);
    CPPUNIT_ASSERT (caps->GetPolygonVertexOrderStrictness (L"Data2") == false);
    
	FdoPtr<FdoPropertyDefinition> gpd = pdc->GetItem(L"Data2");
    CPPUNIT_ASSERT_MESSAGE("Expected a geometry property type", gpd->GetPropertyType() == FdoPropertyType_GeometricProperty );
	FdoGeometricPropertyDefinition *geom = (FdoGeometricPropertyDefinition*)gpd.p;
	CPPUNIT_ASSERT_MESSAGE("Expected a not null geometry property Spatial context", geom->GetSpatialContextAssociation() != NULL );

	cdef = ccol->GetItem(L"AbsClass");
    // TODO: we don't support abstract classes
	//CPPUNIT_ASSERT_MESSAGE("Expected an abstract class", cdef->GetIsAbstract() );

    //set up the insert command
    //we will execute it multiple times with different property values
    FdoPtr<FdoIInsert> insert = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);

    FdoPtr<FdoPropertyValueCollection> propvals = insert->GetPropertyValues();

    FdoPtr<FdoStringValue> svname = FdoStringValue::Create(L"");
    FdoPtr<FdoStringValue> svkey = FdoStringValue::Create(L"");
    FdoPtr<FdoStringValue> svurl = FdoStringValue::Create(L"");
	FdoPtr<FdoInt32Value> svNumb = FdoInt32Value::Create();

    FdoPtr<FdoGeometryValue> gvgeom = FdoGeometryValue::Create(NULL);
    FdoPtr<FdoGeometryValue> gvgeom2 = FdoGeometryValue::Create(NULL);

    FdoPtr<FdoPropertyValue> pvname = FdoPropertyValue::Create(L"Name", svname);
    FdoPtr<FdoPropertyValue> pvkey = FdoPropertyValue::Create(L"Key", svkey);
	FdoPtr<FdoPropertyValue> pvurl = FdoPropertyValue::Create(L"Url", svurl);
    FdoPtr<FdoPropertyValue> pvgeom = FdoPropertyValue::Create(L"Data", gvgeom);
    FdoPtr<FdoPropertyValue> pvgeom2 = FdoPropertyValue::Create(L"Data2", gvgeom2);

	FdoPtr<FdoPropertyValue> pvNumb = FdoPropertyValue::Create(L"Numb", svNumb);

	FdoDateTime dt(2006,4,21,19,40,(float)10.0001);
	FdoPtr<FdoDateTimeValue> dtv = FdoDateTimeValue::Create(dt);
	FdoPtr<FdoPropertyValue> pvDateTime = FdoPropertyValue::Create(L"datetime", dtv);

    propvals->Add(pvname);
    propvals->Add(pvkey);
    propvals->Add(pvurl);
    propvals->Add(pvgeom);
    propvals->Add(pvgeom2);

	if( className == NULL )
		insert->SetFeatureClassName(L"Parcel");  
	else
	{
		propvals->Add(pvNumb);
		propvals->Add(pvDateTime);
		insert->SetFeatureClassName(className);
	}

	clock_t start;
    clock_t finish;
	if( threadId == -1 )
		start = clock ();

	FdoPtr<FdoIConnection> shpConn = UnitTestUtil::OpenConnection( SOURCE_FILE, false);
	FdoPtr<FdoISelect> select = (FdoISelect*)shpConn->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (L"DaKlass");
	FdoPtr<FdoIFeatureReader> shpReader = select->Execute();

	int count = 1;
	int numbProp = 100;
	char tmp[32];
	tmp[0] = '\0';
	while( shpReader->ReadNext() )
	{
		if( ! shpReader->IsNull(L"Data") )
		{
			gvgeom->SetGeometry( FdoPtr<FdoByteArray>( shpReader->GetGeometry(L"Data") ) );
			gvgeom2->SetGeometry( FdoPtr<FdoByteArray>( shpReader->GetGeometry(L"Data") ) ); 
		}
		else
		{
			gvgeom->SetGeometry( NULL );
			gvgeom2->SetGeometry( NULL );
		}
		if( ! shpReader->IsNull(L"Name") )
			svname->SetString( shpReader->GetString(L"Name") );
		else
			svname->SetString( L"" );

		if( ! shpReader->IsNull(L"Key") )
			svkey->SetString( shpReader->GetString(L"Key") );
		else
			svkey->SetString( L"" );

		if( ! shpReader->IsNull(L"Url") )
			svurl->SetString( shpReader->GetString(L"Url") );
		else
			svurl->SetString( L"" );

		dt.minute = numbProp%60;
		dtv->SetDateTime( dt );
		svNumb->SetInt32( numbProp++ );

        FdoPtr<FdoIFeatureReader> rdr = insert->Execute();
        if ( (count % 1000) == 0 && threadId == -1)
        {
            for(unsigned int i=0;i<strlen(tmp);i++)putchar(8);
            sprintf(tmp,"Count = %d",count);
            printf(tmp);

            rdr->ReadNext();
            int id = rdr->GetInt32(L"ID");
            if( create )
            {
                CPPUNIT_ASSERT(id == count);
            }
        }
	   count++;
       if( featCount != -1 && count>featCount )
			break;
    };

    conn->Flush();

    if( threadId == -1 )
	{
		for(unsigned int i=0;i<strlen(tmp);i++)putchar(8);
		sprintf(tmp,"Count = %d",count);
		printf(tmp);
		finish = clock ();
		printf ("  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	}
	if( inConn == NULL )
		conn->Close();
}
