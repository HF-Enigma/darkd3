// DarkD3SimpleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Routines.h"
#include "../DarkD3/Camera.h"


bool CGameManager::GetD3process(DWORD &pid, HWND &hWnd)
{
    hWnd = FindWindow(NULL, _T("Diablo III"));

    if(hWnd)
    {
        GetWindowThreadProcessId(hWnd, &pid);
        return true;
    }
    else
        return false;
}

DWORD CGameManager::AttachToGame()
{
    DWORD pid = 0;
    HWND hWnd;

    //Search for D3 window
    if(!GetD3process(pid, hWnd))
        return 0;

    //Attach to game process
    CProcess::Instance().Attach(pid);
    CWindow::Instance().Attach(hWnd);

	CSNOManager::Instance().LoadDB();

	//CCamera::Instance().SetZoom(0);
    return pid;
}

void CGameManager::SetQuest( int questNo, int subquestNo )
{
	ds_utils::CDSString strQuestName, strSubQuestName;

	const wchar_t* pTemplateQuest = L"Root.NormalLayer.BattleNetQuestSelection_main.LayoutRoot.OverlayContainer.QuestMenu.NavigationMenuList._content._stackpanel._item%d";
	const wchar_t* pTemplateSubQuest = L"Root.NormalLayer.BattleNetQuestSelection_main.LayoutRoot.OverlayContainer.QuestMenu.NavigationMenuList._content._stackpanel._item%d.NavigationMenuListItemContainer.MenuListItem.MenuSubItem.MenuSubList._content._stackpanel._item%d";

	strQuestName.format(pTemplateQuest, questNo);
	strSubQuestName.format(pTemplateSubQuest, questNo, subquestNo);

	mapUIElements elems, elems2;


	CUIManager::Instance().EnumUI();

    //Open quest window
    UIComponent *pChQuestBtn = CUIManager::Instance().GetUIElementByHash(HASH_CH_QUEST_BTN);
    if(pChQuestBtn)
    {
        CUIManager::Instance().ClickElement(*pChQuestBtn);
		CUIManager::Instance().EnumUI();

        //Set normal difficulty
        UIComponent *pDiffBox = CUIManager::Instance().GetUIElementByHash(HASH_DIFF_BOX);
        if(pDiffBox)
			CUIManager::Instance().SetCBIndex(*pDiffBox, 0);

        //Select quest and subquest
        UIComponent *pQuestBtn = CUIManager::Instance().GetUIElementByName(strQuestName.data_mb());
        if(pQuestBtn)
        {
            CUIManager::Instance().ClickElement(*pQuestBtn);

			UIComponent *pSubQuestBtn = CUIManager::Instance().GetUIElementByName(strSubQuestName.data_mb());
			if(pSubQuestBtn)
				CUIManager::Instance().ClickSPElement(*pSubQuestBtn);
        }

        //Accept change
        UIComponent *pAcceptBtn = CUIManager::Instance().GetUIElementByHash(HASH_ACCEPT_BTN);
        if(pAcceptBtn)
		{
            CUIManager::Instance().ClickElement(*pAcceptBtn);
			CUIManager::Instance().EnumUI();

			//Confirm change
			UIComponent *pConfirmBtn = CUIManager::Instance().GetUIElementByHash(HASH_CONFIRM_BTN);
			if(pConfirmBtn)
				CUIManager::Instance().ClickElement(*pConfirmBtn);
		}
    }
}

void CGameManager::EnterGame()
{
    UIComponent *pStartGameBtn = CUIManager::Instance().GetUIElementByHash(HASH_START_BTN);
    if(pStartGameBtn)
    {
        //Start game
        CUIManager::Instance().ClickElement(*pStartGameBtn);

        DWORD res = CActorManager::GetPlayer(player);

        //Wait for player
        while(res != ERROR_SUCCESS)
        {
            Sleep(1000);
            res = CActorManager::GetPlayer(player);
        }
        Sleep(100);
    }
}

void CGameManager::BuildSnoDB(std::string path)
{
    //Build initial database
    CSNOManager::Instance().BuildDBFromFile(path);
}

void CGameManager::GoToNeighbor()
{
    smgr.EnumScenes();

    CActorManager::GetPlayer(player);

    //Get player scene
    CD3Scene *pScene = smgr.GetSceneByCoords(player.location(), player.RActor.guid_world);

    if(pScene)
    {
        std::vector<NavCell> cells;

        //Get plater cell
        NavCell *pCell = pScene->GetCellByCoords(player.location());

        //Get neighbor cells
        if(pCell)
        {
            pScene->GetCellNeighbours(*pCell, cells);

            //Move to first neighbor cell if any
            if(cells.size() > 0)
                player.ClickToMove((cells.begin()->Max - cells.begin()->Min)/2);
        }
    }
}

