#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "stdafx.h"
#include "Defines.h"
#include "Structs.h"
#include "Process.h"
#include "Scene.h"

typedef std::vector<CD3Scene> vecScenes;

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
	DWORD EnumScenes(vecScenes *pOut = NULL);

	/*
		Get scene holding actor

		IN:
			pos - actor location

		OUT:
			void

		RETURN:
			Pointer to found scene
	*/
	CD3Scene* GetActorScene(Vec3 pos);

	/*
		Get min and max coordinates of loaded scenes

		RETURN:
			Limits
	*/
	AABB GetScenesLimits();

private:
	vecScenes m_Scenes;		//Loaded scenes
};

#endif//_SCENE_MANAGER_H_