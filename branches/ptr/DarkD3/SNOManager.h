#ifndef _SNO_MANAGER_H_
#define _SNO_MANAGER_H_

#include "stdafx.h"
#include "Defines.h"
#include "Process.h"
#include "SNOClasses.h"
#include "MPQReader/mpq.h"
#include "Compressor.h"

#define DB_FILENAME "sno_db.ddb"
#define DB_COOKIE	'D3DB'

typedef std::map<DWORD, DWORD>				mapSNO;
typedef std::map<std::string, std::string>	mapNames;

/*
	Records header
*/
typedef struct _DBRECHEADER
{
	DWORD file_offset;
	DWORD count;
}DBRECHEADER, *PDBRECHEADER;

/*
	Internal database header
*/
typedef struct _DBHEADER
{
	DWORD cookie;
	DBRECHEADER actors;
	DBRECHEADER mobs;
	DBRECHEADER items;
}DBHEADER, *PDBHEADER;

class CSNOManager
{
public:
	~CSNOManager(void);

	/*
		Get singleton instance
	*/
	static
	CSNOManager& Instance();

	/*
		Initialize Database

		RETURN:
			Error code
	*/
	DWORD InitDB();

	/*
		Load Database from file

		RETURN:
			Error code
	*/
	DWORD LoadDB();

	/*
		Build database from MPQ file

		IN:
			strMPQfile - path to MPQ (CoreData.mpq)

		OUT:
			pProgress - build progress

		RETURN:
			Error code
	*/
	DWORD BuildDBFromFile(std::string strMPQfile, float *pProgress = NULL);

	
	/*
		Get latest SNO records available from memory

		RETURN:
			Error code
	*/
	DWORD RefreshSNOMemRecords();

	/*
		Read serialized data records

		IN:
			_Type - data type
			ptr - data ptr
			dwBase - data base ( != 0 for memory reads)

		OUT:
			out - found records

		RETURN:
			Error code
	*/
	template<class _Type>
	DWORD GetSerializedRecords(std::vector<_Type> &out, DataPtr2 ptr, DWORD dwBase = 0);

public:
	mapSNO			Strings, GameBalance, Scenes;			//SNO record addresses
	mapNames		ActorNames, GizmosNames, ItemNames;		//String records

	//SNO records
	std::map<DWORD, SNOGBItem>	Items;
	std::map<DWORD, SNOActor>	Actors;
	std::map<DWORD, SNOMonster>	Mobs;

private:
	CSNOManager(){ };
	CSNOManager(CSNOManager& root){ };
	CSNOManager& operator = ( CSNOManager& ){ };

	/*
		Parse SNO file in memory

		IN:
			base - SNO offset

		OUT:
			out - Parsed records

		RETURN:
			Error code
			
	*/
	DWORD ParseMemorySNO( DWORD base, mapSNO& out );

	/*
		Build string map from Strings SNO

		IN:
			base - SNO offset

		OUT:
			out - Parsed records

		RETURN:
			Error code
			
	*/
	DWORD GetStrings(DWORD base, mapNames& out); 

	/*
		Build string database	
	*/
	void BuildNameDB();

	/*
		Build items database
	*/
	void BuildItemsDB();

	/*
		Save database to file

		RETURN:
			Error code
	*/
	DWORD SaveDB();

private:
	bool m_bInitialized;			//Database initialized
};

/*
	Read serialized data records

	IN:
		_Type - data type
		ptr - data ptr
		dwBase - data base ( != 0 for memory reads)

	OUT:
		out - found records

	RETURN:
		Error code
*/
template<class _Type>
DWORD CSNOManager::GetSerializedRecords(std::vector<_Type> &out, DataPtr2 ptr, DWORD dwBase /*= 0*/)
{
	//Real data start
	DWORD dwDataStart = dwBase + ptr.file_offset;

	//Read records
	while( dwDataStart < dwBase + ptr.file_offset + ptr.size )
	{
		_Type data;

		CProcess::Instance().Core.Read(dwDataStart, sizeof(_Type), &data);

		out.push_back(data);

		dwDataStart += sizeof(_Type);
	}

	return ERROR_SUCCESS;
}

#endif//_SNO_MANAGER_H_