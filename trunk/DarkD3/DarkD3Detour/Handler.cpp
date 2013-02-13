#include "Handler.h"
#include "../SNOClasses.h"

CD3DPresentDetour::CD3DPresentDetour():
    CDetour(),
	m_hCallMap(NULL),
	m_pCallData(NULL)
{
	m_CallHandlers[CallType_ClickUI]	= &CD3DPresentDetour::ClickUICall;
	m_CallHandlers[CallType_ClickSQUI]	= &CD3DPresentDetour::ClickSQUICall;
	m_CallHandlers[CallType_UsePower]	= &CD3DPresentDetour::UsePowerCall;
	m_CallHandlers[CallType_SellItem]	= &CD3DPresentDetour::SellItemCall;

	m_hCallMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CALL) + sizeof(CALL_RET), CALL_MAP_NAME);

	if(m_hCallMap)
	{
		m_pCallData = (CALL*)MapViewOfFile(m_hCallMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if(m_pCallData)
			m_pRetData = (CALL_RET*)((BYTE*)m_pCallData + CShared::RetOffset);
	}
}

/*
	Get singleton instance
*/
CD3DPresentDetour& CD3DPresentDetour::Instance()
{
	static CD3DPresentDetour ms_Instance;

	return ms_Instance;
}


CD3DPresentDetour::~CD3DPresentDetour()
{
	if(m_hCallMap)
		CloseHandle(m_hCallMap);
}   
   
