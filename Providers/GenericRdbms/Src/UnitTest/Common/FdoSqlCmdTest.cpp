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
#include "FdoSqlCmdTest.h"
#include "UnitTestUtil.h"
#include "FdoRdbmsGetSpatialContexts.h"
#include "FdoRdbmsCreateSpatialContext.h"
#include "FdoRdbmsDestroySpatialContext.h"
#include "FdoRdbmsActivateSpatialContext.h"
#include "FdoRdbmsConnection.h"

#ifdef _DEBUG
#define  DBG(X)    X
#else
#define  DBG(X)
#endif

#define  SQLCMD_TABLE_NAME  L"FdoSqlTest"

FdoSqlCmdTest::FdoSqlCmdTest(void): mConnection(NULL)
{
#ifdef _WIN32
        _putenv("GVC_TRACE_FILE=d:\\geometrytest.txt");
#else
        putenv("GVC_TRACE_FILE=d:\\geometrytest.txt");
#endif
}

FdoSqlCmdTest::~FdoSqlCmdTest(void)
{

}

void FdoSqlCmdTest::setUp ()
{
    set_provider();
    connect();
}

void FdoSqlCmdTest::tearDown ()
{
    if( mConnection != NULL )
    {
        mConnection->Close();
        mConnection->Release();
        mConnection = NULL;
    }
}

void FdoSqlCmdTest::connect ()
{
    try
    {
        mConnection = (FdoIConnection *) UnitTestUtil::GetConnection("", true);

    }
    catch (FdoException *ex)
    {
        ( printf("FDO error: %ls\n", ex->GetExceptionMessage()) );
        if( mConnection != NULL )
        {
            mConnection->Close();
            mConnection->Release();
            mConnection= NULL;
        }
    ex->Release();
        throw;
    }
}

void FdoSqlCmdTest::CreateTable ()
{
    FdoISQLCommand *sqlCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            wchar_t *number_type = get_number_type();
            FdoStringP sql = FdoStringP::Format(L"create table %ls (Name varchar(64) not null, id %ls(10,0), amount %ls(38,8), bool_val %ls(1), date_val %ls )", 
                                SQLCMD_TABLE_NAME, number_type, number_type, number_type, get_datetime_type());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();

            if( sqlCmd)
                sqlCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( sqlCmd)
                sqlCmd->Release();

			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }

}

void FdoSqlCmdTest::BasicInsert ()
{
    FdoISQLCommand *sqlCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            FdoStringP sql = FdoStringP::Format(L"insert into %ls values ('name1',10,2000.3456,1, %ls)", SQLCMD_TABLE_NAME, get_datetime_sys());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();

            // Need to use the following date format: "YYYY-MM-DD-HH24-MI-SS"
            sql = FdoStringP::Format(L"insert into %ls values ('name3',10,2000.3456,1, %ls)", SQLCMD_TABLE_NAME, get_datetime());
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();

            if( sqlCmd)
                sqlCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( sqlCmd)
                sqlCmd->Release();

			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));	
            throw;
        }
    }

}
void FdoSqlCmdTest::BindInsert ()
{
    return;
}

void FdoSqlCmdTest::Delete ()
{
    FdoISQLCommand *sqlCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            FdoStringP sql = FdoStringP::Format(L"delete from %ls where id > 0", SQLCMD_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            int colDeleted = sqlCmd->ExecuteNonQuery();

            DBG(printf("Deleted %d columns\n", colDeleted ));
            if( sqlCmd)
                sqlCmd->Release();
        }
        catch( FdoException *ex )
        {
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }

}

