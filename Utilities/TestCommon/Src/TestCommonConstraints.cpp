 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "TestCommonConstraints.h"
#include "TestCommonMiscUtil.h"
#include "Parse/Parse.h"

#ifndef _WIN32
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)
#endif

#define            SCHEMA_NAME            L"constraints"
#define            CLASS_NAME             L"cdataclass"    // lower case to compensate for MySQl on Linux
#define            CLASS_NAME_BASE        L"CDataBaseClass"

#define            PROP_UNIQUE1           L"unique1"
#define            PROP_UNIQUE2_1         L"unique2_1"
#define            PROP_UNIQUE2_2         L"unique2_2"

#define            PROP_BYTE_R            L"ByteRange"
#define            PROP_DATE_R            L"DateRange"
#define            PROP_DECIMAL_R         L"DecimalRange"
#define            PROP_DOUBLE_R          L"DoubleRange"
#define            PROP_INT16_R           L"Int16Range"
#define            PROP_INT32_R           L"Int32Range"
#define            PROP_INT64_R           L"Int64Range"
#define            PROP_SINGLE_R          L"SingleRange"
#define            PROP_STRING_R          L"StringRange"


#define            PROP_BYTE_L            L"ByteList"
#define            PROP_DATE_L            L"DatetimeList"
#define            PROP_DOUBLE_L          L"DoubleList"
#define            PROP_INT32_L           L"Int32List"
#define            PROP_INT64_L           L"Int64List"
#define            PROP_SINGLE_L          L"SingleList"
#define            PROP_STRING_L          L"StringList"
#define            PROP_LSTR_L            L"LargeStringList"

#define            NUM_UNIQUE_KEYS        3

#define            MIN_INCLUSIVE          true
#define            MAX_INCLUSIVE          false

#define            INT_MIN_INCLUSIVE      true
#define            INT_MAX_INCLUSIVE      false

static int         LARGE_STRING_COUNT     = 100;

static FdoByte    BYTE_RANGE[2]           = {11, 22};
static FdoDouble  DOUBLE_RANGE[2]         = {50.123456789012345678901234567890, 100.123456789012345678901234567890};
static FdoInt16   INT16_RANGE[2]          = {-100, 200};
static FdoInt32   INT32_RANGE[2]          = {10, 20};
static FdoInt64   INT64_RANGE[2]          = {100, 200};
static FdoFloat   SINGLE_RANGE[1]         = { (float) 0.000001};
static FdoString* STRING_RANGE[2]         = {L"MOM", L"PAPA"};

static FdoByte    BYTE_LIST[3]            = {1, 2, 3};
static FdoString* DATE_LIST[3]            = {L"TIMESTAMP '2003-10-31 03:02:01'", L"TIMESTAMP '2005-10-31 03:02:01'", L"TIMESTAMP '2005-10-31 15:02:01'"};
static FdoDouble  DOUBLE_LIST[3]          = {0.123456789012345678901234567890, 100, 0.123456789012345678901234567890};
static FdoInt32   INT32_LIST[5]           = {10, 20, 30, LONG_MIN, LONG_MAX};
static FdoInt64   INT64_LIST[4]           = {LLONG_MIN, 52, LLONG_MAX - 1, LLONG_MAX};
static FdoFloat   SINGLE_LIST[3]          = { (float) 0.1234567, (float) 100, (float) 1.12345678};
static FdoString* STRING_LIST[]           = { L"open", L"close" };
static wchar_t    LARGE_STRING_LIST[395][20];

static FdoByte    UPD_BYTE_RANGE[2]       = {11, 23};
static FdoInt32   UPD_INT32_RANGE[2]      = {10, 2001};
static FdoInt32   UPD_INT32_LIST[6]       = {10, 20, 30, LONG_MIN, LONG_MAX, 77};
static FdoString* UPD_STRING_LIST[]       = { L"semiclosed", L"close" };

TestCommonConstraints::TestCommonConstraints(void)
{
}

TestCommonConstraints::~TestCommonConstraints(void)
{
}

void TestCommonConstraints::setUp ()
{
}

void TestCommonConstraints::TestCreateConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    try {
        // delete, re-create and open the datastore
        printf( "Initializing Connection ... \n" );
        connection = CreateConnection( true );

         printf( "Creating Constraints Schema ... \n" );
        CreateConstraintsSchema( connection );

        printf( "Closing Connection ... \n" );
        connection->Close();
    }
    catch ( FdoException* e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        TestCommonFail( e );
    }
    catch ( CppUnit::Exception e ) 
    {
        if (connection) connection->Close(); 
        throw;
    }
    catch (...)
    {
        try {
            if (connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
           CPPUNIT_FAIL ("caught unexpected exception");
       }
}

void TestCommonConstraints::TestDescribeConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
        printf( "Initializing Connection ... \n" );
        connection = CreateConnection();

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME_BASE );
        DescribeConstraintsSchema( connection, CLASS_NAME_BASE, NUM_UNIQUE_KEYS, GetExpectedCheckConstraintCount(connection), false );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME );
        DescribeConstraintsSchema( connection, CLASS_NAME, 0, 0, false );

        printf( "Closing Connection ... \n" );
        connection->Close();
    }
    catch ( FdoException* e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        TestCommonFail( e );
    }
    catch ( CppUnit::Exception e ) 
    {
        if (connection) connection->Close(); 
        throw;
    }
    catch (...)
    {
        try {
            if (connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
           CPPUNIT_FAIL ("caught unexpected exception");
       }
}

void TestCommonConstraints::TestUpdateCheckConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
        printf( "Initializing Connection ... \n" );
        connection = CreateConnection();

        printf( "Describe Constraints Schema ... \n" );
        UpdateCheckConstraints( connection );

        printf( "Closing Connection ... \n" );
        connection->Close();
    }
    catch ( FdoException* e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        TestCommonFail( e );
    }
    catch ( CppUnit::Exception e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        throw;
    }
       catch (...)
       {
           CPPUNIT_FAIL ("caught unexpected exception");
       }
}

void TestCommonConstraints::TestUpdateUniqueConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
        printf( "Initializing Connection ... \n" );
        connection = CreateConnection();

        printf( "Updating Constraints Schema ... \n" );
        UpdateUniqueConstraints( connection );

        connection->Close();
    }
    catch ( FdoException* e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        TestCommonFail( e );
    }
    catch ( CppUnit::Exception e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        throw;
    }
       catch (...)
       {
           CPPUNIT_FAIL ("caught unexpected exception");
       }
}

void TestCommonConstraints::TestDescribeUpdatedConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
        printf( "Initializing Connection ... \n" );
        connection = CreateConnection();

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME_BASE );

        FdoInt32 NumCheckConstraints = GetExpectedCheckConstraintCount(connection);

        DescribeConstraintsSchema( 
            connection, 
            CLASS_NAME_BASE, NUM_UNIQUE_KEYS - 1, 
            CanRestrictCheckConstraint() ? NumCheckConstraints : NumCheckConstraints - 1, 
            true 
        );

        printf( "Describe Constraints Schema class %ls... \n", (FdoString *)CLASS_NAME );
        DescribeConstraintsSchema( connection, CLASS_NAME, 0, 0, false );

        printf( "Closing Connection ... \n" );
        connection->Close();
    }
    catch ( FdoException* e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        TestCommonFail( e );
    }
    catch ( CppUnit::Exception e ) 
    {
        try {
            if ( connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
        throw;
    }
       catch (...)
       {
        try {
            if (connection) connection->Close(); 
        }
        catch ( ... ) 
        {
        }
           CPPUNIT_FAIL ("caught unexpected exception");
       }
}

void TestCommonConstraints::TestRestrictCheckConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    if ( !CanRestrictCheckConstraint() ) {
        try
        {
            // delete, re-create and open the datastore
            printf( "Initializing Connection ... \n" );
            connection = CreateConnection( true );

             printf( "Creating Constraints Schema ... \n" );
            CreateConstraintsSchema( connection );

            printf( "Restrict Constraints  ... \n" );
            RestrictCheckConstraints( connection );

            printf( "Closing Connection ... \n" );
            connection->Close();
        }
        catch ( FdoException* e ) 
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            TestCommonFail( e );
        }
        catch ( CppUnit::Exception e ) 
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            throw;
        }
        catch (...)
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            CPPUNIT_FAIL ("caught unexpected exception");
        }
    }
}