void CGameManager::DoLogin(std::string Login, std::string Pass)
{
    UIComponent *ploginField = CUIManager::Instance().GetUIElementByHash(HASH_LOGIN_FIELD);
    UIComponent *pPassField = CUIManager::Instance().GetUIElementByHash(HASH_PASS_FIELD);
    UIComponent *pLoginBtn = CUIManager::Instance().GetUIElementByHash(HASH_LOGIN_BTN);

    if(!ploginField || !pPassField || !pLoginBtn)
        return;

    //Fill login fields and login
    CUIManager::Instance().SetText(*ploginField, Login);
    CUIManager::Instance().SetText(*pPassField, Pass);
    CUIManager::Instance().ClickElement(*pLoginBtn);

    return;
}

bool CGameManager::OperateAH( vecAHItems &ahItems )
{
	mapUIElements elems;

    CUIManager::Instance().EnumUI();

    ahItems.clear();

    UIComponent *pAhBtn = CUIManager::Instance().GetUIElementByHash(HASH_AH_BTN);

    if(!pAhBtn)
        return false;

	//Open AH
	CUIManager::Instance().ClickElement(*pAhBtn);

	Sleep(10);

	CUIManager::Instance().EnumUI();

	//Get "Equipment" button
	UIComponent *pItemsBtn = CUIManager::Instance().GetUIElementByHash(HASH_EQUIP_BTN);

	if(!pItemsBtn)
		return false;

	CUIManager::Instance().ClickElement(*pItemsBtn);

	Sleep(10);

	CUIManager::Instance().EnumUI();

	//Get "Search" button
	UIComponent *pSearchBtn = CUIManager::Instance().GetUIElementByHash(HASH_SEARCH_BTN);

	if(!pSearchBtn)
		return false;

	//Search items
	CUIManager::Instance().ClickElement(*pSearchBtn);

	Sleep(500);

	//Get items on first page
	auc.GetItems(ahItems);

	//Close AH
	pAhBtn = CUIManager::Instance().GetUIElementByHash(HASH_AH_BTN);
	CUIManager::Instance().ClickElement(*pAhBtn);

	return true;
}


DWORD CGameManager::PwnMobs( float distance )
{
	vecD3Actors mobs;
    std::vector<POWER> powers;
    std::map<AttributeID, ATTRIB_INFO> atbs;

    CActorManager::GetPlayer(player);

    //Get all player attributes
    player.GetAllAttribs(atbs);

    //Get player skills
    player.GetPowers(powers);

    //Cast buff on self
    player.UsePower(player, Monk_MantraOfConviction);

	for(;;)
	{
		mobs.clear();

		//Get mob list
		amgr.EnumActors();
		amgr.FilterMobs(mobs);
		amgr.SortByDistance(mobs);

		//No mobs or not in range
		if(mobs.empty() || mobs[0].distance() > distance*distance)
		{
			//Wait for loot to drop from last mob
			Sleep(500);

			Loot(distance);
			break;
		}

		amgr.GetPlayer(player);
		player.UsePower(mobs[0], Monk_FistsofThunder);
	}

	return ERROR_SUCCESS;
}

DWORD CGameManager::Loot(float distance)
{
	vecD3Actors items;

	for(;;)
	{
		items.clear();

		//Get items list
		amgr.EnumACD();
		amgr.FilterDroppedItems(items, 3);		//Magic and better
		amgr.SortByDistance(items);

		//No items or not in range
		if(items.empty() || items[0].distance() > distance*distance)
			break;

		amgr.GetPlayer(player);

		//Go to
		player.UsePower(items[0], Axe_Operate_Gizmo);

		Sleep(10);

		//Pickup
		player.UsePower(items[0], Axe_Operate_Gizmo);
	}

	return ERROR_SUCCESS;
}

DWORD CGameManager::MoveToWindowPoint( HWND hwnd, POINT &pt )
{
    RECT rc;
    int width = 0;
	mapScenes *pScenes;
    Vec3 PosInWorld;

    CGlobalData::Instance().RefreshOffsets();

    CHK_RES(amgr.GetPlayer(player));

	if((pScenes = smgr.GetWorldScenes(player.RActor.guid_world)) == NULL)
		return ERROR_NOT_FOUND;

    AABB bounds = smgr.GetScenesLimits(player.RActor.guid_world);

    GetWindowRect(hwnd, &rc);

    width = rc.right - rc.left - 20;

    float mult = min((width + 5) / (bounds.Max.x - bounds.Min.x), (rc.bottom - rc.top - 40) / (bounds.Max.y - bounds.Min.y));

    //Window coordinates to scene coordinates
    PosInWorld.x = bounds.Min.x + (width - pt.x)/mult;
    PosInWorld.y = bounds.Min.y + pt.y / mult;
    PosInWorld.z = 0;

    //Get scene containing point
    CD3Scene *pScene = smgr.GetSceneByCoords(PosInWorld, player.RActor.guid_world);

    if(pScene)
    {
        NavCell* pCell = pScene->GetCellByCoords(PosInWorld);

        //Cell is walkable
        if(pCell && pCell->Flag & NavCellFlagW_AllowWalk)
        {
            PosInWorld.z = (pCell->Max.z + pCell->Min.z) / 2;

            player.ClickToMove(PosInWorld);

			InvalidateRect(hwnd, &rc, TRUE);
        }
    }

	return ERROR_SUCCESS;
}


