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
#include "ExtendedSelectTest.h"
#include <stdio.h>
#include "SDF/IExtendedSelect.h"
#include "SDF/SdfCommandType.h"
#ifndef _WIN32
#include <unistd.h>
#include <limits.h>
#endif
#define WORLD  L"../../TestData/World_Countries.sdf"
#define SDF_FILE  L"../../TestData/PARCEL_linuxtest.SDX"
#define DEST_FILE  L"../../TestData/CACHE.SDF"
#define DEST_FILE_mb  "../../TestData/CACHE.SDF"
#define WORLD_N1  L"../../TestData/~`!@#$%^&()_-+}{[]',.sdf"
#define WORLD_N2  L"../../TestData/~`!@#$%^&()_-+=}{[];',.sdf"
#define WORLD_N3  L"../../TestData/C~`!@#$%^&()_-+=}{[];',re.sdf"


CPPUNIT_TEST_SUITE_REGISTRATION( ExtendedSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ExtendedSelectTest, "ExtendedSelectTest");


ExtendedSelectTest::ExtendedSelectTest(void)
{
}

ExtendedSelectTest::~ExtendedSelectTest(void)
{
}

void ExtendedSelectTest::setUp ()
{
}

void ExtendedSelectTest::OpenConnection(FdoIConnection* conn, const wchar_t* path , bool placeDQ)
{
#ifdef _WIN32
    wchar_t fullpath[1024];
    _wfullpath(fullpath, path, 1024);
#else
    char cpath[PATH_MAX];
    char cfullpath[PATH_MAX];
    wcstombs(cpath, path, PATH_MAX);
    realpath(cpath, cfullpath);
    wchar_t fullpath[PATH_MAX];
    mbstowcs(fullpath, cfullpath, PATH_MAX);
#endif

    std::wstring connStr;
    if (placeDQ)
        connStr = std::wstring(L"File=\"") + std::wstring(fullpath) + std::wstring(L"\"");
    else
        connStr = std::wstring(L"File=") + std::wstring(fullpath);
    connStr += std::wstring(L";ReadOnly=TRUE");

    conn->SetConnectionString(connStr.c_str());
    conn->Open();
}


void ExtendedSelectTest::MainTest(bool fileSort, bool extendProps)
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        OpenConnection(connection,SDF_FILE);

        FdoPtr<SdfIExtendedSelect> select = (SdfIExtendedSelect*)connection->CreateCommand(SdfCommandType_ExtendedSelect); 

        select->SetFeatureClassName(L"DaKlass");
        FdoPtr<FdoIdentifierCollection>selecProperties = select->GetPropertyNames();
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Key")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ID")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Name")) );

#ifdef _WIN32
    _wremove(DEST_FILE);
#else
    unlink( DEST_FILE_mb );
