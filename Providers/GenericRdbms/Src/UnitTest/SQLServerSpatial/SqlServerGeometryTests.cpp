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
#include "SqlServerGeometryTests.h"
#include "UnitTestUtil.h"
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerGeometryTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerGeometryTests, "GeometryTests");

void SqlServerGeometryTests::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

int SqlServerGeometryTests::do_rdbi_init ()
{
#ifdef RDBI_STATIC
    return (rdbi_init (&mRdbiContext, (initializer*)odbcdr_rdbi_init));
#else
    return (rdbi_initialize (&mRdbiContext, "SqlServerDriver"));
#endif
}

int SqlServerGeometryTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    FdoStringP odbcConnectString = FdoStringP::Format(
        L"DRIVER={SQL Server Native Client 10.0};MARS_Connection=yes;SERVER=%ls; UID=%hs; PWD=%hs", 
        (FdoString*)(UnitTestUtil::GetEnviron("service")), 
        userName, 
        userPassword
    );
    
    if (mRdbiContext->dispatch.capabilities.supports_unicode == 1)
    {
        return (rdbi_connectW (
            mRdbiContext,
            odbcConnectString,
            L"nm_demo",
            L"test",
            &mId));
    }
    else
    {
        return (rdbi_connect (
            mRdbiContext,
            odbcConnectString,
            "nm_demo",
            "test",
            &mId));
    }
}

char* SqlServerGeometryTests::get_geometry_text (FdoIGeometry *geometry)
{
    const wchar_t *wchars;
    char *chars;
    char *ret;

    wchars = geometry->GetText ();
    wide_to_multibyte (chars, wchars);
    ret = new char[strlen (chars) + 10]; /* includes "''" */
    strcpy (ret, "'");
    strcat (ret, chars);
    strcat (ret, "'");

    return (ret);
}

int SqlServerGeometryTests::do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry )
{
    int     rc;
    // Do bind instead of literals
    if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
        rc  = rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor, L"insert into foo values (?, ?)",
            RDBI_LONG, sizeof(long), featId, RDBI_GEOMETRY, sizeof (geometry), geometry, RDBI_VA_EOL, RDBI_VA_EOL);
    }else{
        rc  = rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor, "insert into foo values (?, ?)",
            RDBI_LONG, sizeof(long), featId, RDBI_GEOMETRY, sizeof (geometry), geometry, RDBI_VA_EOL, RDBI_VA_EOL);
    }
    return rc;
}
