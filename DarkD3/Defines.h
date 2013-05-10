#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdafx.h"

//Player GUID
#define PLAYER_GUID				0x77BC0000	//Obsolete as for 1.0.5

//Invalid value
#define INVALID_VALUE			0xFFFFFFFF

//Main pointers
#define OBJECT_MGR_BASE			0x18CD394	//
#define AUC_BASE				0x16E07C8	//

//Functions
#define FUNC_D3D_PRESENT_OFFSET	0x0007B6FC	//IDirect3DDevice9::Present
#define FUNC_USE_POWER			0x00951520	//D3::UsePowerToLocation
#define FUNC_SELL_ITEM			0x00B42AD0	//D3::ACD::SellItem
#define FUNC_UI_GET_PTR			0x008F1A80	//
#define FUNC_UI_UPDATE			0x00B01710	//

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