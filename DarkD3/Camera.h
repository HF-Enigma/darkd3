#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"
#include "Process.h"
#include "D3Actor.h"

class CCamera
{
public:
	~CCamera(void);

	/*
		Get singleton instance
	*/
	static
	CCamera& Instance();

	/*
		Attach camera to actor

		IN:
			actor - actor to attach to

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD AttachToActor(CD3Actor actor);

	/*
		Set camera zoom

		IN:
			val - zoom value

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SetZoom(float val);

	/*
		Set camera position.
		Will work only if camera is not attached to any actor

		IN:
			pos - position

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD SetPosition(Vec3 pos);

private:
	CCamera();
	CCamera( CCamera& root ){};
	CCamera& operator = ( CCamera& ){};

private:
	CameraRaw m_camRaw;			//Camera struct in memory
};

#endif//_CAMERA_H_