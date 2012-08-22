#include "Detour.h"


CDetour::CDetour() :
    m_RealAdress(NULL),
    m_NewAdress(NULL),
    m_OldCode(NULL),
    m_bDetoured(FALSE)
{
    RtlZeroMemory(&m_FarJump,sizeof(JUMPCODE));
}

CDetour::~CDetour()
{
    Restore();

    if(m_OldCode)
        HeapFree(GetProcessHeap(), 0, m_OldCode);
}

/*
	Suspend/resume threads of process while writing trampoline

	IN:
		start - true if Resume, otherwise - suspend

	OUT:
		void

	RETURN:
		void
*/
void CDetour::StartStopThreads(bool start)
{
    DWORD dwThread = NULL, dwProc = NULL;
    HANDLE hThread = NULL, hSnap = NULL;
    THREADENTRY32 Thread = {0};

    dwProc = GetCurrentProcessId();
    dwThread = GetCurrentThreadId();

    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
    if(hSnap == INVALID_HANDLE_VALUE)
        return;

    Thread.dwSize = sizeof(THREADENTRY32);

    if(Thread32First(hSnap, &Thread))
    {
        for(;;)
        {
			//For each thread of current process
            if (Thread.th32ThreadID!=dwThread && Thread.th32OwnerProcessID==dwProc)
            {
                hThread = OpenThread(THREAD_SUSPEND_RESUME,FALSE,Thread.th32ThreadID);
                if(hThread != INVALID_HANDLE_VALUE)
                {
                    if(start)
                        ResumeThread(hThread);
                    else
                        SuspendThread(hThread);

                    CloseHandle(hThread);
                }
            }
            if(!Thread32Next(hSnap, &Thread))
                break;
        }
        CloseHandle(hSnap);
    }
}

/*
	Detour function by address.
	It doesn't check for detours already present there. 
	So in such cases everything may fail miserably.

	IN:
		pOldFunc - function to detour
		pNewFunc - replacement

	OUT:
		void

	RETURN:
		error code
*/
DWORD CDetour::DetourFunction( void* pOldFunc, void* pNewFunc )
{
    SIZE_T dwRead = 0;
    DWORD dwStructSize = sizeof(JUMPCODE);

    if(!m_bDetoured)
    {
        m_RealAdress = pOldFunc;

        if(!m_RealAdress)
            return GetLastError();

        m_OldCode = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwStructSize);

        if(!ReadProcessMemory(GetCurrentProcess(), m_RealAdress, m_OldCode, dwStructSize, &dwRead))
        {
            if(m_OldCode)
            {
                HeapFree(GetProcessHeap(), 0, m_OldCode);
                m_OldCode = NULL;
            }
            return GetLastError();
        }

        m_NewAdress = pNewFunc;

#ifdef _AMD64_
        m_FarJump.FirstByte = 0x48;             //First byte of mov
        m_FarJump.SecondByte = 0xB8;            //Second byte of mov
        m_FarJump.ThirdByte = 0x50;             //push RAX
#else
        m_FarJump.OpCode = 0x68;                //push 
#endif

        m_FarJump.Address = (SIZE_T)pNewFunc;   //Function address
        m_FarJump.OpRet = 0xC3;                 //ret

        StartStopThreads(false);
        if(!WriteProcessMemory(GetCurrentProcess(), m_RealAdress, &m_FarJump, dwStructSize, &dwRead))
        {
            StartStopThreads(true);

            if(m_OldCode)
            {
                HeapFree(GetProcessHeap(),0,m_OldCode);
                m_OldCode = NULL;
            }
            return GetLastError();
        }
        StartStopThreads(true);

        m_bDetoured = TRUE;
    }

    return ERROR_SUCCESS;
}

/*
	Restore original function contents
*/
void CDetour::Restore()
{
    SIZE_T dwWritten = 0;
    DWORD dwStructSize = sizeof(JUMPCODE);

    if(m_bDetoured)
    {
        StartStopThreads(false);
        WriteProcessMemory(GetCurrentProcess(), m_RealAdress, m_OldCode, dwStructSize, &dwWritten);
        StartStopThreads(true);

        m_bDetoured = FALSE;
    }
}