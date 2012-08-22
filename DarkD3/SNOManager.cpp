#include "SNOManager.h"

CSNOManager::~CSNOManager(void)
{
}

/*
	Get singleton instance
*/
CSNOManager& CSNOManager::Instance()
{
	static CSNOManager ms_Instance;

	return ms_Instance;
}

/*
	Initialize Database

	RETURN:
		Error code
*/
DWORD CSNOManager::InitDB()
{
	CHK_RES(ParseMemorySNO(SNOGroup_Scene_Addr, Scenes));

	BuildNameDB();
	BuildItemsDB();

	return ERROR_SUCCESS;
}

/*
	Build database from MPQ file

	IN:
		strMPQfile - path to MPQ (CoreData.mpq)

	OUT:
		pProgress - build progress

	RETURN:
		Error code
*/
DWORD CSNOManager::BuildDBFromFile( std::string strMPQfile, float *pProgress /*= NULL*/ )
{
	mpq_archive_s *mpq_archive = NULL;
	libmpq__off_t out_size = 0;
	char *out_buf = NULL;
	uint32_t files = 0;

	/* open the mpq archive given as first parameter. */
	if(libmpq__archive_open(&mpq_archive, strMPQfile.data(), -1) != ERROR_SUCCESS)
		return ERROR_OPEN_FAILED;

	libmpq__archive_files(mpq_archive, &files);

	for(uint32_t i = 0; i< files; i++)
	{
		/* get size of first file (0) and malloc output buffer. */
		libmpq__file_size_unpacked(mpq_archive, i, &out_size);
		out_buf = (char*)malloc((size_t)out_size);

		/* read, decrypt and unpack file to output buffer. */
		libmpq__file_read(mpq_archive, i, (uint8_t*)out_buf, out_size, NULL);

		DWORD deadbeef = (*((DWORD*)out_buf));
		SNOGroup grp = (SNOGroup)(*((DWORD*)(out_buf+4)));

		if(deadbeef == 0xDEADBEEF)
		{
			switch(grp)
			{
                //Get actor SNO from file
				case SNOGroup_Actor:
					{
						SNOActor actor;

                        //0x10 because SNOActor uses memory layout without DEADBEEF stuff
						memcpy(&actor, out_buf + 0x10, sizeof(actor));

						Actors[actor.header.id] = actor;
					}
					break;

                 //Get monster SNO from file
				case SNOGroup_Monster:
					{
						SNOMonster mob;

						memcpy(&mob, out_buf + 0x10, sizeof(mob));

						Mobs[mob.header.id] = mob;
					}
					break;

				default:
					break;
			}	
		}

        //Parsing progress
		if(pProgress)
			pProgress[0] = (float)i/(float)files;

		free(out_buf);
	}

	/* close the mpq archive. */
	libmpq__archive_close(mpq_archive);

	SaveDB();

	return ERROR_SUCCESS;
}

/*
	Parse SNO file in memory

	IN:
		base - SNO offset

	OUT:
		out - Parsed records

	RETURN:
		Error code
			
*/
DWORD CSNOManager::ParseMemorySNO( DWORD base, mapSNO& out )
{
	char SNOName[64]	= {0};
	DWORD ignoreCount	= 0;
	DWORD dwMainOffset	= CProcess::Instance().Core.Read<DWORD>(base);

    //Pointer to Index base
	DWORD dwPointer	= CProcess::Instance().Core.Read<DWORD>(dwMainOffset + SNO_DEF_PTR);

    //Loaded records count
	DWORD dwCount = CProcess::Instance().Core.Read<DWORD>(dwPointer + SNO_COUNT);

	if(dwCount == INVALID_VALUE)
		return ERROR_INVALID_ADDRESS;

	DWORD SnoIndex = CProcess::Instance().Core.Read<DWORD>(dwPointer + SNO_DEF_INDEX_OFF);
	CProcess::Instance().Core.Read(dwPointer, sizeof(SNOName), SNOName);					
	DWORD IndexOffset = SnoIndex + 0xC;											

	out.clear();

	//Iterating through all records
	for(DWORD i = 0; i<dwCount; i++)
	{
		DWORD CurSnoOffset	= CProcess::Instance().Core.Read<DWORD>(IndexOffset);	
		DWORD CurSnoID		= CProcess::Instance().Core.Read<DWORD>(CurSnoOffset);				

		out[CurSnoID] = CurSnoOffset;

		//Next item
		IndexOffset = IndexOffset + 0x10;	
	}

	return ERROR_SUCCESS;
}

/*
	Build string map from Strings SNO

	IN:
		base - SNO offset

	OUT:
		out - Parsed records

	RETURN:
		Error code
			
*/
DWORD CSNOManager::GetStrings( DWORD base, mapNames& out )
{
	DWORD dwStrucSize = 0x50;
	DWORD dwCount = CProcess::Instance().Core.Read<DWORD>(base + 0xC);
	DWORD dwCurrentOffset = base + 0x28;
	char szProxy[34] = {0}, szLocalized[34] = {0};

	out.clear();

	for (DWORD i = 0; i<dwCount; i++)
	{
		//Proxy Name, like "Priest_Male_B_NoLook"
		CProcess::Instance().Core.Read(CProcess::Instance().Core.Read<DWORD>(dwCurrentOffset), sizeof(szProxy) - 1, szProxy);

		//Localized name, like "Brother Malachi the Healer"
		CProcess::Instance().Core.Read(CProcess::Instance().Core.Read<DWORD>(dwCurrentOffset + 0x10), sizeof(szLocalized) - 1, szLocalized);

		out[szProxy] = szLocalized;

		dwCurrentOffset = dwCurrentOffset + dwStrucSize;
	}

	return ERROR_SUCCESS;
}

