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
DWORD CSceneManager::EnumScenes( mapWorlds *pOut /*= NULL*/ )
{
	tContainer2<SceneRaw> tScenes;

	//Refresh SNO scene records
	CSNOManager::Instance().RefreshSNOMemRecords();

	//Scene container
	CHK_RES(CProcess::Instance().Core.Read((DWORD)CGlobalData::Instance().ObjMgr.Storage.Scenes, sizeof(tScenes), &tScenes));

	//Iterate through each scene
	for(DWORD i = 0; i<= min(tScenes.Count, tScenes.Limit); i++)
	{
		CD3Scene scene((DWORD)tScenes.List+ i*tScenes.SizeOf);

		m_Worlds[scene.SceneRawData.navmesh.id_world][scene.SceneRawData.id_scene] = scene;	
	}

	if(pOut)
		*pOut = m_Worlds;

	return ERROR_SUCCESS;
}

/*
	Get world scene by ID

	IN:
		worldID - world ID
		sceneID - scene ID

	OUT:
		void

	RETURN:
		Pointer to found scene
*/
CD3Scene* CSceneManager::GetSceneByID( DWORD worldID, DWORD sceneID )
{
	//If coordinates match
	if(m_Worlds.find(worldID) != m_Worlds.end() && m_Worlds[worldID].find(sceneID) != m_Worlds[worldID].end())
		return &m_Worlds[worldID][sceneID];

	return NULL;
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
CD3Scene* CSceneManager::GetSceneByCoords( Vec3 pos, DWORD worldID )
{
	//If coordinates match
	if(m_Worlds.find(worldID) != m_Worlds.end())
	{
		for(mapScenes::iterator i = m_Worlds[worldID].begin(); i != m_Worlds[worldID].end(); i++)
		{
			if(pos.x >= i->second.SceneRawData.navmesh.MeshMin.x && pos.x <= i->second.SceneRawData.navmesh.MeshMax.x &&
				pos.y >= i->second.SceneRawData.navmesh.MeshMin.y && pos.y <= i->second.SceneRawData.navmesh.MeshMax.y)
			{
				return &i->second;
			}
		}
	}

	return NULL;
}

/*
	Get min and max coordinates of loaded scenes

	RETURN:
		Limits
*/
AABB CSceneManager::GetScenesLimits( DWORD worldID )
{
	AABB retval = {{500000, 500000, 0}, {0, 0, 0}};

	if(m_Worlds.find(worldID) != m_Worlds.end())
	{
		for(mapScenes::iterator i = m_Worlds[worldID].begin(); i != m_Worlds[worldID].end(); i++)
		{
			if(i->second.SceneRawData.id_scene != INVALID_VALUE)
			{
				if(i->second.SceneRawData.navmesh.MeshMin.x < retval.Min.x)
					retval.Min.x = i->second.SceneRawData.navmesh.MeshMin.x;

				if(i->second.SceneRawData.navmesh.MeshMin.y < retval.Min.y)
					retval.Min.y = i->second.SceneRawData.navmesh.MeshMin.y;

				if(i->second.SceneRawData.navmesh.MeshMax.x > retval.Max.x)
					retval.Max.x = i->second.SceneRawData.navmesh.MeshMax.x;

				if(i->second.SceneRawData.navmesh.MeshMax.y > retval.Max.y)
					retval.Max.y = i->second.SceneRawData.navmesh.MeshMax.y;
			}	
		}
	}

	return retval;
}

/*
	Reset scene cache for particular world. Or all cached scenes;

	IN:
		worldID - world to reset, if INVALID_VALUE - reset everything

	OUT:
		void

	RETURN:
		Error code
*/
DWORD CSceneManager::Reset( DWORD worldID /*= INVALID_VALUE*/ )
{
	if(worldID == INVALID_VALUE)
	{
		m_Worlds.clear();

		return ERROR_SUCCESS;
	}
	else if(m_Worlds.find(worldID) != m_Worlds.end())
	{
		m_Worlds[worldID].clear();

		return ERROR_SUCCESS;
	}
	else
		return ERROR_NOT_FOUND;
}

/*
	Retrieve world scenes

	IN:
		worldID - world

	OUT:
		void

	RETURN:
		Cached scenes if any
*/
mapScenes* CSceneManager::GetWorldScenes(DWORD worldID)
{
	if(m_Worlds.find(worldID) != m_Worlds.end())
		return &m_Worlds[worldID];
	else 
		return NULL;
}