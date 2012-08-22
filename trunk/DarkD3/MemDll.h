#ifndef _MEM_DLL_H_
#define _MEM_DLL_H_

#include "stdafx.h"
#include "Defines.h"
#include "MemCore.h"
#include "DarkD3Detour/Handler.h"

#define DLL_NAME	"DarkD3Detour.dll"

class CMemDll
{
public:
	CMemDll(void);
	~CMemDll(void);

	/*
		Inject Dll into process

		RETURN:
			Error code
	*/
	DWORD Inject();

	/*
		Unload Dll from process

		RETURN:
			Error code
	*/
	DWORD Unload();

	/*
		Get specific module address

		IN:
			proc - process ID
			modname - module name

		OUT:
			void

		RETURN:
			Module address
			0 - if not found
	*/
	DWORD GetModuleAddress(DWORD proc, const TCHAR *modname);
};

#endif//_MEM_DLL_H_