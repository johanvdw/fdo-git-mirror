// TestMain.cpp : Defines the entry point for the unit test application.
//

#include "stdafx.h"

#ifdef __AFXWIN_H__
#include "HostApp.h"
// CppUnit: MFC TestRunner
#include <cppunit/ui/mfc/TestRunner.h>
extern int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow);
#endif

#include <cppunit/ui/text/TestRunner.h>
// CppUnit: TestFactoryRegistry to retreive the top test suite that contains all registered tests.
#include <cppunit/extensions/TestFactoryRegistry.h>

int _tmain(int argc, _TCHAR* argv[])
{
    // Parameters:
    //    -GUI          Run the tests using the Windows dialog interface
    //    -NoWAIT       When using the text-based interface, do not wait
    //                  for the user to press RETURN before exiting.
    //    <test>*       When using the text based interface, run only the
    //                  specified test(s). Requires support in each tests
    //                  cpp file to add the test name to a registry of the
    //                  same name, e.g.:
    //          CPPUNIT_TEST_SUITE_REGISTRATION (CreateDeleteTests);
    //          CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (CreateDeleteTests, "CreateDeleteTests");
    //
    //  The connect strings values can be passed thru the commandline.
    //  The valid options are
    //    service=<service value>
    //    username=<username value>
    //    password=<password value>
    //    datastore=<datastore value>

    LPSTR cmd_line = GetCommandLine ();

    char *tmpCmdLine = new char[strlen(cmd_line)+1];
    strcpy(tmpCmdLine, cmd_line);
    char *tok = strtok(tmpCmdLine, " ");
    while (tok != NULL)
    {
        char *str;
        if (str = strstr(tok, "="))
            _putenv(tok);
        tok = strtok(NULL, " ");
    }
    bool gui = false;
    strcpy (tmpCmdLine, cmd_line);
    _strupr (tmpCmdLine);
    gui = (NULL != strstr (tmpCmdLine, "-GUI"));
    delete [] tmpCmdLine;

	#ifndef __AFXWIN_H__
        gui = false; // Revert command line requirement to false
	#endif

    if (gui)
	#ifdef __AFXWIN_H__
        return (AfxWinMain (GetModuleHandle (NULL), NULL, cmd_line, SW_SHOW));
	#else
		return 1; // GUI required and MFC not included....
	#endif
    else
    {
        CppUnit::TextUi::TestRunner runner;
        bool wait;
        bool all;
        int ret;

        wait = true;
        all = true;
        if (1 < argc)
        {
            for (int i = 1; i < argc; i++)
                if ('-' == argv[i][0])
                    wait &= (0 != _stricmp (argv[i], "-NoWAIT"));
                else
                {
                    if (!strstr(argv[i], "="))
                    {
                        CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry (argv[i]);
                        runner.addTest (registry.makeTest ());
                        all = false;
                    }
                }
        }
        if (all)
            runner.addTest (CppUnit::TestFactoryRegistry::getRegistry().makeTest ());
        if (runner.run ("", wait))
            ret = 0;
        else
            ret = 1;
	    return (ret);
    }
}

