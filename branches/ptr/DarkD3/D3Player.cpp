#include "D3Player.h"


CD3Player::CD3Player(void)
    : CD3Actor()
{
}

CD3Player::CD3Player( DWORD dwBase, Vec3* playerpos /*= NULL*/, bool bACD /*= true*/ )
    : CD3Actor(dwBase, playerpos, bACD)
{
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
	mov.MoveTo = location;
	mov.IsMoving = 1;
	mov.fix2 = 0x120010;
	mov.Flags = 0x11068;
	//mov.unknown_C0[2] = (DWORD)INVALID_VALUE;

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
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Data + local.Index*0xD0D0, sizeof(data), &data));
	CHK_RES(GetAllAttribs(attribs));

	for(DWORD i = 0; i < sizeof(data.Data.skills) / sizeof(ObSkill); i++)
	{
		POWER power = {(PowerIds)0, false, 0};

		power.id = data.Data.skills[i].id;
		power.rune = data.Data.skills[i].rune;

		//Get cooldown time if any
		if(attribs[Power_Cooldown].values.count(power.id) && attribs[Power_Cooldown_Start].values.count(power.id))
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
#if(RCALL_TYPE == USE_REMOTE_THD)
	ds_utils::ds_memory::CDSWinDataBlock pCopy;
	void *pFuncData = 0, *pRemoteData;
	DWORD dwFuncSize;
	int index = 0;
	usePowerData power;

	//Allocate space for player pointer and usePower struct
	CHK_RES(CProcess::Instance().Core.Allocate(sizeof(power) + sizeof(CRActor*), pRemoteData));

	power.acd_id	= actor.ACD.id_acd;
	power.end		= 0xffffffff;
	power.cmd		= 2;
	power.power_1	= power.power_2 = Power;
	power.zero		= 0;
	power.pos		= actor.ACD.PosWorld;
	power.world_id	= RActor.guid_world;

	CHK_RES(CProcess::Instance().Core.Write<DWORD>((DWORD)pRemoteData, m_dwBaseRact));
	CHK_RES(CProcess::Instance().Core.Write<usePowerData>((DWORD)pRemoteData + sizeof(CRActor*), power));

	UsePowerWrapper(&pFuncData, &dwFuncSize);

	pCopy.Allocate(dwFuncSize);

	memcpy(pCopy, pFuncData, dwFuncSize);

	DWORD dwReplacement[3] = {(DWORD)pRemoteData, (DWORD)pRemoteData + sizeof(CRActor*), m_dwBaseRact};

	//Replace stubs with proper addresses
	for(DWORD i = 0; i < dwFuncSize; i++)
	{
		if(((DWORD*)(&((BYTE*)pCopy)[i]))[0] == 0xDEADBEEF)
		{
			memcpy(&((BYTE*)pCopy)[i], &dwReplacement[index], sizeof(DWORD));
			index++;

			if(index >= ARRAYSIZE(dwReplacement))
				break;
		}
	}

	//Do remote call
	CProcess::Instance().RemoteCall(pCopy, dwFuncSize);

	//Free allocated memory
	CProcess::Instance().Core.Free(pRemoteData);

#elif(RCALL_TYPE == USE_DLL)

	CALL call    = {0};
	CALL_RET ret = {0};

	call.valid = VALID_CALL;
	call.state = CallState_Pending;
	call.type  = CallType_UsePower;
	call.arg1  = m_dwBaseRact;

	call.usepower.acd_id   = actor.ACD.id_acd;
	call.usepower.end      = -1;
	call.usepower.cmd      = 1;
	call.usepower.power_1  = call.usepower.power_2 = Power;
	call.usepower.zero     = 0;
	call.usepower.pos      = actor.ACD.PosWorld;
	call.usepower.world_id = RActor.guid_world;

	CProcess::Instance().shared.DoCall(call, ret);

#else

	return ERROR_NOT_SUPPORTED;

#endif

	return ERROR_SUCCESS;
}


/*
	ASM code container for UsePower RemoteThread call

	IN:
		void

	OUT:
		FuncData - pointer to function body
		pdwSize - size of code

	RETURN:
		Error code
*/
DWORD CD3Player::UsePowerWrapper( void** FuncData, DWORD* pdwSize )
{
	__asm
	{
		//calculate size of executable code
		mov eax, start
		mov ebx, codeend
		sub ebx, eax
		mov edi, pdwSize
		mov dword ptr [edi], ebx

		//get pointer to code start
		mov eax, FuncData
		mov ebx, start
		mov dword ptr [eax],ebx
		jmp codeend

	start: 
		push 0xDEADBEEF			//pPlayerPtr
		push 1
		push 0
		mov esi, 0xDEADBEEF		//dwPacketAddr
		mov eax, 0xDEADBEEF		//dwPlayerPtr
			
		mov ecx, FUNC_USE_POWER	
		call ecx

		add esp, 0Ch

		mov ecx, 64
		mov edi, fs:[18h]
		add edi, 0E10h
		xor esi, esi

	zero_tls:
		mov dword ptr [edi], esi
		add edi, 4
		dec ecx
		jnz zero_tls

		ret

	codeend:
	}

	return ERROR_SUCCESS;
}