void TestCommonConstraints::TestDateTimeConstraints ()
{
    FdoPtr<FdoIConnection> connection;

    if ( !CanRestrictCheckConstraint() ) {
        try
        {
            // delete, re-create and open the datastore
            printf( "Initializing Connection ... \n" );
            connection = CreateConnection( true );

             printf( "Creating Constraints Schema ... \n" );
            CreateConstraintsSchema( connection );

            printf( "Testing DateTime Constraints  ... \n" );
            DateTimeConstraints( connection );

            printf( "Closing Connection ... \n" );
            connection->Close();
        }
        catch ( FdoException* e ) 
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            TestCommonFail( e );
        }
        catch ( CppUnit::Exception e ) 
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            throw;
        }
        catch (...)
        {
            try {
                if ( connection) connection->Close(); 
            }
            catch ( ... ) 
            {
            }
            CPPUNIT_FAIL ("caught unexpected exception");
        }
    }
}

void TestCommonConstraints::CreateConstraintsSchema( FdoIConnection* connection )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();
    FdoInt32                          typeCount;
    FdoInt32                          idx;
    FdoDataType*                      supportedTypes = schemaCap->GetDataTypes( typeCount );

    FdoPtr<FdoIApplySchema>            pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    FdoFeatureSchemasP pSchemas = FdoFeatureSchemaCollection::Create(NULL);

    FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( SCHEMA_NAME, L"AutoCAD schema" );
    pSchemas->Add( pSchema );

    FdoPtr<FdoFeatureClass> pCData = FdoFeatureClass::Create( CLASS_NAME_BASE, L"Constraints" );
    pCData->SetIsAbstract(true);

    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPropertiesP(pCData->GetProperties())->Add( pProp );
    FdoDataPropertiesP(pCData->GetIdentityProperties())->Add( pProp );

    for ( idx = 0; idx < typeCount; idx++ ) {
        switch ( supportedTypes[idx] ) {
        case FdoDataType_Byte:
            //////////////  Byte range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_BYTE_R, 
                FdoDataType_Byte, 
                BYTE_RANGE, 
                sizeof(BYTE_RANGE) / sizeof(FdoByte),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            //////////////  Byte List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_BYTE_L, 
                FdoDataType_Byte, 
                BYTE_LIST, 
                sizeof(BYTE_LIST) / sizeof(FdoByte)
            );

            break;
        
        case FdoDataType_DateTime:
            {
                //////////////  DateTime Range property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pDatetimeRange9 = FdoDataPropertyDefinition::Create( PROP_DATE_R, L"" );
                pDatetimeRange9->SetDataType( FdoDataType_DateTime );
                pDatetimeRange9->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
                {
                    FdoDateTime dateTime;
                    dateTime.day = 15;
                    dateTime.month = 5;
                    dateTime.year = 2005;
                    dateTime.hour = 0;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;

                    FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr9R =  FdoPropertyValueConstraintRange::Create();
                    newRangeConstr9R->SetMinInclusive(MIN_INCLUSIVE);
                    FdoPtr<FdoDataValue>   val9R = FdoDataValue::Create( dateTime );
                    newRangeConstr9R->SetMinValue( val9R );

                    dateTime.day = 1;
                    dateTime.month = 2;
                    dateTime.year = 2006;
                    dateTime.hour = 18;
                    dateTime.minute = 0;
                    dateTime.seconds = 0;

                    newRangeConstr9R->SetMaxInclusive(MAX_INCLUSIVE);
                    val9R = FdoDataValue::Create( dateTime );
                    newRangeConstr9R->SetMaxValue( val9R );
                    
                    pDatetimeRange9->SetValueConstraint(newRangeConstr9R);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pDatetimeRange9  );

                //////////////  DateTime List property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pDatetimeList9 = FdoDataPropertyDefinition::Create( PROP_DATE_L, L"" );
                pDatetimeList9->SetDataType( FdoDataType_DateTime );
                pDatetimeList9->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsValueConstraintsList() )
                {
                    FdoPtr<FdoPropertyValueConstraintList>  newListConstr9 =  FdoPropertyValueConstraintList::Create();
                    FdoPtr<FdoDataValueCollection> list9 = newListConstr9->GetConstraintList();

                    FdoDateTime dateTime;
                    dateTime.day = 31;
                    dateTime.month = 10;
                    dateTime.year = 2003;
                    dateTime.hour = 3;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;

                    FdoPtr<FdoDataValue> val39 = FdoDataValue::Create(dateTime);

                    list9->Add( val39 );

                    dateTime.year = 2005;
                    val39 = FdoDataValue::Create(dateTime);
                    list9->Add( val39 );

                    dateTime.hour = 15;
                    val39 = FdoDataValue::Create(dateTime);
                    list9->Add( val39 );

                    pDatetimeList9->SetValueConstraint(newListConstr9);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pDatetimeList9  );
            }
            break;

        case FdoDataType_Decimal:
            {
                //////////////  Decimal Range property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pProp = AddDoubleRangeProperty(
                    schemaCap,
                    pCData, 
                    PROP_DECIMAL_R, 
                    FdoDataType_Decimal, 
                    DOUBLE_RANGE, 
                    sizeof(DOUBLE_RANGE) / sizeof(FdoDouble),
                    MIN_INCLUSIVE,
                    MAX_INCLUSIVE
                );

                pProp->SetPrecision(8);
                pProp->SetScale(2);
            }
            break;

        case FdoDataType_Double:
            //////////////  Double Range property ///////////////
            AddDoubleRangeProperty(
                schemaCap,
                pCData, 
                PROP_DOUBLE_R, 
                FdoDataType_Double, 
                DOUBLE_RANGE, 
                sizeof(DOUBLE_RANGE) / sizeof(FdoDouble),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            //////////////  Double List property ///////////////
            AddDoubleListProperty(
                schemaCap,
                pCData, 
                PROP_DOUBLE_L, 
                FdoDataType_Double, 
                DOUBLE_LIST, 
                sizeof(DOUBLE_LIST) / sizeof(FdoDouble)
            );

            break;

        case FdoDataType_Int16:
            //////////////  Int16 Range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT16_R, 
                FdoDataType_Int16, 
                INT16_RANGE, 
                sizeof(INT16_RANGE) / sizeof(FdoInt16),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            break;

        case FdoDataType_Int32:
            //////////////  Int32 Range property ///////////////
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT32_R, 
                FdoDataType_Int32, 
                INT32_RANGE, 
                sizeof(INT32_RANGE) / sizeof(FdoInt32),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            //////////////  Int32 List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_INT32_L, 
                FdoDataType_Int32, 
                INT32_LIST, 
                sizeof(INT32_LIST) / sizeof(FdoInt32)
            );

            break;

        case FdoDataType_Int64:
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_INT64_R, 
                FdoDataType_Int64, 
                INT64_RANGE, 
                sizeof(INT64_RANGE) / sizeof(FdoInt64),
                INT_MIN_INCLUSIVE,
                INT_MAX_INCLUSIVE
            );

            //////////////  Int64 List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_INT64_L, 
                FdoDataType_Int64, 
                INT64_LIST, 
                sizeof(INT64_LIST) / sizeof(FdoInt64)
            );

            break;

        case FdoDataType_Single:
            AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_SINGLE_R, 
                FdoDataType_Single, 
                SINGLE_RANGE, 
                sizeof(SINGLE_RANGE) / sizeof(FdoFloat),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            //////////////  Float List property ///////////////
            AddListProperty(
                schemaCap,
                pCData, 
                PROP_SINGLE_L, 
                FdoDataType_Single, 
                SINGLE_LIST, 
                sizeof(SINGLE_LIST) / sizeof(FdoFloat)
            );

            break;

        case FdoDataType_String:
            //////////////  String range property ///////////////
            FdoPtr<FdoDataPropertyDefinition> pRangeString = AddRangeProperty(
                schemaCap,
                pCData, 
                PROP_STRING_R, 
                FdoDataType_String, 
                STRING_RANGE, 
                sizeof(STRING_RANGE) / sizeof(FdoString*),
                MIN_INCLUSIVE,
                MAX_INCLUSIVE
            );

            pRangeString->SetLength(10);

            //////////////  String List property ///////////////
            FdoPtr<FdoDataPropertyDefinition> pListString = AddListProperty(
                schemaCap,
                pCData, 
                PROP_STRING_L, 
                FdoDataType_String, 
                STRING_LIST, 
                sizeof(STRING_LIST) / sizeof(FdoString*)
            );

            pListString->SetLength(10);

            {
                //////////////  Long String List property ///////////////
                FdoPtr<FdoDataPropertyDefinition> pListLargeString = FdoDataPropertyDefinition::Create( PROP_LSTR_L, L"" );
                pListLargeString->SetDataType( FdoDataType_String );
                pListLargeString->SetLength(20);
                pListLargeString->SetNullable(true);

                // Add CHECK constraint
                if ( schemaCap->SupportsValueConstraintsList() )
                {
                    FdoPtr<FdoPropertyValueConstraintList>  newListConstr12 =  FdoPropertyValueConstraintList::Create();
                    FdoPtr<FdoDataValueCollection> list12 = newListConstr12->GetConstraintList();

                    FdoInt32 idx;
                    for ( idx = 0; idx < LARGE_STRING_COUNT; idx++ ) {
                        wcscpy( LARGE_STRING_LIST[idx], FdoStringP::Format( L"%06d", idx ) );
                        FdoPtr<FdoDataValue>   val338 = FdoDataValue::Create( LARGE_STRING_LIST[idx] );
                        list12->Add( val338 );
                    }

                    pListLargeString->SetValueConstraint(newListConstr12);
                }

                FdoPropertiesP(pCData->GetProperties())->Add( pListLargeString  );
            }
            break;
        }
    }

    //// Add the class to schema
    FdoClassesP(pSchema->GetClasses())->Add( pCData );

    // Add UNIQUE constraints - 
    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        FdoClassDefinitionP pClass = FdoClassesP( pSchema->GetClasses() )->GetItem( CLASS_NAME_BASE );
        FdoPtr<FdoUniqueConstraintCollection> constraints = pClass->GetUniqueConstraints();

        //////////////  1st unique property - single ///////////////
        FdoPtr<FdoDataPropertyDefinition> pUnique1Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE1, L"" );
        pUnique1Int->SetDataType( FdoDataType_Int32 );
        pUnique1Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique1Int  );

        FdoPtr<FdoUniqueConstraint>  newUniqueConstr1 = FdoUniqueConstraint::Create();
        FdoPtr<FdoDataPropertyDefinitionCollection> pDataPropColl = newUniqueConstr1->GetProperties();
        pDataPropColl->Add( pUnique1Int );
        constraints->Add( newUniqueConstr1 );

        //////////////  2nd unique property - Composite ///////////////
        FdoPtr<FdoDataPropertyDefinition> pUnique21Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_1, L"" );
        pUnique21Int->SetDataType( FdoDataType_Int32 );
        pUnique21Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique21Int  );

        FdoPtr<FdoDataPropertyDefinition> pUnique22Int = FdoDataPropertyDefinition::Create( PROP_UNIQUE2_2, L"" );
        pUnique22Int->SetDataType( FdoDataType_Int32 );
        pUnique22Int->SetNullable(true);
        FdoPropertiesP(pCData->GetProperties())->Add( pUnique22Int  );

        FdoPtr<FdoUniqueConstraint>  newUniqueConstr2 = FdoUniqueConstraint::Create();
        pDataPropColl = newUniqueConstr2->GetProperties();
        pDataPropColl->Add( pUnique21Int );
        pDataPropColl->Add( pUnique22Int );
        constraints->Add( newUniqueConstr2 );
    }

    // Create a new class based on the previous ...
    FdoPtr<FdoFeatureClass> pCDataDerived = FdoFeatureClass::Create( CLASS_NAME, L"Constraits" );
    pCDataDerived->SetBaseClass( pCData );
    pCDataDerived->SetIsAbstract(false);

    //// Add the class to schema
    FdoClassesP(pSchema->GetClasses())->Add( pCDataDerived );

    ///////// Done.
    pCmd->SetFeatureSchema( pSchema );
    pCmd->Execute();

    FdoPtr<FdoIInsert> insertCmd;

    //////////////////////////////// Test UNIQUE ////////////////////////////////////////

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        // This should succeed
        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"open",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            PROP_SINGLE_R, FdoDataType_Single, (FdoFloat) 0.000002,
            (FdoString*) NULL
        );

        bool    uniqueSuccess1 = true;
        try {
            TestCommonMiscUtil::InsertObject(
                connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 30,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violation exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            uniqueSuccess1 = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #4", uniqueSuccess1 == false );

        // This should succeed
        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"open",
            PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
            (FdoString*) NULL
        );

        bool    uniqueSuccess2 = true;
        try {
            TestCommonMiscUtil::InsertObject(
                connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 30,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"close",
                PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
                PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 2000,
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violationexception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            uniqueSuccess2 = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get unique constraint violation prop #[5,6]", uniqueSuccess2 == false );
    }

    //////////////////////////////// Test CHECK ////////////////////////////////////////

    if ( schemaCap->SupportsValueConstraintsList() )
    {
        bool    checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 22,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"open",
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected RangeInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #1", checkSuccess == false );

        checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 40,
                PROP_STRING_L,  FdoDataType_String, L"open",
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected ListInt constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #2", checkSuccess == false );

        checkSuccess = true;
        try {
            TestCommonMiscUtil::InsertObject(
                connection,
                insertCmd,
                SCHEMA_NAME,
                CLASS_NAME,
                PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                PROP_STRING_L,  FdoDataType_String, L"xxxxx",
                (FdoString*) NULL
            );
        } catch (FdoException *ex) {
            DBG(printf("Expected ListString constraint exception: %ls", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            checkSuccess = false;
        }
        CPPUNIT_ASSERT_MESSAGE("Should get check constraint violation prop #3", checkSuccess == false );
    }
}

void TestCommonConstraints::DescribeConstraintsSchema( FdoIConnection* connection, FdoString* className, int numUkeys, int numCkeys, bool afterUpdate )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
    FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
pSchemas2->WriteXml( L"constraints.xml" );
    FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
    FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( className );
    FdoPtr<FdoClassDefinition> pClassBase2 = pClass2->GetBaseClass();

    int count1 = pClass2->GetProperties()->GetCount();
    int count2 = pClass2->GetBaseProperties()->GetCount();

    printf("#props=%ld #baseProps=%ld\n", count1, count2);

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        ///////////////// UNIQUE() CONSTRAINTS //////////////////////////////////////////
        FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

        int        count = 0;
        bool    found_unique1 = false;
        bool    found_unique2 = false;
        bool    found_unique3 = false;

        //if ( pUniqueCs->GetCount() == 0 )
        //    pUniqueCs = pClassBase2->GetUniqueConstraints();

        for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
            FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
            FdoDataPropertiesP             pProps = pUniqueC->GetProperties();
            
            printf("Unique key #%d:\n", i);

            for ( int j = 0; j < pProps->GetCount(); j++ ) {
                FdoDataPropertyP    pProp = pProps->GetItem(j);

                printf("\t%d %ls\n", j, pProp->GetName());
                count++;
            
                if ( wcscmp(pProp->GetName(), PROP_UNIQUE1 ) == 0 )
                    found_unique1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_1 ) == 0 )
                    found_unique2 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_2 ) == 0 )
                    found_unique3 = true;
            }    
        }

        CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == numUkeys );
        if ( numUkeys != 0 ) {
            if ( afterUpdate ) 
                CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique3);
            else
                CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && found_unique2 && found_unique3);
        }
    }

    ///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////

    /// Validate only if check constraints supported
    if ( schemaCap->SupportsValueConstraintsList() &&
         schemaCap->SupportsExclusiveValueRangeConstraints() &&
         schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoInt32 count = 0;

        FdoPropertiesP    pProps = (FdoPropertyDefinitionCollection *)pClass2->GetProperties();
        for ( int i = 0; i < pProps->GetCount(); i++ ) {
            FdoPtr<FdoDataPropertyDefinition>    pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

            if (pProp == NULL )
                continue;

            FdoPtr<FdoPropertyValueConstraint>        pConstr = pProp->GetValueConstraint();

            if (pConstr == NULL )
                continue;

            if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
            {
                FdoPropertyValueConstraintRange*        pConstrR = (FdoPropertyValueConstraintRange*)(pConstr.p);

                if ( wcscmp( pProp->GetName(), PROP_INT32_R ) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( afterUpdate ? UPD_INT32_RANGE[0] : INT32_RANGE[0] );
                    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( afterUpdate ? UPD_INT32_RANGE[1] : INT32_RANGE[1] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop1R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop1R",  wcscmp(valMax->ToString(), val2->ToString()) == 0 );
                }
                else if ( wcscmp( pProp->GetName(), PROP_BYTE_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoByteValue>   expMin = FdoByteValue::Create( afterUpdate ? UPD_BYTE_RANGE[0] : BYTE_RANGE[0] );
                    FdoPtr<FdoByteValue>   expMax = FdoByteValue::Create( afterUpdate ? UPD_BYTE_RANGE[1] : BYTE_RANGE[1] );

                    FdoByteValue*    valMin1 = dynamic_cast<FdoByteValue*>(valMin.p);
                    FdoByteValue*    valMax1 = dynamic_cast<FdoByteValue*>(valMax.p);

                    if ( valMin1 == NULL || valMax1 == NULL )
                    {
                        CPPUNIT_ASSERT_MESSAGE("Wrong data type for Prop2R", false);
                    }

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop2R",  valMin1->GetByte() == expMin->GetByte() );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop2R",  valMax1->GetByte() == expMax->GetByte() );
                }
                else if ( wcscmp( pProp->GetName(), PROP_STRING_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( STRING_RANGE[0] );
                    FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( STRING_RANGE[1] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop3R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop3R",  wcscmp(valMax->ToString(), val2->ToString()) == 0 );
                }
                else if ( wcscmp( pProp->GetName(), PROP_SINGLE_R) == 0 ) 
                {
                    CPPUNIT_ASSERT_MESSAGE("Wrong MinInclusive", pConstrR->GetMinInclusive() == MIN_INCLUSIVE );
                    CPPUNIT_ASSERT_MESSAGE("Wrong MaxInclusive", pConstrR->GetMaxInclusive() == MAX_INCLUSIVE );

                    FdoPtr<FdoDataValue>    valMin = pConstrR->GetMinValue();
                    FdoPtr<FdoDataValue>    valMax = pConstrR->GetMaxValue();

                    FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( SINGLE_RANGE[0] );

                    CPPUNIT_ASSERT_MESSAGE("Wrong Min Value Prop4R",  wcscmp(valMin->ToString(), val1->ToString()) == 0 );
                    CPPUNIT_ASSERT_MESSAGE("Wrong Max Value Prop4R (not null)",  valMax == NULL );
                }
                
                count++;
            }
            else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
            {
                // NOTE: SQL SErver is reversing the order of items in the list. Therefore we scan 
                // the entire list for a match.
                FdoPropertyValueConstraintList*        pConstrL = (FdoPropertyValueConstraintList*)(pConstr.p);
                FdoPtr<FdoDataValueCollection>        pList = pConstrL->GetConstraintList();

                if ( wcscmp( pProp->GetName(), PROP_INT32_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( 
                        pProp->GetName(), 
                        pList, 
                        afterUpdate ? UPD_INT32_LIST : INT32_LIST, 
                        (afterUpdate ? sizeof(UPD_INT32_LIST) : sizeof(INT32_LIST)) / sizeof(int) 
                    );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_STRING_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( 
                        pProp->GetName(), 
                        pList, 
                        afterUpdate ? UPD_STRING_LIST : STRING_LIST, 
                        (afterUpdate ? sizeof(UPD_STRING_LIST) : sizeof(STRING_LIST)) / sizeof(wchar_t*) 
                    );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_BYTE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, BYTE_LIST, sizeof(BYTE_LIST) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_DOUBLE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckDoubleListConstraint( pProp->GetName(), pList, DOUBLE_LIST, sizeof(DOUBLE_LIST) / sizeof(double), FdoDataType_Double );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_DATE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckDateListConstraint( pProp->GetName(), pList, DATE_LIST, sizeof(DATE_LIST) / sizeof(wchar_t*) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_INT64_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, INT64_LIST, sizeof(INT64_LIST) / sizeof(FdoInt64) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_SINGLE_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));
                    CheckListConstraint( pProp->GetName(), pList, SINGLE_LIST, sizeof(SINGLE_LIST) / sizeof(float) );
                    count++;
                }
                else if ( wcscmp( pProp->GetName(), PROP_LSTR_L ) == 0) {
                    DBG(printf("Check List key #%d: (%ls in (", count, pProp->GetName()));

                    FdoInt32 idx;
                    for ( idx = 0; idx < LARGE_STRING_COUNT; idx++ ) {
                        wcscpy( LARGE_STRING_LIST[idx], FdoStringP::Format( L"%06d", idx ) );
                    }

                    CheckListConstraint( pProp->GetName(), pList, LARGE_STRING_LIST, LARGE_STRING_COUNT );
                    count++;
                }
            }
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong number of CHECK constraints", count == numCkeys );
    }
}

void TestCommonConstraints::UpdateUniqueConstraints( FdoIConnection* connection )
{
    int        count = 0;
    bool    found_unique1 = false;
    bool    found_unique2 = false;
    bool    found_unique3 = false;

    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsUniqueValueConstraints() && schemaCap->SupportsCompositeUniqueValueConstraints() )
    {
        printf(".UpdateUniqueConstraints ...\n");

        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        ///////////////// UPDATE UNIQUE() CONSTRAINTS //////////////////////////////////////////
        
        FdoPtr<FdoUniqueConstraintCollection> pUniqueCs = pClass2->GetUniqueConstraints();

        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        /// MySql reverses the order of the constraints and also the constrainted props in the list
        int        index1;
        int        index2 = -1;

        for ( index1 = 0; index1 < pUniqueCs->GetCount(); index1++ ) 
        {
            //////// Remove the unique key on PROP_UNIQUE1
            FdoPtr<FdoUniqueConstraint>  pUniqueC1 = pUniqueCs->GetItem(index1);
            FdoDataPropertiesP             pProps1 = pUniqueC1->GetProperties();
            FdoDataPropertyP             pProp1 = pProps1->GetItem(PROP_UNIQUE1);
            if ( pProp1 ) 
                index2 = index1;
        }

        printf("Unique key on (%ls) removed\n", PROP_UNIQUE1);
        pUniqueCs->RemoveAt(index2);
        pApplyCmd->Execute();

        // Just one constraint left
        index2 = 0;

        ///////// Combined ukey (PROP_UNIQUE2_1, PROP_UNIQUE2_2). Update by removing (PROP_UNIQUE2_1)
        FdoPtr<FdoUniqueConstraint>  pUniqueC0 = pUniqueCs->GetItem(index2);
        FdoDataPropertiesP             pProps0 = pUniqueC0->GetProperties();
        printf("Unique key on (%ls, %ls) updated to (%ls)\n", PROP_UNIQUE2_1, PROP_UNIQUE2_2, PROP_UNIQUE2_2);

        pProps0->RemoveAt( pProps0->IndexOf(PROP_UNIQUE2_1) );
        pApplyCmd->Execute();


        // Add some duplicate data. It should succeed because the constraints on PROP_UNIQUE1 and PROP_UNIQUE2_1 were removed above
        FdoStringP    insertSql2 = FdoStringP::Format(L"insert into %ls (classid, revisionnumber, %ls, %ls, %ls, %ls, %ls, %ls) values ", 
                        CLASS_NAME, PROP_INT32_R , PROP_INT32_L, PROP_STRING_L, PROP_UNIQUE1, PROP_UNIQUE2_1, PROP_UNIQUE2_2 );

        FdoInt32   value = 7777;

        FdoPtr<FdoIInsert> insertCmd;
        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"open",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_1,  FdoDataType_Int32,  (FdoInt32) 1000,
            PROP_UNIQUE2_2,  FdoDataType_Int32,  value,
            (FdoString*) NULL
        );

        // Restore the uniqueness of PROP_UNIQUE1
        FdoPtr<FdoDataPropertyDefinition> pUnique4 = FdoDataPropertyDefinition::Create( PROP_UNIQUE1, L"" );
        FdoPtr<FdoUniqueConstraint>  newUniqueConstr4 = FdoUniqueConstraint::Create();
        FdoPtr<FdoDataPropertyDefinitionCollection> pDatPropCol = newUniqueConstr4->GetProperties();
        pDatPropCol->Add( pUnique4 );
        pUniqueCs->Add( newUniqueConstr4 );

        bool error = false;
        try    {
            pApplyCmd->Execute();
        } catch (FdoException *ex) {
            DBG(printf("Expected unique constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
            ex->Release();
            error = true;
        }
        CPPUNIT_ASSERT_MESSAGE("Expected unique constraint violation exception on PROP_UNIQUE1", error == true);

        // Remove the offending row
        TestCommonMiscUtil::DeleteObjects( 
            connection,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_UNIQUE2_2,
            FdoDataType_Int32,
            value,
            NULL
        );

        // Try again.
        pApplyCmd->Execute();

        // Do a new describe schema
        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
          pUniqueCs = pClass2->GetUniqueConstraints();

        // Check results
        for ( int i = 0; i < pUniqueCs->GetCount(); i++ ) {
            FdoPtr<FdoUniqueConstraint>  pUniqueC = pUniqueCs->GetItem(i);
            FdoDataPropertiesP             pProps = pUniqueC->GetProperties();

            for ( int j = 0; j < pProps->GetCount(); j++ ) {
                FdoDataPropertyP    pProp = pProps->GetItem(j);

                printf("\t%d %ls\n", j, pProp->GetName());
                count++;

                if ( wcscmp(pProp->GetName(), PROP_UNIQUE1 ) == 0 )
                    found_unique1 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_1 ) == 0 )
                    found_unique2 = true;
                else if ( wcscmp(pProp->GetName(), PROP_UNIQUE2_2 ) == 0 )
                    found_unique3 = true;
            }    
        }
        
        CPPUNIT_ASSERT_MESSAGE("Wrong number of unique keys", count == NUM_UNIQUE_KEYS - 1);
        CPPUNIT_ASSERT_MESSAGE("Unique keys properties not found", found_unique1 && !found_unique2 && found_unique3);
    }
}

void TestCommonConstraints::UpdateCheckConstraints( FdoIConnection* connection )
{

    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();
    FdoPtr<FdoIInsert> insertCmd;

    // MySql doesn't have check constraints
    if ( !schemaCap->SupportsValueConstraintsList() )
        return;

    printf(".UpdateCheckConstraints ...\n");

    FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

    pDescCmd->SetSchemaName( SCHEMA_NAME );
    FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
    FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
    FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
    FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
   
    FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
    pApplyCmd->SetFeatureSchema(pSchema2);

    ///////////////// CHECK() CONSTRAINTS //////////////////////////////////////////
    FdoPtr<FdoPropertyDefinitionCollection> pProps = pClass2->GetProperties();

    for ( int i = 0; i < pProps->GetCount(); i++ ) {
        FdoPtr<FdoDataPropertyDefinition>    pProp = (FdoDataPropertyDefinition *)pProps->GetItem(i);

        if (pProp == NULL )
            continue;

        FdoPtr<FdoPropertyValueConstraint>        pConstr = pProp->GetValueConstraint();

        if (pConstr == NULL )
            continue;

        if ( pConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range )
        {
            if ( wcscmp( pProp->GetName(), PROP_INT32_R  ) == 0) {
                // Remove the constraint
                pProp->SetValueConstraint( NULL );

                pApplyCmd->Execute();

                // New value to insert into PROPERTY_1, outside range
                // This should succeed since the constraint on PROPERTY_1 was removed
                FdoInt32 value = 2000;

                TestCommonMiscUtil::InsertObject(
                    connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_INT32_R , FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"open",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6666,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8888,
                    (FdoString*) NULL
                );

                // Restore the range CHECK constraint
                FdoPtr<FdoPropertyValueConstraintRange>  newRangeConstr1 =  FdoPropertyValueConstraintRange::Create();
                newRangeConstr1->SetMinInclusive(INT_MIN_INCLUSIVE);

                FdoPtr<FdoDataValue>   val1 = FdoDataValue::Create( INT32_RANGE[0] );
                newRangeConstr1->SetMinValue( val1 );

                newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
                FdoPtr<FdoDataValue>   val2 = FdoDataValue::Create( INT32_RANGE[1] );
                newRangeConstr1->SetMaxValue( val2 );
                pProp->SetValueConstraint(newRangeConstr1);

                bool error = false;
                try    {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                }
                CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROPERTY_1", error == true);

                // Enlarge the range CHECK constraint
                newRangeConstr1->SetMaxInclusive(INT_MAX_INCLUSIVE);
                val2 = FdoDataValue::Create( value + 1 ); // Note: INT_MAX_INCLUSIVE is FALSE
                newRangeConstr1->SetMaxValue( val2 ); 
                pProp->SetValueConstraint(newRangeConstr1);

                error = false;
                try    {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    if ( CanRestrictCheckConstraint() )
                        throw;
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                    pProp->SetValueConstraint( NULL );
                }
                CPPUNIT_ASSERT_MESSAGE("Expected 2nd check constraint violation exception on PROPERTY_1", error == !CanRestrictCheckConstraint());

                // Try insert again. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_INT32_R , FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"open",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6667,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8889,
                    (FdoString*) NULL
                );
            }
            if ( wcscmp( pProp->GetName(), PROP_BYTE_R ) == 0) {
                // New value to insert into PROPERTY_1, outside range
                // This should fail
                FdoByte value = BYTE_RANGE[1];

                bool error = false;
                try    {
                    TestCommonMiscUtil::InsertObject(
                        connection,
                        insertCmd,
                        SCHEMA_NAME,
                        CLASS_NAME,
                        PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                        PROP_BYTE_R, FdoDataType_Byte,  value,
                        PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                        PROP_STRING_L,  FdoDataType_String, L"open",
                        PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 8000,
                        PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 9000,
                        (FdoString*) NULL
                    );
                } catch (FdoException *ex) {
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                }
                CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROP_BYTE_R", error == true);

                // Enlarge the range CHECK constraint
                FdoPropertyValueConstraintRange*  newRangeConstr1 =  static_cast<FdoPropertyValueConstraintRange*>((FdoPropertyValueConstraint*) pConstr);
                FdoPtr<FdoDataValue> newValue = FdoDataValue::Create( UPD_BYTE_RANGE[1] );
                newRangeConstr1->SetMaxValue( newValue );
                pProp->SetValueConstraint(newRangeConstr1);

                pApplyCmd->Execute();

                // Try insert again. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_BYTE_R, FdoDataType_Int32,  value,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
                    PROP_STRING_L,  FdoDataType_String, L"open",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6667,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8889,
                    (FdoString*) NULL
                );
            }
        }
        else if (pConstr->GetConstraintType() == FdoPropertyValueConstraintType_List)
        {
            if ( wcscmp( pProp->GetName(), PROP_INT32_L ) == 0) {

                // Add a new value to the list.
                FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  FDO_SAFE_ADDREF((FdoPropertyValueConstraintList *)pConstr.p);
                FdoPtr<FdoDataValueCollection>            pList = pConstrList->GetConstraintList();

                FdoInt32        newValue = 77;
                FdoPtr<FdoDataValue>   newVal = FdoDataValue::Create( newValue );

                pList->Add( newVal );
                pProp->SetValueConstraint( pConstrList );
                pApplyCmd->Execute();

                // Now try to insert. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_INT32_L,  FdoDataType_Int32,  newValue,
                    PROP_STRING_L,  FdoDataType_String, L"open",
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6668,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8890,
                    (FdoString*) NULL
                );
            }
            else if ( wcscmp( pProp->GetName(), PROP_STRING_L ) == 0) {

                // Replace the 2nd constraint in ('open', 'close') with ('open', 'semi-close');
                FdoPtr<FdoPropertyValueConstraintList>  pConstrList =  FDO_SAFE_ADDREF((FdoPropertyValueConstraintList *)pConstr.p);
                FdoPtr<FdoDataValueCollection>            pList = pConstrList->GetConstraintList();

                // Note: SqlServer doesn't like constraints like "semi-close" or "semi_close"! 
                // Getting "String or binary data would be truncated" error.
                FdoString*                newValue = L"semiclosed";
                FdoPtr<FdoDataValue>   newVal = FdoDataValue::Create( newValue );
               
                FdoInt32 i;
                for ( i = 0; i < pList->GetCount(); i++ ) 
                {
                    FdoPtr<FdoDataValue> val = pList->GetItem(i);
                    if ( wcscmp(((FdoStringValue*)(FdoDataValue*)val)->GetString(),L"open") == 0 ) 
                    {
                        pList->RemoveAt(i);
                        break;
                    }
                }

                pList->Add( newVal );
                pProp->SetValueConstraint( pConstrList );
                bool error = false;

                // Following fails because some rows have stringlist='open'
                try    {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    DBG(printf("Expected check constraint violation exception: %ls\n", (FdoString* )ex->GetExceptionMessage()));
                    ex->Release();
                    error = true;
                }
                CPPUNIT_ASSERT_MESSAGE("Expected check constraint violation exception on PROP_STRING_L", error == true);

                // Now try to delete. It should succeed.
                TestCommonMiscUtil::DeleteObjects( 
                    connection,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_STRING_L,
                    FdoDataType_String,
                    L"open",
                    NULL
                );

                // No "open" stringlist's, ApplySchema should succeed this time.
                pApplyCmd->Execute();
                
                // Now try to insert. It should succeed.
                TestCommonMiscUtil::InsertObject(
                    connection,
                    insertCmd,
                    SCHEMA_NAME,
                    CLASS_NAME,
                    PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 10,
                    PROP_STRING_L,  FdoDataType_String, newValue,
                    PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 6669,
                    PROP_UNIQUE2_2,  FdoDataType_Int32,  (FdoInt32) 8891,
                    (FdoString*) NULL
                );
            }
        }
    }
}