DWORD CGameManager::DrawScenes( RECT &rc, Graphics &g )
{
	int width = 0;
	mapScenes *pScenes;
	vecD3Actors mobs;

	//CGlobalData::Instance().RefreshOffsets();

	CHK_RES(amgr.GetPlayer(player));

	smgr.EnumScenes();
	if((pScenes = smgr.GetWorldScenes(player.ACD.id_world)) == NULL)
		return ERROR_NOT_FOUND;

	CD3Scene *pScene = smgr.GetSceneByCoords(player.location(), player.ACD.id_world);
	std::vector<NavCell> cells;

	if(pScene)
		NavCell* pCell = pScene->GetCellByCoords(player.location());

	AABB bounds = smgr.GetScenesLimits(player.ACD.id_world);

	width = rc.right - rc.left - 20;

	float mult = min((width + 5) / (bounds.Max.x - bounds.Min.x), (rc.bottom - rc.top - 40) / (bounds.Max.y - bounds.Min.y));

	Pen			BlackPen(Color::Black, 1.0f);
	Font		ArialFont(_T("Arial"), 8);

	SolidBrush	GreenBrush(Color::Green);
	SolidBrush	BkgndBrush(Color::DarkRed);
	SolidBrush	MobBrush(Color::OrangeRed);
	SolidBrush	BlackBrush(Color::Black);
	SolidBrush	BlueBrush(Color::Blue);

	g.Clear(Color::Gray);

	for(mapScenes::iterator i = pScenes->begin(); i != pScenes->end(); i++)
	{
		//Draw scene contents
		if(i->second.SceneRawData.id_scene != INVALID_VALUE)
		{
			//Scene offset on screen
			Vec3 scene_off = {i->second.SceneRawData.navmesh.MeshMin.x - bounds.Min.x, i->second.SceneRawData.navmesh.MeshMin.y - bounds.Min.y, 0};
			size_t pos;

			Rect scn_rect(  (int)(width - (i->second.SceneRawData.navmesh.MeshMax.x*mult - i->second.SceneRawData.navmesh.MeshMin.x*mult + scene_off.x*mult)),
							(int)(scene_off.y*mult),
							(int)((i->second.SceneRawData.navmesh.MeshMax.x - i->second.SceneRawData.navmesh.MeshMin.x)*mult),
							(int)((i->second.SceneRawData.navmesh.MeshMax.y - i->second.SceneRawData.navmesh.MeshMin.y)*mult));

			g.FillRectangle(&BkgndBrush, scn_rect);
			g.DrawRectangle(&BlackPen, scn_rect);

			//Draw walkable cells
			for(DWORD j = 0; j < i->second.Cells.size(); j++)
			{
				Rect rect(  (int)(width - (i->second.Cells[j].Max.x + scene_off.x)*mult),
							(int)((i->second.Cells[j].Min.y + scene_off.y)*mult),
							(int)((i->second.Cells[j].Max.x - i->second.Cells[j].Min.x)*mult),
							(int)((i->second.Cells[j].Max.y - i->second.Cells[j].Min.y)*mult));

				//Draw cell
				if(i->second.Cells[j].Flag & NavCellFlagW_AllowWalk)
				{
					g.FillRectangle(&GreenBrush, rect);
					g.DrawRectangle(&BlackPen, rect);
				}
			}	
		}
	}

	Rect player_rect(   (int)(width - (player.location().x - bounds.Min.x)*mult),
						(int)((player.location().y - bounds.Min.y)*mult),
						8, 8);

	//Player location
	g.FillEllipse(&BlueBrush, player_rect);

	amgr.EnumActors();
	amgr.FilterMobs(mobs);

	//Draw monsters
	for(DWORD i = 0; i < mobs.size(); i++)
	{
		Rect mob_rect(  (int)(width - (mobs[i].ACD.PosWorld.x - bounds.Min.x)*mult),
						(int)((mobs[i].ACD.PosWorld.y - bounds.Min.y)*mult),
						4,4);

		g.FillEllipse(&MobBrush, mob_rect);
    }

	return ERROR_SUCCESS;
}

DWORD CALLBACK RedrawWatch(LPVOID lpParam)
{
	CGameManager* pClass = (CGameManager*)lpParam;

	while(pClass->bWatchActive)
	{
		static DWORD LastRedraw = 0;

		//Redraw every 33ms (30 fps)
		if(hMapDlg && GetTickCount() - LastRedraw > 33)
		{
			InvalidateRect(hMapDlg, NULL, FALSE);
			UpdateWindow(hMapDlg);
			LastRedraw = GetTickCount();
		}

		Sleep(1);
	}

	return ERROR_SUCCESS;
}