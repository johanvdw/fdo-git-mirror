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

#ifndef POSTGIS_FDOSPATIALCONTEXTTEST_H
#define POSTGIS_FDOSPATIALCONTEXTTEST_H

#include "FdoSpatialContextTest.h"

class PostGisFdoSpatialContextTest : public FdoSpatialContextTest
{
    CPPUNIT_TEST_SUB_SUITE (PostGisFdoSpatialContextTest, FdoSpatialContextTest);
    CPPUNIT_TEST( testAxes );
    CPPUNIT_TEST_SUITE_END ();

protected:
    virtual void set_provider();

    virtual void testAxes();

    //TODO: port these tests to PostGIS
    virtual void testMeta () {};
    virtual void testNoMeta() {};

    // Unit test methods:

};

#endif // PostGis_FDOSPATIALCONTEXTTEST_H
