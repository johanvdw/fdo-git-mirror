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
#include "SqlServerFdoImportExportTest.h"
#include "UnitTestUtil.h"

//TODO: not ready yet: CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoImportExportTest );
#pragma message ("TODO: not ready yet: CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoImportExportTest );")
//TODO: not ready yet: CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoImportExportTest, "FdoImportExportTest");
#pragma message ("TODO: not ready yet: CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoImportExportTest, \"FdoImportExportTest\");")

void SqlServerFdoImportExportTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoImportExportTest::AddMapping( FdoXmlWriter* writer, FdoString* name, FdoInt32 providerVersion )
{
    _addMapping( writer, name, 
        (providerVersion == 3) ? L"OSGeo.SQLServerSpatial.3.7" :
        ((providerVersion == 2) ? L"OSGeo.SQLServerSpatial.2.1" : L"Autodesk.SQLServer.1.9"),
        L"http://www.autodesk.com/isd/fdo/SQLServerSpatialProvider" );
}

FdoStringP SqlServerFdoImportExportTest::OverrideBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile);

    _overrideBend( inFile, outFile,
        L"OSGeo.SQLServer.3.7",
        L"http://www.autodesk.com/isd/fdo/SQLServerSpatialProvider" );

    return outFile;
}
