#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"
#include "Process.h"
#include "UI_structs.h"
#include "DSMemory/DSWinDataBlock.h"

typedef std::map<ULONGLONG, UIComponent> mapUIElements;

#define CHAT_MSG_SIZE			0x1018
#define UI_OPTIONS_BTN_HASH		0x8eabe558c441b3a1

class CUIManager
{
public:
	~CUIManager(void);

	/*
		Get singleton instance
	*/
	static 
	CUIManager& Instance();

	/*
		Enumerate UI elements

		IN:
			bVisibleOnly - enumerate only visible elements

		OUT:
			pOut - found elements

		RETURN:
			Error code

	*/
	DWORD EnumUI(mapUIElements *pOut = NULL, bool bVisibleOnly = false);

	/*
		Get component by hash.
		Call only after EnumUI.

		IN:
			hash - component hash

		OUT:
			void

		RETURN:
			Pointer to found element
			NULL if nothing found

	*/
	UIComponent* GetUIElementByHash(ULONGLONG hash);

	/*
		Get component by name.
		Call only after EnumUI.

		IN:
			name - component name

		OUT:
			void

		RETURN:
			Pointer to found component
			NULL if nothing found

	*/
	UIComponent* GetUIElementByName(std::string name);

	/*
		Get components by text.
		Call only after EnumUI.

		IN:
			name - element text

		OUT:
			out - found components

		RETURN:
			Error code

	*/
	DWORD GetUIElementByText(std::string text, std::map<ULONGLONG, UIComponent> &out);

	/*
		Get component children (only one level deep)

		IN:
			element - component to get children of

		OUT:
			out - found components

		RETURN:
			Error code
	*/
	DWORD GetChildren(UIComponent& element, mapUIElements &out);

	/*
		Simulate component click

		IN:
			element - target component

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD ClickElement(UIComponent& element);

	/*
		Set component text

		IN:
			element - target component
			text - text to set

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SetText(UIComponent& element, std::string text);

	/*
		Set ComboBox selection index

		IN:
			element - target component
			index - index value

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SetCBIndex(UIComponent& element, int index);

	/*
		Set component visibility

		IN:
			element - target component
			bVisible - true to show component, false to hide

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SetVisible(UIComponent& element, bool bVisible = true);

private:
	CUIManager(void){ };
	CUIManager(const CUIManager& root){ };
	CUIManager& operator = (const CUIManager& root){ };

	/*
		ASM code container for ClickElement RemoteThread call

		IN:
			void

		OUT:
			FuncData - pointer to function body
			pdwSize - size of code

		RETURN:
			Error code
	*/
	static 
	DWORD ClickCallWrapper(void** FuncData, DWORD* pdwSize);

	/*
		ASM code container for ClickElement APC

		IN:
			void

		OUT:
			FuncData - pointer to function body
			pdwSize - size of code

		RETURN:
			Error code
	*/
	static 
	DWORD ClickCallWrapperAPC( void** FuncData, DWORD* pdwSize );

private:
	mapUIElements m_Elements;			//Enumerated UI elements
};

#endif//_UI_MANAGER_H_