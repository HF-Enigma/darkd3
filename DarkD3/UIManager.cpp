#include "UIManager.h"


CUIManager::~CUIManager(void)
{
}

/*
	Get singleton instance
*/
CUIManager& CUIManager::Instance()
{
	static CUIManager ms_Instance;

	return ms_Instance;
}

/*
	Enumerate UI elements

	IN:
		bVisibleOnly - enumerate only visible elements

	OUT:
		pOut - found elements

	RETURN:
		Error code

*/
DWORD CUIManager::EnumUI( mapUIElements *pOut /*= NULL*/, bool bVisibleOnly /*= false*/ )
{
	CHK_RES(EnumUIElements(pOut, bVisibleOnly));
	//CHK_RES(EnumUIHandlers(NULL));

	return ERROR_SUCCESS;
}

DWORD CUIManager::EnumUIElements( mapUIElements *pOut /*= NULL*/, bool bVisibleOnly /*= false*/ )
{
	UIManager uimgr;
	UIComponentMap comp_map;

	m_Elements.clear();

	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.ui_mgr, sizeof(uimgr), &uimgr));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)uimgr.component_map, sizeof(comp_map), &comp_map));

	for(DWORD i = 0; i <= comp_map.mask; i++)
	{
		UIComponentMap::Pair pair;

        DWORD tmp = CProcess::Instance().Core.Read<DWORD>((DWORD)comp_map.table + i*sizeof(UIComponentMap::Pair*));
		if(tmp == 0 || CProcess::Instance().Core.Read(tmp, sizeof(pair), &pair) != ERROR_SUCCESS)
			continue;

		//Read linked list nodes
		for(; pair.value;)
		{
			UIComponent element;

			CProcess::Instance().Core.Read((DWORD)pair.value, sizeof(element), &element); 

			element.dwBaseAddr = (DWORD)pair.value;

			if(!bVisibleOnly || (bVisibleOnly && element.visible) )
			{	
				//if(element.click_handler != 0)
				m_Elements[element.self.hash] = element;
			}

			if(CProcess::Instance().Core.Read((DWORD)pair.next, sizeof(pair), &pair) != ERROR_SUCCESS)
				break;
		}
	}

	if(pOut)
		*pOut = m_Elements;

	return ERROR_SUCCESS;
}


DWORD CUIManager::EnumUIHandlers( mapUIHandlers *pOut /*= NULL*/ )
{
	UIManager uimgr;
	UIHandlerMap handler_map;

	m_Handlers.clear();

	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.ui_mgr, sizeof(uimgr), &uimgr));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)uimgr.handler_map, sizeof(handler_map), &handler_map));

	for(DWORD i = 0; i < 0x100; i++)
	{
		UIHandlerMap::Pair pair;

		if(CProcess::Instance().Core.Read(CProcess::Instance().Core.Read<DWORD>((DWORD)handler_map.table +  i*sizeof(UIHandlerMap::Pair*)), sizeof(pair), &pair) != ERROR_SUCCESS)
			continue;

		//Read linked list nodes
		for(; pair.value;)
		{
			UIHandler handler;

			CProcess::Instance().Core.Read((DWORD)pair.value, sizeof(handler), &handler); 

			m_Handlers[handler.hash] = handler;

			if(CProcess::Instance().Core.Read((DWORD)pair.next, sizeof(pair), &pair) != ERROR_SUCCESS)
				break;
		}
	}

	if(pOut)
		*pOut = m_Handlers;

	return ERROR_SUCCESS;
}



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
UIComponent* CUIManager::GetUIElementByHash(ULONGLONG hash)
{
	if(m_Elements.find(hash) != m_Elements.end())
		return &m_Elements[hash];
	else
		 return NULL;
}

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
UIComponent* CUIManager::GetUIElementByName(std::string name)
{
	for(mapUIElements::iterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++)
	{
		if(strcmp(iter->second.self.name, name.c_str()) == 0)
			return &iter->second;
	}
	
	return NULL;
}

