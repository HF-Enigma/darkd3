#include "Process.h"

CProcess::CProcess():
	hDevice(INVALID_HANDLE_VALUE),
	Core(CMemCore::Instance())
{
	shared.Init();

    //Apc driver handle
	hDevice = CreateFile
		(
			_T("\\\\.\\Global\\APCDRV"),
			GENERIC_READ | GENERIC_WRITE, 
			0, NULL, 
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 
			NULL
		);
}

CProcess::~CProcess(void)
{
	if(hDevice != INVALID_HANDLE_VALUE)
		CloseHandle(hDevice);

#if(RCALL_TYPE == USE_DLL)
	Dll.Unload();
#endif
}

CProcess& CProcess::Instance()
{
	static CProcess ms_Instance;

	return ms_Instance;
}

/*
	Set working Game process

	IN:
		hProcess - handle to game process
	OUT:
		void

	RETURN:
		void
*/
void CProcess::Attach( DWORD pid )
{
	Core.m_hProcess		= OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ| PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, pid);
	Core.m_dwImageBase	= Dll.GetModuleAddress(pid, _T("Diablo III.exe"));
	Core.m_hMainThd		= OpenThread(THREAD_QUERY_INFORMATION | SYNCHRONIZE | THREAD_SET_CONTEXT | THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, GetMainThreadID());

	CGlobalData::Instance().RefreshOffsets();
	CSNOManager::Instance().InitDB();
	CUIManager::Instance().EnumUI();

	//EnumAttribList();

#if(RCALL_TYPE == USE_DLL)
	Dll.Inject();
#endif
}

/*
	Return address of main module

	IN:
		void

	OUT:
		void

	RETURN:
		Address of main module
*/
DWORD CProcess::ModuleBase()
{
	return Core.m_dwImageBase;
}

/*
	Perform function call in remote process

	IN:
		pCodeCave - code to execute
		size - size of code

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CProcess::RemoteCall( PVOID pCode, DWORD size)
{
	DWORD dwResult = ERROR_SUCCESS;
	HANDLE hThread = NULL;
	void *pCodecave = NULL, *pOutput = NULL;

	Core.Allocate(size + 0x10, pCodecave);
	Core.Allocate(0x1000, pOutput);

	if(Core.Write((DWORD)pCodecave, size, pCode) != ERROR_SUCCESS)
	{
		if(pCodecave)
			Core.Free(pCodecave);

		if(pOutput)
			Core.Free(pOutput);

		return GetLastError();
	}
	
#if (RCALL_TYPE == USE_REMOTE_THD)
	SuspendThread(Core.m_hMainThd);

	//Create suspended
	hThread = CreateRemoteThread(Core.m_hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pCodecave, NULL, CREATE_SUSPENDED, NULL);

	CopyTLS(Core.m_hMainThd, hThread);

	ResumeThread(hThread);

	WaitForSingleObject(hThread, 500);

	ResumeThread(Core.m_hMainThd);
#elif(RCALL_TYPE == USE_APC)
	dwResult = QueueUserAPCEx((PAPCFUNC)pCodecave, Core.m_hMainThd, (DWORD)pOutput);

	Sleep(100);

#else
	dwResult = ERROR_NOT_SUPPORTED;

#endif
	
	if(pCodecave)
		Core.Free(pCodecave);

	if(pOutput)
		Core.Free(pOutput);

	return dwResult;
}

/*
	Copy TLS slots from one thread to another

	IN:
		hSourceThd - source thread 
		hTargetThd - target thread

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CProcess::CopyTLS(HANDLE hSourceThd, HANDLE hTargetThd)
{
	CONTEXT context_src = {0}, context_tgt = {0};
	LDT_ENTRY entry_src = {0}, entry_tgt = {0};
	DWORD dwTibAddr_src = 0, dwTibAddr_tgt = 0;
	TEB teb_src = {0};

	context_src.ContextFlags = context_tgt.ContextFlags = CONTEXT_ALL;

	GetThreadContext(hSourceThd, &context_src);
	GetThreadContext(hTargetThd, &context_tgt);

	//Get fs selector value for source thread
	if(GetThreadSelectorEntry(hTargetThd, context_tgt.SegFs, &entry_tgt))
		dwTibAddr_tgt = (entry_tgt.HighWord.Bytes.BaseHi << 24) | (entry_tgt.HighWord.Bytes.BaseMid << 16) | entry_tgt.BaseLow;
	else
		return GetLastError();

	//Get FS selector value for target thread
	if(GetThreadSelectorEntry(hSourceThd, context_src.SegFs, &entry_src))
		dwTibAddr_src = (entry_src.HighWord.Bytes.BaseHi << 24) | (entry_src.HighWord.Bytes.BaseMid << 16) | entry_src.BaseLow;
	else
		return GetLastError();

	//Copy main TLS slot values
	Core.Read(dwTibAddr_src, sizeof(teb_src), &teb_src);
	Core.Write(dwTibAddr_tgt + 0xE10, sizeof(teb_src.TlsSlots), &teb_src.TlsSlots);

	return ERROR_SUCCESS;
}

/*
	Get process main thread TID

	RETURN:
		Main TID
*/
DWORD CProcess::GetMainThreadID()
{
	DWORD dwMainThreadID = 0;

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (hThreadSnap != INVALID_HANDLE_VALUE) 
	{
		THREADENTRY32 th32;
		th32.dwSize = sizeof(THREADENTRY32);

		DWORD dwProcID = GetProcessId(Core.m_hProcess);

		for(BOOL bOk = Thread32First(hThreadSnap, &th32); bOk; bOk = Thread32Next(hThreadSnap, &th32))
		{
			if (th32.th32OwnerProcessID == dwProcID) 
			{
				LDT_ENTRY entry;
				CONTEXT context = {0};
				context.ContextFlags = CONTEXT_ALL;

				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, TRUE, th32.th32ThreadID);

				if(hThread)
				{
					SuspendThread(hThread);
					GetThreadContext(hThread, &context);
					ResumeThread(hThread);				

					//Get TIB address
					if(GetThreadSelectorEntry(hThread, context.SegFs, &entry))
					{
						DWORD dwAddr = (entry.HighWord.Bytes.BaseHi << 24) | (entry.HighWord.Bytes.BaseMid << 16) | entry.BaseLow;

						//CSR Client Thread
						dwMainThreadID = Core.Read<DWORD>(dwAddr + 36);
					}

					CloseHandle(hThread);
					break;
				}
			}
		}

		CloseHandle(hThreadSnap);
	}

	return dwMainThreadID;
}

