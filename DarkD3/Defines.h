#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdafx.h"

//Player GUID
#define PLAYER_GUID				0x77BC0000	//Obsolete as for 1.0.5

//Invalid value
#define INVALID_VALUE			0xFFFFFFFF

//Main pointers
#define OBJECT_MGR_BASE			0x0186FA3C	//0x01543B9C   //0x15A0BEC;
//#define INTERACT_BASE			0x01543B84  //0x15A0BD4;
#define AUC_BASE				0x00E8A468	//0x00FC7560  //0x00FC75B0

//Functions
#define FUNC_D3D_PRESENT_OFFSET	0x0007B6FC	//IDirect3DDevice9::Present
#define FUNC_USE_POWER			0x008AD940	//0x0097EF80  //0x0097C810	//D3::UsePowerToLocation
#define FUNC_SELL_ITEM			0x00BBE880	//D3::ACD::SellItem
#define FUNC_UI_GET_PTR			0x00852270	//
#define FUNC_UI_UPDATE			0x00A4E5A0	//

//SNO offsets
#define SNO_DEF_PTR 			0x10	//0x3C
#define SNO_COUNT				0x10C
#define SNO_DEF_INDEX_OFF 		0x148

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

#endif//_DEFINES_H_