#ifndef _D3_WINDOW_H_
#define _D3_WINDOW_H_

#include "stdafx.h"

//Mouse click type
typedef enum ClickType
{
	LClick,
	RClick
};

//Static class for interacting with process window
class CWindow
{
public:
	~CWindow(void);

	/*
		Get singleton instance
	*/
	static
	CWindow& Instance();

	/*
		Set working window

		IN:
			hWnd - window handle

		OUT:
			void

		RETURN:
			void
	*/
	void Attach(HWND hWnd);


	/*
		Simulate key press event

		IN:
			dwKeyCode - virtual key code

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD KeyPress(DWORD dwKeyCode);

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
	DWORD Click(POINT pt, ClickType type);

	/*
		Simulate mouse move

		IN:
			pt - new cursor point

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD MouseMove(POINT pt);

private:
	CWindow(void);
	CWindow(const CWindow&) { };
	CWindow& operator = (const CWindow&) { };
		
private:
	HWND m_hWnd;			// Main window handle
};

#endif//_D3_WINDOW_H_