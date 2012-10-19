#include "AHItem.h"


CAHItem::CAHItem(void)
{
}

CAHItem::CAHItem( DWORD dwBase, char name[64], AHUIHoverHeader& dps, char stats[1024], char sockets[3][96] ):
	m_dwBaseAddr(dwBase)
{
	CProcess::Instance().Core.Read(dwBase, sizeof(ItemRawData), &ItemRawData);

	int off = FIELD_OFFSET(AHItemRaw, curbid);

	RtlCopyMemory(Name, name, sizeof(Name));
	RtlCopyMemory(&Dps_armor, &dps, sizeof(AHUIHoverHeader));
	RtlCopyMemory(Stats_string, stats, sizeof(Stats_string));
	RtlCopyMemory(Socket_strings, sockets, sizeof(Socket_strings));
}


CAHItem::~CAHItem(void)
{
}
