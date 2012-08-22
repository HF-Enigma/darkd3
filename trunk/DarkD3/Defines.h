#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdafx.h"

//Player GUID
#define PLAYER_GUID				0x77BC0000

//Invalid value
#define INVALID_VALUE			0xFFFFFFFF

//Main pointers
#define OBJECT_MGR_BASE			0x01543B9C   //0x15A0BEC;
#define INTERACT_BASE			0x01543B84   //0x15A0BD4;
#define AUC_BASE				0x00FC7560   //0x00FC75B0

//Functions
#define FUNC_USE_POWER			0x0097C810	//D3::UsePowerToLocation
#define FUNC_SELL_ITEM			0x00BBE880	//D3::ACD::SellItem

//SNO offsets
#define SNO_DEF_PTR 			0x3C
#define SNO_COUNT				0x10C
#define SNO_DEF_INDEX_OFF 		0x148

//Object iteration offsets
extern DWORD ITER_STRUCT_BASE;

#define ITER_STRUCT_OFF1	 	0xA8
#define ITER_STRUCT_OFF2		0x58

#define ITER_STRUCT_MOV_OFF 	0x7F20
#define ITER_STRUCT_ACTION_OFF	0x7F44
#define ITER_STRUCT_POWER1_OFF	0x7F7C
#define ITER_STRUCT_POWER2_OFF	0x7F80
#define ITER_STRUCT_MOUSE_OFF	0x7F84
#define ITER_STRUCT_GUID_OFF	0x7F88


/*
	Some useful routines
*/
//Macro for function result checking
#define CHK_RES(h) \
{ \
	DWORD r = h; \
	if(r != ERROR_SUCCESS) \
		return r; \
}

//Sleep and decrement counter
inline void SleepAndDecrement(DWORD& dwCounter, DWORD dwTime)
{
	dwCounter -= dwTime;
	Sleep(dwTime);
}

#endif//_DEFINES_H_