void FdoSqlCmdTest::doGetSC()
{
    FdoRdbmsGetSpatialContexts  *gscCmd = 0;
    FdoISpatialContextReader*   reader = 0;
    int                         n = 0;
    bool        active_only = true;

    DBG(printf("\n.Getting Spatial Contexts (active_only=%s)\n", active_only? "true":"false" ));
    if( mConnection != NULL )
    {
        try
        {
            ///////////////////////////////////////////////////////////////////////////////////////////
            // This will initialize for fetching the SC
            gscCmd = (FdoRdbmsGetSpatialContexts *)mConnection->CreateCommand( FdoCommandType_GetSpatialContexts );

            gscCmd->SetActiveOnly(active_only);
        
            // Get a SC reader
            reader = gscCmd->Execute();

            // Iterate ...
            while ( reader->ReadNext() )
            {
                const wchar_t* sc_name = reader->GetName();
    	        const wchar_t* cs_name = reader->GetCoordinateSystem();
                const wchar_t* descr = reader->GetDescription();
                const wchar_t* cs_wkt = reader->GetCoordinateSystemWkt();

                FdoByteArray*  extent = reader->GetExtent();
                extent->Release();

                FdoSpatialContextExtentType type = reader->GetExtentType();
                bool is_active = reader->IsActive();
     
                if ( true )
                {
                    DBG(printf("\tsc_name=%ls\n\t\t   cs_name=%ls\n\t\t   descr=%ls\n\t\t   type=%d\n\t\t   active=%d\n", 
                             sc_name, cs_name, descr, type, is_active));
                    DBG(printf("\tWKT: '%ls'\n", cs_wkt ));
                }
                n++;
            }
            if (true )
                DBG(printf("\t    retrieved %d SCs\n", n));
        }
        catch (FdoException *ex )
        {
 	        if ( gscCmd != 0 )
                gscCmd->Release();

            if ( reader != 0 )
                reader->Release();   

            printf("doGetSC(): Failed (active=%d).\n", active_only);
		    CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
		    throw;
        }
    }

 	if ( gscCmd != 0 )
        gscCmd->Release();

    if ( reader != 0 )
        reader->Release();   

}

void FdoSqlCmdTest::CreateActivateDestroySC()
{
    FdoPtr<FdoRdbmsCreateSpatialContext>   cscCmd;
    FdoPtr<FdoRdbmsDestroySpatialContext>  dscCmd;
    FdoPtr<FdoRdbmsActivateSpatialContext> ascCmd;

    DBG(printf("\n.Creating/Activating/Destroying Spatial Context\n"));
    if( mConnection != NULL )
    {
        try
        {
            cscCmd = (FdoRdbmsCreateSpatialContext *)mConnection->CreateCommand( FdoCommandType_CreateSpatialContext );

    #pragma message ("ToDo: investigate trancation failure with longer SC names on MySQL.")
            cscCmd->SetName(L"SC_X"); 
            cscCmd->SetDescription(L"For testing only");
            cscCmd->SetCoordinateSystem(L"");

            // envelope as Polygon
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIEnvelope> env = gf->CreateEnvelopeXY(-10000000, -10000000, 10000000, 10000000);
            FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(env); 
            FdoPtr<FdoByteArray> ext = gf->GetFgf( geom );
            cscCmd->SetExtent( ext );
		    cscCmd->SetCoordinateSystemWkt(L"test WKT");

    #pragma message ("ToDo: add CreateSpatialContext test that updates.")

            cscCmd->SetXYTolerance(.0000001);
            cscCmd->SetZTolerance(.001);
            
            cscCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context creation.\n");
            throw ex;
        }

        /////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection("", false);

            ascCmd = (FdoRdbmsActivateSpatialContext *)mConn->CreateCommand( FdoCommandType_ActivateSpatialContext );

            ascCmd->SetName(L"SC_X");
      
            ascCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context activation.\n");
            throw ex;
        }

        /////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection("", false);

            ascCmd = (FdoRdbmsActivateSpatialContext *)mConn->CreateCommand( FdoCommandType_ActivateSpatialContext );

            ascCmd->SetName(L"SC_X");
      
            ascCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context activation.\n");
            throw ex;
        }

        //////////////////////////////////

        try
        {
            FdoPtr<FdoIConnection> mConn = UnitTestUtil::GetConnection("", false);

            dscCmd = (FdoRdbmsDestroySpatialContext *)mConn->CreateCommand( FdoCommandType_DestroySpatialContext );

            dscCmd->SetName(L"SC_X");
      
            dscCmd->Execute();
        }
        catch (FdoException *ex )
        {
            printf("CreateActivateDestroySC(): Failed spatial context desctruction.\n");
            throw ex;
        }
    }
}

void FdoSqlCmdTest::DropTable ()
{
    FdoISQLCommand *sqlCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"drop table %ls", SQLCMD_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();

            if( sqlCmd)
                sqlCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( sqlCmd)
                sqlCmd->Release();

			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
			throw;
        }
    }

}


