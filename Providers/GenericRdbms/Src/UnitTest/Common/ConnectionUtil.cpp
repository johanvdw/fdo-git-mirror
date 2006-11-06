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
#include "ConnectionUtil.h"

extern    void ut_thread_mutex_init ();
extern    void ut_thread_mutex_destroy ();

/// <summary>This class initializes the mutecies for nls_msg_get.</summary>
/// <remark>It is only needed if this initialization is not done in the DLL initialization code.</remark>
class FdoMsgInitializerSingleton
{
public:
    FdoMsgInitializerSingleton ()
    {
        ut_thread_mutex_init ();
    }

    ~FdoMsgInitializerSingleton ()
    {
        ut_thread_mutex_destroy ();
    }
};

static FdoMsgInitializerSingleton Singleton;

StaticConnection::StaticConnection (void) :
    m_rdbi_context(NULL),
    m_gdbi_conn(NULL)
{
    mDatastore[0] = 0;
}

StaticConnection::~StaticConnection (void)
{
    disconnect();
}

void StaticConnection::SetSchema ( char* suffix ) 
{
    sprintf( mDatastore, "%s%s", UnitTestUtil::GetEnviron( "datastore" ), suffix );
    FdoStringP strDatastore = mDatastore;
    if (m_rdbi_context->dispatch.capabilities.supports_unicode == 1)
        ::rdbi_set_schemaW( m_rdbi_context, strDatastore );
    else
        ::rdbi_set_schema( m_rdbi_context, strDatastore );
}

int StaticConnection::do_rdbi_init (rdbi_context_def** rdbi_context)
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int StaticConnection::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

void StaticConnection::init ()
{
    rdbi_context_def* rdbi_context;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::connect ()
{
    int id;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&m_rdbi_context));
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (m_rdbi_context, id));
    
            m_gdbi_conn = new GdbiConnection(m_rdbi_context);

        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&m_rdbi_context);
            throw exception;
        }
    }
    catch (FdoException* e ) 
    {
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::disconnect ()
{
    try
    {
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }

        if ( m_rdbi_context ) {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (m_rdbi_context));
            CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&m_rdbi_context));
        }

    }
    catch (FdoException* e ) 
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        UnitTestUtil::FailOnException(e);
    }
    catch (CppUnit::Exception exception)
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        throw exception;
    }
    catch (...)
    {
        if ( m_rdbi_context ) {
            delete m_rdbi_context;
            m_rdbi_context = NULL;
        }
        if ( m_gdbi_conn ) {
            delete m_gdbi_conn;
            m_gdbi_conn = NULL;
        }
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::name ()
{
    rdbi_context_def *rdbi_context;
    char* name;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        name = rdbi_vndr_name (rdbi_context);
        printf ("%s\n", name);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void StaticConnection::info ()
{
    rdbi_context_def *rdbi_context;
    int id;
    rdbi_vndr_info_def info;

    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init (&rdbi_context));
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (rdbi_context, id));
            CPPUNIT_ASSERT_MESSAGE ("rdbi_vndr_info failed", RDBI_SUCCESS == rdbi_vndr_info (rdbi_context, &info));
            printf ("%s version %ld, precision(%ld,%ld), scale(%ld,%ld), string(%ld,%ld)\n",
                info.name, info.dbversion,
                info.minPrecision, /* Minimum allowed precision on numeric columns */
                info.maxPrecision, /* Maximum allowed precision on numeric columns */
                info.minScale, /* Minimum allowed scale on numeric columns */
                info.maxScale, /* Maximum allowed scale on numeric columns */
                info.minStrLen, /* Minimum length for string columns */
                info.maxStrLen); /* Maximum length for string columns */
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (rdbi_context));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&rdbi_context);
            throw exception;
        }
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&rdbi_context));
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

ConnectionUtil::ConnectionUtil(void)
{
	m_SetupDone = false;
}

ConnectionUtil::~ConnectionUtil(void)
{

}

wchar_t *ConnectionUtil::GetConnectionString(StringConnTypeRequest pTypeReq, const char *suffix)
{
    char *service = UnitTestUtil::GetEnviron("service");
    char *username = UnitTestUtil::GetEnviron("username");
    char *password = UnitTestUtil::GetEnviron("password");
    char *datastore = UnitTestUtil::GetEnviron("datastore", suffix);
	
	static wchar_t connectString[200];
	connectString[0] = L'\0';

    if (Connection_WithDatastore == pTypeReq)
		swprintf( connectString, sizeof(connectString)/sizeof(wchar_t), L"service=%hs;username=%hs;password=%hs;datastore=%hs", service, username, password, datastore);
	else
		swprintf( connectString, sizeof(connectString)/sizeof(wchar_t), L"service=%hs;username=%hs;password=%hs;", service, username, password);
	return connectString;
}

const char* ConnectionUtil::GetEnviron(const char *name, const char *suffix)
{
    static char    uname [1024];
    int   size=1024;

    UnitTestUtil::GetRealUserName(uname, size);

    if (_stricmp(name, "service") == 0)
    {
        return getenv("service");
    }
    else if (_stricmp(name, "username") == 0)
    {
        return getenv("username");
    }
    else if (_stricmp(name, "password") == 0)
    {
        return getenv("password");
    }
    else if (_stricmp(name, "datastore") == 0)
    {
        static char source[128];
        char *datastore = getenv("datastore");
        if (datastore == NULL)
            sprintf(source, "fdo_%s", uname);
        else
            strcpy(source, datastore);
        strcat(source, suffix);
		if (GetSuffixTest() != NULL)
			strcat(source, GetSuffixTest());
        return source;
    }
    else if (_stricmp(name, "provider") == 0)
    {
        static char provider[128];
        char *prov = getenv("provider");
        if (prov != NULL )
            strcpy(provider, prov);
        return provider;
    }
    else if (_stricmp(name, "dsnname") == 0)
    {
        return getenv("dsnname");
    }
    return NULL;
}