/*
	Queue APC to thread

	IN:
		pfnApc - function address
		hThread - host thread
		dwData - function argument

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CProcess::QueueUserAPCEx(PAPCFUNC pfnApc, HANDLE hThread, DWORD dwData)
{
	DWORD dwReturned = 0;

	//Currently 8 bytes for x64 driver
	//Must be set to 4 bytes for x86 driver
	ULONGLONG wrap = (ULONGLONG)hThread;	

	if (hDevice == INVALID_HANDLE_VALUE)
		return ERROR_NOT_READY;

	SuspendThread(hThread);

	//Send the APC
	if (!QueueUserAPC(pfnApc, hThread, dwData))
	{
		ResumeThread(hThread);
		return GetLastError();
	}

	//Ensure the execution of the APC
	if (!DeviceIoControl(hDevice, (DWORD)IOCTL_APCDRV_SET_ALERTABLE2, &wrap, sizeof(wrap), NULL, 0, &dwReturned, 0))
	{
		ResumeThread(hThread);
		return GetLastError();
	}

	ResumeThread(hThread);

	return ERROR_SUCCESS;
}

//////////////////////////// Debug functions ////////////////////////////

void CProcess::EnumAttribList()
{
	DWORD dwAddr = 0x168D77C; //0x1520518;

	AttributeDesc desk;
	char sszName[64];

	for(int i = 0; ;i++)
	{
		if(CProcess::Instance().Core.Read(dwAddr + i*sizeof(AttributeDesc), sizeof(desk), &desk) != ERROR_SUCCESS || 
			desk.Name == NULL || desk.Name == (char*)INVALID_VALUE)
		{
			break;
		}

		CProcess::Instance().Core.Read((DWORD)desk.Name, sizeof(sszName), &sszName);

		ds_utils::CDSString strName, strType;

		if(desk.Type == 0)
			strType = L"float";
		else if(desk.Type == 1)
			strType = L"int";
		else
			strType = L"unknown";

		strName.format(L"%ws = 0x%x, // %ws\r\n", ds_utils::CDSString(sszName).data(), desk.id, strType.data());

		OutputDebugStringW(strName.data());
	}
}
