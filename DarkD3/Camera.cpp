#include "Camera.h"


CCamera::CCamera(void)
{
	RtlZeroMemory(&m_camRaw, sizeof(m_camRaw));
	RtlZeroMemory(&m_CamSub, sizeof(m_CamSub));
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
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)m_camRaw.ptr, sizeof(m_CamSub), &m_CamSub));

	m_CamSub.actor_id = actor.RActor.id_actor;

	CHK_RES(CProcess::Instance().Core.Write((DWORD)m_camRaw.ptr, m_CamSub));

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
	CameraRaw2 cam2;
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)m_camRaw.ptr, sizeof(m_CamSub), &m_CamSub));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr2, sizeof(cam2), &cam2));

	m_CamSub.zoom = val;

	CHK_RES(CProcess::Instance().Core.Write((DWORD)m_camRaw.ptr, m_CamSub));

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
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, sizeof(m_camRaw), &m_camRaw));
	CHK_RES(CProcess::Instance().Core.Read((DWORD)m_camRaw.ptr, sizeof(m_CamSub), &m_CamSub));

	m_camRaw.cam_mode = 0;
	//m_CamSub.actor_id = (DWORD)INVALID_VALUE;
	m_CamSub.position = pos;

	CHK_RES(CProcess::Instance().Core.Write((DWORD)CGlobalData::Instance().ObjMgr.Storage.CameraPtr, m_camRaw));
	CHK_RES(CProcess::Instance().Core.Write((DWORD)m_camRaw.ptr, m_CamSub));

	return ERROR_SUCCESS;
}