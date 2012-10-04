#include "D3Actor.h"

CD3Actor::CD3Actor(void):
	m_dwBaseRact(0),
	m_dwGUID(0),
	m_dist(0),
	m_bLinked(false),
	m_MobRace(MobRace_Unknown),
	m_MobType(MobType_Unknown),
	m_Type(ActorType_Invalid)
{
	RtlZeroMemory(&RActor, sizeof(RActor));
	RtlZeroMemory(&ACD, sizeof(ACD));
	RtlZeroMemory(&ActorSNO, sizeof(ActorSNO));
	RtlZeroMemory(&ItemSNO, sizeof(ItemSNO));
}

CD3Actor::CD3Actor( DWORD dwBase, Vec3* playerpos /*= NULL*/, bool bACD/*= true*/ ):
	m_dist(0),
	m_bLinked(false),
	m_MobRace(MobRace_Unknown),
	m_MobType(MobType_Unknown),
	m_Type(ActorType_Invalid)
{
	DWORD SnoID;

	RtlZeroMemory(&RActor, sizeof(RActor));
	RtlZeroMemory(&ACD, sizeof(ACD));
	RtlZeroMemory(&ActorSNO, sizeof(ActorSNO));
	RtlZeroMemory(&ItemSNO, sizeof(ItemSNO));

	//Actor is based on ACD
	if(bACD)
	{
		CProcess::Instance().Core.Read(dwBase, sizeof(ACD), &ACD);

		m_dwBaseACD = dwBase;
		m_dwGUID	= ACD.id_acd;
		SnoID		= ACD.id_sno;
		m_Name		= ACD.name;
		m_pos		= ACD.PosWorld;

		if(playerpos != NULL)
			m_dist = PointDistanceSquare(ACD.PosWorld, *playerpos);
	}
	//Actor is based on RActor
	else
	{
		CProcess::Instance().Core.Read(dwBase, sizeof(RActor), &RActor);

		m_dwBaseRact= dwBase;
		m_dwGUID	= RActor.id_acd;
		SnoID		= RActor.id_sno;
		m_Name		= RActor.Name;
		m_pos		= RActor.Pos;
		
		if(playerpos != NULL)
			m_dist = PointDistanceSquare(RActor.Pos, *playerpos);
	}

	//Get actor SNO
	if(CSNOManager::Instance().Actors.find(SnoID) != CSNOManager::Instance().Actors.end())
	{
		ActorSNO = CSNOManager::Instance().Actors[SnoID];

		//Get corresponding monster or item SNO
		switch(ActorSNO.type)
		{
			case ActorType_Item:
				{
					if(ACD.gb_type == GBType_Items)
						ItemSNO = CSNOManager::Instance().Items[ACD.id_gb];
				}
				break;

			case ActorType_Monster:
				{
					if(CSNOManager::Instance().Mobs.find(ActorSNO.MonsterSNO) != CSNOManager::Instance().Mobs.end())
						MonsterSNO = CSNOManager::Instance().Mobs[ActorSNO.MonsterSNO];
				}
				break;
		}
	}
}

CD3Actor::~CD3Actor(void)
{
}

/*
	Sell item (if actor is an item)
*/
DWORD CD3Actor::Sell()
{
#if (RCALL_TYPE == USE_DLL)
	CALL call = {0};
	CALL_RET ret = {0};

	if(ActorSNO.type != ActorType_Item)
		return ERROR_INVALID_OPERATION;

	call.valid = VALID_CALL;
	call.type = CallType_SellItem;
	call.state = CallState_Pending;
	call.arg1 = m_dwBaseRact;

	CProcess::Instance().shared.DoCall(call, ret);

	return ERROR_SUCCESS;
#else
	return ERROR_NOT_SUPPORTED;
#endif
}

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
DWORD CD3Actor::GetAttribGroup(tContainer<CAttribGroup>* pct, ULONG guid, CAttribGroup &group) 
{ 
	if(guid == INVALID_VALUE || !pct) 
		return ERROR_NOT_FOUND; 

	USHORT id = (guid & 0xFFFF); 

	if(id > pct->Limit) 
		return ERROR_NOT_FOUND; 

	USHORT l = (id >> pct->Bits); 
	USHORT e = id & ((1 << pct->Bits) - 1); 

	DWORD dwFirst = CProcess::Instance().Core.Read<DWORD>((DWORD)pct->List + l*sizeof(pct->List));
	DWORD dwSecond = dwFirst + pct->SizeOf * e;

	CHK_RES(CProcess::Instance().Core.Read(dwSecond, sizeof(CAttribGroup), &group));

	if(group.id_attrib == guid)
		return ERROR_SUCCESS;
	else
		return ERROR_NOT_FOUND;
} 

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
int CD3Actor::GetAttribInt( AttributeID attrib, DWORD param /*= 0xFFFFF000*/ )
{
	BYTE data[4];

	if(GetAttribRaw(data, attrib, param) == ERROR_SUCCESS)
		return *((int*)data);
	else
		return -1;
}

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
float CD3Actor::GetAttribFloat( AttributeID attrib, DWORD param /*= 0xFFFFF000*/ )
{
	BYTE data[4];

	if(GetAttribRaw(data, attrib, param) == ERROR_SUCCESS)
		return *((float*)data);
	else
		return -1;
}

