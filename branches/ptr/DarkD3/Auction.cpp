#include "Auction.h"


CAuction::CAuction(void)
{
}


CAuction::~CAuction(void)
{
}

/*
	Get items from current auc page

	IN:
		out - collection reference

	OUT:
		out - enumerated items

	RETURN:
		Error code
*/
DWORD CAuction::GetItems( vecAHItems &out )
{
	AHList list;
	DWORD dwBaseOffset = 0;

	dwBaseOffset = CProcess::Instance().Core.Read<DWORD>(AUC_BASE);
	dwBaseOffset = CProcess::Instance().Core.Read<DWORD>(dwBaseOffset);

	CHK_RES(CProcess::Instance().Core.Read(dwBaseOffset, sizeof(list), &list));

	m_Items.clear();

	for(DWORD i=0; i< list.count; i++)
	{
		AHUIHoverHeader dps;
		char name[64] = {0};
		char stats[1024] = {0};
		char sockets[3][96] = {0};

		//Get some info from UI elements
		UIComponent* pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_NAME_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(name), name);

		pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_DPS_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(dps), &dps);

		pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_ATTRIBS_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(stats), stats);

		pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_SOCKET_0_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(sockets[0]), sockets[0]);

		pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_SOCKET_1_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(sockets[1]), sockets[1]);

		pElement = CUIManager::Instance().GetUIElementByHash(UI_SELECTED_ITEM_SOCKET_2_HASH);

		if(pElement)
			CProcess::Instance().Core.Read(pElement->text_ptr, sizeof(sockets[2]), sockets[2]);

		//Add item to collection
		m_Items.push_back(CAHItem(list.addr_items + i*sizeof(AHItemRaw), name, dps, stats, sockets));
	}

	out = m_Items;

	return ERROR_SUCCESS;
}

DWORD CAuction::SelectItem(int pos)
{

	return ERROR_SUCCESS;
}
