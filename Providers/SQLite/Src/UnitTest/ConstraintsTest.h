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

#ifndef CONSTRAINTSTEST_H
#define CONSTRAINTSTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo/Expression/DataValueCollection.h>
#include <Fdo.h>
#include <TestCommonConstraints.h>

#define DBG(x)        //x
#define DBG_MAX(x)        //x

class ConstraintsTest : TestCommonConstraints
{
    CPPUNIT_TEST_SUB_SUITE( ConstraintsTest, TestCommonConstraints );
    CPPUNIT_TEST_SUITE_END();

public:
    ConstraintsTest(void);
    virtual ~ConstraintsTest(void);

    virtual FdoString* GetDefaultSchemaName(void);

protected:
    virtual void CreateConnection( Context& context, FdoBoolean recreateDb = false );
    virtual FdoBoolean FdoValidatesData();

    // till we fix and issue we add this function...
    virtual FdoDouble GetDoubleRounding(FdoDataType dataType);
    virtual FdoBoolean CanRestrictCheckConstraint();

    // TODO - handle following unit tests 
    // these tests needs to be changes since provider not supports inheritence
    virtual void TestDescribeConstraints(void) {};
    virtual void TestUpdateUniqueConstraints(void) {};
    virtual void TestUpdateCheckConstraints(void) {};
    virtual void TestDescribeUpdatedConstraints(void) {};
    virtual void TestRestrictCheckConstraints(void) {};
    virtual void TestCheckConstraintsData(void) {};
    virtual void TestDateTimeConstraints(void) {};

    // TODO - Fix UnitTests!
    // HACK: Disable hanging unit test on linux 64bit until further investigation can determine exact cause -- EXPRESSION_21_VALUETRUNCATED?
#ifndef _WIN32
    virtual void TestBaseReferences(void) {};
#endif

};

#endif    //CONSTRAINTSTEST_H
