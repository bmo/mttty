
/*-----------------------------------------------------------------------------
    This is a part of the Microsoft Source Code Samples. 
    Copyright (C) 1995 Microsoft Corporation.
    All rights reserved. 
    This source code is only intended as a supplement to 
    Microsoft Development Tools and/or WinHelp documentation.
    See these sources for detailed information regarding the 
    Microsoft samples programs.

    MODULE: MTTTY.h

    PURPOSE: Contains global definitions and variables

-----------------------------------------------------------------------------*/

//
// File: MTTTY.h
//
#include <TCHAR.H>
#include "strsafe.h"
#include "resource.h"
#include "ttyinfo.h"

//
// GLOBAL DEFINES
//
#define TTY_BUFFER_SIZE         MAXROWS * MAXCOLS
#define MAX_STATUS_BUFFER       20000
#define MAX_WRITE_BUFFER        1024
#define MAX_READ_BUFFER         2048
#define READ_TIMEOUT            500
#define STATUS_CHECK_TIMEOUT    500
#define WRITE_CHECK_TIMEOUT     500
#define PURGE_FLAGS             PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR 
#define EVENTFLAGS_DEFAULT      EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD
#define FLAGCHAR_DEFAULT        '\n'

//
// Write request types
//
#define WRITE_CHAR          0x01
#define WRITE_FILE          0x02
#define WRITE_FILESTART     0x03
#define WRITE_FILEEND       0x04
#define WRITE_ABORT         0x05
#define WRITE_BLOCK         0x06

//
// Read states
//
#define RECEIVE_TTY         0x01
#define RECEIVE_CAPTURED    0x02

//
// window coords
//
#define MAXXWINDOW          750
#define MAXYWINDOW          530
#define STARTXWINDOW        80
#define STARTYWINDOW        70

#define SETTINGSFACTOR      6
#define STATUSFACTOR        5

//
// window timer ids
// 
#define TIMERID             1

//
// GLOBAL VARIABLES
//
#ifdef CAN_USE_GETVERSIONINFOEX
OSVERSIONINFO gOSV;
#endif
HINSTANCE     ghInst;
HACCEL        ghAccel;
HWND          ghwndMain;
HWND          ghWndToolbarDlg;
HWND          ghWndStatusDlg;
HWND          ghWndTTY;
HWND          ghWndHidden;

//
// COMMTIMEOUTS is init'd in Init.c
//
extern COMMTIMEOUTS gTimeoutsDefault;

//
//  Window placement variables
//
WORD gwBaseY;
LONG gcyMinimumWindowHeight;

//
//  Flags controlling thread actions
//
HANDLE ghThreadExitEvent;
BOOL gfAbortTransfer;

//
//  File transfer variables
//
DWORD  gdwFileTransferLeft;
DWORD  gdwReceiveState;
HANDLE ghFileCapture;

//
//  Status updating
//
CRITICAL_SECTION gStatusCritical;
HANDLE ghStatusMessageEvent;
HANDLE ghStatusMessageHeap;
HFONT ghFontStatus;
int   gnStatusIndex;


typedef struct STATUS_MESSAGE
{
    struct STATUS_MESSAGE * lpNext;     // pointer to next node
    TCHAR chMessageStart;                // variable length string start here
} STATUS_MESSAGE;

struct STATUS_MESSAGE * glpStatusMessageHead;
struct STATUS_MESSAGE * glpStatusMessageTail;

//
//  Port name
//
TCHAR gszPort[10];

//
//  Writer heap variables
//
CRITICAL_SECTION gcsWriterHeap;
CRITICAL_SECTION gcsDataHeap;
HANDLE ghWriterHeap;
HANDLE ghWriterEvent;
HANDLE ghTransferCompleteEvent;

//
//  Write request data structure; look in Writer.c for more info
//
struct WRITEREQUEST;

struct WRITEREQUEST *gpWriterHead;
struct WRITEREQUEST *gpWriterTail;

typedef struct WRITEREQUEST
{
  DWORD      dwWriteType;        // TCHAR, file start, file abort, file packet
  DWORD      dwSize;             // size of buffer
  TCHAR       ch;                 // ch to send
  TCHAR *     lpBuf;              // address of buffer to send
  HANDLE     hHeap;              // heap containing buffer
  HWND       hWndProgress;       // status bar window handle
  struct WRITEREQUEST *pNext;    // next node in the list
  struct WRITEREQUEST *pPrev;    // prev node in the list
} WRITEREQUEST, *PWRITEREQUEST;


//
// Prototypes of functions called between source files
//

//
//  Error functions
//
void ErrorReporter( TCHAR * szMessage );
void ErrorHandler( TCHAR * szMessage );
void ErrorInComm( TCHAR * szMessage );

//
//  Initialization/deinitialization/settings functions
//
HANDLE SetupCommPort( void );
void ChangeConnection( HWND, BOOL );
BOOL BreakDownCommPort( void );
BOOL UpdateConnection( void );
void GlobalInitialize( void );
void DestroyTTYInfo( void );
void GlobalCleanup( void );
void UpdateTTYInfo( void );
BOOL DisconnectOK( void );
BOOL InitTTYInfo( void );
void InitNewFont( LOGFONT, COLORREF );

//
//  TTY functions
//
void OpenSettingsToolbar( HWND );
void OpenStatusToolbar( HWND );
BOOL CmdAbout( HWND );

//
//  TTY functions
//
BOOL MoveTTYCursor( HWND );
BOOL KillTTYFocus( HWND );
BOOL SetTTYFocus( HWND );
BOOL SizeTTY( HWND, WORD, WORD );

//
//  Thread procedures
//
DWORD WINAPI ReaderAndStatusProc( LPVOID );
DWORD WINAPI WriterProc( LPVOID );

//
//  File transfer functions
//
void CALLBACK TransferRepeatDo( UINT, UINT, DWORD, DWORD, DWORD );  
void TransferRepeatCreate( LPCTSTR, DWORD );
void TransferRepeatDestroy( void );
void TransferFileTextStart( LPCTSTR );
void TransferFileTextEnd( void );
// void TransferFileText( LPCTSTR );
void ReceiveFileText( LPCTSTR );
DWORD GetAFrequency( void );

//
//  Buffer manipulation functions
//
void OutputABufferToWindow( HWND, char *, DWORD );
void OutputABuffer( HWND, char *, DWORD );
BOOL ClearTTYContents( void );

//
//  Status functions
//
HFONT CreateStatusEditFont( void );
void ReportStatusEvent( DWORD ); 
void CheckModemStatus( BOOL );
void ReportCommError( void );
void ReportComStat( COMSTAT );
void StatusMessage( void );
void UpdateStatus( TCHAR * );
void CheckComStat( BOOL );

//
//  Writer heap functions
//
BOOL WriterAddNewNode( DWORD, DWORD, TCHAR, TCHAR *, HANDLE, HWND );
BOOL WriterAddExistingNode( PWRITEREQUEST, DWORD, DWORD, TCHAR, TCHAR *, HANDLE, HWND );
BOOL WriterAddNewNodeTimeout( DWORD, DWORD, TCHAR, TCHAR *, HANDLE, HWND, DWORD );
BOOL WriterAddFirstNodeTimeout( DWORD, DWORD, TCHAR, TCHAR *, HANDLE, HWND, DWORD );

