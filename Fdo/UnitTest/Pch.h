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

#ifdef _WIN32
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib> 
#include <cstdio>

#include <Fdo.h>

// NOTE: Define __CPPUNIT_MFC_APP in order to have the CPPUNIT UI appear... 
#ifdef __CPPUNIT_MFC_APP
    #ifdef WIN32
	    #ifdef _DEBUG
		    #pragma comment(lib, "testrunnerd.lib")
	    #else
		    #pragma comment(lib, "testrunner.lib")
	    #endif
    #endif
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

// This behaves like CPPUNIT_ASSERT, but you can trace 
// into it easily using the debugger.
#define FDO_CPPUNIT_ASSERT(condition)                      \
  if (!(condition)) ::CppUnit::Asserter::failIf( true,   \
                                 (#condition),             \
                                 CPPUNIT_SOURCELINE() )


#ifdef CPPUNIT_MODERN_API // Newer API

// This works with the next macro to wrap each unit test so that any 
// uncaught FdoExceptions are caught and reported.
#define FDO_CPPUNIT_DEFINE( testMethod )\
    void UNITTEST_##testMethod()\
    {\
        try\
        {\
            TestFixtureType::testMethod();\
        }\
        catch (FdoException* exception)\
        {\
            PrintException(exception);\
            exception->Release();\
            CPPUNIT_ASSERT_MESSAGE("Unhandled FdoException in " #testMethod, false);\
        }\
    }\

// This is a modification of CPPUNIT_TEST
#undef  CPPUNIT_TEST
#define CPPUNIT_TEST( testMethod )\
    CPPUNIT_TEST_SUITE_ADD_TEST(\
        ( new CPPUNIT_NS::TestCaller<TestFixtureType>( \
              context.getTestNameFor( #testMethod ),\
              &TestFixtureType::UNITTEST_##testMethod,\
              context.makeFixture() ) ) )

#else //CPPUNIT_MODERN_API

// This works with the next macro to wrap each unit test so that any 
// uncaught FdoExceptions are caught and reported.
#define FDO_CPPUNIT_DEFINE( testMethod )\
    void UNITTEST_##testMethod()\
    {\
        try\
        {\
            __ThisTestFixtureType::testMethod();\
        }\
        catch (FdoException* exception)\
        {\
            PrintException(exception);\
            exception->Release();\
            CPPUNIT_ASSERT_MESSAGE("Unhandled FdoException in " #testMethod, false);\
        }\
    }\

// This is a modification of CPPUNIT_TEST
#undef  CPPUNIT_TEST
#define CPPUNIT_TEST( testMethod )\
    builder.addTestCaller( #testMethod,\
        &__ThisTestFixtureType::UNITTEST_##testMethod,\
        (__ThisTestFixtureType*)factory->makeFixture() ) 

#endif // CPPUNIT_MODERN_API

// This is a method to dump the messages from an exception and all its parents.
void PrintException(FdoException* exception);


