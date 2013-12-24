#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"
#include "Process.h"
#include "Scene.h"

typedef std::map<DWORD, CD3Scene> mapScenes;
typedef std::map<DWORD, mapScenes> mapWorlds;

class CSceneManager
{
public:
	CSceneManager(void);
	~CSceneManager(void);

	/*
		Enumerate loaded scenes

		IN:
			void

		OUT:
			pOut - found scenes

		RETURN:
			Error code
	*/
	DWORD EnumScenes(mapWorlds *pOut = NULL);

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
	CD3Scene* GetSceneByID( DWORD worldID, DWORD sceneID );

	/*
		Get scene holding actor

		IN:
			pos - actor location

		OUT:
			void

		RETURN:
			Pointer to found scene
	*/
	CD3Scene* GetSceneByCoords(Vec3 pos, DWORD worldID);

	/*
		Get min and max coordinates of loaded scenes

		RETURN:
			Limits
	*/
	AABB GetScenesLimits(DWORD worldID);

	/*
		Reset scene cache for particular world. Or all cached scenes;

		IN:
			worldID - world to reset, if INVALID_VALUE - reset everything

		OUT:
			void

		RETURN:
			Error code
	*/
	DWORD Reset(DWORD worldID = INVALID_VALUE);

	/*
		Retrieve world scenes

		IN:
			worldID - world

		OUT:
			void

		RETURN:
			Cached scenes if any
	*/
	mapScenes* GetWorldScenes(DWORD worldID);
private:
	mapWorlds m_Worlds;		//Loaded scenes
};

#endif//_SCENE_MANAGER_H_