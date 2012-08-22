#include "Camera.h"


CCamera::CCamera(void)
{
	RtlZeroMemory(&m_camRaw, sizeof(m_camRaw));
}

CCamera::~CCamera(void)
{
}

CCamera& CCamera::Instance()
{
	static CCamera ms_Instance;

	return ms_Instance;
}

/*
	Attach camera to actor

	IN:
		actor - actor to attach to

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CCamera::AttachToActor(CD3Actor actor)
{
	CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw);

	m_camRaw.actor_id = actor.RActor.id_actor;

	CProcess::Instance().Core.Write((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, m_camRaw);

	return ERROR_SUCCESS;
}

/*
	Set camera zoom

	IN:
		val - zoom value

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CCamera::SetZoom(float val)
{
	CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw);

	m_camRaw.zoom = val;

	CProcess::Instance().Core.Write((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, m_camRaw);

	return ERROR_SUCCESS;
}

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
DWORD CCamera::SetPosition(Vec3 pos)
{
	CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw);

	m_camRaw.actor_id = (DWORD)INVALID_VALUE;
	m_camRaw.position = pos;

	CProcess::Instance().Core.Write((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, m_camRaw);

	return ERROR_SUCCESS;
}