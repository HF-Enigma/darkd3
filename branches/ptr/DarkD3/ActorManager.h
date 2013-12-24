#ifndef _ACTOR_MANAGER_H_
#define _ACTOR_MANAGER_H_

//class CD3Actor;

#include "stdafx.h"
#include "Defines.h"
#include "Process.h"
#include "D3Actor.h"
#include "D3Player.h"

//Typedef for objects vector array
typedef std::vector<CD3Actor> vecD3Actors;

//class used in distance comparison
class Comparator
{
public:
	bool operator()(CD3Actor& t1, CD3Actor& t2)
	{
		return t1.distance() < t2.distance();
	}
};

//Class for managing actors
class CActorManager
{
public:
	CActorManager(void);
	~CActorManager(void);

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
	DWORD EnumActors( vecD3Actors* pRActors = NULL, vecD3Actors* pACDs = NULL );

	/*
		Enumerate actors through ACD

		IN:
			actors - output vector for ACDs

		OUT:
			actors - populated ACDs

		RETURN:
			Error code
	*/
	DWORD EnumACD(vecD3Actors *actors = NULL);

	/*
		Enumerate actors through RActor

		IN:
			pRActors - output vector for RActors

		OUT:
			pRActors - populated RActors

		RETURN:
			Error code
	*/
	DWORD EnumRActor(vecD3Actors* pRActors = NULL);
	
	/*
		Filters mobs from RActors

		IN:
			pRActors - output vector for Mobs

		OUT:
			pRActors - filtered Mobs

		RETURN:
			Error code
	*/
	DWORD FilterMobs(vecD3Actors& mobs);

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
	DWORD FilterDroppedItems(vecD3Actors& items, int qLvl = 1);

	/*
		Filters items in player backpack from ACDs

		IN:
			items - output vector for items

		OUT:
			items - filtered items

		RETURN:
			Error code
	*/
	DWORD FilterBackpackItems(vecD3Actors& items);

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
	DWORD SortByDistance(vecD3Actors& actors, bool bDescending = false);

	/*
		Get player

		IN:
			player - player actor reference

		OUT:
			player - player actor

		RETURN:
			Error code
	*/
	static 
	DWORD GetPlayer(CD3Player &player);


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
	CD3Actor* FindActor(DWORD dwGUID);

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
	CD3Actor* FindActor( std::string name, bool bIsProxy = true );
private:

	/*
		Link RActors with corresponding ACDs and vise versa

		IN:
			void

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD LinkActors();

private:
	vecD3Actors m_RActors;			//RActor collection
	vecD3Actors m_ACDs;				//ACD collection
};

#endif//_ACTOR_MANAGER_H_