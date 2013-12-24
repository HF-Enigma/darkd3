#include "stdafx.h"
#include "DarkD3SimpleClient.h"

#define D3D_DEBUG_INFO


#pragma comment(lib, "d3d9.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    return CMainDlg::Instance().Run();
}