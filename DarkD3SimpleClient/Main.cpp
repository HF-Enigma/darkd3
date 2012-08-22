#include "stdafx.h"
#include "DarkD3SimpleClient.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    ExitProcess(CMainDlg::Instance().Run());
}