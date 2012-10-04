#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "stdafx.h"
#include "Detour.h"
#include "../Shared.h"
#include <d3d9.h>

//IDirect3DDevice9::Present
typedef HRESULT (__stdcall *d3d_present_t)( IDirect3DDevice9 *device, 
											const RECT *pSourceRect, 
											const RECT *pDestRect, 
											HWND hDestWindowOverride, 
											const RGNDATA *pDirtyRegion);
//Function calling convention
enum CallingConvention
{
	CC_stdcall,
	CC_cdecl,
	CC_fastcall,

	CC_thiscall,			//this in ecx, stdcall stack handling
	CC_thiscall_cdecl,		//this in hidden 1 arg, cdecl stack handling
	CC_thiscall_stdcall		//this in hidden 1 arg, stdcall stack handling
};

class CD3DPresentDetour :  public CDetour
{
	typedef void(CD3DPresentDetour::*pfnCallHandler)(const CALL&, DWORD&);
	typedef std::map<CallType, pfnCallHandler> mapHandlers;

public:
    ~CD3DPresentDetour();

	/*
		Get singleton instance
	*/
	static
	CD3DPresentDetour& Instance();

	/*
		Get NewD3DPresent address
	*/
    virtual PVOID FuncAddress();

	/*
		Get IDirect3DDevice9::Present address. Hardcoded value
	*/
	virtual PVOID PresentAddr();

private:
	CD3DPresentDetour();

	/*
		New function
	*/
	static 
	HRESULT __stdcall NewD3DPresent(IDirect3DDevice9 *device, 
									const RECT *pSourceRect, 
									const RECT *pDestRect, 
									HWND hDestWindowOverride, 
									const RGNDATA *pDirtyRegion);
	/*
		Call original function
	*/
	HRESULT __stdcall CallOriginal( IDirect3DDevice9 *device, 
									const RECT *pSourceRect, 
									const RECT *pDestRect, 
									HWND hDestWindowOverride, 
									const RGNDATA *pDirtyRegion);

	/*
		Handle request from main library
	*/
	void HandleCall();

	/*
		Execute UI component click handler

		IN:
			callparams - call parameters

		OUT:
			retval - function return value

		RETURN:
			Error code

	*/
	void ClickUICall(const CALL& callparams, DWORD& retval);

	
	/*
	*/
	void ClickSQUICall( const CALL& callparams, DWORD& retval );

	/*
		Sell item by ACD

		IN:
			callparams - call parameters

		OUT:
			retval - function return value

		RETURN:
			Error code

	*/
	void SellItemCall( const CALL& callparams, DWORD& retval );

	/*
		Use power

		IN:
			callparams - call parameters

		OUT:
			retval - function return value

		RETURN:
			Error code

	*/
	void UsePowerCall( const CALL& callparams, DWORD& retval );

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
	void CallMethodAsm( DWORD pClass, DWORD dwMethod, CallingConvention type, DWORD arg_count = 0, ... );

	/*
		Get IDirect3DDevice9::Present address

		RETURN:
			Function address
	*/
	DWORD GetD3DPresent();

private:
	HANDLE		m_hCallMap;			//File mapping handle
	CALL*		m_pCallData;		//Pointer to Call data
	CALL_RET*	m_pRetData;			//Pointer to Return data
	mapHandlers	m_CallHandlers;		//Call handle functions
};

#endif//_HANDLER_H_