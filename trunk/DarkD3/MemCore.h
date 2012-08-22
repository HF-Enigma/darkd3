#ifndef _MEM_CORE_H_
#define _MEM_CORE_H_

#include "stdafx.h"

class CMemCore
{
	friend class CProcess;
	friend class CMemDll;
	friend class CGlobalData;

public:
	~CMemCore(void);

	/*
		Allocate memory in process

		IN:
			size - amount to allocate in bytes

		OUT:
			pAddr - address of allocated memory

		RETURN:
			Error code
	*/
	DWORD Allocate(DWORD size, PVOID &pAddr);

	/*
		Free allocated memory in process

		IN:
			pAddr - address to release

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD Free(PVOID pAddr);

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
	DWORD Read(DWORD dwAddress, DWORD dwSize, PVOID pResult);

	/*
		Read process memory (templated)

		IN:
			dwAddress - read starting address

		OUT:
			void

		RETURN:
			Read data
	*/
	template<class T>
	T Read(DWORD dwAddress)
	{
		T res;

		if(Read(dwAddress, sizeof(T), &res) != ERROR_SUCCESS)
			return (T)INVALID_VALUE;

		return res;
	};

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
	DWORD Write(DWORD dwAddress, DWORD dwSize, PVOID pData);

	/*
		Write process memory (templated)

		IN:
			dwAddress - read starting address
			data - data to be written

		OUT:
			void

		RETURN:
			Error code
	*/
	template<class T>
	DWORD Write(DWORD dwAddress, T data)
	{
		CHK_RES(Write(dwAddress, sizeof(T), &data));

		return ERROR_SUCCESS;
	}

private:
	CMemCore(void){ };

	/*
		Get singleton instance
	*/
	static
	CMemCore& Instance();

private:
	HANDLE  m_hProcess;			//Process handle
	HANDLE  m_hMainThd;			//Process main thread handle
	DWORD	m_dwImageBase;		//Address of "DiabloIII.exe" module
};

#endif//_MEM_CORE_H_