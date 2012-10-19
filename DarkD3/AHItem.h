#ifndef _AH_ITEM_H_
#define _AH_ITEM_H_

#include "stdafx.h"
#include "Defines.h"
#include "Process.h"


#pragma pack(push, 4)
//sizeof = 0x108
struct AHItemRaw 
{
	DWORD unknown_0[8];		// 0x000

	char D3_ITEM[8];		// 0x024
	DWORD unknown_02C[21];	// 0x02C
	char D3_GOLD[8];		// 0x080
	DWORD unknown_098[10];	// 0x088
	DWORD timeleft_id;		// 0x0B0
	DWORD dwUnknown_0C4;	// 0x0B4
	DWORD dwUnknown_0C8;	// 0x0B8
	DWORD dwUnknown_0CC[2];	// 0x0BC
	ULONGLONG curbid;		// 0x0CC
	ULONGLONG buyout;		// 0x0D4
	ULONGLONG minbid;		// 0x0DC
	DWORD flags;			// 0x0E4
	DWORD unknown4[11];		// 0x0F8
};
#pragma pack(pop)

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