// DarkD3SimpleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Routines.h"

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

    return pid;
}

void CGameManager::SetQuest()
{
    //Set current quest
    UIComponent *pChQuestBtn = CUIManager::Instance().GetUIElementByHash(HASH_CH_QUEST_BTN);
    if(pChQuestBtn)
    {
        CUIManager::Instance().ClickElement(*pChQuestBtn);

        //Set normal difficulty
        UIComponent *pDiffBox = CUIManager::Instance().GetUIElementByHash(HASH_DIFF_BOX);
        if(pDiffBox)
            CUIManager::Instance().SetCBIndex(*pDiffBox, 0);

        //Select quest and subquest
        UIComponent *pQuestBtn = CUIManager::Instance().GetUIElementByHash(HASH_QUEST_BTN);
        UIComponent *pSubQuestBtn = CUIManager::Instance().GetUIElementByHash(HASH_SUBQUEST_BTN);
        if(pQuestBtn && pQuestBtn)
        {
            CUIManager::Instance().ClickElement(*pQuestBtn);
            CUIManager::Instance().ClickElement(*pSubQuestBtn);
        }

        //Accept change
        UIComponent *pAcceptBtn = CUIManager::Instance().GetUIElementByHash(HASH_ACCEPT_BTN);
        if(pAcceptBtn)
            CUIManager::Instance().ClickElement(*pAcceptBtn);
    }
}

void CGameManager::EnterGame()
{
    UIComponent *pStartGameBtn = CUIManager::Instance().GetUIElementByHash(HASH_START_BTN);
    if(pStartGameBtn)
    {
        //Start game
        CUIManager::Instance().ClickElement(*pStartGameBtn);

        CActorManager::GetPlayer(player);

        //Wait for player
        while(!player.valid())
        {
            Sleep(1000);
            CActorManager::GetPlayer(player);
        }
        Sleep(100);
    }
}

void CGameManager::BuildSnoDB(std::string path)
{
    //Build initial database
    //if(CSNOManager::Instance().LoadDB() != ERROR_SUCCESS)
    CSNOManager::Instance().BuildDBFromFile(path);
}