/*
	Get all valid actor attributes

	IN:
		out - container for attributes

	OUT:
		out - collected attributes

	RETURN:
		Error code
*/
DWORD CD3Actor::GetAllAttribs( std::map<AttributeID, ATTRIB_INFO> &out )
{
	CAttribGroupsContainer		container;
	tContainer<CAttribGroup>	attribs;
	CAttribGroup				group;
	CAttribFormula				formula;
	tAttribLink					link;

	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.AttribGroups, sizeof(container), &container));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)container.FastAttribGroups, sizeof(attribs), &attribs));

	if(ACD.id_attrib == (DWORD)INVALID_VALUE || GetAttribGroup(&attribs, ACD.id_attrib, group) != ERROR_SUCCESS)
		return ERROR_INVALID_PARAMETER;

	CHK_RES(CProcess::Instance().Core.Read((DWORD)group.Formula, sizeof(formula), &formula));

	for(DWORD x = 0; x < formula.Map.Count; x++) 
	{ 
		//If map value is invalid
		if(CProcess::Instance().Core.Read((DWORD)formula.Map.Data[x], sizeof(link), &link) != ERROR_SUCCESS)
			continue;

		//Attribute ID
		ULONG id = link.AttribIndex & 0xFFF; 

		if(CGlobalData::Instance().Attributes.find((AttributeID)id) != CGlobalData::Instance().Attributes.end())
		{
			ATTRIB_INFO info;

			info.id = (AttributeID)id;
			info.type = CGlobalData::Instance().Attributes[(AttributeID)id].type;
			info.name = CGlobalData::Instance().Attributes[(AttributeID)id].name;

			//Get linked list values
			for(;;) 
			{ 
				AttribVal val;

				memcpy(&val.ival, &link.Value, sizeof(link.Value));

				if(info.values.find(link.AttribIndex >> 12) == info.values.end())
					info.values[link.AttribIndex >> 12] = val;
				
				if(CProcess::Instance().Core.Read((DWORD)link.Next, sizeof(link), &link) != ERROR_SUCCESS)
					break;
			} 

			//Update attribute if it was already found
			if(out.find(info.id) == out.end())
				out[info.id] = info;
			else
				out[info.id].values.insert(info.values.begin(), info.values.end());
		}
	} 

	return ERROR_SUCCESS;
}

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
DWORD CD3Actor::GetAttribRaw( BYTE val[4], AttributeID attrib, DWORD param /*= 0xFFFFF000*/ )
{
	CAttribGroupsContainer		container;
	tContainer<CAttribGroup>	attribs;
	CAttribGroup				group;
	CAttribFormula				formula;
	tAttribLink					link;

	CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.AttribGroups, sizeof(container), &container);
	CProcess::Instance().Core.Read((DWORD)container.FastAttribGroups, sizeof(attribs), &attribs);

	if(ACD.id_attrib == (DWORD)INVALID_VALUE || GetAttribGroup(&attribs, ACD.id_attrib, group) != ERROR_SUCCESS)
		return ERROR_INVALID_PARAMETER;

	DWORD attribMask = (DWORD)attrib | param;
	DWORD IndexMask  = attribMask ^ (attribMask >> 16);

	CHK_RES(CProcess::Instance().Core.Read((DWORD)group.Formula, sizeof(formula), &formula));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)formula.Map.Data[formula.Map.Mask & IndexMask], sizeof(link), &link));

	//First list node
	if(link.AttribIndex == (LONG)attribMask)
	{
		memcpy(val, &link.Value, sizeof(val));

		return ERROR_SUCCESS;
	}

	//Try to get value from other list nodes
	while(link.Next)
	{
		if(CProcess::Instance().Core.Read((DWORD)link.Next, sizeof(link), &link) != ERROR_SUCCESS)
			return ERROR_INVALID_PARAMETER;

		if(link.AttribIndex == (LONG)attribMask)
		{
			memcpy(val, &link.Value, sizeof(val));

			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_PARAMETER;
}

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
float CD3Actor::PointDistanceSquare( Vec3 p1, Vec3 p2 )
{
	float dx ,dy, dz;

	dx = p2.x - p1.x;
	dy = p2.y - p1.y;
	dz = p2.z - p1.z;

	return dx*dx + dy*dy + dz*dz;
}


/*
	Return actor GUID
*/
DWORD CD3Actor::guid()
{
	return m_dwGUID;
}

/*
	Return actor memory base address
*/
DWORD CD3Actor::base()
{
	return m_dwBaseRact;
}


/*
	Return object location
*/
Vec3 CD3Actor::location()
{
	return m_pos;
}

/*
	Return actor transformed name
*/
std::string CD3Actor::name()
{
	std::string name2 = m_Name.substr(0, m_Name.rfind("-"));

	switch(ActorSNO.type)
	{
		case ActorType_Monster:
		case ActorType_Player:
			{
				if(CSNOManager::Instance().ActorNames.find(name2) != CSNOManager::Instance().ActorNames.end())
					return CSNOManager::Instance().ActorNames[name2];
			}
			break;

		case ActorType_Gizmo:
			{
				if(CSNOManager::Instance().GizmosNames.find(name2) != CSNOManager::Instance().GizmosNames.end())
					return CSNOManager::Instance().GizmosNames[name2];
			}
			break;

		case ActorType_Item:
			{
				if(CSNOManager::Instance().ItemNames.find(name2) != CSNOManager::Instance().ItemNames.end())
					return CSNOManager::Instance().ItemNames[name2];
			}
			break;
	}

	return "";
}

/*
	Return actor game name
*/
std::string CD3Actor::name_proxy()
{
	return m_Name;
}

/*
	Return distance to player
*/
float CD3Actor::distance()
{
	return m_dist;
}

/*
	Return actor type
*/
ActorType CD3Actor::type()
{
	return ActorSNO.type;
}

/*
	Return monster type (if actor is a monster)
*/
MonsterType CD3Actor::mobtype()
{
	return MonsterSNO.Type;
}

/*
	Return monster race (if actor is a monster)
*/
MonsterRace CD3Actor::mobrace()
{
	return MonsterSNO.Race;
}