void TestCommonConstraints::RestrictCheckConstraints( FdoIConnection* connection )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        FdoPtr<FdoIInsert> insertCmd;
        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        FdoInt32 idx;

        printf( "Testing invalid list modifications ... \n" );
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoByteValue* byteVal = (FdoByteValue*) val.p;
                    if ( byteVal->GetByte() == 2 ) 
                        listL->RemoveAt(idx);
                }

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintList* constrL = (FdoPropertyValueConstraintList*) constr.p;
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                
                for ( idx = (listL->GetCount() - 1); idx >= 0; idx-- ) {
                    FdoPtr<FdoDataValue> val = listL->GetItem(idx);
                    FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                    if ( int32Val->GetInt32() == 30 ) 
                        listL->RemoveAt(idx);
                }

                FdoPtr<FdoInt32Value> newVal = FdoInt32Value::Create(40);
                listL->Add(newVal);

                dataProp->SetValueConstraint(constr);
            }

            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"yes")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"no")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"maybe")) );                

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            // Change Range constraint to List (should fail)
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintList> constrL = FdoPropertyValueConstraintList::Create();
                FdoPtr<FdoDataValueCollection> listL = constrL->GetConstraintList();
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"on")) );                
                listL->Add( FdoPtr<FdoDataValue>(FdoStringValue::Create(L"off")) );                

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrL.p);
            }

            // Change List constraint to Range (should fail)
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_L );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraintRange> constrR = FdoPropertyValueConstraintRange::Create();
                FdoPtr<FdoDataValue> val = FdoInt64Value::Create( 10LL ); 
                constrR->SetMinValue( val );
                val = FdoInt64Value::Create( 20LL ); 
                constrR->SetMaxValue( val );

                dataProp->SetValueConstraint( (FdoPropertyValueConstraint*) constrR.p);
            }

            FdoStringP messages;
            try {
                pApplyCmd->Execute();
            } catch (FdoException *ex) {
                messages = ex->GetExceptionMessage();
                ex->Release();
            }
            CPPUNIT_ASSERT_MESSAGE("Restrictive list modifications should have failed", messages != L"" );
                
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteList'; property has values and new constraint is more restrictive" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32List'; property has values and new constraint is more restrictive" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringList'; property has values and new constraint is more restrictive" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint type for property 'constraints:CDataBaseClass.StringRange'; property has values" ) );
            if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_L) > -1 )
                CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint type for property 'constraints:CDataBaseClass.Int64List'; property has values" ) );
        }

        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
        pApplyCmd->SetFeatureSchema( pSchema2 );

        // Simple range modifications. Iteration -1 shrinks range (fails), Iteration 1 expands range (succeeds)
        printf( "Testing simple range modifications ... \n" );
        for ( idx = -1; idx < 2; idx += 2 ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() - idx) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DECIMAL_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDecimalValue* decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( decimalVal->GetDecimal() + (idx * 1e-10) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                if ( idx < 0 ) {
                    dateTime.hour = 17;
                    dateTime.minute = 59;
                    dateTime.seconds = (FdoFloat) 59.99999;
                }
                else {
                    dateTime.seconds = (FdoFloat) 0.00001;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( doubleVal->GetDouble() - (idx * 1e-10) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoInt16Value* int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(int16Val->GetInt16() + idx) );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( int32Val->GetInt32() - idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_SINGLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoSingleValue* singleVal = (FdoSingleValue*) val.p;
                singleVal->SetSingle( (FdoFloat)(singleVal->GetSingle() - (idx * 1e-5)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoStringValue* stringVal = (FdoStringValue*) val.p;
                stringVal->SetString( (idx < 0) ? L"MOMM" : L"MOL" );
                dataProp->SetValueConstraint(constr);
            }

            if ( idx < 0 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Simple restrictive range modifications should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DECIMAL_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DecimalRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_SINGLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.SingleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );
        }

        // Modify both endpoints. Iteration -1 shrinks or shifts range (fails), Iteration 1 expands range (succeeds)
        printf( "Testing both end point modifications ... \n" );
        for ( idx = -1; idx < 2; idx += 2 ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() - idx) );

                val = constrR->GetMaxValue();
                byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(byteVal->GetByte() + idx) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                    
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                if ( idx < 0 ) 
                    doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );
                else
                    doubleVal->SetDouble( doubleVal->GetDouble() - 2e-10 );

                val = constrR->GetMaxValue();
                doubleVal = (FdoDoubleValue*) val.p;
                if ( idx < 0 ) 
                    doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;

                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );

                val = constrR->GetMaxValue();
                int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( int64Val->GetInt64() + idx );

                dataProp->SetValueConstraint(constr);
            }

            if ( idx <= 0 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Restricting both endpoints should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }
        }

        // Tests restricting range by changing inclusive settings.

        printf( "Testing inclusive settings ... \n" );
        FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
            constrR->SetMinInclusive(false);
            dataProp->SetValueConstraint(constr);

            FdoStringP messages;
            try {
                pApplyCmd->Execute();
            } catch (FdoException *ex) {
                messages = ex->GetExceptionMessage();
                ex->Release();
            }
            CPPUNIT_ASSERT_MESSAGE("Setting Mininclusive false should have failed", messages != L"" );
            CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
        
            constrR->SetMinInclusive(false);
            FdoPtr<FdoDataValue> val = constrR->GetMinValue();
            FdoStringValue* stringVal = (FdoStringValue*) val.p;
            stringVal->SetString( L"LZZZZZZZZZZZZZ" );
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

            constrR->SetMinInclusive(true);
            constrR->SetMaxInclusive(true);
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

            constrR->SetMaxInclusive(false);
            dataProp->SetValueConstraint(constr);
            try {
                pApplyCmd->Execute();
            } catch (FdoException *ex) {
                messages = ex->GetExceptionMessage();
                ex->Release();
            }
            CPPUNIT_ASSERT_MESSAGE("Setting ranges to exclusive should have failed.", messages != L"" );
            CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
            constrR->SetMaxInclusive(true);
        }

        prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
            constrR->SetMinInclusive(false);
            constrR->SetMaxInclusive(true);
            FdoPtr<FdoDataValue> val = constrR->GetMinValue();
            FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
            doubleVal->SetDouble( doubleVal->GetDouble() - 1e-10 );
            dataProp->SetValueConstraint(constr);

            pApplyCmd->Execute();
        
            constrR->SetMaxInclusive(false);
            val = constrR->GetMaxValue();
            doubleVal = (FdoDoubleValue*) val.p;
            doubleVal->SetDouble( doubleVal->GetDouble() + 1e-10 );
            dataProp->SetValueConstraint(constr);
            pApplyCmd->Execute();

        }

        // Tests boundary values for numerics
        // Iteration -1 sets range ends to 1 in from boundary values (succeeds)
        // Iteration 0 set range ends to boundary values.            (succeeds)
        // Iteration 1 sets ends back to 1 in from boundaries        (fails)
        // Both Iterations should succeed.

        printf( "Testing boundary values ... \n" );
        for ( idx = -1; idx < 2; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(abs(idx)) );

                val = constrR->GetMaxValue();
                byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(UCHAR_MAX - abs(idx)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DECIMAL_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDecimalValue* decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( (FdoDouble)(-1.797693134862315e308 + (abs(idx) * 1e294)) );

                val = constrR->GetMaxValue();
                decimalVal = (FdoDecimalValue*) val.p;
                decimalVal->SetDecimal( (FdoDouble)(1.797693134862315e308 - (abs(idx) * 1e294)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( (FdoDouble)(-1.797693134862315e308 + (abs(idx) * 1e294)) );

                val = constrR->GetMaxValue();
                doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( (FdoDouble)(1.797693134862315e308 - (abs(idx) * 1e294)) );

                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt16Value* int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(SHRT_MIN + abs(idx)) );
                
                val = constrR->GetMaxValue();
                int16Val = (FdoInt16Value*) val.p;
                int16Val->SetInt16( (FdoInt16)(SHRT_MAX - abs(idx)) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT32_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt32Value* int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( LONG_MIN + abs(idx) );
                
                val = constrR->GetMaxValue();
                int32Val = (FdoInt32Value*) val.p;
                int32Val->SetInt32( LONG_MAX - abs(idx) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT64_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoInt64Value* int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( LLONG_MIN + abs(idx) );
                
                val = constrR->GetMaxValue();
                int64Val = (FdoInt64Value*) val.p;
                int64Val->SetInt64( LLONG_MAX - abs(idx) );
                
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_SINGLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoSingleValue* singleVal = (FdoSingleValue*) val.p;

                singleVal->SetSingle( (FdoFloat)(-3.4028233e38 + (abs(idx) * 1e32)) );

                dataProp->SetValueConstraint(constr);
            }
                
            if ( idx == 1 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Restricting maximum ranges should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DECIMAL_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DecimalRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT32_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int32Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT64_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int64Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_SINGLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.SingleRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }
        }


        pSchemas2 = pDescCmd->Execute();
        pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        pClasses2 = pSchema2->GetClasses();
        pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
        pApplyCmd->SetFeatureSchema( pSchema2 );

        // Tests setting range ends to null (succeeds)
        // Iteration 0 tests not-null to null
        // Iteration 1 tests null to null (no change).

        printf( "Testing null endpoints ... \n" );
        for ( idx = 0; idx < 2; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                val->SetNull();
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                constrR->SetMaxValue((FdoDataValue*) NULL);
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                val->SetNull();
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                constrR->SetMinValue((FdoDataValue*) NULL);
                dataProp->SetValueConstraint(constr);
            }
                
            pApplyCmd->Execute();
        }

        // Tests modifying endpoints from null to not null
        // Iteration 0 fails (class as data)
        // Iteration 1 succeeds (class has no data)
        // Iteration 2 succeeds (this tests restricting a range when class has no data).

        printf( "Testing null to not null plus mods without data ... \n" );
        for ( idx = 0; idx < 3; idx++ ) 
        {
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_BYTE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoByteValue* byteVal = (FdoByteValue*) val.p;
                byteVal->SetByte( (FdoByte)(5 + idx) );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_INT16_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = FdoDataValue::Create( (FdoInt16) (5000 - idx) );
                constrR->SetMaxValue(val);
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DOUBLE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDoubleValue* doubleVal = (FdoDoubleValue*) val.p;
                doubleVal->SetDouble( 20000.12 + idx );
                dataProp->SetValueConstraint(constr);
            }
                
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_STRING_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                FdoPtr<FdoDataValue> val = FdoDataValue::Create( idx > 1 ? L"bbbb" : L"aaaa" );
                constrR->SetMinValue(val);
                dataProp->SetValueConstraint(constr);
            }

            if ( idx == 0 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Change null to not null should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_BYTE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.ByteRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_INT16_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.Int16Range'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DOUBLE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DoubleRange'; property has values and new constraint is more restrictive" ) );
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_STRING_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.StringRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            TestCommonMiscUtil::DeleteObjects( 
                connection,
                SCHEMA_NAME,
                CLASS_NAME_BASE,
                NULL
            );
        }
    }
}

