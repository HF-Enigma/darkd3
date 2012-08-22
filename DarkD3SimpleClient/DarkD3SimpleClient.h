#ifndef _DARK_D3_SIMPLE_CLENT_H_
#define _DARK_D3_SIMPLE_CLENT_H_

#include "stdafx.h"
#include "resource.h"
#include "Routines.h"

using namespace Gdiplus;

class CMainDlg
{
    typedef INT_PTR ( CMainDlg::*PDLGPROC)(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    typedef std::map<UINT, PDLGPROC> mapMsgProc;
    typedef std::map<WORD, PDLGPROC> mapCtrlProc;

public:
	
	~CMainDlg();

    static 
    CMainDlg& Instance();

	DWORD Run();

private:
    CMainDlg();
    CMainDlg(CMainDlg& root){};
    CMainDlg& operator=(CMainDlg&){};

private:
	static 
	INT_PTR CALLBACK DialogProcWrapper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static 
    INT_PTR CALLBACK RenderProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void ShowError(LPTSTR lpszFunction,DWORD code);

	//////////////////////////////////////////////////////////////////////////////
    INT_PTR OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);      //
    INT_PTR OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);   //
    INT_PTR OnClose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);     //
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
    INT_PTR OnFileExit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);  //
    INT_PTR OnBtnLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnToggleMap(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnOpenAH(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnSetQuest(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnEnterGame(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnLogout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnKillMobs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnBuildDB(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    //////////////////////////////////////////////////////////////////////////////
private:
	HWND				m_hMainDlg;
	HWND				m_hMapWnd;
    HWND                m_hStatusBar;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR			gdiplusToken;
    static 	mapMsgProc	Messages;
    mapCtrlProc			Events;
	CGameManager		d3;
};

#endif//_DARK_D3_SIMPLE_CLENT_H_