void FdoSqlCmdTest::ReadQueryResult( FdoISQLDataReader *myReader )
{
    try
    {
        for( int i = 0; i<myReader->GetColumnCount(); i++ )
        {
            const wchar_t * name = myReader->GetColumnName( i);
            if(  myReader->IsNull( name ) )
            {
                DBG(printf("Null value  "));
                continue;
            }
            switch( myReader->GetColumnType( name ) )
            {
                case FdoDataType_Boolean:
                        DBG(printf("%s ",myReader->GetBoolean( name )?"True":"False"));
                    break;

                case FdoDataType_Byte:
                        DBG(printf("%x ",myReader->GetByte( name ) ));
                    break;

                case FdoDataType_DateTime:
                    {
                     FdoDateTime  time = myReader->GetDateTime( name );
                     DBG(printf("%s ", ctime((const time_t*)&time )));
                    }
                    break;

                case FdoDataType_Decimal:
                    DBG(printf("%g ",myReader->GetDouble( name ) ));
                    break;

                case FdoDataType_Double:
                    DBG(printf("%g ",myReader->GetDouble( name ) ));
                    break;


                case FdoDataType_Int16:
                    DBG(printf("%d ",myReader->GetInt16( name ) ));
                    break;


                case FdoDataType_Int32:
                    DBG(printf("%d ",myReader->GetInt32( name ) ));
                    break;


                case FdoDataType_Int64:
                    DBG(printf("%d ",myReader->GetInt64( name ) ));
                    break;


                case FdoDataType_Single:
                    DBG(printf("%f ",myReader->GetSingle( name ) ));
                    break;


                case FdoDataType_String:
                    DBG(printf("%ls ",myReader->GetString( name ) ));
                    break;


                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                    throw "Not supported";
                    break;


                default:
                    throw "Unknown fdo data type";
            }
        }
    }
    catch( FdoException *ex )
    {
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
		throw;
    }
    printf("\n");
}

void FdoSqlCmdTest::Query ()
{
    FdoISQLDataReader *myReader = NULL;
    FdoISQLCommand *selCmd = NULL;

    if( mConnection != NULL )
    {
        try
        {
            FdoISQLCommand *selCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"select * from %ls", SQLCMD_TABLE_NAME);
            selCmd->SetSQLStatement( (FdoString *)sql );
            myReader = selCmd->ExecuteReader();
            selCmd->Release();
            selCmd = NULL;
            if( myReader != NULL  )
            {
                for( int i = 0; i<myReader->GetColumnCount(); i++ )
                {
                    const wchar_t * name = myReader->GetColumnName( i);
                    DBG(printf("%ls(%s) ", name, FdoTypeToStr(myReader->GetColumnType( name ) ) ));
                }
                DBG(printf("\n"));
                while ( myReader->ReadNext() )
                {
                    ReadQueryResult( myReader );
                }
                myReader->Release();
                myReader = NULL;
            }

            if( selCmd)
                selCmd->Release();
            selCmd = NULL;
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            if( selCmd)
                selCmd->Release();

			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }

}

void FdoSqlCmdTest::QueryGeometry()
{
    FdoPtr<FdoISQLDataReader>myReader;
    FdoPtr<FdoISQLCommand>selCmd;

    if( mConnection != NULL )
    {
        try
        {
            FdoISQLCommand *selCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            selCmd->SetSQLStatement(L"select * from acdb3dpolyline");
            myReader = selCmd->ExecuteReader();

            FdoStringP  geomColName = L"Geometry";

            selCmd->SetSQLStatement(L"select Geometry from acdb3dpolyline");

            myReader = selCmd->ExecuteReader();
            if( myReader != NULL  )
            {
                for( int i = 0; i<myReader->GetColumnCount(); i++ )
                {
                    const wchar_t * name = myReader->GetColumnName( i);
                    if( myReader->GetPropertyType( name ) != FdoPropertyType_GeometricProperty )
                    {
                        printf("Geometric property is Expected and got something else (it must be a data property)\n" );
                    }
                }
                int count = 0;
                while ( myReader->ReadNext() && count++ < 5 )
                {
                    if ( !myReader->IsNull( geomColName) )
                    {
                        FdoByteArray* byteArray = myReader->GetGeometry( geomColName );

                        DBG(printf(" \tGeometry byte array size: %d\n", byteArray->GetCount()));
                        byteArray->Release();
                    }
                }
            }
        }
        catch( FdoException *ex )
        {
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }

}

char* FdoSqlCmdTest::FdoTypeToStr( FdoDataType type )
{
    switch( type )
    {
        case FdoDataType_Boolean:
            return "Boolean";

        case FdoDataType_Byte:
            return "Byte";

        case FdoDataType_DateTime:
            return "DateTime";

        case FdoDataType_Decimal:
            return "Decimal";

        case FdoDataType_Double:
            return "Double";

        case FdoDataType_Int16:
            return "Int16";

        case FdoDataType_Int32:
            return "Int32";

        case FdoDataType_Int64:
            return "Int64";

        case FdoDataType_Single:
            return "Single";

        case FdoDataType_String:
            return "String";

        case FdoDataType_BLOB:
            return "BLOB";

        case FdoDataType_CLOB:
            return "CLOB";

        default:
            return "Unknown fdo data type"; // pass type as parameter
    }
}
