#include "MemCore.h"


CMemCore::~CMemCore(void)
{
	if(m_hProcess)
		CloseHandle(m_hProcess);

	if(m_hMainThd)
		CloseHandle(m_hMainThd);
}


/*
	Get singleton instance
*/
CMemCore& CMemCore::Instance()
{
	static CMemCore ms_Instance;

	return ms_Instance;
}


/*
	Allocate memory in process

	IN:
		size - amount to allocate in bytes

	OUT:
		pAddr - address of allocated memory

	RETURN:
		Error code
*/
DWORD CMemCore::Allocate( DWORD size, PVOID &pAddr )
{
	SetLastError(ERROR_SUCCESS);

	pAddr = VirtualAllocEx(m_hProcess, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	return GetLastError();
}

/*
	Free allocated memory in process

	IN:
		pAddr - address to release

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CMemCore::Free( PVOID pAddr )
{
	SetLastError(ERROR_SUCCESS);

	VirtualFreeEx(m_hProcess, pAddr, 0, MEM_RELEASE);

	return GetLastError();
}

/*
	Read process memory

	IN:
		dwAddress - read starting address
		dwSize - bytes to read
		pResult - pointer to receiving buffer

	OUT:
		pResult - read data

	RETURN:
		Error code
*/
DWORD CMemCore::Read( DWORD dwAddress, DWORD dwSize, PVOID pResult )
{
	DWORD dwRead = 0;

	if(dwAddress == 0)
		return ERROR_INVALID_ADDRESS;

	if(!ReadProcessMemory(m_hProcess, (LPCVOID)dwAddress, pResult, dwSize, &dwRead) || dwRead != dwSize)
		return GetLastError();

	return ERROR_SUCCESS;
}

/*
	Write process memory

	IN:
		dwAddress - read starting address
		dwSize - bytes to read
		pResult - pointer to data to be written

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CMemCore::Write( DWORD dwAddress, DWORD dwSize, PVOID pData )
{
	DWORD dwWritten = 0;

	if(dwAddress == 0)
		return ERROR_INVALID_ADDRESS;

	if(!WriteProcessMemory(m_hProcess, (LPVOID)dwAddress, pData, dwSize, &dwWritten) || dwWritten != dwSize)
		return GetLastError();

	return ERROR_SUCCESS;
}