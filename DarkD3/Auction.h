#ifndef _AUCTION_H_
#define _AUCTION_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"
#include "Process.h"
#include "SNOManager.h"
#include "ActorManager.h"
#include "UIManager.h"
#include "AHItem.h"

//AUC related UI elements hashes
#define UI_SELECTED_ITEM_NAME_HASH			0x74E938288B53294A		// Root.TopLayer.item 2.stack.top_wrapper.stack.name
#define UI_SELECTED_ITEM_DPS_HASH			0xCE00DC04C14F53CD		// Root.TopLayer.item 2.stack.frame body.stack.main.large_itembutton
#define UI_SELECTED_ITEM_ATTRIBS_HASH		0x1F934DDB805BDEE0		// Root.TopLayer.item 2.stack.frame body.stack.stats
#define UI_SELECTED_ITEM_SOCKET_0_HASH		0x5ECD6A0DF9E9C561		// Root.TopLayer.item 2.stack.frame body.stack.socket 0.text
#define UI_SELECTED_ITEM_SOCKET_1_HASH		0xC64C26F58A508220		// Root.TopLayer.item 2.stack.frame body.stack.socket 1.text
#define UI_SELECTED_ITEM_SOCKET_2_HASH		0x897B771009A8C29F		// Root.TopLayer.item 2.stack.frame body.stack.socket 2.text

//Pattern for item table info
#define UI_AUC_ITEM_PATTERN	\
"Root. \
NormalLayer. \
BattleNetAuctionHouse_main. \
LayoutRoot.OverlayContainer. \
TabContentContainer. \
SearchTabContent. \
SearchListContent. \
SearchItemList. \
ItemListContainer. \
ItemList. \
item %d list. \
ItemListSizingContainer. \
ItemListColumns.column %d. \
ColumnText"

//sizeof = 0x20
struct AHList
{
	DWORD pad_000[5];		// 0x000
	DWORD count;			// 0x014
	DWORD pad_018;			// 0x018
	DWORD addr_items;		// 0x01C
};

typedef std::vector<CAHItem> vecAHItems;

class CAuction
{
public:
	CAuction(void);
	~CAuction(void);

	/*
		Get items from current auc page

		IN:
			out - collection reference

		OUT:
			out - enumerated items

		RETURN:
			Error code
	*/
	DWORD GetItems(vecAHItems &out);

	/*
		Hover cursor over specified item

		IN:
			pos - item index

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SelectItem(int pos);

private:
	vecAHItems m_Items;			//Items collection
};

#endif//_AUCTION_H_