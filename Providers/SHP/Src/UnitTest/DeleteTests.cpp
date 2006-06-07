/*
 * 
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
#include "DeleteTests.h"

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Testing"
#define SCHEMA_NAME L"\\schema.xml"
#else
#define LOCATION L"../../TestData/Testing"
#define SCHEMA_NAME L"/schema.xml"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (DeleteTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (DeleteTests, "DeleteTests");

FdoPtr<FdoIConnection> DeleteTests::mConnection;

DeleteTests::DeleteTests (void)
{
}

DeleteTests::~DeleteTests (void)
{
}

void DeleteTests::setUp ()
{
    if (!FdoCommonFile::FileExists (LOCATION))
        FdoCommonFile::MkDir (LOCATION);
    mConnection = ShpTests::GetConnection ();
	ShpTests::sLocation = LOCATION;
    mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void DeleteTests::tearDown ()
{
    // delete the class, if its there:
    CleanUpClass(mConnection, NULL, L"Test");

    mConnection->Close ();
    if (FdoCommonFile::FileExists (LOCATION SCHEMA_NAME))
        FdoCommonFile::Delete (LOCATION SCHEMA_NAME);
    if (FdoCommonFile::FileExists (LOCATION))
        FdoCommonFile::RmDir (LOCATION);
}

void DeleteTests::create_schema (FdoGeometricType type, bool elevation, bool measure)
{
    // delete the class, if its there:
    CleanUpClass(mConnection, NULL, L"Test");

    // create the class
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (L"TheSchema", L" test schema");
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

    FdoPtr<FdoDataPropertyDefinition> featid = FdoDataPropertyDefinition::Create (L"FeatId", L"integer");
    featid->SetDataType (FdoDataType_Int32);
    featid->SetIsAutoGenerated (true);
    featid->SetNullable (false);

    FdoPtr<FdoDataPropertyDefinition> id = FdoDataPropertyDefinition::Create (L"Id", L"integer");
    id->SetDataType (FdoDataType_Decimal);
    id->SetPrecision(10);
    id->SetScale(0);

    FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
    street->SetDataType (FdoDataType_String);
    street->SetLength (64);

    FdoPtr<FdoDataPropertyDefinition> area = FdoDataPropertyDefinition::Create (L"Area", L"double");
    area->SetDataType (FdoDataType_Decimal);
    area->SetPrecision (20);
    area->SetScale (8);

    FdoPtr<FdoDataPropertyDefinition> vacant = FdoDataPropertyDefinition::Create (L"Vacant", L"boolean");
    vacant->SetDataType (FdoDataType_Boolean);

    FdoPtr<FdoDataPropertyDefinition> birthday = FdoDataPropertyDefinition::Create (L"Birthday", L"date");
    birthday->SetDataType (FdoDataType_DateTime);

    // build a location geometry property
    FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
    location->SetGeometryTypes (type);
    location->SetHasElevation (elevation);
    location->SetHasMeasure (measure);

    //// assemble the feature class
    FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (L"Test", L"test class created with apply schema");
    FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
    properties->Add (featid);
    identities->Add (featid);
    properties->Add (id);
    properties->Add (street);
    properties->Add (area);
    properties->Add (vacant);
    properties->Add (birthday);
    properties->Add (location);
    feature->SetGeometryProperty (location);

    // submit the new schema
    classes->Add (feature);
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
    apply->SetFeatureSchema (schema);
    apply->Execute ();
	SaveSchema(mConnection);
}

void DeleteTests::del ()
{
    try
    {
        create_schema (FdoGeometricType_Point, true, true);
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (L"Test");
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"24", FdoDataType_Decimal);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"Id", expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'1147 Trafford Drive'");
        value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        // add NULL geometry value:
        FdoPtr<FdoGeometryValue> geometry = FdoGeometryValue::Create ();
        geometry->SetNullValue ();
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        FdoInt32 featid;
        featid = -1;
        while (reader->ReadNext ())
        {
            if (-1 != featid)
                CPPUNIT_FAIL ("too many features inserted");
            featid = reader->GetInt32 (L"FeatId");
        }
        reader->Close ();
        if (-1 == featid)
            CPPUNIT_FAIL ("too few features inserted");

        // delete all features
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (L"Test");
        FdoInt32 n = del->Execute ();

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"Test");
        reader = select->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("still features left", !reader->ReadNext ());
        reader->Close ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void DeleteTests::del_geometry ()
{
    try
    {
        create_schema (FdoGeometricType_Point, false, false);
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (L"Test");
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"24", FdoDataType_Decimal);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"Id", expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'1147 Trafford Drive'");
        value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        // add real geometry value:
        FdoPtr<FdoGeometryValue> geometry = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POINT XY ( 999000 -999000 )')");
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        FdoInt32 featid;
        featid = -1;
        while (reader->ReadNext ())
        {
            if (-1 != featid)
                CPPUNIT_FAIL ("too many features inserted");
            featid = reader->GetInt32 (L"FeatId");
        }
        reader->Close ();
        if (-1 == featid)
            CPPUNIT_FAIL ("too few features inserted");

        // delete all features
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (L"Test");
        FdoInt32 n = del->Execute ();

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"Test");
        reader = select->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("still features left", !reader->ReadNext ());
        reader->Close ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void DeleteTests::del_one ()
{
    try
    {
        create_schema (FdoGeometricType_Point, false, false);
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (L"Test");
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();

        FdoInt32 featids[3];
        int index = 0;

        // feat #1
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"24", FdoDataType_Decimal);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"Id", expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'1147 Trafford Drive'");
        value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        FdoPtr<FdoGeometryValue> geometry = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POINT XY ( 999000 -999000 )')");
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        while (reader->ReadNext ())
        {
            if (index > 2)
                CPPUNIT_FAIL ("too many features inserted");
            featids[index++] = reader->GetInt32 (L"FeatId");
        }
        reader->Close ();

        // feat #2
        expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"9191", FdoDataType_Decimal);
        value = FdoPropertyValue::Create (L"Id", expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'2611 Misener Crescent'");
        value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        geometry = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POINT XY ( 324.65 7687.0 )')");
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        reader = insert->Execute ();
        while (reader->ReadNext ())
        {
            if (index > 2)
                CPPUNIT_FAIL ("too many features inserted");
            featids[index++] = reader->GetInt32 (L"FeatId");
        }
        reader->Close ();

        // feat #3
        expression = (FdoValueExpression*)ShpTests::ParseByDataType (L"46", FdoDataType_Decimal);
        value = FdoPropertyValue::Create (L"Id", expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'99 Rockcliffe Place'");
        value = FdoPropertyValue::Create (L"Street", expression);
        values->Add (value);
        geometry = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POINT XY ( 2828.26 78127.83 )')");
        value = FdoPropertyValue::Create (L"Geometry", geometry);
        values->Add (value);
        reader = insert->Execute ();
        while (reader->ReadNext ())
        {
            if (index > 2)
                CPPUNIT_FAIL ("too many features inserted");
            featids[index++] = reader->GetInt32 (L"FeatId");
        }
        reader->Close ();

        if (2 > index)
            CPPUNIT_FAIL ("too few features inserted");

        // delete feature # 2
        FdoPtr<FdoIDelete> del = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        del->SetFeatureClassName (L"Test");
        wchar_t buffer[1024];
        swprintf (buffer, ELEMENTS(buffer), L"FeatId = %d", featids[1]);
        del->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (buffer)));
        FdoInt32 n = del->Execute ();

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"Test");
        reader = select->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("not enough features", reader->ReadNext ());
        CPPUNIT_ASSERT_MESSAGE ("wrong feat id", featids[0] == reader->GetInt32 (L"FeatId"));
        CPPUNIT_ASSERT_MESSAGE ("not enough features", reader->ReadNext ());
        CPPUNIT_ASSERT_MESSAGE ("wrong feat id", featids[2] == reader->GetInt32 (L"FeatId"));
        CPPUNIT_ASSERT_MESSAGE ("still features left", !reader->ReadNext ());
        reader->Close ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void DeleteTests::del_without_dbf ()
{
    try
    {
        // Connect:
        FdoPtr<FdoIConnection> conn = ShpTests::GetConnection ();
        conn->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Testing");
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == conn->Open ());

        // Delete any old data:
        CleanUpClass(mConnection, NULL, L"ShpWithoutDbf");

        // Get existing schema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classDefs = schema->GetClasses();

        // Create new class:
        FdoPtr<FdoFeatureClass> classDef = FdoFeatureClass::Create(L"ShpWithoutDbf", L"");
        FdoPtr<FdoPropertyDefinitionCollection> propDefs = classDef->GetProperties();
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = FdoGeometricPropertyDefinition::Create(L"Geometry", L"");
        geomProp->SetGeometryTypes(FdoGeometricType_Point);
        propDefs->Add(geomProp);
        classDef->SetGeometryProperty(geomProp);
        classDefs->Add(classDef);
        FdoPtr<FdoIApplySchema> applySchema = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
        applySchema->SetFeatureSchema(schema);
        applySchema->Execute();
        applySchema = NULL;

        // Insert some data:
        FdoPtr<FdoIInsert> insertCmd = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);
        insertCmd->SetFeatureClassName(L"ShpWithoutDbf");
        FdoPtr<FdoPropertyValueCollection> propVals = insertCmd->GetPropertyValues();
        FdoPtr<FdoGeometryValue> expr = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POINT XY ( 999000 -999000 )')");
        FdoPtr<FdoPropertyValue> propVal = FdoPropertyValue::Create(L"Geometry", expr);
        propVals->Add(propVal);
        FdoPtr<FdoIFeatureReader> reader;
        for (int i=0; i<50; i++)
            reader = insertCmd->Execute();
        reader = NULL;

        // Disconnect:
        conn->Close();

        // Delete the DBF file explicitly, required to test SHP-without-DBF functionality:
        bool bSuccess = FdoCommonFile::Delete(L"..\\..\\TestData\\Testing\\ShpWithoutDbf.dbf");

        // Reconnect:
        conn->Open();

        // Select all the features:
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ShpWithoutDbf");
        reader = selectCmd->Execute();
        FdoInt32 featIDs[1000];
        FdoInt32 featIDCount = 0;
        while (reader->ReadNext())
        {
            FdoInt32 id = reader->GetInt32(L"FeatId");
            featIDs[featIDCount++] = id;
        }
        reader = NULL;

        // Delete some of the features:
        FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(L"ShpWithoutDbf");
        FdoInt32 numDeleted = -1;
        for (int i=0; i<20; i++)
        {
            deleteCmd->SetFilter(FdoStringP::Format(L"FeatId = %d", featIDs[i]));
            numDeleted = deleteCmd->Execute();
        }

        // Disconnect & Reconnect:
        conn->Close();
        conn->Open();

        // Select all the features to verify they have been deleted correctly:
        selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ShpWithoutDbf");
        reader = selectCmd->Execute();
        featIDs[1000];
        featIDCount = 0;
        while (reader->ReadNext())
        {
            FdoInt32 id = reader->GetInt32(L"FeatId");
            for (int i=0; i<50; i++)
            {
                if (featIDs[i] == id)
                    CPPUNIT_ASSERT_MESSAGE("Feature not deleted properly", i >= 20);
            }
            featIDCount++;
        }
        reader = NULL;
        CPPUNIT_ASSERT_MESSAGE("Expected 30 features in SHP file", featIDCount==30);

        // Delete the rest of the features:
        deleteCmd = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(L"ShpWithoutDbf");
        numDeleted = deleteCmd->Execute();

        // Disconnect & Reconnect:
        conn->Close();
        conn->Open();

        // Select all the features to verify they have been deleted correctly:
        selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ShpWithoutDbf");
        reader = selectCmd->Execute();
        featIDCount = 0;
        while (reader->ReadNext())
            featIDCount++;
        reader = NULL;
        CPPUNIT_ASSERT_MESSAGE("Expected 0 features in SHP file", featIDCount==0);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
