#include "Scene.h"

CD3Scene::CD3Scene():
	m_dwBase(0)
{
	RtlZeroMemory(&ZoneRawData, sizeof(ZoneRawData));
}

CD3Scene::CD3Scene( DWORD dwBase ):
	m_dwBase(dwBase)
{
	RtlZeroMemory(&ZoneRawData, sizeof(ZoneRawData));
	RtlZeroMemory(&SceneRawData, sizeof(SceneRawData));

	CProcess::Instance().Core.Read(dwBase, sizeof(SceneRaw), &SceneRawData);
	CProcess::Instance().Core.Read((DWORD)SceneRawData.navmesh.pZone, sizeof(NavZoneRaw), &ZoneRawData);

	if(CSNOManager::Instance().Scenes.find(SceneRawData.navmesh.id_sno) != CSNOManager::Instance().Scenes.end())
	{
		DWORD dwSNOAddress = CSNOManager::Instance().Scenes[SceneRawData.navmesh.id_sno];

		CProcess::Instance().Core.Read(dwSNOAddress, sizeof(SceneSNO), &SceneSNO);

		//m_Squares.reserve(SceneSNO.navmesh.SquareCount);
		//CSNOManager::Instance().GetSerializedRecords(m_Squares, SceneSNO.navmesh.Squares, dwSNOAddress);

		//Nav Cells
		Cells.reserve(SceneSNO.NavZone.NavCellCount);
		CSNOManager::Instance().GetSerializedRecords(Cells, SceneSNO.NavZone.NavCells, dwSNOAddress);

		//Cell neighbors
		m_CellNeighbours.reserve(SceneSNO.NavZone.NeighbourCount);
		CSNOManager::Instance().GetSerializedRecords(m_CellNeighbours, SceneSNO.NavZone.NavCellNeighbours, dwSNOAddress);

		//Cell lookups
		//CSNOManager::Instance().GetSerializedRecords(m_CellLookups, SceneSNO.NavZone.CellLookups, dwSNOAddress);

		//Grid mesh
		//CSNOManager::Instance().GetSerializedRecords(m_GridSquares, SceneSNO.NavZone.GridSquares, dwSNOAddress);
	}

	return;
}

CD3Scene::~CD3Scene()
{
}

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
NavCell* CD3Scene::GetCellByCoords(Vec3 pos)
{
	Vec3 pos_offset;

	pos_offset.x = pos.x - ZoneRawData.zoneMin.x;
	pos_offset.y = pos.y - ZoneRawData.zoneMin.y;
	pos_offset.z = 0;

	//If coordinates contains player
	for(DWORD i = 0; i < Cells.size(); i++)
	{
		if(pos_offset.x >= Cells[i].Min.x && pos_offset.x <= Cells[i].Max.x &&
			pos_offset.y >= Cells[i].Min.y && pos_offset.y <= Cells[i].Max.y)
		{
			return &Cells[i];
		}
	}

	return NULL;
}

/*
	Get neighbor cells of selected one

	IN:
		cell - target cell

	OUT:
		out - found neighbors

	RETURN:
		Error code
*/
DWORD CD3Scene::GetCellNeighbours( NavCell &cell, std::vector<NavCell> &out )
{
	out.clear();

	for(DWORD i = cell.NeighborsIndex; i < cell.NeighborsIndex + cell.NeighbourCount; i++)
	{
		WORD index = m_CellNeighbours[i].WCell;

		if(index < Cells.size())
			out.push_back(Cells[index]);
		//Here goes border cells in another scene
		//else if(index > 0x8000)
	}

	return ERROR_SUCCESS;
}