/*
	New function
*/
HRESULT __stdcall CD3DPresentDetour::NewD3DPresent(	IDirect3DDevice9 *device, 
													const RECT *pSourceRect, 
													const RECT *pDestRect, 
													HWND hDestWindowOverride, 
													const RGNDATA *pDirtyRegion )
{
	Instance().HandleCall();

	return Instance().CallOriginal(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/*
	Call original function
*/
HRESULT __stdcall CD3DPresentDetour::CallOriginal(  IDirect3DDevice9 *device, 
													const RECT *pSourceRect, 
													const RECT *pDestRect,
													HWND hDestWindowOverride, 
													const RGNDATA *pDirtyRegion )
{
	HRESULT status				= S_OK;
	SIZE_T dwWritten			= 0;
	DWORD dwStructSize			= sizeof(JUMPCODE);
	d3d_present_t D3DPresent	= (d3d_present_t)m_RealAdress;;

	WriteProcessMemory(GetCurrentProcess(), m_RealAdress, m_OldCode, dwStructSize, &dwWritten);

	status = D3DPresent(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	WriteProcessMemory(GetCurrentProcess(), m_RealAdress, &m_FarJump, dwStructSize, &dwWritten);

	return status;
}

/*
	Handle request from main library
*/
void CD3DPresentDetour::HandleCall()
{
	if(m_pCallData && m_pCallData->valid && m_pCallData->state == CallState_Pending)
	{
		m_pCallData->state = CallState_Executing;

		RtlZeroMemory(m_pRetData, sizeof(CALL_RET));

		if(m_CallHandlers.find(m_pCallData->type) != m_CallHandlers.end())
		{
			(this->*m_CallHandlers[m_pCallData->type])(*m_pCallData, m_pRetData->RetVal);

			m_pRetData->valid = VALID_CALL;
		}

		m_pRetData->type	= m_pCallData->type;
		m_pCallData->state	= CallState_Completed;
	}
}

/*
	Execute UI component click handler

	IN:
		callparams - call parameters

	OUT:
		retval - function return value

	RETURN:
		Error code

*/
void CD3DPresentDetour::ClickUICall( const CALL& callparams, DWORD& retval )
{
	CallMethodAsm(NULL, m_pCallData->arg1, CC_cdecl, 1, callparams.arg2);

	__asm
	{
		mov edx, retval
		mov dword ptr [edx], eax
	}
}

/*
	Clicker for SubQuest buttons

	IN:
		callparams - call parameters

	OUT:
		retval - function return value

	RETURN:
		Error code

*/
void CD3DPresentDetour::ClickSQUICall( const CALL& callparams, DWORD& retval )
{
	DWORD dwTmp = 0;

	//Get class
	CallMethodAsm(NULL, FUNC_UI_GET_PTR, CC_cdecl, 1, callparams.arg1 + FIELD_OFFSET(UIComponent, val_C90));

	__asm
	{
		mov dwTmp, eax
	}
	
	CallMethodAsm(dwTmp, FUNC_UI_UPDATE, CC_thiscall, 1, *((DWORD*)(callparams.arg1 + FIELD_OFFSET(UIComponent, cb_index2))));

	//Click handler
	if(callparams.arg2 != 0)
		CallMethodAsm(NULL, callparams.arg2, CC_cdecl, 1, callparams.arg1 + FIELD_OFFSET(UIComponent, self));


	//CallMethodAsm(dwTmp, 0x941A80, CC_cdecl, 1, 0x1530780);
	
	__asm
	{
		mov edx, retval
		mov dword ptr [edx], eax
	}
}

/*
	Sell item by ACD

	IN:
		callparams - call parameters

	OUT:
		retval - function return value

	RETURN:
		Error code

*/
void CD3DPresentDetour::SellItemCall( const CALL& callparams, DWORD& retval )
{
	CallMethodAsm(m_pCallData->arg1, FUNC_SELL_ITEM, CC_thiscall_cdecl);	

	__asm
	{
		mov edx, retval
		mov dword ptr [edx], eax
	}
}

/*
	Use power

	IN:
		callparams - call parameters

	OUT:
		retval - function return value

	RETURN:
		Error code

*/
void CD3DPresentDetour::UsePowerCall( const CALL& callparams, DWORD& retval )
{
	DWORD dwPlayerptr = callparams.arg1;
	DWORD *pPlayerPtr = &dwPlayerptr;
	DWORD dwPacketAddr = (DWORD)&callparams.usepower;

	__asm
	{
		push pPlayerPtr
		push 1
		push 0
		mov esi, dwPacketAddr
		mov eax, dwPlayerptr
			
		mov ecx, FUNC_USE_POWER	
		call ecx

		add esp, 0Ch

		mov edx, retval
		mov dword ptr [edx], eax
	}
}

/*
	Wrapper for process function calls

	IN:
		pClass - class pointer(if needed)
		dwMethod - function address
		type - function calling convention
		arg_count - argument count
		... - arguments to pass into function

	OUT:
		void

	RETURN:
		void

*/
__declspec (naked) void CD3DPresentDetour::CallMethodAsm( DWORD pClass, DWORD dwMethod, CallingConvention type, DWORD arg_count /*= 0*/, ... )
{
	__asm
	{
		pop edi		// return address
		pop eax		// "this" pointer (due to varargs - __cdecl call)
		pop ecx		// pClass
		pop eax		// dwMethod
		pop ebx		// type
		pop esi		// arg_count

		// skip pushing hidden "this" for class members
		cmp ebx, CC_thiscall_cdecl
		jl skip_thiscall

		push ecx

	skip_thiscall:
		call eax
		xor ecx, ecx

		// skip popping hidden "this" from stack
		cmp ebx, CC_fastcall
		jle skip_hidden

		sub ecx, 4

	skip_hidden:

		cmp ebx, CC_cdecl
		jz skip_vararg

		cmp ebx, CC_thiscall_cdecl
		jz skip_vararg
	
		// called function popped arguments from stack, so esp must be adjusted
		mov ebx, eax

		mov eax, esi
		mov edx, 4
		mul edx
		mov ecx, eax

		mov eax ,ebx

	skip_vararg:

		// additional esp adjustment for CProcess::Call stack params
		add ecx, 20
		sub esp, ecx
	
		push edi	//return address
		ret
	}
}

/*
	Get NewD3DPresent address
*/
PVOID CD3DPresentDetour::FuncAddress()
{
    return (PVOID)&this->NewD3DPresent;
}

/*
	Get IDirect3DDevice9::Present address. Hardcoded value
*/
PVOID CD3DPresentDetour::PresentAddr()
{
	HMODULE hD3DBase = GetModuleHandle(_T("d3d9.dll"));

	//Offset for my Win8 x64
	return (PVOID)((DWORD)hD3DBase + FUNC_D3D_PRESENT_OFFSET);
}

/*
	Get IDirect3DDevice9::Present address

	RETURN:
		Function address
*/
DWORD CD3DPresentDetour::GetD3DPresent()
{
	IDirect3D9 *d3d;
	IDirect3DDevice9 *device;
	HINSTANCE instance = GetModuleHandle(NULL);

	if((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == 0)
		return 0;

	WNDCLASSEX wndclass = {sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, &DefWindowProc, 0L, 0L, instance, NULL, NULL, NULL, NULL, _T("WindowClass"), NULL};

	if(RegisterClassEx(&wndclass) == 0)
	{
		d3d->Release();
		return 0;
	}

	HWND window = CreateWindow(_T("WindowClass"), _T(""), WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, NULL, instance, 0);

	if(!window)
	{
		d3d->Release();
		UnregisterClass(_T("WindowClass"), instance);

		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	RtlZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = window;
	d3dpp.Windowed = TRUE;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &d3dpp, &device);

	if(FAILED(result))
	{
		d3d->Release();
		UnregisterClass(_T("WindowClass"), instance);

		return 0;
	}

	DWORD dwRet = (DWORD)((*(size_t**)device)[17]);

	d3d->Release();
	device->Release();
	UnregisterClass(_T("WindowClass"), instance);

	return dwRet;
}


