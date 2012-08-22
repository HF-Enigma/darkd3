#include "stdafx.h"
#include "DarkD3SimpleClient.h"

CMainDlg::mapMsgProc CMainDlg::Messages;

CMainDlg::CMainDlg():
    m_hMainDlg(NULL),
    m_hStatusBar(NULL),
    m_hMapWnd(NULL)
{
	Messages[WM_INITDIALOG]     =   &CMainDlg::OnInit;
	Messages[WM_COMMAND]        =   &CMainDlg::OnCommand;
	Messages[WM_CLOSE]          =   &CMainDlg::OnClose;

    Events[IDC_BUTTON_LOGIN]    =   &CMainDlg::OnBtnLogin;

	Events[ID_FILE_EXIT]        =   &CMainDlg::OnFileExit;

    Events[ID_TOOLS_OPENAH]     =   &CMainDlg::OnOpenAH;
    Events[ID_TOOLS_SETQUEST]   =   &CMainDlg::OnSetQuest;
    Events[ID_TOOLS_ENTERGAME]  =   &CMainDlg::OnEnterGame;
    Events[ID_TOOLS_KILLMOBS]   =   &CMainDlg::OnKillMobs;
    Events[ID_TOOLS_LOGOUT]     =   &CMainDlg::OnLogout;

    Events[ID_TOOLS_BUILDDB]    =   &CMainDlg::OnBuildDB;
    Events[ID_TOOLS_TOGGLEMAP]  =   &CMainDlg::OnToggleMap;
}

CMainDlg::~CMainDlg()
{
}

CMainDlg& CMainDlg::Instance() 
{
    static CMainDlg ms_Instanse;

    return ms_Instanse;
}

DWORD CMainDlg::Run()
{
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAIN), NULL, CMainDlg::DialogProcWrapper);
}

INT_PTR CALLBACK CMainDlg::DialogProcWrapper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(Messages.find(message) != Messages.end())
        return (Instance().*Messages[message])(hDlg, message, wParam, lParam); 

    return 0;
}

INT_PTR CALLBACK CMainDlg::RenderProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
		case WM_INITDIALOG:
			{
				return (INT_PTR)TRUE;
			}
			break;

		case WM_PAINT:
			{
				static HDC hdc;
				PAINTSTRUCT ps;

				hdc = BeginPaint(hDlg, &ps);

				Graphics g(hdc);

				//Draw scenes if any
				Instance().d3.DrawScenes(hDlg, g);

				EndPaint(hDlg, &ps);

				return (INT_PTR)TRUE;
			}
			break;

		case WM_CLOSE:
			{
				EndDialog(hDlg, 0);
				Instance().m_hMapWnd = NULL;
				return (INT_PTR)TRUE;	  
			}
			break;

		default:
			break;
	}

	return 0;
}


void CMainDlg::ShowError(LPTSTR lpszFunction,DWORD code) 
{ 
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf = NULL;
	LPVOID lpDisplayBuf = NULL;

	FormatMessage
        (
		    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		    FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    code,
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		    (LPTSTR) &lpMsgBuf,
		    0, NULL 
        );

	// Display the error message and exit the process
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(_tcslen((LPCTSTR)lpMsgBuf) + _tcslen(lpszFunction) + 40) * sizeof(TCHAR)); 

	StringCbPrintf
        (
            (LPTSTR)lpDisplayBuf, 
		    LocalSize(lpDisplayBuf),
		    TEXT("%s failed with error %d: %s"), 
		    lpszFunction, code, lpMsgBuf
        ); 

	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), 0x10L); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}