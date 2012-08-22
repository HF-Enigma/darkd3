#ifndef _D3_PLAYER_H_
#define _D3_PLAYER_H_

#include "stdafx.h"
#include "Defines.h"
#include "D3Actor.h"
#include "Process.h"

typedef struct _POWER
{
	PowerIds id;	
	int rune;		//skill rune
	int cd;			//cooldown time, in ms (1s = 60ms)
}POWER, *PPOWER;

class CD3Player : public CD3Actor
{
public:
	CD3Player(void);
	CD3Player(DWORD dwBase, Vec3* playerpos = NULL, bool bACD = true);
	~CD3Player(void);

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
	DWORD ClickToMove( Vec3 location);

	/*
		Get current ClickToMove status

		IN:
			void

		OUT:
			status - CTM state

		RETURN:
			Error code
	*/
	DWORD IsMoving(DWORD& status);

	/*
		Get active player powers

		IN:
			out - container reference

		OUT:
			out - player powers

		RETURN:
			Error code
	*/
	DWORD GetPowers(std::vector<POWER> &out);

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
	DWORD UsePower( CD3Actor actor, PowerIds Power );

	/*
		Check if player exists and valid
		
		RETURN:
			Validity flag
	*/
	bool valid();
private:

	bool m_bValid;		//Player is valid
};

#endif//_D3_PLAYER_H_