#include "D3Window.h"

CWindow::CWindow(void)
{
}

CWindow::~CWindow(void)
{
}

/*
	Get singleton instance
*/
CWindow& CWindow::Instance()
{
	static CWindow ms_Instanse;

	return ms_Instanse;
}

/*
	Set working window

	IN:
		hWnd - window handle

	OUT:
		void

	RETURN:
		void
*/
void CWindow::Attach( HWND hWnd )
{
	m_hWnd = hWnd;
}

/*
	Simulate key press event

	IN:
		dwKeyCode - virtual key code

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CWindow::KeyPress( DWORD dwKeyCode )
{
	PostMessage(m_hWnd, WM_KEYDOWN, dwKeyCode, 1);
	Sleep(75);
	PostMessage(m_hWnd, WM_KEYUP, dwKeyCode, (1<<31 | 1));

	return ERROR_SUCCESS;
}

/*
	Simulate mouse click

	IN:
		pt - cursor point
		type - click type (LClick, RClick)

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CWindow::Click( POINT pt, ClickType type )
{
	PostMessage(m_hWnd, WM_MOUSEMOVE, NULL, MAKELPARAM(pt.x, pt.y));
	Sleep(100);

	//Left click
	if(type == LClick)
	{		
		PostMessage(m_hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
		Sleep(100);
		PostMessage(m_hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
	}

	//Right click
	else if(type == RClick)
	{
		PostMessage(m_hWnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
		Sleep(100);
		PostMessage(m_hWnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
	}

	return ERROR_SUCCESS;
}

/*
	Simulate mouse move

	IN:
		pt - new cursor point

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CWindow::MouseMove( POINT pt )
{
	PostMessage(m_hWnd, WM_MOUSEMOVE, NULL, MAKELPARAM(pt.x, pt.y));

	return ERROR_SUCCESS;
}
