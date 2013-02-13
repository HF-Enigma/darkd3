#include "ActorManager.h"

CActorManager::CActorManager(void)
{
}

CActorManager::~CActorManager(void)
{
}

/*
	Get player

	IN:
		player - player actor reference

	OUT:
		player - player actor

	RETURN:
		Error code
*/
DWORD CActorManager::GetPlayer( CD3Player &player )
{
	tContainer<CRActor> racts;
	tContainer<CACD> acds;

	CD3Player player_ract, player_acd;
	CObLocal local;
	CObDataContainer data;

	//Get local players struct
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Local, sizeof(local), &local));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Data + local.Index*0x7FF8, sizeof(data), &data));

	//Get RActor and ACD lists
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Actors, sizeof(racts), &racts));
	CHK_RES(CProcess::Instance().Core.Read(CProcess::Instance().Core.Read<DWORD>((DWORD)CGlobalData::Instance().ObjMgr.Storage.ACD), sizeof(acds), &acds));

	//Find RActor instance
	for (DWORD i = 0; i<= min(racts.Count, racts.Limit); i++)
	{
		//2nd level pointer
		DWORD dwOffset = CProcess::Instance().Core.Read<DWORD>((DWORD)racts.List) + i*racts.SizeOf;

		DWORD dwGUID = CProcess::Instance().Core.Read<DWORD>(dwOffset + FIELD_OFFSET(CRActor, id_acd));

		if(dwGUID == data.Data.id_acd)
		{
			player_ract = CD3Player(dwOffset, NULL, false);
			break;
		}
	}

	//Find ACD instance
	for (DWORD i = 0; i<= min(acds.Count, acds.Limit); i++)
	{
		//2nd level pointer
		DWORD dwOffset = CProcess::Instance().Core.Read<DWORD>((DWORD)acds.List) + i*acds.SizeOf;

		DWORD dwGUID = CProcess::Instance().Core.Read<DWORD>(dwOffset + FIELD_OFFSET(CACD, id_acd));

		if(dwGUID == data.Data.id_acd)
		{
			player_acd = CD3Player(dwOffset, NULL, true);
			break;
		}
	}
	
	//Link RActor with ACD
	memcpy(&player_ract.ACD, &player_acd.ACD, sizeof(CACD));

	player_ract.m_dwBaseACD = player_acd.m_dwBaseACD;

	player = player_ract;

	return ERROR_SUCCESS;
}

/*
	Enumerate actors through RActor and ACD

	IN:
		pRActors - output vector for RActors
		pACDs - output vector for ACDs

	OUT:
		pRActors - populated RActors
		pACDs - populated ACDs

	RETURN:
		Error code
*/
DWORD CActorManager::EnumActors( vecD3Actors* pRActors /*= NULL*/, vecD3Actors* pACDs /*= NULL*/ )
{
	CHK_RES(EnumRActor());
	CHK_RES(EnumACD());
	CHK_RES(LinkActors());

	//Refresh output after link
	if(pRActors)
		*pRActors = m_RActors;

	if(pACDs)
		*pACDs = m_ACDs;

	return ERROR_SUCCESS;
}

/*
	Enumerate actors through RActor

	IN:
		pRActors - output vector for RActors

	OUT:
		pRActors - populated RActors

	RETURN:
		Error code
*/
DWORD CActorManager::EnumRActor( vecD3Actors* pRActors /*= NULL*/ )
{
	tContainer<CRActor> objects;
	CD3Player player;

	m_RActors.clear();

	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Actors, sizeof(objects), &objects));
	CHK_RES(GetPlayer(player));

	for (DWORD i = 0; i<= min(objects.Count, objects.Limit); i++)
	{
		//2nd level pointer
		DWORD dwOffset = CProcess::Instance().Core.Read<DWORD>((DWORD)objects.List) + i*objects.SizeOf;

		m_RActors.push_back(CD3Actor(dwOffset, &player.RActor.Pos, false));
	}

	if(pRActors)
		pRActors[0] = m_RActors;

	return ERROR_SUCCESS;
}

/*
	Enumerate actors through ACD

	IN:
		actors - output vector for ACDs

	OUT:
		actors - populated ACDs

	RETURN:
		Error code
*/
DWORD CActorManager::EnumACD( vecD3Actors *actors/*= NULL*/ )
{
	tContainer<CACD> objects;
	CD3Player player;

	m_ACDs.clear();

	CHK_RES(CProcess::Instance().Core.Read(CProcess::Instance().Core.Read<DWORD>((DWORD)CGlobalData::Instance().ObjMgr.Storage.ACD), sizeof(objects), &objects));
	CHK_RES(GetPlayer(player));

	for (DWORD i = 0; i<= min(objects.Count, objects.Limit); i++)
	{
		//2nd level pointer
		DWORD dwOffset = CProcess::Instance().Core.Read<DWORD>((DWORD)objects.List) + i*objects.SizeOf;

		m_ACDs.push_back(CD3Actor(dwOffset, &player.RActor.Pos));
	}

	if(actors)
		actors[0] = m_ACDs;

	return ERROR_SUCCESS;
}

