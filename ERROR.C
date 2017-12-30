
/*-----------------------------------------------------------------------------

    This is a part of the Microsoft Source Code Samples. 
    Copyright (C) 1995 Microsoft Corporation.
    All rights reserved. 
    This source code is only intended as a supplement to 
    Microsoft Development Tools and/or WinHelp documentation.
    See these sources for detailed information regarding the 
    Microsoft samples programs.

    MODULE: Error.c

    PURPOSE: Implement error handling functions 
             called to report errors.

    FUNCTIONS:
        ErrorExtender - Calls FormatMessage to translate error code to
                        error text
        ErrorReporter - Reports errors to user
        ErrorHandler  - Reports errors, then exits the process
        ErrorInComm   - Reports errors, closes comm connection, then exits

-----------------------------------------------------------------------------*/

#include <windows.h>
#include "mttty.h"

/*
    Prototypes of functions called only in this module
*/
DWORD ErrorExtender(DWORD, TCHAR **);


/*-----------------------------------------------------------------------------

FUNCTION: ErrorExtender(DWORD, char **)

PURPOSE: Translates error codes into error strings

PARAMETERS:
    dwError  - error code to be translated
    szBuffer - pointer to error string buffer

COMMENTS: If code can't be translated, then a 1 byte NULL string
          created.  The buffer, whether created by FormatMessage, or
          directly is supposed to be freed by the caller.

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
DWORD ErrorExtender(DWORD dwError, TCHAR ** szBuffer)
{
    DWORD dwRes = 0;

    dwRes = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | 80 ,
                          NULL, dwError,
                          MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                          (LPTSTR) szBuffer, 0, NULL);

    if (dwRes == 0) {
        *szBuffer = LocalAlloc(LPTR, sizeof(TCHAR));
        return 1;
    }

    return dwRes;
}


/*-----------------------------------------------------------------------------

FUNCTION: ErrorReporter(char *)

PURPOSE: Report error to user

PARAMETERS:
    szMessage - Error message from app

COMMENTS: Reports error string in console and in debugger

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void ErrorReporter(TCHAR * szMessage)
{
    TCHAR * szFormat = _T("Error %d: %s.\n\r%s\r\n");    // format for wsprintf
    TCHAR * szExtended;      // error string translated from error code
    TCHAR * szFinal;         // final string to report
    DWORD dwExtSize;
    DWORD dwErr;

    dwErr = GetLastError();

    /*
        Get error string from system
    */
    dwExtSize = ErrorExtender(dwErr, &szExtended);
    
    /*
        allocate buffer for error string from system, passed in string
        and extra stuff from the szFormat string
    */
	int ilenFinal = _tcslen(szMessage) + dwExtSize  + 30 ;
	szFinal = (TCHAR *)LocalAlloc(LPTR, ilenFinal * sizeof(TCHAR));

    if (szFinal == NULL)	// if no final buffer, then can't format error
        MessageBox(ghwndMain, _T("Cannot properly report error."), _T("Fatal Error"), MB_OK);
    else {	
		//int nwritten = wsprintf(szFinal, szFormat, dwErr, szMessage, szExtended);
		int nwritten = StringCchPrintf(szFinal, ilenFinal, szFormat, dwErr, szMessage, szExtended);

        OutputDebugString(szFinal);

        if (DISPLAYERRORS(TTYInfo))
            MessageBox(ghwndMain, szFinal, NULL, MB_OK);

        LocalFree(szFinal);                  // free final buffer
    }

    /*
        free extended string buffer
    */
    LocalFree(szExtended);

    return;
}


/*-----------------------------------------------------------------------------

FUNCTION: ErrorHandler( char * )

PURPOSE: Handle a fatal error (before comm port is opened)

PARAMETERS:
    szMessage - Error message from app

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void ErrorHandler(TCHAR * szMessage)
{	
    ErrorReporter(szMessage);
    ExitProcess(0);
}


/*-----------------------------------------------------------------------------

FUNCTION: ErrorInComm( char * )

PURPOSE: Handle a fatal error after comm port is opened

PARAMETERS:
    szMessage - Error message from app

HISTORY:   Date:      Author:     Comment:
           10/27/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void ErrorInComm(TCHAR * szMessage)
{
    ErrorReporter(szMessage);
    BreakDownCommPort();
    ExitProcess(0);
}
