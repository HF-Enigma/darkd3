#include "Shared.h"


CShared::CShared(void):
	m_hDataMap(NULL),
	m_pCallData(NULL),
	m_pRetData(NULL)
{
}


CShared::~CShared(void)
{
	Dispose();
}

/*
	Initialize shared memory

	RETURN:
		Error code
*/
DWORD CShared::Init()
{
	if(m_hDataMap)
		return ERROR_SUCCESS;

	m_hDataMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CALL) + sizeof(CALL_RET), CALL_MAP_NAME);

	if(m_hDataMap)
	{
		m_pCallData = (CALL*)MapViewOfFile(m_hDataMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);

		if(m_pCallData)
			m_pRetData = (CALL_RET*)((BYTE*)m_pCallData + RetOffset);
		else
		{
			Dispose();
			return GetLastError();
		}
	}
	else
		return GetLastError();

	return ERROR_SUCCESS;
}

/*
	Free resources

	RETURN:
		Error code
*/
DWORD CShared::Dispose()
{
	if(!m_hDataMap)
	{
		if(m_pCallData)
			UnmapViewOfFile(m_pCallData);

		m_pCallData = NULL;
		m_pRetData = NULL;

		CloseHandle(m_pCallData);

		m_pCallData = NULL;
	}
	else
		return ERROR_NOT_FOUND;

	return ERROR_SUCCESS;
}

/*
	Perform call through injected Dll

	IN:
		call - call params
		ret - return value reference

	OUT:
		ret - return value

	RETURN:
		Error code
	*/
DWORD CShared::DoCall( CALL& call, CALL_RET& ret )
{
	memcpy(m_pCallData, &call, sizeof(CALL));

	//Wait for execution
	while(m_pCallData->valid == VALID_CALL && m_pCallData->state != CallState_Completed)
		Sleep(100);

	//Reset call state
	RtlZeroMemory(m_pCallData, sizeof(CALL));

	memcpy(&ret, m_pRetData, sizeof(CALL_RET));

	return ERROR_SUCCESS;
}
