// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Handler.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			CD3DPresentDetour::Instance().DetourFunction
				(
					CD3DPresentDetour::Instance().PresentAddr(), 
					CD3DPresentDetour::Instance().FuncAddress()
				);
		break;

		case DLL_PROCESS_DETACH:
			CD3DPresentDetour::Instance().Restore();
		break;
	}
	return TRUE;
}

