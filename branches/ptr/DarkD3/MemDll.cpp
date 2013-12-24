#include "MemDll.h"


CMemDll::CMemDll(void)
{
}


CMemDll::~CMemDll(void)
{
}



/*
	Inject Dll into process

	RETURN:
		Error code
*/
DWORD CMemDll::Inject()
{
    HANDLE hThread = NULL;
    HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD  dwBytesWritten = 0;
    LPVOID pCode = NULL;
    DWORD* pfnThreadRtn = NULL;    
    char   DllName[255];

    if(!CMemCore::Instance().m_hProcess)
        return ERROR_INVALID_HANDLE;

    //Already loaded
    if(GetModuleAddress(GetProcessId(CMemCore::Instance().m_hProcess), TEXT(DLL_NAME)) !=0 )
        return ERROR_SUCCESS;

    hFile = CreateFileA(DLL_NAME, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return GetLastError();
    else
        CloseHandle(hFile);

    GetFullPathNameA(DLL_NAME, sizeof(DllName), DllName, NULL);

    CHK_RES(CMemCore::Instance().Allocate(0x200, pCode));

    if(!WriteProcessMemory(CMemCore::Instance(). m_hProcess, pCode, (LPCVOID)DllName, strlen(DllName)+1, &dwBytesWritten))
	{
		CMemCore::Instance().Free(pCode);
        return GetLastError();
	}

    pfnThreadRtn = (DWORD*)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"); 
    if(pfnThreadRtn==NULL)
	{
		CMemCore::Instance().Free(pCode);
        return GetLastError();
	}

    hThread = CreateRemoteThread(CMemCore::Instance().m_hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnThreadRtn, pCode, 0, NULL);
    if(hThread == NULL)
	{
		CMemCore::Instance().Free(pCode);
        return GetLastError();
	}

	WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

	CMemCore::Instance().Free(pCode);

    return ERROR_SUCCESS;
}

/*
	Unload Dll from process

	RETURN:
		Error code
*/
DWORD CMemDll::Unload()
{
	HANDLE hThread = NULL;
	HMODULE hDll = NULL;

    if(!CMemCore::Instance().m_hProcess)
        return ERROR_INVALID_HANDLE;

	//Search for dll in process
	if((hDll = (HMODULE)GetModuleAddress(GetProcessId(CMemCore::Instance().m_hProcess), TEXT(DLL_NAME))) !=0 )
	{
		hThread = CreateRemoteThread
			(
				CMemCore::Instance().m_hProcess, 
				NULL, 0, 
				(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "FreeLibrary"), 
				(void*)hDll, 
				0, NULL
			);

		if(hThread == NULL)
		{
			MessageBox(NULL, TEXT("Cannot create thread"), TEXT("Error"), MB_ICONERROR);
			return GetLastError();
		}
		//Wait for completion
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

    return ERROR_SUCCESS;
}


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
DWORD CMemDll::GetModuleAddress(DWORD proc, const TCHAR *modname)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc);

	if( snapshot == INVALID_HANDLE_VALUE )
		return 0;

	MODULEENTRY32 mod = {0};
	mod.dwSize = sizeof(MODULEENTRY32);

	if( Module32First(snapshot, &mod) )
	{
		if( _tcsicmp(mod.szModule, modname) == 0 )
		{
			CloseHandle(snapshot);
			return (DWORD)mod.modBaseAddr;
		}

		while( Module32Next(snapshot, &mod) )
		{
			if( _tcscmp(mod.szModule, modname) == 0 )
			{
				CloseHandle(snapshot);
				return (DWORD)mod.modBaseAddr;
			}
		}

		CloseHandle(snapshot);
		return 0;
	}
	else
	{
		CloseHandle(snapshot);
		return 0;
	}
}