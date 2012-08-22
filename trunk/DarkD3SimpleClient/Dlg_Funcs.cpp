#include "DarkD3SimpleClient.h"

INT_PTR CMainDlg::OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    DWORD pid = 0;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    m_hMainDlg = hDlg;

    m_hStatusBar = CreateWindow
                    ( 
                        STATUSCLASSNAME,
                        TEXT("No game found"), WS_CHILD | WS_VISIBLE,
                        -100, -100, 10, 10,
                        hDlg, (HMENU)IDC_STATUSBAR, GetModuleHandle(NULL), NULL
                    );

    if((pid = d3.AttachToGame()) > 0)
    {
        TCHAR szText[255];
        
        StringCbPrintf(szText, sizeof(szText), _T("Attached to process %d"), pid);
        SendMessage(m_hStatusBar, SB_SETTEXT, MAKEWORD(0, SBT_NOBORDERS), (LPARAM)szText);
    }

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(Events.find(LOWORD(wParam)) != Events.end())
        return (this->*Events[LOWORD(wParam)])(hDlg,message,wParam,lParam);                
    
    return (INT_PTR)FALSE;
}

INT_PTR CMainDlg::OnClose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    EndDialog(hDlg, 0);
	GdiplusShutdown(gdiplusToken);
    return (INT_PTR)TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////

INT_PTR CMainDlg::OnFileExit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hDlg, 0);

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnBtnLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    char szLogin[255] = {0}, szPass[16] = {0};
    
    GetDlgItemTextA(hDlg, IDC_EDIT_LOGIN, szLogin, sizeof(szLogin));
    GetDlgItemTextA(hDlg, IDC_EDIT_PASS,  szPass,  sizeof(szPass));

    d3.DoLogin(szLogin, szPass);

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnToggleMap(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(!m_hMapWnd)
        m_hMapWnd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_RENDER), NULL, &RenderProc);
        
    if(!IsWindowVisible(m_hMapWnd))
        ShowWindow(m_hMapWnd, SW_SHOW);
    else
        ShowWindow(m_hMapWnd, SW_HIDE);

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnOpenAH(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    vecAHItems items;

    d3.OperateAH(items);

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnSetQuest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    d3.SetQuest();

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnEnterGame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    d3.EnterGame();

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnKillMobs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    d3.PwnMobs(100);

    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnLogout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return (INT_PTR)TRUE;
}

INT_PTR CMainDlg::OnBuildDB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAMEA ofn = {0};          //common dialog box structure
    char szPath[MAX_PATH] = {0};
    BOOL bSucc = FALSE;                 

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szPath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "All (*.*)\0*.*\0CoreData.mpq\0CoreData.mpq\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if(GetOpenFileNameA(&ofn))
        d3.BuildSnoDB(szPath);

    return (INT_PTR)TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK CMainDlg::RenderProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;

    switch(message)
    {
        case WM_INITDIALOG:
                return (INT_PTR)TRUE;
            break;

        case WM_PAINT:
            {
                hdc = BeginPaint(hDlg, &ps);
                Graphics g(hDlg);

				Instance().d3.DrawScenes(hDlg, g);

                EndPaint(hDlg, &ps);
                return (INT_PTR)TRUE;
            }
            break;

		case WM_SIZE:
			{
				RECT rc;

				GetWindowRect(hDlg, &rc);
				InvalidateRect(hDlg, &rc, FALSE);
			}
			break;

        case WM_LBUTTONUP:
            {
                POINT pt = {LOWORD(lParam), HIWORD(lParam)};

                Instance().d3.MoveToWindowPoint(hDlg, pt);
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