/*
	DEBUG
	Get UI element with name containing string
*/
DWORD CUIManager::NameMatch( std::string name, mapUIElements &out )
{
	out.clear();

	for(mapUIElements::iterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++)
	{
		std::string current = iter->second.self.name;

		if( current.find(name.c_str()) != current.npos)
		{
			out[iter->second.self.hash] = iter->second;

			OutputDebugStringA(iter->second.self.name);
			OutputDebugStringA("\r\n");
		}
	}

	return NULL;
}

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
DWORD CUIManager::GetUIElementByText( std::string text, std::map<ULONGLONG, UIComponent> &out )
{
	out.clear();

	for(mapUIElements::iterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++)
	{
		char data[128];
		std::string strText;

		CProcess::Instance().Core.Read(iter->second.text_ptr, sizeof(data), data);

		strText = data;

		if(strText.find(text.c_str()) != strText.npos )
			out[iter->first] = iter->second;
	}

	return ERROR_SUCCESS;
}

/*
	Get component children (only one level deep)

	IN:
		element - component to get children of

	OUT:
		out - found components

	RETURN:
		Error code
*/
DWORD CUIManager::GetChildren( UIComponent& element, mapUIElements &out )
{
	out.clear();

	for(mapUIElements::iterator iter = m_Elements.begin(); iter != m_Elements.end(); iter++)
	{
		if(element.self.hash == iter->second.parent.hash)
			out[iter->first] = iter->second;
	}

	return ERROR_SUCCESS;
}

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
DWORD CUIManager::SetText( UIComponent& element, std::string text )
{
	CProcess::Instance().Core.Write(element.text_ptr, text.length() + 1, (PVOID)text.c_str());
	//CProcess::Instance().Core.Write<int>(element.dwBaseAddr + FIELD_OFFSET(UIComponent, tb_length), text.length());

	return ERROR_SUCCESS;
}

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
DWORD CUIManager::SetCBIndex( UIComponent& element, int index )
{
	mapUIElements children;

	//CProcess::Instance().Core.Write<int>(element.dwBaseAddr + FIELD_OFFSET(UIComponent, cb_index), index);
	//CProcess::Instance().Core.Write<int>(element.dwBaseAddr + FIELD_OFFSET(UIComponent, cb_index2), index);

	GetChildren(element, children);

	if(children.empty())
		return ERROR_INVALID_PARAMETER;

	for(mapUIElements::iterator iter = children.begin(); iter != children.end(); iter++)
	{
		std::string strName = iter->second.self.name;

		if(strName.rfind(".arrow") != strName.npos)
		{
			ClickElement(iter->second);
			EnumUIElements();

			ds_utils::CDSString strListItem;

			strListItem.format(L"Root.TopLayer.DropDown._content._stackpanel._item%d", index);

			UIComponent* pListItem = CUIManager::Instance().GetUIElementByName(strListItem.data_mb());

			if(pListItem)
				ClickSPElement(*pListItem);
			else
				return ERROR_INVALID_PARAMETER;
	
			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_PARAMETER;
}

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
DWORD CUIManager::SetVisible( UIComponent& element, bool bVisible /*= true*/ )
{
	CProcess::Instance().Core.Write<DWORD>(element.visible, (DWORD)bVisible);

	return ERROR_SUCCESS;
}

/*
	Simulate component click

	IN:
		element - target component

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CUIManager::ClickElement( UIComponent& element )
{
#if (RCALL_TYPE == USE_REMOTE_THD || RCALL_TYPE == USE_APC)
	ds_utils::ds_memory::CDSWinDataBlock pCopy;
	void* pFuncData = 0, *pRemoteData;
	DWORD dwFuncSize;
	int index = 0;
	
#if (RCALL_TYPE == USE_REMOTE_THD)
	ClickCallWrapper(&pFuncData, &dwFuncSize);
#else
	ClickCallWrapperAPC(&pFuncData, &dwFuncSize);
#endif
	pCopy.Allocate(dwFuncSize);

	memcpy(pCopy, pFuncData, dwFuncSize);

	UIEvent evnt;

	memset(&evnt, 0x00, sizeof(evnt));

	CProcess::Instance().Core.Allocate(sizeof(UIEvent) + 4, pRemoteData);

	evnt.type = UIEvent::LeftMouseUp;
	evnt.x = 169;
	evnt.y = 361;

	CProcess::Instance().Core.Write<UIEvent>((DWORD)pRemoteData, evnt);
	CProcess::Instance().Core.Write<DWORD>((DWORD)pRemoteData + sizeof(evnt), 1);

	DWORD dwReplacement[2] = {element.dwBaseAddr + FIELD_OFFSET(UIComponent, self), element.click_handler};

	//Replace stubs with proper addresses
	for(DWORD i = 0; i < dwFuncSize; i++)
	{
		if(((DWORD*)(&((BYTE*)pCopy)[i]))[0] == 0xDEADBEEF)
		{
			memcpy(&((BYTE*)pCopy)[i], &dwReplacement[index], sizeof(DWORD));
			index++;

			if(index >= ARRAYSIZE(dwReplacement))
				break;
		}
	}

	CProcess::Instance().Core.Write((DWORD)CGlobalData::Instance().ObjMgr.Storage.ui_mgr + FIELD_OFFSET(UIManager, u_1), sizeof(UIReference), &element.self);
	CProcess::Instance().RemoteCall(pCopy, dwFuncSize);

#elif(RCALL_TYPE == USE_DLL)

	CALL call = {0};
	CALL_RET ret = {0};

	call.valid = VALID_CALL;
	call.state = CallState_Pending;
	call.type = CallType_ClickUI;
    call.arg1 = CProcess::Instance().Core.Read<DWORD>( element.click_handler );
	call.arg2 = element.dwBaseAddr + FIELD_OFFSET(UIComponent, self);

	CProcess::Instance().shared.DoCall(call, ret);
#else
	return	ERROR_NOT_SUPPORTED;

#endif

	return ERROR_SUCCESS;
}

/*
	Simulate click on StackPanel element

	IN:
		element - target component

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CUIManager::ClickSPElement( UIComponent& element )
{
#if(RCALL_TYPE == USE_DLL)

	CALL call = {0};
	CALL_RET ret = {0};

	call.valid = VALID_CALL;
	call.state = CallState_Pending;
	call.type = CallType_ClickSQUI;
	call.arg1 = element.dwBaseAddr;
	call.arg2 = element.click_handler;
	//call.arg3 = element.dwBaseAddr + FIELD_OFFSET(UIComponent, self);

	CProcess::Instance().shared.DoCall(call, ret);

	return ERROR_SUCCESS;

#else
	return	ERROR_NOT_SUPPORTED;

#endif
}

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
DWORD CUIManager::ClickCallWrapper( void** FuncData, DWORD* pdwSize )
{
	__asm
	{
		//calculate size of executable code
		mov eax, start
		mov ebx, codeend
		sub ebx, eax
		mov edi, pdwSize
		mov dword ptr [edi], ebx

		//get pointer to code start
		mov eax, FuncData
		mov ebx, start
		mov dword ptr [eax],ebx
		jmp codeend

	start: 
		push 0xDEADBEEF
		mov eax, 0xDEADBEEF
		call eax
		add esp, 4

		/*push 0x1530780
		mov eax, 0x941A80
		call eax
		add esp, 4*/

		mov ecx, 64
		mov edi, fs:[18h]
		add edi, 0E10h
		xor esi, esi

	zero_tls:
		mov dword ptr [edi], esi
		add edi, 4
		dec ecx
		jnz zero_tls

		ret

	codeend:
	}

	return ERROR_SUCCESS;
}

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
DWORD CUIManager::ClickCallWrapperAPC( void** FuncData, DWORD* pdwSize )
{
	__asm
	{
		//calculate size of executable code
		mov eax, start
		mov ebx, codeend
		sub ebx, eax
		mov edi, pdwSize
		mov dword ptr [edi], ebx

		//get pointer to code start
		mov eax, FuncData
		mov ebx, start
		mov dword ptr [eax],ebx
		jmp codeend

start: 
		push 0xDEADBEEF
		mov eax, 0xDEADBEEF
		call eax
		add esp, 4
		ret 4
codeend:
	}

	return ERROR_SUCCESS;
}

