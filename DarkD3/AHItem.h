#ifndef _AH_ITEM_H_
#define _AH_ITEM_H_

#include "stdafx.h"
#include "Defines.h"
#include "Process.h"


//sizeof = 0x118
struct AHItemRaw 
{
	DWORD unknown_0[9];		// 0x000
	char D3_ITEM[8];		// 0x024
	DWORD unknown_02C[25];	// 0x02C
	char D3_GOLD[8];		// 0x090
	DWORD unknown_098[10];	// 0x098
	DWORD timeleft_id;		// 0x0C0
	DWORD dwUnknown_0C4;	// 0x0C4
	DWORD dwUnknown_0C8;	// 0x0C8
	DWORD dwUnknown_0CC;	// 0x0CC
	ULONGLONG curbid;		// 0x0D0
	ULONGLONG buyout;		// 0x0D8
	ULONGLONG minbid;		// 0x0E0
	DWORD flags;			// 0x0E8
	DWORD unknown4[11];		// 0x0EC
};

//sizeof = 0x98
struct AHUIHoverHeader
{
	DWORD pad_000[20];		// 0x000
	char type_name[32];		// 0x050
	DWORD pad_070[2];		// 0x070
	char dpsarmor[32];		// 0x078
};

//sizeof = 0x400
struct AHUIHoverStats
{
	char text[1024];		// 0x000
};

//sizeof = 0x60
struct AHUIHoverSocket
{
	char text[96];			// 0x000
};

class CAHItem
{
public:
	CAHItem(void);
	CAHItem(DWORD dwBase, char name[64], AHUIHoverHeader& dps, char stats[1024], char sockets[3][96]);
	~CAHItem(void);

public:
	AHItemRaw ItemRawData;			//Item struct
	char Name[64];					//Item name
	AHUIHoverHeader Dps_armor;		//Dps/armor value
	char Stats_string[1024];		//Stats description
	char Socket_strings[3][96];		//Sockets description

private:
	DWORD m_dwBaseAddr;				//Item address in memory
};

#endif//_AH_ITEM_H_