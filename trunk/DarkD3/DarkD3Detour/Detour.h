#ifndef _DETOUR_H
#define _DETOUR_H

#include "stdafx.h"
#include "../Defines.h"


//Trampoline code
#ifdef _AMD64_

#pragma pack(push, 1)
typedef struct _JumpCode64
{
    BYTE FirstByte;
    BYTE SecondByte;  
    size_t Address;
    BYTE ThirdByte;
    BYTE OpRet;
}JumpCode64, *PJumpCode64;
#pragma pack(pop)

#define  JUMPCODE  JumpCode64
#else    

#pragma pack(push, 1)
typedef struct _JumpCode
{
    BYTE OpCode;
    DWORD Address;
    BYTE OpRet;
}JumpCode, *PJumpCode;
#pragma pack(pop)

#define  JUMPCODE  JumpCode
#endif  


class CDetour
{
public:
    CDetour();
    ~CDetour();

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
	DWORD DetourFunction(void* pOldFunc, void* pNewFunc);

	/*
		Restore original function contents
	*/
    void Restore();

private:

	/*
		Suspend/resume threads in process while writing trampoline

		IN:
			start - true if Resume, otherwise - suspend

		OUT:
			void

		RETURN:
			void
	*/
    void StartStopThreads(bool start);

protected:
    PVOID		m_RealAdress, m_NewAdress;		//Function addresses
    PVOID		m_OldCode;						//Original function code
    JUMPCODE	m_FarJump;						//Trampoline
    BOOL		m_bDetoured;					//Detour flag
};

#endif//_DETOUR_H