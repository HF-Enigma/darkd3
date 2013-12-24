#ifndef _SHARED_H_
#define _SHARED_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"

//Valid call cookie
#define VALID_CALL		'D3VC'

//File mapping for interprocess calls
//Because name is hardcoded it can be used only with one remote process.
//Otherwise either additional synchronization or multiple mappings must be implemented.
#define CALL_MAP_NAME	_T("_MAP_DD3_CALL:13432_")

enum CallType
{
	CallType_Unknown = 0,
	CallType_ClickUI,
	CallType_ClickSQUI,
	CallType_UsePower,
	CallType_SellItem
};

enum CallState
{
	CallState_Unknown = -1,
	CallState_Pending,			//Call was just queued
	CallState_Executing,		//Call is executing now in dll
	CallState_Completed			//Call finished
};

typedef struct _CALL
{
	DWORD valid;				//Structure is valid
	CallType type;				//Type of function to execute
	CallState state;			//Current call state

	//Some call args
	DWORD arg1;
	DWORD arg2;
	DWORD arg3;	
	DWORD arg4;

	usePowerData usepower;		//used only in CallType_UsePower
}CALL, *PCALL;

//Call return value
typedef struct _CALL_RET
{
	DWORD valid;
	CallType type;

	DWORD RetVal;
	BYTE data[64];
}CALL_RET, *PCALL_RET;

class CShared
{
public:
	CShared(void);
	~CShared(void);

	/*
		Initialize shared memory

		RETURN:
			Error code
	*/
	DWORD Init();

	/*
		Free resources

		RETURN:
			Error code
	*/
	DWORD Dispose();

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
	DWORD DoCall(CALL& call, CALL_RET& ret);

public:
	static
	const 
	int			RetOffset = sizeof(CALL);	//Offset of return value in shared memory

private:
	HANDLE		m_hDataMap;					//File mapping handle
	CALL*		m_pCallData;				//Pointer to call params in shared memory
	CALL_RET*	m_pRetData;					//Pointer to return data in shared memory
};

#endif//_SHARED_H_