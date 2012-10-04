#ifndef _SCENE_H_
#define _SCENE_H_

#include "stdafx.h"
#include "Structs.h"
#include "Process.h"

class CD3Scene 
{
public:
	CD3Scene();
	CD3Scene(DWORD dwBase);
	~CD3Scene();

	/*
		Get cell with player

		IN:
			pos - player position

		OUT:
			void

		RETURN:
			Pointer to found cell
			NULL if not found
	*/
	NavCell* GetCellByCoords(Vec3 pos);

	/*
		Get neighbor cells of selected one

		IN:
			cell - target cell

		OUT:
			out - found neighbors

		RETURN:
			Error code
	*/
	DWORD GetCellNeighbours(NavCell &cell, std::vector<NavCell> &out);
public:
	SceneRaw					SceneRawData;		//NavScene
	NavZoneRaw					ZoneRawData;		//NavZone
	SNOScene					SceneSNO;			//Scene SNO record
	std::vector<NavCell>		Cells;				//Scene cells collection

private:
	DWORD m_dwBase;

	//std::vector<NavMeshSquare> m_Squares;
	std::vector<NavCellLookup>	m_CellNeighbours;	//Neighbor cells lookup
	//std::vector<NavCellLookup> m_CellLookups;
	//std::vector<NavGridSquare> m_GridSquares;
};

#endif//_SCENE_H_