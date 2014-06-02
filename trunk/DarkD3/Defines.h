#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "stdafx.h"

//Invalid value
#define INVALID_VALUE			0xFFFFFFFF

//Main pointers
#define OBJECT_MGR_BASE			0x1CDD7F4   //
#define AUC_BASE				0x1CB06E8   //

//Functions
#define FUNC_D3D_PRESENT_OFFSET	0x00034F70  //IDirect3DDevice9::Present
#define FUNC_USE_POWER			0x00000000  //D3::UsePowerToLocation
#define FUNC_SELL_ITEM			0x00000000  //D3::ACD::SellItem
#define FUNC_UI_GET_PTR			0x00000000  //
#define FUNC_UI_UPDATE			0x00000000  //

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