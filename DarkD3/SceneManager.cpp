#include "SceneManager.h"


CSceneManager::CSceneManager(void)
{
}


CSceneManager::~CSceneManager(void)
{
}

/*
	Enumerate loaded scenes

	IN:
		void

	OUT:
		pOut - found scenes

	RETURN:
		Error code
*/
DWORD CSceneManager::EnumScenes( vecScenes *pOut /*= NULL*/)
{
	tContainer<SceneRaw> tScenes;

	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Scenes, sizeof(tScenes), &tScenes));

	//Iterate through each scene
	for(DWORD i = 0; i<= min(tScenes.Count, tScenes.Limit); i++)
		m_Scenes.push_back(CD3Scene((DWORD)tScenes.List+ i*sizeof(SceneRaw)));		

	if(pOut)
		*pOut = m_Scenes;

	return ERROR_SUCCESS;
}

/*
	Get scene holding actor

	IN:
		pos - actor location

	OUT:
		void

	RETURN:
		Pointer to found scene
*/
CD3Scene* CSceneManager::GetActorScene(Vec3 pos)
{
	//If coordinates match
	for(DWORD i = 0; i < m_Scenes.size(); i++)
	{
		if(pos.x >= m_Scenes[i].SceneRawData.navmesh.MeshMin.x && pos.x <= m_Scenes[i].SceneRawData.navmesh.MeshMax.x &&
			pos.y >= m_Scenes[i].SceneRawData.navmesh.MeshMin.y && pos.y <= m_Scenes[i].SceneRawData.navmesh.MeshMax.y)
		{
			return &m_Scenes[i];
		}
	}

	return NULL;
}

/*
	Get min and max coordinates of loaded scenes

	RETURN:
		Limits
*/
AABB CSceneManager::GetScenesLimits()
{
	AABB retval = {{500000, 500000, 0}, {0, 0, 0}};

	for(DWORD i = 0; i < m_Scenes.size(); i++)
	{
		if(m_Scenes[i].SceneRawData.id_scene != INVALID_VALUE)
		{
			if(m_Scenes[i].SceneRawData.navmesh.MeshMin.x < retval.Min.x)
				retval.Min.x = m_Scenes[i].SceneRawData.navmesh.MeshMin.x;

			if(m_Scenes[i].SceneRawData.navmesh.MeshMin.y < retval.Min.y)
				retval.Min.y = m_Scenes[i].SceneRawData.navmesh.MeshMin.y;

			if(m_Scenes[i].SceneRawData.navmesh.MeshMax.x > retval.Max.x)
				retval.Max.x = m_Scenes[i].SceneRawData.navmesh.MeshMax.x;

			if(m_Scenes[i].SceneRawData.navmesh.MeshMax.y > retval.Max.y)
				retval.Max.y = m_Scenes[i].SceneRawData.navmesh.MeshMax.y;
		}
		
	}

	return retval;
}