void TestCommonConstraints::DateTimeConstraints( FdoIConnection* connection )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();

    if ( schemaCap->SupportsExclusiveValueRangeConstraints() && schemaCap->SupportsInclusiveValueRangeConstraints())
    {
        FdoPtr<FdoIDescribeSchema>  pDescCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);

        pDescCmd->SetSchemaName( SCHEMA_NAME );
        FdoPtr<FdoFeatureSchemaCollection> pSchemas2 = pDescCmd->Execute();
        FdoPtr<FdoFeatureSchema> pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
        FdoPtr<FdoClassCollection> pClasses2 = pSchema2->GetClasses();
        FdoPtr<FdoClassDefinition> pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
       
        FdoPtr<FdoIApplySchema>  pApplyCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pApplyCmd->SetFeatureSchema(pSchema2);

        FdoPtr<FdoIInsert> insertCmd;
        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        FdoInt32 idx;

        printf( "Testing date value changes ... \n" );
        for ( idx = 0; idx < 13; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.year = 2005;
                    dateTime.day = 15;
                    dateTime.month = 5;
                    dateTime.day = 15;
                    dateTime.hour = 0;
                    dateTime.minute = 2;
                    dateTime.seconds = 1;
                    break;
                case 1:
                    dateTime.year = 2004;
                    dateTime.month = 12;
                    break;
                case 2:
                    dateTime.month = 11;
                    dateTime.day = 30;
                    break;
                case 3:
                    dateTime.day = 29;
                    dateTime.hour = 23;
                    break;
                case 4:
                    dateTime.hour = 22;
                    dateTime.minute = 59;
                    break;
                case 5:
                    dateTime.minute = 58;
                    dateTime.seconds = 59;
                    break;
                case 6:
                    dateTime.seconds = (FdoFloat) 58.99999;
                    break;
                case 7:
                    dateTime.year = 2005;
                    break;
                case 8:
                    dateTime.month = 12;
                    break;
                case 9:
                    dateTime.day = 30;
                    break;
                case 10:
                    dateTime.hour = 23;
                    break;
                case 11:
                    dateTime.minute = 59;
                    break;
                case 12:
                    dateTime.seconds = 59;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( idx > 6 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Date value modification should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );

        }

        printf( "Testing undefined date or time components ... \n" );
        for ( idx = 0; idx < 4; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMinValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.hour = -1;
                    break;
                case 1:
                    dateTime.year = 2003;
                    break;
                case 2:
                    dateTime.year = -1;
                    break;
                case 3:
                    dateTime.hour = 0;
                    dateTime.seconds = (FdoFloat) 0.00001;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( idx > 1 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Date modifications should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );

        }

        TestCommonMiscUtil::DeleteObjects( 
            connection,
            SCHEMA_NAME,
            CLASS_NAME_BASE,
            NULL
        );

        FdoPropertyP prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
        if ( prop ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

            FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
            FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
            FdoPtr<FdoDataValue> val = constrR->GetMinValue();
            FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
            FdoDateTime dateTime;

            dateTime.year = -1;
            dateTime.hour = 3;
            dateTime.minute = 0;
            dateTime.seconds = 0;
            dateVal->SetDateTime(dateTime);

            val = constrR->GetMaxValue();
            dateVal = (FdoDateTimeValue*) val.p;

            dateTime.year = -1;
            dateTime.hour = 15;
            dateTime.minute = 10;
            dateTime.seconds = 15;
            dateVal->SetDateTime(dateTime);

            dataProp->SetValueConstraint(constr);

        }
                
        pApplyCmd->Execute();

        TestCommonMiscUtil::InsertObject(
            connection,
            insertCmd,
            SCHEMA_NAME,
            CLASS_NAME,
            PROP_INT32_R , FdoDataType_Int32,  (FdoInt32) 10,
            PROP_INT32_L,  FdoDataType_Int32,  (FdoInt32) 20,
            PROP_STRING_L,  FdoDataType_String, L"close",
            PROP_UNIQUE1,  FdoDataType_Int32,  (FdoInt32) 5000,
            PROP_STRING_R, FdoDataType_String, L"PA",
            (FdoString*) NULL
        );

        printf( "Testing date value changes ... \n" );
        for ( idx = 0; idx < 9; idx++ ) 
        {
            printf( "   Iteration %d \n", idx );
            prop = FdoPropertiesP(pClass2->GetProperties())->FindItem( PROP_DATE_R );
            if ( prop ) {
                FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*)(prop.p);

                FdoPtr<FdoPropertyValueConstraint> constr = dataProp->GetValueConstraint();
                FdoPropertyValueConstraintRange* constrR = (FdoPropertyValueConstraintRange*) constr.p;
                
                FdoPtr<FdoDataValue> val = constrR->GetMaxValue();
                FdoDateTimeValue* dateVal = (FdoDateTimeValue*) val.p;
                FdoDateTime dateTime = dateVal->GetDateTime();
                switch (idx) {
                case 0:
                    dateTime.hour = 15;
                    dateTime.minute = 10;
                    dateTime.seconds = 15;
                    break;
                case 1:
                    dateTime.hour = 16;
                    dateTime.minute = 5;
                    break;
                case 2:
                    dateTime.minute = 6;
                    dateTime.seconds = 10;
                    break;
                case 3:
                    dateTime.seconds = (FdoFloat) 10.00001;
                    break;
                case 4:
                    dateTime.hour = 15;
                    break;
                case 5:
                    dateTime.minute = 5;
                    break;
                case 6:
                    dateTime.seconds = 10;
                    break;
                case 7:
                    dateTime.hour = -1;
                    break;
                case 8:
                    dateTime.year = 2005;
                    dateTime.month = 5;
                    dateTime.day = 24;
                    break;
                }
                dateVal->SetDateTime(dateTime);

                dataProp->SetValueConstraint(constr);
            }
                
            if ( idx > 3 ) 
            {
                FdoStringP messages;
                try {
                    pApplyCmd->Execute();
                } catch (FdoException *ex) {
                    messages = ex->GetExceptionMessage();
                    ex->Release();
                }
                CPPUNIT_ASSERT_MESSAGE("Time value modification should have failed", messages != L"" );
                
                if ( FdoPropertiesP(pClass2->GetProperties())->IndexOf(PROP_DATE_R) > -1 )
                    CPPUNIT_ASSERT( messages.Contains( L"Cannot modify constraint for property 'constraints:CDataBaseClass.DateRange'; property has values and new constraint is more restrictive" ) );
            }
            else 
            {
                pApplyCmd->Execute();
            }

            pSchemas2 = pDescCmd->Execute();
            pSchema2 = pSchemas2->GetItem( SCHEMA_NAME );
            pClasses2 = pSchema2->GetClasses();
            pClass2 = pClasses2->GetItem( CLASS_NAME_BASE );
            pApplyCmd->SetFeatureSchema( pSchema2 );
        }
    }
}