#endif

        clock_t start;
        clock_t finish;
        start = clock ();

        FdoPtr<FdoIdentifierCollection> orders = select->GetOrdering();
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Key")));
        select->SetFilter(L"ID >= 10000 and ID < 20000");
        FdoPtr<FdoIScrollableFeatureReader>reader;
        if( fileSort )
        {
            if( extendProps )
            {
                // Add a couple of properties
                FdoPtr<FdoDataPropertyDefinitionCollection>dataProps = FdoDataPropertyDefinitionCollection::Create(NULL);
                FdoPtr<FdoPropertyValueCollection>pvcs = FdoPropertyValueCollection::Create();
                
                FdoPtr<FdoDataPropertyDefinition>dpd = FdoDataPropertyDefinition::Create(L"Extended1", L"A string extended property");
                dpd->SetDataType(FdoDataType_String);
                dataProps->Add(dpd);

                dpd = FdoDataPropertyDefinition::Create(L"Extended2", L"A int extended property");
                dpd->SetDataType(FdoDataType_Int32);
                dataProps->Add(dpd);

                FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create(L"Extended1", FdoPtr<FdoStringValue>(FdoStringValue::Create(L"Exended Test string")));
                pvcs->Add( pv );

                pv = FdoPropertyValue::Create(L"Extended2", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(99)));
                pvcs->Add( pv );
                
                reader = select->ExecuteScrollable(DEST_FILE, dataProps, pvcs);
            }
            else
                reader = select->ExecuteScrollable(DEST_FILE, NULL, NULL);
        }
        else
            reader = select->ExecuteScrollable();

        finish = clock ();
        printf ("Creation  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
        
        start = clock ();
        int count = 0;
        if( reader->ReadFirst() )
        do
        {
            if( count++ < 10 )
            {
                if( extendProps ) 
                    printf("%ls | %ls | %d\n",reader->GetString(L"Key"),reader->GetString(L"Extended1"),reader->GetInt32(L"Extended2"));
                else
                    printf("%ls\n",reader->GetString(L"Key"));
            }
        }while( reader->ReadNext() );
        finish = clock ();
        printf ("Read forward(%d)  time: %2.3f seconds\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==10000 );
        count = 0;
        start = clock ();
        if( reader->ReadLast() )
        do
        {
            if( count++ < 10 )
                printf("%ls %d\n",reader->GetString(L"Key"),reader->GetInt32(L"ID"));
        }while( reader->ReadPrevious() );
        finish = clock ();
        printf ("Read backward(%d)  time: %2.3f seconds\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==10000 );

        // Start reading in the midle using a key
        FdoPtr<FdoPropertyValueCollection>key = FdoPropertyValueCollection::Create();
        FdoPtr<FdoPropertyValue> pvid = FdoPropertyValue::Create(L"ID", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(19830)));
        key->Add( pvid );
        pvid = FdoPropertyValue::Create(L"Key", FdoPtr<FdoStringValue>(FdoStringValue::Create(L"KV0317")));
        key->Add( pvid );
        start = clock ();
        count = 0;
        if( reader->ReadAt( key ) )
        do
        {
            if( count++ < 10 )
            {
                printf("%ls %d\n",reader->GetString(L"Key"),reader->GetInt32(L"ID"));
            }
        }while( reader->ReadNext() );
        finish = clock ();
        printf ("Read from the midle using key: count(%d)  time: %2.3f seconds\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==10 );
        printf("The index of key(ID=19830, key=KV0317 is: %d\n", reader->IndexOf( key ) );
        CPPUNIT_ASSERT_MESSAGE("Wronf index", reader->IndexOf( key ) == 9991 );
        // Start reading in the midle using a record index
        start = clock ();
        count = 0;
        if( reader->ReadAtIndex( 1000 ) )
        do
        {
            if( count++ < 10 )
            {
                printf("%ls %d\n",reader->GetString(L"Key"),reader->GetInt32(L"ID"));
            }
        }while( reader->ReadNext() );
        finish = clock ();
        printf ("Read from the midle using a record index. count(%d)  time: %2.3f seconds\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==9001 );
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
    }
}

void ExtendedSelectTest::WorldTest(FdoIConnection* connection)
{
    WorldTestWoFile(connection);
    WorldTestWFile(connection);
}

void ExtendedSelectTest::WorldTestWFile(FdoIConnection* connection)
{
    try
    {
        FdoPtr<SdfIExtendedSelect> select = (SdfIExtendedSelect*)connection->CreateCommand(SdfCommandType_ExtendedSelect); 

        select->SetFeatureClassName(L"World_Countries");
        FdoPtr<FdoIdentifierCollection>selecProperties = select->GetPropertyNames();
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"MAPKEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"KEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Autogenerated_ID")) );


        FdoPtr<FdoIdentifierCollection> orders = select->GetOrdering();
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")));
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Autogenerated_ID")));
        select->SetOrderingOption(L"Autogenerated_ID",FdoOrderingOption_Descending);
#ifdef _WIN32
    _wremove(DEST_FILE);
#else
    unlink( DEST_FILE_mb );
#endif
        clock_t start;
        clock_t finish;
        start = clock ();

        FdoPtr<SdfIScrollableFeatureReader>reader = select->ExecuteScrollable(DEST_FILE, NULL, NULL);

        finish = clock ();
        printf ("Creation  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
        
        // Iterate in the forward direction from first to last
        const wchar_t*   expected[] = {L"Afghanistan",L"Albania",L"Algeria",L"Andorra",L"Angola",L"Angola",L"Antigua and Barbuda",L"Argentina",L"Argentina",L"Argentina"};
        start = clock ();
        int count = 0;
        if( reader->ReadFirst() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadNext() );

        // Iterate in the backward direction from last to first
        finish = clock ();
        printf ("Read forward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

        const wchar_t*   expected2[] = {L"Zimbabwe",L"Zambia",L"Zaire",L"Western Sahara",L"Virgin Islands",L"Vietnam",L"Venezuela",L"Venezuela",L"Uzbekistan",L"Uruguay"};
        count = 0;
        start = clock ();
        if( reader->ReadLast() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", wcscmp(name,expected2[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadPrevious() );
        finish = clock ();
        printf ("Read backward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

        // Jump to a particular record
        const wchar_t*   expected3[] = {L"Tunisia",L"Turkey",L"Turkmenistan",L"Turks and Caicos Islands",L"Turks and Caicos Islands",L"Uganda",L"Ukraine",L"United Kingdom",L"United Kingdom",L"United Kingdom"};
        FdoPtr<FdoPropertyValueCollection>key = FdoPropertyValueCollection::Create();
        FdoPtr<FdoStringValue> svname = FdoStringValue::Create(L"Tunisia");
        key->Add( FdoPtr<FdoPropertyValue>( FdoPropertyValue::Create(L"NAME", svname) ) );
        count = 0;
        start = clock ();
        if( reader->ReadAt( key ) )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", wcscmp(name,expected3[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadNext() );
        finish = clock ();
        printf ("Jump to record(count=%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==40 );

        // Open the cache file as a regular SDF file
        connection->Close();
        OpenConnection(connection,DEST_FILE);
        FdoPtr<FdoISelect> fdo_select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select); 

        fdo_select->SetFeatureClassName(L"World_Countries");
        FdoPtr<FdoIFeatureReader>rdr = fdo_select->Execute();
        count = 0;
        printf("Regular select on cache file\n");
        while(rdr->ReadNext())
        {
            if( count++ < 10 )
            {
                const wchar_t *name = rdr->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected[count-1])==0 );
                printf(" %d \n",  rdr->GetInt32( L"Autogenerated_ID" ) );
            }
        }

    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        throw;
    }

}

void ExtendedSelectTest::WorldTestWoFile(FdoIConnection* connection)
{
    try
    {
        FdoPtr<FdoIExtendedSelect> select = (FdoIExtendedSelect*)connection->CreateCommand(FdoCommandType_ExtendedSelect); 

        select->SetFeatureClassName(L"World_Countries");
        FdoPtr<FdoIdentifierCollection>selecProperties = select->GetPropertyNames();
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"MAPKEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"KEY")) );
        selecProperties->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Autogenerated_ID")) );


        FdoPtr<FdoIdentifierCollection> orders = select->GetOrdering();
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME")));
        orders->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Autogenerated_ID")));
        select->SetOrderingOption(L"Autogenerated_ID",FdoOrderingOption_Descending);
#ifdef _WIN32
    _wremove(DEST_FILE);
#else
    unlink( DEST_FILE_mb );
#endif
        clock_t start;
        clock_t finish;
        start = clock ();

        FdoPtr<FdoIScrollableFeatureReader>reader = select->ExecuteScrollable();

        finish = clock ();
        printf ("Creation  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
        
        // Iterate in the forward direction from first to last
        const wchar_t*   expected[] = {L"Afghanistan",L"Albania",L"Algeria",L"Andorra",L"Angola",L"Angola",L"Antigua and Barbuda",L"Argentina",L"Argentina",L"Argentina"};
        start = clock ();
        int count = 0;
        if( reader->ReadFirst() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected value", wcscmp(name,expected[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadNext() );

        // Iterate in the backward direction from last to first
        finish = clock ();
        printf ("Read forward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

        const wchar_t*   expected2[] = {L"Zimbabwe",L"Zambia",L"Zaire",L"Western Sahara",L"Virgin Islands",L"Vietnam",L"Venezuela",L"Venezuela",L"Uzbekistan",L"Uruguay"};
        count = 0;
        start = clock ();
        if( reader->ReadLast() )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", wcscmp(name,expected2[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadPrevious() );
        finish = clock ();
        printf ("Read backward(%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==419 );

        // Jump to a particular record
        const wchar_t*   expected3[] = {L"Tunisia",L"Turkey",L"Turkmenistan",L"Turks and Caicos Islands",L"Turks and Caicos Islands",L"Uganda",L"Ukraine",L"United Kingdom",L"United Kingdom",L"United Kingdom"};
        FdoPtr<FdoPropertyValueCollection>key = FdoPropertyValueCollection::Create();
        FdoPtr<FdoStringValue> svname = FdoStringValue::Create(L"Tunisia");
        key->Add( FdoPtr<FdoPropertyValue>( FdoPropertyValue::Create(L"NAME", svname) ) );
        count = 0;
        start = clock ();
/* TODO: log defect
        if( reader->ReadAt( key ) )
        do
        {
            if( count++ < 10 )
            {
                const wchar_t *name = reader->GetString(L"NAME");
                CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", wcscmp(name,expected3[count-1])==0 );
                printf("%ls \t %d\n", name, reader->GetInt32( L"Autogenerated_ID" ) );
            }
        }while( reader->ReadNext() );
        finish = clock ();
        printf ("Jump to record(count=%d)  time: %2.3f seconds\n\n\n",count, (double)(finish - start) / CLOCKS_PER_SEC);
        CPPUNIT_ASSERT_MESSAGE("Unexpected number of records", count==40 );
*/
        FdoOrderingOption opt = select->GetOrderingOption(L"Autogenerated_ID");
        CPPUNIT_ASSERT_MESSAGE("Unexpected ordering option", opt==FdoOrderingOption_Descending );

        select->ClearOrderingOptions();
        opt = select->GetOrderingOption(L"Autogenerated_ID");
        CPPUNIT_ASSERT_MESSAGE("Unexpected ordering option", opt==FdoOrderingOption_Ascending );

    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        CPPUNIT_FAIL((char*)(const char*) FdoStringP(ex->GetExceptionMessage()));
    }

}

void ExtendedSelectTest::WorldTest_SDF()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        OpenConnection(connection,WORLD);

        WorldTest( connection );
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        throw;
    }
}

void ExtendedSelectTest::WorldTest_SDF_name1()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        OpenConnection(connection,WORLD_N1);

        WorldTest( connection );
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
    }
}

void ExtendedSelectTest::WorldTest_SDF_name2()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        OpenConnection(connection, WORLD_N2, true);

        WorldTest( connection );
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
    }
}

void ExtendedSelectTest::WorldTest_SDF_name3()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        OpenConnection(connection, WORLD_N2);

        CPPUNIT_ASSERT_MESSAGE ("open connection should fail", false);
    }
    catch (FdoException *ex )
    {
        printf("\nExpected error: %ls\n", ex->GetExceptionMessage());
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ExtendedSelectTest::WorldTest_SDF_name4()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        connection->SetConnectionString(L"Fi_le=\"../../TestData/C~`!@#$%^&()_-+=}{[];',re.sdf\"");
        connection->Open();

        CPPUNIT_ASSERT_MESSAGE ("open connection should fail", false);
    }
    catch (FdoException *ex )
    {
        printf("\nExpected error: %ls\n", ex->GetExceptionMessage());
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ExtendedSelectTest::CreateSDF_SPchars()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");

        std::wstring connStr = std::wstring(L"File=\"") + std::wstring(WORLD_N3) + std::wstring(L"\";ReadOnly=FALSE");
        connection->SetConnectionString(connStr.c_str());
        
        FdoPtr<FdoIDestroyDataStore> pDestroyCmd;
        try
        {
            // Destroy the data base if exist
            pDestroyCmd = (FdoIDestroyDataStore*) connection->CreateCommand(FdoCommandType_DestroyDataStore);
            FdoPtr<FdoIDataStorePropertyDictionary> dict = pDestroyCmd->GetDataStoreProperties();
            int    count;
            FdoString **names = dict->GetPropertyNames(count);
            dict->SetProperty( names[0], L"\"" WORLD_N3 L"\"");
            pDestroyCmd->Execute();
        }
        catch (FdoException *e )
        {
            e->Release();
        }
        catch(...){}

        FdoPtr<FdoICreateDataStore>    pCreateCmd = (FdoICreateDataStore*) connection->CreateCommand(FdoCommandType_CreateDataStore);
            
        FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();
        int    count;
        FdoString **names = dictionary->GetPropertyNames(count);
        dictionary->SetProperty( names[0], L"\"" WORLD_N3 L"\"" );
        pCreateCmd->Execute();    

        connection->Open();

        FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> pTestSchema = FdoFeatureSchema::Create(L"SDF Test",L"SDF temporary Test Schema");
        
        FdoClassDefinition*    pfeatureclass;
        FdoPtr<FdoDataPropertyDefinition> pProp;
        pfeatureclass = FdoClass::Create(L"TestFeatureClass", L"FeatureClass Desc");
        pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );

        // Add first name and last name properties
        pProp = FdoDataPropertyDefinition::Create( L"First Name", L"First Name" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"Last Name" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        FdoClassesP(pTestSchema->GetClasses())->Add( pfeatureclass );

        FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pCmd->SetFeatureSchema( pTestSchema );
        pCmd->Execute();

        connection->Close();
    }
    catch (FdoException *ex )
    {
        printf("Create SDF with special characters name Error: %ls\n", ex->GetExceptionMessage() );
        ex->Release();
        throw "Create SDF with special characters name exception";
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ExtendedSelectTest::UpdateTest()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"OSGeo.SDF");

        OpenConnection(conn, DEST_FILE);    


        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 

        update->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'KV0320')");

        update->SetFilter(filter);

        FdoPtr<FdoPropertyValueCollection> pvc = update->GetPropertyValues();

        FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create(L"Extended1", FdoPtr<FdoStringValue>(FdoStringValue::Create(L"Update extended string")));
        pvc->Add(pv);
        pv = FdoPropertyValue::Create(L"Extended2", FdoPtr<FdoInt32Value>(FdoInt32Value::Create(33)));
        pvc->Add(pv);

        printf("\n\n\nTesting Update");
           
        int count = update->Execute();
        conn->Close();

        // Did it change?
        OpenConnection(conn, DEST_FILE);
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"DaKlass");
        select->SetFilter(filter);
        FdoPtr<FdoIFeatureReader>reader = select->Execute();
        while( reader->ReadNext() )
        {
            const wchar_t *ext1 = reader->GetString( L"Extended1" );
            int ext2 = reader->GetInt32( L"Extended2" );
            printf("%ls \t %d\n", ext1, ext2 );
        }
    }
    catch (FdoException *ex )
    {
        printf("FDO error: %ls\n", ex->GetExceptionMessage());
        return;
    }
}

