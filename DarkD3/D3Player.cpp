#include "D3Player.h"


CD3Player::CD3Player(void):
	CD3Actor()
{
}

CD3Player::CD3Player( DWORD dwBase, Vec3* playerpos /*= NULL*/, bool bACD /*= true*/ ):
	CD3Actor(dwBase, playerpos, bACD)
{
	if(ACD.id_acd == PLAYER_GUID && RActor.id_acd == PLAYER_GUID)
		m_bValid = true;
}


CD3Player::~CD3Player(void)
{
}

/*
	ClickToMove function

	IN:
		location - location to move to
		player - local player

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CD3Player::ClickToMove( Vec3 location )
{
	CActorMovement mov;

	CHK_RES(CProcess::Instance().Core.Read((DWORD)RActor.Mov, sizeof(mov), &mov));

	//mov.Pos3 = mov.Pos2 = player.Pos;
	mov.guid_world = mov.guid_world2 = RActor.guid_world;
	mov.MovTo = location;
	mov.IsMoving = 1;
	mov.fix2 = 0x00220018;
	mov.Flags = 69736;
	mov.unknown_C0[2] = (DWORD)INVALID_VALUE;

	CHK_RES(CProcess::Instance().Core.Write((DWORD)RActor.Mov, sizeof(mov), &mov));

	return ERROR_SUCCESS;
}

/*
	Get current ClickToMove status

	IN:
		void

	OUT:
		status - CTM state

	RETURN:
		Error code
*/
DWORD CD3Player::IsMoving(DWORD& status)
{
	CActorMovement mov;

	//Get current CTM status
	CHK_RES(CProcess::Instance().Core.Read((DWORD)RActor.Mov, sizeof(mov), &mov));

	status = mov.IsMoving;

	return ERROR_SUCCESS;
}

/*
	Get active player powers

	IN:
		out - container reference

	OUT:
		out - player powers

	RETURN:
		Error code
*/
DWORD CD3Player::GetPowers( std::vector<POWER> &out )
{
	std::map<AttributeID, ATTRIB_INFO> attribs;
	std::map<int, AttribVal> cds;
	CObLocal local;
	CObDataContainer data;

	out.clear();

	//Get local players struct
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Local, sizeof(local), &local));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Data + local.Index*0x7FF8, sizeof(data), &data));
	CHK_RES(GetAllAttribs(attribs));

	for(DWORD i = 0; i < sizeof(data.Data.skills) / sizeof(ObSkill); i++)
	{
		POWER power = {(PowerIds)0, false, 0};

		power.id = data.Data.skills[i].id;
		power.rune = data.Data.skills[i].rune;

		//Get cooldown time if any
		if(attribs[Power_Cooldown].values.find(power.id) != attribs[Power_Cooldown].values.end() &&
			attribs[Power_Cooldown_Start].values.find(power.id) != attribs[Power_Cooldown_Start].values.end())
		{
			power.cd = attribs[Power_Cooldown].values[power.id].ival
					 - attribs[Power_Cooldown_Start].values[power.id].ival;
		}

		out.push_back(power);
	}

	return ERROR_SUCCESS;
}

/*
	Use power on actor

	IN:
		actor - target actor
		Power - power ID to use

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CD3Player::UsePower( CD3Actor actor, PowerIds Power )
{
#if(RCALL_TYPE != USE_DLL)
	DWORD status = 0;

	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_MOV_OFF, Walk);				//Set to Walk if you want player to move to target, -1 otherwise
	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_ACTION_OFF, 1);				//Action type
	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_POWER1_OFF, Power);
	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_POWER2_OFF, Power);
	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_MOUSE_OFF, 1);				//Mouse state
	CProcess::Instance().Core.Write<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_GUID_OFF, actor.ACD.id_acd);

	//Perform action
	ClickToMove(actor.location());

	DWORD tempvalue = CProcess::Instance().Core.Read<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_ACTION_OFF);

	IsMoving(status);

	while( tempvalue == 1 && status == 1)
	{
		Sleep(10);
		tempvalue = CProcess::Instance().Core.Read<DWORD>(ITER_STRUCT_BASE + ITER_STRUCT_ACTION_OFF);
		IsMoving(status);
	}
#else
	CALL call = {0};
	CALL_RET ret = {0};

	call.valid = VALID_CALL;
	call.state = CallState_Pending;
	call.type = CallType_UsePower;
	call.arg1 = m_dwBaseAddr;

	call.usepower.acd_id = actor.ACD.id_acd;
	call.usepower.end = Walk;
	call.usepower.cmd = 1;
	call.usepower.power_1 = call.usepower.power_2 = Power;
	call.usepower.zero = 0;
	call.usepower.pos = actor.ACD.PosWorld;
	call.usepower.world_id = RActor.guid_world;

	CProcess::Instance().shared.DoCall(call, ret);

#endif
	return ERROR_SUCCESS;
}

bool CD3Player::valid()
{
	return m_bValid;
}