FdoIConnection* TestCommonConstraints::CreateConnection( FdoBoolean )
{
    throw FdoException::Create( L"base TestCommonConstraints::CreateConnection called, need implementation on subclass" );
}

FdoBoolean TestCommonConstraints::CanRestrictCheckConstraint()
{
    return true;
}

FdoInt32 TestCommonConstraints::GetExpectedCheckConstraintCount( FdoIConnection* connection )
{
    FdoPtr<FdoISchemaCapabilities>    schemaCap = connection->GetSchemaCapabilities();
    FdoInt32                          typeCount;
    FdoInt32                          idx;
    FdoDataType*                      supportedTypes = schemaCap->GetDataTypes( typeCount );
    FdoInt32                          checkCount = 0;

    for ( idx = 0; idx < typeCount; idx++ ) {
        switch ( supportedTypes[idx] ) {
        case FdoDataType_Byte:
            checkCount += 2;
            break;
        
        case FdoDataType_DateTime:
            checkCount += 2;
            break;

        case FdoDataType_Decimal:
            checkCount++;
            break;

        case FdoDataType_Double:
            checkCount += 2;
            break;

        case FdoDataType_Int16:
            checkCount++;
            break;


        case FdoDataType_Int32:
            checkCount += 2;
            break;

        case FdoDataType_Int64:
            checkCount += 2;
            break;

        case FdoDataType_Single:
            checkCount += 2;
            break;

        case FdoDataType_String:
            checkCount += 3;
            break;
        }
    }

    return checkCount;
}

