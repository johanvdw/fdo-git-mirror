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
 */

#ifndef CONNECTTESTS_H
#define CONNECTTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class ConnectTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (ConnectTests);
    CPPUNIT_TEST (connect);
    CPPUNIT_TEST (connect2);
    CPPUNIT_TEST (connect3);
    CPPUNIT_TEST (connect4);
    CPPUNIT_TEST (dual_connect);
    CPPUNIT_TEST (connect_no_slash);
    CPPUNIT_TEST (connect_back_slash);
#ifdef ADSK_FDO
    CPPUNIT_TEST (connect_double_slash);
#endif
    CPPUNIT_TEST (connect_wrong_params);
    CPPUNIT_TEST (connect_wrong_params2);
    CPPUNIT_TEST (connect_params1);
    CPPUNIT_TEST (connect_params2);
    CPPUNIT_TEST (connect_params3);
    CPPUNIT_TEST (connect_params4);
    CPPUNIT_TEST (connect_params5);
    CPPUNIT_TEST (connect_params6);
    CPPUNIT_TEST (connect_params7);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    ConnectTests (void);
    virtual ~ConnectTests (void);
    void setUp ();
    void tearDown ();

protected:
    void connect ();
    void connect2 ();
    void connect3 ();
    void connect4 ();
    void dual_connect ();
    void connect_no_slash ();
    void connect_back_slash ();
    void connect_double_slash ();
    void connect_wrong_params ();
    void connect_wrong_params2 ();
    void connect_params1 ();
    void connect_params2 ();
    void connect_params3 ();
    void connect_params4 ();
    void connect_params5 ();
    void connect_params6 ();
    void connect_params7 ();
};

#endif // CONNECTTESTS_H

