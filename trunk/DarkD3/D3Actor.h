#ifndef _D3_ACTOR_H_
#define _D3_ACTOR_H_

#include "stdafx.h"
#include "Process.h"
#include "Structs.h"
#include "SNOManager.h"
#include "Attributes.h"

//D3 Actor class
class CD3Actor
{
public:
	CD3Actor(void);
	CD3Actor(DWORD dwBase, Vec3* playerpos = NULL, bool bACD = true);
	~CD3Actor(void);

	/*
		Set actor linked flag
		Indicates actor having both RActor and ACD structs
	*/
	void SetLinkFlag(bool bSet = true);	

	/*
		Get actor int attribute

		IN:
			attrib - attribute ID
			param - attribute parameter

		OUT:
			void

		RETURN:
			Attribute value
	*/
	int	GetAttribInt(AttributeID attrib, DWORD param = 0xFFFFF000);

	/*
		Get actor float attribute

		IN:
			attrib - attribute ID
			param - attribute parameter

		OUT:
			void

		RETURN:
			Attribute value
	*/
	float GetAttribFloat(AttributeID attrib, DWORD param = 0xFFFFF000);

	/*
		Get all valid actor attributes

		IN:
			out - container for attributes

		OUT:
			out - collected attributes

		RETURN:
			Error code
	*/
	DWORD GetAllAttribs(std::map<AttributeID, ATTRIB_INFO> &out);

	/*
		Sell item (if actor is an item)
	*/
	DWORD Sell();

	/*
		Return actor GUID
	*/
	DWORD guid();

	/*
		Return actor memory base address
	*/
	DWORD base();
	
	/*
		Return actor location
	*/
	Vec3 location();

	/*
		Return actor transformed name
	*/
	std::string name();

	/*
		Return actor game name
	*/
	std::string name_proxy();

	/*
		Return distance to player
	*/
	float distance();

	/*
		Return actor type
	*/
	ActorType type();

	/*
		Return monster type (if actor is a monster)
	*/
	MonsterType mobtype();

	/*
		Return monster race (if actor is a monster)
	*/
	MonsterRace mobrace();

	
	/*
		Returns squared distance between two points in 3D world

		IN:
			p1 - point1
			p2 - point2

		OUT:
			void

		RETURN:
			Distance squared
	*/
	static 
	float PointDistanceSquare( Vec3 p1, Vec3 p2 );

	/*
	*/
public:
	CRActor		RActor;				//RActor data
	CACD		ACD;				//ACD data
	SNOActor	ActorSNO;			//SNO data

	union
	{
		SNOGBItem ItemSNO;			//Item Game balance SNO
		SNOMonster MonsterSNO;		//Monster SNO
	};

protected:
	/*
		Get actor attributes group

		IN:
			pct - pointer to container
			guid - attrib group GUID
			group - found group reference

		OUT:
			group - found group

		RETURN:
			Error code
	*/
	DWORD		GetAttribGroup(tContainer<CAttribGroup>* pct, ULONG guid, CAttribGroup &group);

	/*
		Get actor attribute as raw 4-byte value

		IN:
			val - pointer to output value
			attrib - attribute ID
			param - attribute parameter

		OUT:
			val - attribute value

		RETURN:
			Error code
	*/
	DWORD		GetAttribRaw(BYTE val[4], AttributeID attrib, DWORD param = 0xFFFFF000);

protected:
	DWORD		m_dwBaseAddr;		//Actor base address
	bool		m_bLinked;			//Indicates that actor has both structs (ACD and RActor) after linking
	Vec3		m_pos;				//Actor world position
	float		m_dist;				//Distance from player
	std::string m_Name;				//Actor proxy name
	DWORD		m_dwGUID;			//Actor GUID
	ActorType	m_Type;				//Actor type
	MonsterRace	m_MobRace;			//Monster race
	MonsterType	m_MobType;			//Monster type
};

#endif//_D3_ACTOR_H_