/*
	Link RActors with corresponding ACDs and vise versa

	IN:
		void

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CActorManager::LinkActors()
{
	for (DWORD i = 0; i< m_RActors.size(); i++)
	{
		for (DWORD j = 0; j< m_ACDs.size(); j++)
		{
			if((m_RActors[i].RActor.id_acd != INVALID_VALUE && m_RActors[i].RActor.id_acd != 0 && m_RActors[i].RActor.id_acd == m_ACDs[j].ACD.id_acd) ||
				(m_RActors[i].RActor.id_actor != INVALID_VALUE && m_RActors[i].RActor.id_actor != 0 && m_RActors[i].RActor.id_actor == m_ACDs[j].ACD.id_actor))
			{
				//Link objects with same guid
				memcpy(&m_RActors[i].ACD, &m_ACDs[j].ACD, sizeof(CACD));
				memcpy(&m_ACDs[j].RActor, &m_RActors[i].RActor, sizeof(CRActor));

				//Base addresses
				m_RActors[i].m_dwBaseACD = m_ACDs[j].m_dwBaseACD;
				m_ACDs[j].m_dwBaseRact = m_RActors[i].m_dwBaseRact;

				//Linkage flag
				m_RActors[i].m_bLinked = m_ACDs[j].m_bLinked = true;

				break;
			}
		}
	}

	return ERROR_SUCCESS;
}

/*
	Filters mobs from RActors

	IN:
		pRActors - output vector for Mobs

	OUT:
		pRActors - filtered Mobs

	RETURN:
		Error code
*/
DWORD CActorManager::FilterMobs(vecD3Actors& mobs)
{
	mobs.clear();

	for (DWORD i = 0; i< m_ACDs.size(); i++)
	{
		//Not a monster
		if(m_ACDs[i].type() != ActorType_Monster)
			continue;

		//Not a monster
		if(m_ACDs[i].MonsterSNO.Type != MobType_Demon && 
			m_ACDs[i].MonsterSNO.Type != MobType_Beast && 
			m_ACDs[i].MonsterSNO.Type != MobType_Undead && 
			m_ACDs[i].MonsterSNO.Type != MobType_Undead)
			continue; 

		//Invulnerable
		if(m_ACDs[i].GetAttribInt(Invulnerable) > 0)
			continue;

		//Dead
		if(m_ACDs[i].GetAttribFloat(Hitpoints_Cur) <= 0) 
			continue;

		//Actor is a valid Mob
		mobs.push_back(m_ACDs[i]);
	}

	return ERROR_SUCCESS;
}

/*
	Filters items on the ground from ACDs

	IN:
		items - output vector for items
		qLvl - minimum quality level for items to filter

	OUT:
		items - filtered items

	RETURN:
		Error code
*/
DWORD CActorManager::FilterDroppedItems( vecD3Actors& items, int qLvl /*= 1*/ )
{
	items.clear();

	for (DWORD i = 0; i< m_ACDs.size(); i++)
	{
		//Not an item
		if(m_ACDs[i].type() != ActorType_Item)
			continue;

		//Too low quality, not gold, not gem (no filter for HealthGlobe though)
		if(m_ACDs[i].GetAttribInt(Item_Quality_Level) < qLvl && m_ACDs[i].name_proxy().find("Gold") == std::string::npos && 
			m_ACDs[i].GetAttribInt(GemQuality) < 4)
			continue;

		//Already in inventory
		if(m_ACDs[i].ACD.itemLoc != ItemLoc_UnknownLoc)
			continue;

		//Invalid item
		if(m_ACDs[i].guid() == INVALID_VALUE)
			continue;

		//Actor is a valid item
		items.push_back(m_ACDs[i]);
	}

	return ERROR_SUCCESS;
}

/*
	Filters items in player backpack from ACDs

	IN:
		items - output vector for items

	OUT:
		items - filtered items

	RETURN:
		Error code
*/
DWORD CActorManager::FilterBackpackItems( vecD3Actors& items )
{
	items.clear();

	for (DWORD i = 0; i< m_ACDs.size(); i++)
	{
		//Not an item
		if(m_ACDs[i].type() != ActorType_Item)
			continue;

		//Not in backpack
		if(m_ACDs[i].ACD.itemLoc != ItemLoc_Backpack)
			continue;

		//Invalid item
		if(m_ACDs[i].guid() == INVALID_VALUE)
			continue;

		//Actor is a valid item
		items.push_back(m_ACDs[i]);
	}

	return ERROR_SUCCESS;
}


/*
	Sort actors by distance to player (ascending)

	IN:
		actors - vector to sort
		bDescending - descending sort (not used actually)

	OUT:
		actors - sorted actors

	RETURN:
		Error code
*/
DWORD CActorManager::SortByDistance( vecD3Actors& actors, bool bDescending /*= false*/ )
{
	Comparator comp;

	std::sort(actors.begin(), actors.end(), comp);

	return ERROR_SUCCESS;
}


/*
	Find actor by GUID

	IN:
		dwGUID - actor GUID

	OUT:
		void

	RETURN:
		Pointer to found actor
		NULL if nothing found
*/
CD3Actor* CActorManager::FindActor( DWORD dwGUID )
{
	//Find first matched object
	for (DWORD i = 0; i < m_ACDs.size(); i++)
		if(m_ACDs[i].guid() == dwGUID)
			return &m_ACDs[i];

	return NULL;
}

/*
	Find actor by name

	IN:
		name - actor name
		bIsProxy - actor name is proxy name

	OUT:
		void

	RETURN:
		Pointer to found actor
		NULL if nothing found
*/
CD3Actor* CActorManager::FindActor( std::string name, bool bIsProxy /*= true*/ )
{
	//Find first matched object
	for (DWORD i = 0; i < m_ACDs.size(); i++)
		if(bIsProxy)
		{
			if(m_ACDs[i].name_proxy().substr(0, m_ACDs[i].name_proxy().rfind("-")).compare(name) == 0)
				return &m_ACDs[i];
		}
		else
		{
			if(m_ACDs[i].name().compare(name) == 0)
				return &m_ACDs[i];
		}

	return NULL;
}