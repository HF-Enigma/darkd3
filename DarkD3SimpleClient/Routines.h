#ifndef _ROUTINES_H_
#define _ROUTINES_H_

#include "stdafx.h"
#include "../DarkD3/Process.h"
#include "../DarkD3/D3Window.h"
#include "../DarkD3/UIManager.h"
#include "../DarkD3/ActorManager.h"
#include "../DarkD3/Auction.h"
#include "../DarkD3/SceneManager.h"
#include "../DarkD3/SNOManager.h"


#define HASH_LOGIN_FIELD	0xde8625fccffdfc28
#define HASH_PASS_FIELD		0xba2d3316b4bb4104
#define HASH_LOGIN_BTN		0x50893593b5db22a9
#define HASH_AH_BTN			0xC66282304743ECB4
#define HASH_EQUIP_BTN		0x85fe1f4a7c2a9a9c
#define HASH_SEARCH_BTN		0x27fa61f93f895e08
#define HASH_CH_QUEST_BTN	0x0c4a9cc94c0a929b
#define HASH_DIFF_BOX		0x24c050d154bb1689
#define HASH_QUEST_BTN		0xf60e9396faec1cdf
#define HASH_SUBQUEST_BTN	0x1523a2c96e5dfec9
#define HASH_ACCEPT_BTN		0x1ae2209980aaea69
#define HASH_CONFIRM_BTN	0xb4433da3f648a992
#define HASH_START_BTN		0x51a3923949dc80b7

using namespace Gdiplus;

class CGameManager
{
public:
	CGameManager() { };
	~CGameManager() { };
	 
    DWORD AttachToGame();
    void GoToNeighbor();
    void DoLogin(std::string Login, std::string Pass);
	bool GetD3process(DWORD &pid, HWND &hWnd);
	DWORD DrawScenes(RECT &rc, Graphics &g);
    bool OperateAH(vecAHItems &ahItems);
    DWORD PwnMobs(float distance);
	DWORD Loot(float distance);
	void SetQuest(int questNo, int subquestNo);
    void BuildSnoDB(std::string path);
    void EnterGame();
	DWORD MoveToWindowPoint( HWND hwnd, POINT &pt );
private:
	CSceneManager smgr;
	CActorManager amgr;
	CAuction auc;

	CD3Player player;
};

#endif//_ROUTINES_H_