/*
	Build string database	
*/
void CSNOManager::BuildNameDB()
{
	DWORD MonsteID = 0xCB35;		//Names of monsters and NPC's
	DWORD GizmoID  = 0xCB23;		//Names of Gizmo's
	DWORD ItemsID  = 0xCB28;  		//Names of items

	mapSNO snoStrings;

	if(ParseMemorySNO(SNOGroup_StringList_Addr, snoStrings) == ERROR_SUCCESS)
	{
		ActorNames.clear();
		GizmosNames.clear();
		ItemNames.clear();

		if(snoStrings.find(MonsteID) != snoStrings.end())
			GetStrings(snoStrings[MonsteID], ActorNames);							

		if(snoStrings.find(GizmoID) != snoStrings.end())
			GetStrings(snoStrings[GizmoID], GizmosNames);	

		if(snoStrings.find(ItemsID) != snoStrings.end())
			GetStrings(snoStrings[ItemsID], ItemNames);	
	}
}

/*
	Build items database
*/
void CSNOManager::BuildItemsDB()
{
	SNOGBHeader header;
	DWORD dwOffset;

    //Get GameBalance file addresses in memory (seems they are are always fully loaded)
	ParseMemorySNO(SNOGroup_GameBalance_Addr, GameBalance);

    //Iterate through GameBalance files
	for(mapSNO::iterator iter = GameBalance.begin(); iter != GameBalance.end(); iter++)
	{
		CProcess::Instance().Core.Read(iter->second, sizeof(header), &header);

        //File contains items data
		if(header.type == GBType_Items)
		{
            //Read serialized records
			for (dwOffset = header.Item.mem_offset; dwOffset < header.Item.mem_offset + header.Item.size; dwOffset += sizeof(SNOGBItem))
			{
				SNOGBItem item;

				CProcess::Instance().Core.Read(dwOffset, sizeof(item), &item);

                //Store item by hash (gbid)
				Items[CGlobalData::Instance().HashStringLC(item.Name)] = item;
			}
		}
	}
}

/*
	Save database to file

	RETURN:
		Error code
*/
DWORD CSNOManager::SaveDB()
{
	ds_utils::ds_fs::CDSWinFile<DBHEADER> file;
	ds_utils::ds_memory::CDSWinDataBlock data;
	DBHEADER header;
	DWORD dwDataOffset = 0;
	DWORD dwSize = 0;

	std::map<DWORD, SNOActor>::iterator ActIter;
	std::map<DWORD, SNOMonster>::iterator MobIter;

	file.Create(DB_FILENAME);

	header.cookie = DB_COOKIE;

	header.actors.file_offset = sizeof(header);
	header.actors.count = Actors.size();

	header.mobs.file_offset = header.actors.file_offset + header.actors.count * sizeof(SNOActor);
	header.mobs.count = Mobs.size();

	header.items.file_offset = (DWORD)INVALID_VALUE;
	header.items.count = 0;

	file.WriteHeader(header);

    //Uncompressed data size
	dwSize = Actors.size() * sizeof(SNOActor) + Mobs.size() * sizeof(SNOMonster);

	data.Allocate(dwSize);

	//Records offsets in file
	for(ActIter = Actors.begin(); ActIter != Actors.end(); ActIter++)
	{
		memcpy(data.Data() + dwDataOffset, &ActIter->second, sizeof(SNOActor));

		dwDataOffset += sizeof(SNOActor);
	}

	for(MobIter = Mobs.begin(); MobIter != Mobs.end(); MobIter++)
	{
		memcpy(data.Data() + dwDataOffset, &MobIter->second, sizeof(SNOMonster));

		dwDataOffset += sizeof(SNOMonster);
	}

	CCompressor::Compress(data, dwSize);

	file.Write(sizeof(header), data, dwSize);

	return ERROR_SUCCESS;
}

/*
	Load Database from file

	RETURN:
		Error code
*/
DWORD CSNOManager::LoadDB()
{
	DBHEADER header = {0};
	DWORD dwDataSize = 0;
	DWORD dwReadSize = 0;

	ds_utils::ds_memory::CDSWinDataBlock data;
	ds_utils::ds_fs::CDSWinFile<DBHEADER> file;

	if(!file.Open(DB_FILENAME))
		return file.LastError();

	if(!file.ReadHeader(header))
		return file.LastError();

	if(header.cookie != DB_COOKIE)
		return ERROR_INVALID_DATA;

    //Size of uncompressed data
	dwDataSize = header.actors.count * sizeof(SNOActor) 
			   + header.mobs.count * sizeof(SNOMonster) 
			   + header.items.count * sizeof(SNOGBItem);

	dwReadSize = (DWORD)file.Size() - sizeof(header);

    //Buffer
	if(!data.Allocate(dwDataSize))
	{
		file.Close();
		return GetLastError();
	}

    //Compressed data
	if(!file.Read(sizeof(header), data, dwReadSize))
		return file.LastError();

	file.Close();

	CCompressor::Decompress(data, dwReadSize, dwDataSize);

	if(dwReadSize != dwDataSize)
		return ERROR_INVALID_DATA;

    //Get corresponding records from data
	for(DWORD i=0; i<header.actors.count; i++)
	{   
		SNOActor actor;

		RtlCopyMemory(&actor, data + i*sizeof(SNOActor), sizeof(SNOActor));    

		Actors[actor.header.id] = actor;
	}

	for(DWORD i=0; i<header.mobs.count; i++)
	{   
		SNOMonster mob;

		RtlCopyMemory(&mob, data + header.actors.count*sizeof(SNOActor) + i*sizeof(SNOMonster), sizeof(SNOMonster));    

		Mobs[mob.header.id] = mob;
	}

	m_bInitialized = true;

	return ERROR_SUCCESS;
}