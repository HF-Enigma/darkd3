#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "stdafx.h"
#include "DSMemory/DSUtils.h"
#include "Globals.h"
#include "MemCore.h"
#include "MemDll.h"
#include "Shared.h"
#include "SNOManager.h"
#include "UIManager.h"


#define FILE_DEVICE_APCDRV				0x00008005
#define IOCTL_APCDRV_SET_ALERTABLE2		CTL_CODE(FILE_DEVICE_APCDRV, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)


class CGlobalData;
class CSNOManager;


//Provides operations with game process memory 
class CProcess
{
public:
	~CProcess(void);

	/*
	*/
	static
	CProcess& Instance();
	
	/*
		Set working Game process

		IN:
			hProcess - handle to game process
			dwModuleBase - address of main module

		OUT:
			void

		RETURN:
			void
	*/
	void Attach(DWORD pid);
	
	/*
		Return address of main module

		RETURN:
			Address of main module
	*/
	DWORD ModuleBase();
	

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
	DWORD RemoteCall( PVOID pCodeCave, DWORD size);

	
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
	DWORD QueueUserAPCEx(PAPCFUNC pfnApc, HANDLE hThread, DWORD dwData);

	/*
		Get process main thread TID

		RETURN:
			TID
	*/
	DWORD GetMainThreadID();

public:
	CMemCore& Core;			//Process core routines
	CMemDll   Dll;			//Dll routines
	CShared	  shared;		//Interprocess communication

private:
	CProcess();
	CProcess& operator = ( CProcess& ){ };

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
	DWORD CopyTLS(HANDLE hSourceThd, HANDLE hTargetThd);


	// Debug functions
	void EnumAttribList();

private:
	HANDLE	hDevice;			// driver device handle
};


#endif//_PROCESS_H_