void ExtendedSelectTest::Test_Fix823645()
{
    try
    {
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();

        FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
        FdoPtr<FdoIConnectionInfo> pInfo = connection->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> pProps = pInfo->GetConnectionProperties();
        pProps->SetProperty(L"File", std::wstring(WORLD_N3).c_str());
        pProps->SetProperty(L"ReadOnly", L"false");

        FdoPtr<FdoIDestroyDataStore> pDestroyCmd;
        try
        {
            // Destroy the data base if exist
            pDestroyCmd = (FdoIDestroyDataStore*) connection->CreateCommand(FdoCommandType_DestroyDataStore);
            FdoPtr<FdoIDataStorePropertyDictionary> dict = pDestroyCmd->GetDataStoreProperties();
            int    count;
            FdoString **names = dict->GetPropertyNames(count);
            dict->SetProperty( names[0], L"\"" WORLD_N3 L"\"");
            pDestroyCmd->Execute();
        }
        catch (FdoException *e )
        {
            e->Release();
        }
        catch(...){}

        FdoPtr<FdoICreateDataStore>    pCreateCmd = (FdoICreateDataStore*) connection->CreateCommand(FdoCommandType_CreateDataStore);
            
        FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();
        int    count;
        FdoString **names = dictionary->GetPropertyNames(count);
        dictionary->SetProperty( names[0], L"\"" WORLD_N3 L"\"" );
        pCreateCmd->Execute();    

        connection->Open();

        FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> pTestSchema = FdoFeatureSchema::Create(L"SDF Test",L"SDF temporary Test Schema");
        
        FdoClassDefinition*    pfeatureclass;
        FdoPtr<FdoDataPropertyDefinition> pProp;
        pfeatureclass = FdoClass::Create(L"TestFeatureClass", L"FeatureClass Desc");
        pProp = FdoDataPropertyDefinition::Create( L"Id", L"Id Prop" );
        pProp->SetDataType( FdoDataType_Int32 );
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );

        // Add first name and last name properties
        pProp = FdoDataPropertyDefinition::Create( L"First Name", L"First Name" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        pProp = FdoDataPropertyDefinition::Create( L"Last Name", L"Last Name" );
        pProp->SetDataType( FdoDataType_String );
        pProp->SetLength(32);
        pProp->SetNullable(false);
        FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
        FdoClassesP(pTestSchema->GetClasses())->Add( pfeatureclass );

        FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pCmd->SetFeatureSchema( pTestSchema );
        pCmd->Execute();

        connection->Close();
    }
    catch (FdoException *ex )
    {
        printf("Create SDF with special characters name Error: %ls\n", ex->GetExceptionMessage() );
        ex->Release();
        throw "Create SDF with special characters name exception";
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}
