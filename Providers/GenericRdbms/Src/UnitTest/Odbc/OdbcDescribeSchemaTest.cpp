/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "OdbcDescribeSchemaTest.h"
#include "UnitTestUtil.h"

#define SUFFIX_TEST L"_describeschema"

CPPUNIT_TEST_SUITE_REGISTRATION (OdbcMySqlDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "OdbcMySqlDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION (OdbcSqlServerDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "OdbcSqlServerDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION (OdbcTextDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "OdbcTextDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "OdbcTextTests");
#endif

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION (OdbcAccessDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessDescribeSchemaTest, "OdbcAccessDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessDescribeSchemaTest, "OdbcAccessTests");
#endif

void OdbcDescribeSchemaTest::LoadTestData(FdoIConnection* connection)
{
	UnitTestUtil::Sql2Db( GetSchema(), connection );
}
void OdbcDescribeSchemaTest::describe()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		// Create/open primary connection:
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( userConnectString);
		connection->Open();

		m_OdbcSetup.CreateDataStore(connection.p, SUFFIX_TEST);

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
        LoadTestData(connection /*, sm*/ );

        printf( "Performing Describe ... \n" );
        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();

        fsc->WriteXml( UnitTestUtil::GetOutputFileName( L"describeSchemas.xml" ) );

        FdoInt32 numSchemas = fsc->GetCount();
        CPPUNIT_ASSERT( numSchemas == 1 );

        for (int i=0; i<numSchemas; i++) {
            FdoPtr<FdoFeatureSchema> schema = fsc->GetItem(i);

            FdoString* schemaName = schema->GetName();
            printf("Schema='%ls'\n", schemaName);

            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            //CPPUNIT_ASSERT( numClasses == 1 );

            for (int j=0; j<numClasses; j++) {
                FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                printf("Class='%ls'\n", className);
            }
        }
        printf( "Closing Connection ... \n" );
		
		// close and open again to be able to erase the table.
		connection->Close();
		connection->SetConnectionString ( userConnectString);
		connection->Open();
		// erase the used table.
		m_OdbcSetup.DestroyDataStore(connection.p, SUFFIX_TEST);
		connection->Close();
    }
    catch ( FdoException* e )
    {
        try {
            if (connection)
                connection->Close();
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}

void OdbcMySqlDescribeSchemaTest::set_provider()
{
    mInputData = NULL;
    UnitTestUtil::SetProvider( "OdbcMySql" );
}

#ifdef _WIN32
void OdbcSqlServerDescribeSchemaTest::set_provider()
{
    mInputData = NULL;
    UnitTestUtil::SetProvider( "OdbcSqlServer" );
}

void OdbcSqlServerDescribeSchemaTest::getClassNames()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // Now open the database with the given 
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        // Get the schema names
        FdoPtr<FdoIGetClassNames> cmd =
            (FdoIGetClassNames*)connection->CreateCommand(FdoCommandType_GetClassNames);
        FdoPtr<FdoStringCollection> classNames = cmd->Execute();

        if (classNames == NULL)
            CPPUNIT_FAIL("FAILED - GetClassNames returned NULL collection\n");

        FdoInt32 numClasses = classNames->GetCount();
        CPPUNIT_ASSERT(numClasses == 9);
        for (int i=0; i<numClasses; i++)
        {
            FdoStringP className = classNames->GetItem(i)->GetString();

			wprintf(L"Current class name '%ls'\n", className);
        }
        CPPUNIT_ASSERT(classNames->GetItem(0)->GetString() == L"dbo:acdb3dpolyline");
        CPPUNIT_ASSERT(classNames->GetItem(1)->GetString() == L"dbo:cities");
        CPPUNIT_ASSERT(classNames->GetItem(2)->GetString() == L"dbo:table1");
        CPPUNIT_ASSERT(classNames->GetItem(3)->GetString() == L"dbo:table6");
        CPPUNIT_ASSERT(classNames->GetItem(4)->GetString() == L"dbo:table7");
        CPPUNIT_ASSERT(classNames->GetItem(5)->GetString() == L"dbo:table8");
        CPPUNIT_ASSERT(classNames->GetItem(6)->GetString() == L"dbo:view1");
        CPPUNIT_ASSERT(classNames->GetItem(7)->GetString() == L"guest:cities");
        CPPUNIT_ASSERT(classNames->GetItem(8)->GetString() == L"guest:table3");
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

}

void OdbcTextDescribeSchemaTest::set_provider()
{
    UnitTestUtil::SetProvider( "OdbcText" );
}

void OdbcTextDescribeSchemaTest::describe()
{
    // This unit test is for defects 751218 and 670018.

    FdoPtr<FdoIConnection> connection;
    try
    {
        connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        // The data source has dot '.' and hyphen '-' characters that shows up in the
        // physical schema name.  Make sure that we correctly handle it.  The 
        // provider should map it to an acceptable FDO schema name, rather than 
        // throwing an exception to complain about the offending characters.

        FdoPtr<FdoIDescribeSchema> describeSchemaCmd = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        FdoInt32 numSchemas = schemas->GetCount();
        CPPUNIT_ASSERT( numSchemas == 1 );

        for (int i=0; i<numSchemas; i++) {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();

            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            CPPUNIT_ASSERT( numClasses == 1 );

            for (int j=0; j<numClasses; j++) {
                FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
                select->SetFeatureClassName (className);
                FdoPtr<FdoIFeatureReader> reader = select->Execute();
                FdoString* countryKey;
                while (reader->ReadNext())
                {
                    countryKey = reader->GetString(L"COUNTRY_KEY");
                }
                reader = NULL;

                FdoPtr<FdoISelectAggregates> selectAggr = (FdoISelectAggregates*)connection->CreateCommand (FdoCommandType_SelectAggregates);
                selectAggr->SetFeatureClassName (className);
                FdoPtr<FdoIdentifierCollection> ids = selectAggr->GetPropertyNames();
                ids->Clear();
                FdoPtr<FdoComputedIdentifier> compId = (FdoComputedIdentifier*)FdoExpression::Parse(L"Count(COUNTRY_KEY) as MYCOMPID");
                ids->Add(compId);
                FdoPtr<FdoIDataReader> dataReader = selectAggr->Execute();
                FdoInt64 mycount;
                while (dataReader->ReadNext())
                {
                    mycount = dataReader->GetInt64(L"MYCOMPID");
                    CPPUNIT_ASSERT_MESSAGE("SelectAggregates returned incorrect count", 176 == mycount);
                }
                dataReader = NULL;
            }
        }
    }
    catch ( FdoException* e )
    {
        try {
            if (connection)
                connection->Close();
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}


void OdbcAccessDescribeSchemaTest::set_provider()
{
    UnitTestUtil::SetProvider( "OdbcAccess" );
}

// The function checks the geometric type property value on a describe schema
// command execution. It should indicate POINT only.
// This unit test is for defect 872623
void OdbcAccessDescribeSchemaTest::describe()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        // Now analyse the schema with the mappings in place.
        // This is a modified version of DescribeSchemaTest::SchemaTest().
        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        FdoInt32 numSchemas = schemas->GetCount();
        for (int i=0; i<numSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();
			wprintf(L"Current schema '%ls'\n", schemaName);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            for (int j=0; j<numClasses; j++)
            {
                // Note the assumption here that it is a feature class (it is set up so
                // in the test data).
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);

                // analyze the feature class
                FdoString* className = classDef->GetName();
    			wprintf(L"Current class '%ls'\n", className);
            
                FdoPropertiesP props = classDef->GetProperties();

                FdoInt32 numProps = props->GetCount();
                for (int k=0; k<numProps; k++ ) {
                    FdoPropertyP prop = props->GetItem(k);

                    if ( prop->GetPropertyType() == FdoPropertyType_GeometricProperty ) {
                        FdoGeometricPropertyDefinition* geomProp = (FdoGeometricPropertyDefinition*)(prop.p);

                        wprintf(L"Current geometry property '%ls' '%ls'\n", geomProp->GetName(), geomProp->GetDescription());
                        FdoInt32 geomType = geomProp->GetGeometryTypes();
                        CPPUNIT_ASSERT( geomType == FdoGeometricType_Point );
                        FdoInt32 specGeomCount = 0;
                        FdoGeometryType* specGeomTypes = geomProp->GetSpecificGeometryTypes( specGeomCount );
                        CPPUNIT_ASSERT( specGeomCount == 1 );
                        CPPUNIT_ASSERT( specGeomTypes[0] == FdoGeometryType_Point );

                        FdoClassCapabilitiesP cc = classDef->GetCapabilities();
                        CPPUNIT_ASSERT( cc != NULL );
                        CPPUNIT_ASSERT( cc->GetPolygonVertexOrderRule(geomProp->GetName()) == FdoPolygonVertexOrderRule_None );
                        CPPUNIT_ASSERT( cc->GetPolygonVertexOrderStrictness(geomProp->GetName()) == false );
                    }
                }
            }
        }
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

}

void OdbcAccessDescribeSchemaTest::getSchemaNames()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        // Get the schema names
        FdoPtr<FdoIGetSchemaNames> cmd =
            (FdoIGetSchemaNames*)connection->CreateCommand(FdoCommandType_GetSchemaNames);
        FdoPtr<FdoStringCollection> schemaNames = cmd->Execute();

        if (schemaNames == NULL)
            CPPUNIT_FAIL("FAILED - GetSchemaNames returned NULL collection\n");

        FdoInt32 numSchemas = schemaNames->GetCount();
        CPPUNIT_ASSERT(numSchemas == 1);
        for (int i=0; i<numSchemas; i++)
        {
            FdoStringP schemaName = schemaNames->GetItem(i)->GetString();

			wprintf(L"Current schema '%ls'\n", schemaName);
            CPPUNIT_ASSERT(schemaName == L"Fdo");
        }
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

}

void OdbcAccessDescribeSchemaTest::getClassNames()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        // Get the schema names
        FdoPtr<FdoIGetClassNames> cmd =
            (FdoIGetClassNames*)connection->CreateCommand(FdoCommandType_GetClassNames);
        FdoPtr<FdoStringCollection> classNames = cmd->Execute();

        if (classNames == NULL)
            CPPUNIT_FAIL("FAILED - GetClassNames returned NULL collection\n");

        FdoInt32 numClasses = classNames->GetCount();
        CPPUNIT_ASSERT(numClasses == 15);
        for (int i=0; i<numClasses; i++)
        {
            FdoStringP className = classNames->GetItem(i)->GetString();

			wprintf(L"Current class name '%ls'\n", className);
        }
        CPPUNIT_ASSERT(classNames->GetItem(0)->GetString() == L"Fdo:Cities");
        CPPUNIT_ASSERT(classNames->GetItem(14)->GetString() == L"Fdo:VIEW2");
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

}

void OdbcAccessDescribeSchemaTest::describeWithClassNames()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        // Now analyse the schema with the mappings in place.
        // This is a modified version of DescribeSchemaTest::SchemaTest().
        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoStringCollection> classNames = FdoStringCollection::Create();
        classNames->Add(L"Cities");
        classNames->Add(L"VIEW1");
        describeSchemaCmd->SetClassNames(classNames);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        FdoInt32 numSchemas = schemas->GetCount();
        for (int i=0; i<numSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();
			wprintf(L"Current schema '%ls'\n", schemaName);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            CPPUNIT_ASSERT(numClasses == 2);
            for (int j=0; j<numClasses; j++)
            {
                // Note the assumption here that it is a feature class (it is set up so
                // in the test data).
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);

                // analyze the feature class
                FdoString* className = classDef->GetName();
    			wprintf(L"Current class '%ls'\n", className);
            }
        }
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

}

#endif