void CGameManager::GoToNeighbor()
{
    smgr.EnumScenes();

    CActorManager::GetPlayer(player);

    //Get player scene
    CD3Scene *pScene = smgr.GetActorScene(player.location());

    if(pScene)
    {
        std::vector<NavCell> cells;

        //Get plater cell
        NavCell *pCell = pScene->GetActorCell(player.location());

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
    CUIManager::Instance().EnumUI(NULL, true);

    ahItems.clear();

    UIComponent *pAhBtn = CUIManager::Instance().GetUIElementByHash(HASH_AH_BTN);

    if(!pAhBtn)
        return false;

	//Open AH
	CUIManager::Instance().ClickElement(*pAhBtn);

	//Get "Equipment" button
	UIComponent *pItemsBtn = CUIManager::Instance().GetUIElementByHash(HASH_EQUIP_BTN);

	if(!pItemsBtn)
		return false;

	CUIManager::Instance().ClickElement(*pItemsBtn);

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
		player.UsePower(mobs[0], powers[0].id);
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
		amgr.FilterDroppedItems(items, 3);
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

void CGameManager::MoveToWindowPoint( HWND hwnd, POINT &pt )
{
    RECT rc;
    int width = 0;
    vecScenes scenes;
    Vec3 PosInWorld;

    CGlobalData::Instance().RefreshOffsets();

    if(amgr.GetPlayer(player) != ERROR_SUCCESS)
        return;

    smgr.EnumScenes(&scenes);

    AABB bounds = smgr.GetScenesLimits();

    GetWindowRect(hwnd, &rc);

    width = rc.right - rc.left;

    float mult = min((width - 5) / (bounds.Max.x - bounds.Min.x), (rc.bottom - rc.top - 5) / (bounds.Max.y - bounds.Min.y));

    //Window coordinates to scene coordinates
    PosInWorld.x = bounds.Min.x + (width - pt.x)/mult;
    PosInWorld.y = bounds.Min.y + pt.y / mult;
    PosInWorld.z = 0;

    //Get scene containing point
    CD3Scene *pScene = smgr.GetActorScene(PosInWorld);

    if(pScene)
    {
        NavCell* pCell = pScene->GetActorCell(PosInWorld);

        //Cell is walkable
        if(pCell->Flag & NavCellFlagW_AllowWalk)
        {
            PosInWorld.z = (pCell->Max.z + pCell->Min.z) / 2;

            player.ClickToMove(PosInWorld);
        }
    }
}


void CGameManager::DrawScenes( HWND hwnd, Graphics &g )
{
	RECT rc;
	int width = 0;
	vecScenes scenes;
	vecD3Actors mobs;

	CGlobalData::Instance().RefreshOffsets();

	if(amgr.GetPlayer(player) != ERROR_SUCCESS)
		return;

	smgr.EnumScenes(&scenes);

	CD3Scene *pScene = smgr.GetActorScene(player.location());
	std::vector<NavCell> cells;

	if(pScene)
		NavCell* pCell = pScene->GetActorCell(player.location());

	AABB bounds = smgr.GetScenesLimits();

	GetWindowRect(hwnd, &rc);

	width = rc.right - rc.left;

	float mult = min((width - 5) / (bounds.Max.x - bounds.Min.x), (rc.bottom - rc.top - 5) / (bounds.Max.y - bounds.Min.y));

	Pen			BlackPen(Color::Black, 1.0f);
	Font		ArialFont(_T("Arial"), 8);

	SolidBrush	GreenBrush(Color::Green);
	SolidBrush	BkgndBrush(Color::DarkRed);
	SolidBrush	MobBrush(Color::OrangeRed);
	SolidBrush	BlackBrush(Color::Black);
	SolidBrush	BlueBrush(Color::Blue);

	g.Clear(Color::Gray);

	for(DWORD i = 0; i < scenes.size(); i++)
	{
		//Draw scene contents
		if(scenes[i].SceneRawData.id_scene != INVALID_VALUE)
		{
			//Scene offset on screen
			Vec3 scene_off = {scenes[i].SceneRawData.navmesh.MeshMin.x - bounds.Min.x, scenes[i].SceneRawData.navmesh.MeshMin.y - bounds.Min.y, 0};
			std::wstring strName;
			size_t pos;

			strName.reserve(sizeof(scenes[i].SceneSNO.navmesh.name));

			MultiByteToWideChar(CP_ACP, 0, scenes[i].SceneSNO.navmesh.name, 255, (LPWSTR)strName.c_str(), strName.capacity());

			Rect scn_rect(  (int)(width - (scenes[i].SceneRawData.navmesh.MeshMax.x*mult - scenes[i].SceneRawData.navmesh.MeshMin.x*mult + scene_off.x*mult)),
							(int)(scene_off.y*mult),
							(int)((scenes[i].SceneRawData.navmesh.MeshMax.x - scenes[i].SceneRawData.navmesh.MeshMin.x)*mult),
							(int)((scenes[i].SceneRawData.navmesh.MeshMax.y - scenes[i].SceneRawData.navmesh.MeshMin.y)*mult));

			g.FillRectangle(&BkgndBrush, scn_rect);
			g.DrawRectangle(&BlackPen, scn_rect);

			//Draw walkable cells
			for(DWORD j = 0; j < scenes[i].Cells.size(); j++)
			{
				Rect rect(  (int)(width - (scenes[i].Cells[j].Max.x + scene_off.x)*mult),
							(int)((scenes[i].Cells[j].Min.y + scene_off.y)*mult),
							(int)((scenes[i].Cells[j].Max.x - scenes[i].Cells[j].Min.x)*mult),
							(int)((scenes[i].Cells[j].Max.y - scenes[i].Cells[j].Min.y)*mult));

				//Draw cell
				if(scenes[i].Cells[j].Flag & NavCellFlagW_AllowWalk)
				{
					g.FillRectangle(&GreenBrush, rect);
					g.DrawRectangle(&BlackPen, rect);
				}
			}	

			//Get scene name
			pos = strName.rfind(L"/");

			if(pos == std::string::npos)
				pos = strName.rfind(L"\\");

			strName = strName.substr(pos + 1, std::string::npos);
			strName = strName.substr(0, strName.rfind(L"."));

			//Draw Scene name
			g.DrawString
				(	
					strName.c_str(), 
					strName.length(), 
					&ArialFont, 
					PointF
						(
							width - ((scenes[i].SceneRawData.navmesh.MeshMax.x - scenes[i].SceneRawData.navmesh.MeshMin.x + scene_off.x)*mult),
							scene_off.y*mult
						), 
					&MobBrush
				);
		}
	}

	Rect player_rect(   (int)(width - (player.location().x - bounds.Min.x)*mult),
						(int)((player.location().y - bounds.Min.y)*mult),
						(int)(6*mult),
						(int)(6*mult));

	//Player location
	g.FillEllipse(&BlueBrush, player_rect);

	amgr.EnumActors();
	amgr.FilterMobs(mobs);

	//Draw monsters
	for(DWORD i = 0; i < mobs.size(); i++)
	{
		Rect mob_rect(  (int)(width - (mobs[i].RActor.Pos.x - bounds.Min.x)*mult),
						(int)((mobs[i].RActor.Pos.y - bounds.Min.y)*mult),
						(int)(4*mult),
						(int)(4*mult));

		g.FillEllipse(&MobBrush, mob_rect);
    }

	return;
}