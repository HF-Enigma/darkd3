#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "stdafx.h"
#include "Defines.h"
#include "Attributes.h"
#include "Structs.h"
#include "MemCore.h"

enum AttributeType
{
	AttributeType_invalid = INVALID_VALUE,
	AttributeType_int = 0,
	AttributeType_float = 1
};

struct AttribVal
{
	union
	{
		float fval;
		int ival;
	};
};

//Attribute descriptor
typedef struct _ATTRIB_INFO
{
	AttributeID id;
	
	std::string name;
	AttributeType type;

	std::map<int, AttribVal> values;
}ATTRIB_INFO, *PATTRIB_INFO;


class CActorAttribute
{
public:
	CActorAttribute():
		name(""),
		type(AttributeType_invalid)
	{
	}

	CActorAttribute(std::string attrib_name, AttributeType attrib_type):
		name(attrib_name),
		type(attrib_type)
	{
	};

	virtual ~CActorAttribute()
	{
	};

public:
	std::string name;			//Attribute name
	AttributeType type;			//Int or float
};

typedef std::map<AttributeID, CActorAttribute> mapAttribs;
typedef std::pair<AttributeID, CActorAttribute> pairAttribs;

class CGlobalData
{
public:
	
	~CGlobalData(){ };

	/*
		Get singleton instance
	*/
	static 
	CGlobalData& Instance();

	/*
		Update major offsets (Object manager, Interact base)
	*/
	void RefreshOffsets();

	/*
		Hash string in lower case
		Used in Game balance

		IN:
			str - string to hash

		OUT:
			void

		RETURN:
			Error code

	*/
	DWORD HashStringLC(std::string str);

public:
	mapAttribs	Attributes;		//Attributes map
	ObMan		ObjMgr;			//Object manager instance

private:

	/*
		Init attributes map
	*/
	void InitAttribs();

private:
	CGlobalData(){ };
	CGlobalData(CGlobalData& root){ };
	CGlobalData& operator = ( CGlobalData& ){ };

};

#endif//_GLOBALS_H_